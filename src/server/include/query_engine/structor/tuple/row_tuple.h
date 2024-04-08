#pragma once

#include "tuple.h"

class RowTuple : public Tuple
{
public:
 RowTuple() = default;
 virtual ~RowTuple()
 {
   for (FieldExpr *spec : species_) {
     delete spec;
   }
   species_.clear();
 }

 const TupleType tuple_type() const override { return RowTuple_Type; }

 void get_record(std::vector<Record *> &records) const override
 {
   records.emplace_back(record_);
 }

 void set_record(std::vector<Record *> &records) override
 {
   order_set_ = true;
   _set_record(records.front());
   records.erase(records.begin());
 }

 void _set_record(Record *record)
 {
   this->record_ = record;
   const FieldExpr *filed_expr = (FieldExpr *)(this->species_.back());
   const FieldMeta *null_filed_meta = filed_expr->field().meta();
   this->bitmap_.init(record->data() + null_filed_meta->offset(), null_filed_meta->len());
 }

 void set_schema(const Table *table, const std::string &table_alias, const std::vector<FieldMeta> *fields)
 {
   table_ = table;
   this->species_.reserve(fields->size());
   for (const FieldMeta &field : *fields) {
     species_.push_back(new FieldExpr(table, &field));
     species_[species_.size() - 1]->set_field_table_alias(table_alias);
   }
 }

 int cell_num() const override
 {
   return species_.size();
 }

 void remove_order_set() {
   order_set_ = false;
 }

 bool order_set() const {
   return order_set_;
 }

 RC cell_at(int index, Value &cell) const override
 {
   if (index < 0 || index >= static_cast<int>(species_.size())) {
     LOG_WARN("invalid argument. index=%d", index);
     return RC::INVALID_ARGUMENT;
   }

   FieldExpr *field_expr = species_[index];
   if(bitmap_.get_bit(index)) {
     cell.set_null();
   } else {
     const FieldMeta *field_meta = field_expr->field().meta();
     cell.set_type(field_meta->type());
     cell.set_data(this->record_->data() + field_meta->offset(), field_meta->len());
   }
   return RC::SUCCESS;
 }

 RC find_cell(const TupleCellSpec &spec, Value &cell) const override
 {
   const char *table_name = spec.table_name();
   const char *field_name = spec.field_name();
   if (0 != strcmp(table_name, table_->name())) {
     return RC::NOTFOUND;
   }

   for (size_t i = 0; i < species_.size(); ++i) {
     const FieldExpr *field_expr = species_[i];
     const Field &field = field_expr->field();
     if (0 == strcmp(table_name, field.table_name()) &&
         0 == strcmp(field_name, field.field_name())) {
       return cell_at(i, cell);
     }
   }
   return RC::NOTFOUND;
 }

 Record &record()
 {
   return *record_;
 }

 const Record &record() const
 {
   return *record_;
 }

private:
 Record *record_ = nullptr;
 common::Bitmap bitmap_;
 const Table *table_ = nullptr;
 std::vector<FieldExpr *> species_;
 bool order_set_ = false;
};