# THSS TDB 
TDB是面向清华大学软件学院的数据库原理课程开发的一款关系型数据库管理系统。实现了关系型数据增删改查的基本功能，支持多种SQL语句，架构逻辑清晰，适合初学者用于入门数据库内核的设计与实现。

## 架构介绍
TDB包含了session, query engine以及storage engine三个大模块。

其中session模块支持CS架构和无服务启动两种方式，对外提供了可通过Sql进行交互的终端界面。

查询引擎由解析器，分析器，规划器与执行引擎三个主要模块构成，其中执行引擎会在执行具体查询时与存储引擎进行交互，由存储引擎来负责数据的组织与磁盘读写。

## 编译运行
由于session模块的设计，目前本项目仅支持linux与macOS上运行。

本项目基于C++开发，编译运行前确保已经安装了CMake以及C++17以上的环境。


完成开发环境的配置后，可以进行TDB的搭建，首先使用如下git指令将项目拉取到本地。
```git
git clone git@github.com:THSS-DB/TDB.git
```
进入项目的根目录，如果是第一次编译，运行build.sh脚本安装依赖库。
```
bash build.sh init
```
之后对代码进行编译，每当代码有任何修改，直接运行下面脚本就可以重新编译
```
bash build.sh
```
如果使用上面的命令导致编译卡死，停留在某个进度止步不前，可以使用下面的命令限制并发数
```
bash build.sh --make -j4
```
完成编译后，根目录下会存在build文件夹，可以在该文件夹下运行TDB，目前TDB有两种运行方式：
- 基于TCP运行的Client-Service架构
数据库管理系统的经典CS架构，需要首先启动一个服务作为server，之后通过client来访问server。
```shell
./bin/server -f -p 8888
```
之后新开一个终端启动client，就可以进入TDB cli交互界面
 ```shell
 ./bin/client -p 8888
```
- 无状态服务的启动
除了CS架构，TDB还支持无状态的单线程服务启动（类似于SqlLite），通过单独的指令就可以启动
```
./bin/server -f -P cli
```
完成TDB的启动后，就可以在TDB的交互窗口运行SQL了。

目前TDB支持基本的增（create, insert）,删（delete）,改（update）,查（select）SQL语句，其中Select语句支持单表的基本查询，条件过滤语句(where，having)，聚合计算语句(聚合函数max())以及排序语句(order by)。

以下是部分SQL示例：
```SQL
CREATE TABLE TDB(id int, num int, price float, name char(4), day date);

INSERT INTO TDB VALUES (30, 1, 11.0, 'M', '2025-06-19');
INSERT INTO TDB VALUES (50, 1, 16.0, '5I', '2017-09-16');
INSERT INTO TDB VALUES (70, 1, 1.0, 'O', '1993-01-21');
INSERT INTO TDB VALUES (80, 1, 1.0, 'GQY0', '2007-11-04');
INSERT INTO TDB VALUES (90, 1, 16.0, 'GU', '2022-02-08');  

SELECT * FROM TDB;
SELECT * FROM TDB ORDER BY id desc;
SELECT * FROM TDB ORDER BY price;

SELECT avg(price) FROM TDB; 
SELECT avg(price) FROM TDB WHERE id>70; 
SELECT avg(price) FROM TDB WHERE day>'1993-01-21';


SELECT count(*) FROM TDB;
SELECT min(num) FROM TDB;
SELECT max(price) FROM TDB;

SELECT min(num),max(num),avg(num) FROM TDB;

SHOW TABLES;
DROP TABLE TDB;
SHOW TABLES;

bye
```

## 致谢
该项目的session模块参考了OceanBase的net模块实现，查询引擎则基于IoTDB的架构进行简化。
感谢这些项目开源他们的代码。
 
