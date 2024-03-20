#pragma once

class Trx;
class Session;

class ThreadData
{
public:
  static ThreadData *current() { return thread_data_; }
  static void setup(ThreadData *thread) { thread_data_ = thread; }

public:
  ThreadData() = default;
  ~ThreadData() = default;

  Session *session() const { return session_; }
  Trx * trx() const;

  void set_session(Session *session) { session_ = session; }

private:
  static thread_local ThreadData * thread_data_;

private:
  Session *session_ = nullptr;
};