# DolphinDB C++ API

DolphinDB C++ API支持以下开发环境：

* Linux
* Windows Visual Studio
* Windows GNU(MinGW)

本教程介绍以下内容：
- [1. 编译libDolphinDBAPI](#1-编译libdolphindbapi)
- [2. 项目编译](#2-项目编译)
- [3. 建立DolphinDB连接](#3-建立dolphindb连接)
- [4. 运行DolphinDB脚本](#4-运行dolphindb脚本)
- [5. 运行DolphinDB函数](#5-运行dolphindb函数)
- [6. 上传数据对象](#6-上传数据对象)
- [7. 读取数据示例](#7-读取数据示例)
- [8. 保存数据到DolphinDB数据表](#8-保存数据到dolphindb数据表)
- [9. C++ Streaming API](#9-c-streaming-api)
- [10. openssl 1.0.2版本源码安装](#10-openssl-102版本源码安装)



## 1. 编译libDolphinDBAPI

用户可以使用github或者gitee已经编译好的libDolphinDBAPI, 也可以通过如下方法自己编译libDolphinDBAPI。 

### 1.1 在Linux环境下编译API

#### 编译libuuid

DolphinDB API会调用到libuuid,所以要先编译libuuid的静态库。编译方法如下:

* 下载 [libuuid-1.0.3.tar.gz](https://sourceforge.net/projects/libuuid/files/)

* 解压：tar -xvf libuuid-1.0.3.tar.gz

* cd libuuid-1.0.3 && ./configure

* 修改makefile： 添加 '-fPIC' 到CFLAGS和CPPFLAGS

* 如果编译成功， libuuid.a 会生成在目录 '.libs'下

* 将libuuid.a拷贝到目录DolphinDBAPI

#### 编译libDolphinDBAPI

编译命令：

``` 
cd api-cplusplus
make clean & make -j4
```

如果编译成功，会自动生成libDolphinDBAPI.so 


### 1.2 在Windows环境下用MinGW编译API

编译命令：

```
cd api-cplusplus
mingw32-make -f makefile.win32
```


### 1.3 在Windows环境下，用Visual Studio 2017编译API



#### 创建项目 libDolphinDBAPI

Windows Desktop->Dynamic Link Library (DLL) 

#### 配置属性

配置属性 -> 常规 -> 项目默认值 -> 配置类型 -> 动态库(.dll)

#### 下载并将 [Openssl](https://www.npcglib.org/~stathis/blog/precompiled-openssl/)加入include和lib路径：


1. 配置属性页->VC++ 目录 -> 包含目录 ->C:\openssl-1.0.2l-vs2017\include64;
2. 配置属性页->VC++ 目录 -> 库目录 -> C:\openssl-1.0.2l-vs2017\lib64;

#### 添加下面的宏定义

C/C++ -> 预处理器 -> 预处理器定义 ->WIN32_LEAN_AND_MEAN; _WINSOCK_DEPRECATED_NO_WARNINGS;_CRT_SECURE_NO_WARNINGS;WINDOWS;NOMINMAX;NDEBUG;CPPAPI_EXPORTS;_WINDOWS;_USRDLL;



#### 预编译选项（Precompiled header）
C/C++ -> 预编译头 -> 预编译头-> 不使用预编译头


#### 链接（Linker）: 

连接器 -> 输入 -> 附加依赖项
ws2_32.lib
ssleay32MD.lib
libeay32MD.lib

#### 添加源码：

移除项目中代码，并添加src目录的源码到项目中。

#### 编译

编译的时候选择release和x64.如果编译成功，在/username/source/repos/libDolphinDBPAI/x64/Release目录下会生成：libDolphinDBAPI.lib和libDolphinDBAPI.dll


## 2. 项目编译

### 2.1 在Linux环境下编译项目

#### 2.1.1 环境配置

C++ API需要使用g++ 4.8.5及以上版本。

#### 2.1.2 下载bin文件和头文件

从本GitHub项目中下载以下文件：

- [bin](./bin) (libDolphinDBAPI.so)
- [include](./include) (DolphinDB.h, Exceptions.h, SmartPointer.h, SysIO.h, Types.h, Util.h)

#### 2.1.3 编译main.cpp

在bin和include的同级目录中创建project目录。进入project目录，并创建文件main.cpp：

```C++
#include "DolphinDB.h"
#include "Util.h"
#include <iostream>
#include <string>
using namespace dolphindb; 
using namespace std; 

int main(int argc, char *argv[]){

    DBConnection conn;
    bool ret = conn.connect("111.222.3.44", 8503);
    if(!ret){
        cout<<"Failed to connect to the server"<<endl;
        return 0;
    }
    ConstantSP vector = conn.run("`IBM`GOOG`YHOO");
    int size = vector->rows();
    for(int i=0; i<size; ++i)
        cout<<vector->getString(i)<<endl;
    return 0;
}
``` 

#### 2.1.4 编译

为了兼容旧的编译器，libDolphinDBAPI.so提供了2个版本，一个版本在编译时使用了-D_GLIBCXX_USE_CXX11_ABI=0的选项，放在[bin/linux_x64/ABI0](./bin/linux_x64/ABI0)目录下，另一个版本未使用-D_GLIBCXX_USE_CXX11_ABI=0，放在[bin/linux_x64/ABI1](./bin/linux_x64/ABI1)目录下。

另外由于DolphinDB添加了(Linux64 稳定版>=1.10.17,最新版>=1.20.6) SSL的支持， 所以编译前需要安装openssl。

>注：当前需要openssl版本为1.0.2，高版本的1.1版本会报错。如果系统自带的openssl版本不是1.0.2的话，可以参考[openssl源码安装](#9-openssl-1.0.2版本源码安装)，或者使用已有的二进制包安装。

以下是使用第一个动态库版本的g++编译命令：
```
g++ main.cpp -std=c++11 -DLINUX -D_GLIBCXX_USE_CXX11_ABI=0 -DLOGGING_LEVEL_2 -O2 -I../include   -lDolphinDBAPI -lpthread -lssl -L../bin/linux_x64/ABI0  -Wl,-rpath,.:../bin/linux_x64/ABI0 -o main
```

以下是使用另一个动态库版本的g++编译命令：
```
g++ main.cpp -std=c++11 -DLINUX -D_GLIBCXX_USE_CXX11_ABI=1 -DLOGGING_LEVEL_2 -O2 -I../include   -lDolphinDBAPI -lpthread -lssl -L../bin/linux_x64/ABI1  -Wl,-rpath,.:../bin/linux_x64/ABI1 -o main
```

#### 2.1.5 运行

编译成功后，启动DolphinDB，运行main程序并连接到DolphinDB，连接时需要指定IP地址和端口号，如以上程序中的111.222.3.44:8503。

### 2.2 Windows环境下编译

#### 2.2.1 环境配置

本教程使用了Visual Studio 2017 64位版本。

#### 2.2.2 下载bin文件和头文件

将本GitHub项目下载到本地。

#### 2.2.3 创建Visual Studio项目

创建windows console project，导入[include](./include)目录下头文件，创建1.1.3节中的main.cpp文件，导入libDolphinDBAPI.lib，并且配置lib目录。

请注意：
> 由于VS里默认定义了min/max两个宏，会与头文件中 `min` 和 `max` 函数冲突。为了解决这个问题，在预处理宏定义中需要加入 __NOMINMAX__。
> API源代码中用宏定义LINUX、WINDOWS等区分不同平台，因此在预处理宏定义中需要加入 WINDOWS。

#### 2.2.4 编译和运行

启动编译，将对应的libDolphinDBAPI.dll拷贝到可执行程序的输出目录，即可运行。

Windows gnu开发环境与Linux相似，可以参考上一章的Linux编译。

## 3. 建立DolphinDB连接

DolphinDB C++ API 提供的最核心的对象是DBConnection。C++应用可以通过它在DolphinDB服务器上执行脚本和函数，并在两者之间双向传递数据。DBConnection类提供如下主要方法：

| 方法名        | 详情          |
|:------------- |:-------------|
|connect(host, port, [username, password])|将会话连接到DolphinDB服务器|
|login(username,password,enableEncryption)|登陆服务器|
|run(script)|将脚本在DolphinDB服务器运行|
|run(functionName,args)|调用DolphinDB服务器上的函数|
|upload(variableObjectMap)|将本地数据对象上传到DolphinDB服务器|
|initialize()|初始化连接信息|
|close()|关闭当前会话|

C++ API通过TCP/IP协议连接到DolphinDB。使用 `connect` 方法创建连接时，需要提供DolphinDB server的IP和端口。

```C++
DBConnection conn;
bool ret = conn.connect("127.0.0.1", 8848);
```

声明connection变量的时候，有两个可选参数：enableSSL（支持SSL），asynTask（支持一部分）。这两个参数默认值为false。 目前只支持linux, 稳定版>=1.10.17,最新版>=1.20.6。  

下面例子是，建立支持SSL而非支持异步的connection，同时服务器端应该添加参数enableHTTPS=true(单节点部署，需要添加到dolphindb.cfg;集群部署需要添加到cluster.cfg)。

```C++
DBConnection conn(true,false)
```

下面建立不支持SSL，但支持异步的connection。异步情况下，只能执行DolphinDB脚本和函数， 且不再有返回值。该功能适用于异步写入数据。

```C++
DBConnection conn(false,true)
```

创建连接时也可以使用用户名和密码登录，默认的管理员名称为"admin"，密码是"123456"。

```C++
DBConnection conn; 
bool ret = conn.connect("127.0.0.1", 8848, "admin", "123456"); 
``` 

若未使用用户名及密码连接成功，则脚本在Guest权限下运行。后续运行中若需要提升权限，可以使用 conn.login('admin','123456',true) 登录获取权限。

请注意，DBConnection类的所有函数都不是线程安全的，不可以并行调用，否则可能会导致程序崩溃。

## 4. 运行DolphinDB脚本

通过 `run` 方法运行DolphinDB脚本：

```C++
ConstantSP v = conn.run("`IBM`GOOG`YHOO");
cout<<v->getString()<<endl;
```

输出结果为：

> ["IBM", "GOOG", "YHOO"]

## 5. 运行DolphinDB函数

除了运行脚本之外，run命令还可以直接在远程DolphinDB服务器上执行DolphinDB内置或用户自定义函数。若 `run` 方法只有一个参数，则该参数为脚本；若 `run` 方法有两个参数，则第一个参数为DolphinDB中的函数名，第二个参数是该函数的参数，为ConstantSP类型的向量。

下面的示例展示C++程序通过 `run` 调用DolphinDB内置的 [`add`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/a/add.html) 函数。[`add`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/a/add.html) 函数有两个参数 x 和 y。参数的存储位置不同，也会导致调用方式的不同。可能有以下三种情况：

* 所有参数都在DolphinDB server端

若变量 x 和 y 已经通过C++程序在服务器端生成，

```C++
conn.run("x = [1, 3, 5]; y = [2, 4, 6]"); 
``` 

那么在C++端要对这两个向量做加法运算，只需直接使用 `run` 即可。

```C++
ConstantSP result = conn.run("add(x,y)");
cout<<result->getString()<<endl;
```

输出结果为：

> [3, 7, 11]

* 仅有一个参数在DolphinDB server端存在

若变量 x 已经通过C++程序在服务器端生成，

```C++
conn.run("x = [1, 3, 5]"); 
``` 

而参数 y 要在C++客户端生成，这时就需要使用“部分应用”方式，把参数 x 固化在 [`add`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/a/add.html) 函数内。具体请参考[部分应用文档](https://www.dolphindb.cn/cn/help/Functionalprogramming/PartialApplication.html)。

```C++
vector<ConstantSP> args;
ConstantSP y = Util::createVector(DT_DOUBLE, 3);
double array_y[] = {1.5, 2.5, 7};
y->setDouble(0, 3, array_y);
args.push_back(y);
ConstantSP result = conn.run("add{x,}", args);
cout<<result->getString()<<endl;
```

输出结果为：

> [2.5, 5.5, 12]

* 两个参数都待由C++客户端赋值

```C++
vector<ConstantSP> args; 
ConstantSP x = Util::createVector(DT_DOUBLE, 3); 
double array_x[] = {1.5, 2.5, 7}; 
x->setDouble(0, 3, array_x); 
ConstantSP y = Util::createVector(DT_DOUBLE, 3); 
double array_y[] = {8.5, 7.5, 3}; 
y->setDouble(0, 3, array_y); 
args.push_back(x); 
args.push_back(y); 
ConstantSP result = conn.run("add", args); 
cout<<result->getString()<<endl; 
``` 

输出结果为：
> [10, 10, 10]

## 6. 上传数据对象

C++ API提供 `upload` 方法，将本地对象上传到DolphinDB。

下面的例子在C++定义了一个 `createDemoTable` 函数，该函数创建了一个本地的表对象。

```C++
TableSP createDemoTable(){
    vector<string> colNames = {"name", "date","price"};
    vector<DATA_TYPE> colTypes = {DT_STRING, DT_DATE, DT_DOUBLE};
    int colNum = 3, rowNum = 10000, indexCapacity=10000;
    ConstantSP table = Util::createTable(colNames, colTypes, rowNum, indexCapacity);
    vector<VectorSP> columnVecs;
    for(int i = 0; i < colNum; ++i)
        columnVecs.push_back(table->getColumn(i));

    for(unsigned int i = 0; i < rowNum; ++i){
        columnVecs[0]->set(i, Util::createString("name_"+std::to_string(i)));
        columnVecs[1]->set(i, Util::createDate(2010, 1, i+1));
        columnVecs[2]->set(i, Util::createDouble((rand()%100)/3.0));
    }
    return table;
}
```

需要注意的是，上述例子中采用的 `set` 方法作为一个虚函数，会产生较大的开销，调用 `set` 方法对表的列向量逐个赋值，在数据量很大的情况下会导致效率低下。此外， `createString`, `createDate` 与 `createDouble` 等构造方法要求操作系统分配内存，反复调用同样会产生很大的开销。

相对合理的做法是定义一个相应类型的数组，通过诸如 setInt(INDEX start, int len, const int* buf) 的方式一次或者多次地将数据批量传给列向量。

当表对象的数据量较小时，可以采用上述例子中的方式生成 TableSP 对象的数据，但是当数据量较多时，建议采用如下方式来生成数据。

```C++
TableSP createDemoTable(){

    vector<string> colNames = {"name", "date", "price"};
    vector<DATA_TYPE> colTypes = {DT_STRING, DT_DATE, DT_DOUBLE};
    int colNum = 3, rowNum = 10000, indexCapacity=10000;
    ConstantSP table = Util::createTable(colNames, colTypes, rowNum, indexCapacity);
    vector<VectorSP> columnVecs;
    for(int i = 0; i < colNum; ++i)
        columnVecs.push_back(table->getColumn(i));

    int array_dt_buf[Util::BUF_SIZE]; //定义date列缓冲区数组
    double array_db_buf[Util::BUF_SIZE]; //定义price列缓冲区数组

    int start = 0;
    int no=0;
    while (start < rowNum) {
        size_t len = std::min(Util::BUF_SIZE, rowNum - start);
        int *dtp = columnVecs[1]->getIntBuffer(start, len, array_dt_buf); //dtp指向每次通过 `getIntBuffer` 得到的缓冲区的头部
        double *dbp = columnVecs[2]->getDoubleBuffer(start, len, array_db_buf); //dbp指向每次通过 `getDoubleBuffer` 得到的缓冲区的头部
        for (int i = 0; i < len; ++i) {
            columnVecs[0]->setString(i+start, "name_"+std::to_string(++no)); //对string类型的name列直接进行赋值，不采用getbuffer的方式
            dtp[i] = 17898+i;
            dbp[i] = (rand()%100)/3.0;
        }
        columnVecs[1]->setInt(start, len, dtp); //写完后使用 `setInt` 将缓冲区写回数组
        columnVecs[2]->setDouble(start, len, dbp); //写完后使用 `setDouble` 将缓冲区写回数组
        start += len;
    }
    return table;
}
``` 
上述例子采用的诸如 `getIntBuffer` 等方法能够直接获取一个可读写的缓冲区，写完后使用 `setInt` 将缓冲区写回数组，这类函数会检查给定的缓冲区地址和变量底层储存的地址是否一致，如果一致就不会发生数据拷贝。在多数情况下，用 `getIntBuffer` 获得的缓冲区就是变量实际的存储区域，这样能减少数据拷贝，提高性能。

以下利用自定义的 `createDemoTable` 函数创建表对象之后，通过 `upload` 方法把它上传到DolphinDB，再从DolphinDB获取这个表的数据，保存到本地对象result并打印。
```C++
TableSP table = createDemoTable();
conn.upload("myTable", table);
string script = "select * from myTable;";
ConstantSP result = conn.run(script);
cout<<result->getString()<<endl;
```

输出结果为：

``` console
name    date       price
------- ---------- ---------
name_1  2019.01.02 27.666667
name_2  2019.01.03 28.666667
name_3  2019.01.04 25.666667
name_4  2019.01.05 5
name_5  2019.01.06 31
...
```

## 7. 读取数据示例

DolphinDB C++ API 不仅支持Int, Float, String, Date, DataTime等多种数据类型，也支持向量(VectorSP)、集合(SetSP)、矩阵(MatrixSP)、字典(DictionarySP)、表(TableSP）等多种数据形式。下面介绍如何通过DBConnection对象，读取并操作DolphinDB的各种形式的对象。

首先加上必要的头文件:

```C++
#include "DolphinDB.h"
#include "Util.h"
``` 

### 7.1 向量

创建INT类型的向量：

```C++
VectorSP v = conn.run("1..10");
int size = v->size();
for(int i = 0; i < size; ++i)
    cout<<v->getInt(i)<<endl;
```

创建DATE类型的向量：

```C++
VectorSP v = conn.run("2010.10.01..2010.10.30"); 
int size = v->size(); 
for(int i = 0; i < size; ++i)
    cout<<v->getString(i)<<endl;
``` 

### 7.2 集合

创建一个集合：

```C++
SetSP set = conn.run("set(4 5 5 2 3 11 6)");
cout<<set->getString()<<endl;
```

### 7.3 矩阵

创建一个矩阵：

```C++
ConstantSP matrix = conn.run("1..6$2:3"); 
cout<<matrix->getString()<<endl; 
``` 

### 7.4 字典

创建一个字典：

```C++
DictionarySP dict = conn.run("dict(1 2 3, `IBM`MSFT`GOOG)");
cout << dict->get(Util::createInt(1))->getString()<<endl;
```

上例通过 `Util::createInt` 创建Int类型的值，并使用 `get` 方法来获得key为1对应的值。

### 7.5 表

在C++客户端中执行以下脚本创建一个表：

```C++
string sb; 
sb.append("n=200\n"); 
sb.append("syms= `IBM`C`MS`MSFT`JPM`ORCL`BIDU`SOHU`GE`EBAY`GOOG`FORD`GS`PEP`USO`GLD`GDX`EEM`FXI`SLV`SINA`BAC`AAPL`PALL`YHOO`KOH`TSLA`CS`CISO`SUN\n"); 
sb.append("mytrades=table(09:30:00+rand(18000, n) as timestamp, rand(syms, n) as sym, 10*(1+rand(100, n)) as qty, 5.0+rand(100.0, n) as price); \n"); 
sb.append("select * from mytrades"); 
TableSP table = conn.run(sb); 
```

#### 7.5.1 `getString()`方法获取表的内容

```C++
cout<<table->getString()<<endl; 
``` 

#### 7.5.2 `getColumn()`方法按列获取表的内容

下面的脚本中，首先定义一个VectorSP类型的动态数组columnVecs，用于存放从表中获取的列，然后依次访问columnVecs处理数据。

对于表的各列，我们可以通过`getString()`方法获得每一列的字符串类型数组，再通过C++的数据类型转换函数将数值类型的数据转换成对应的数据类型，从而进行计算。对于时间类型的数据，则需要以字符串的形式存储。

```C++
vector<VectorSP> columnVecs;
int qty[200],sum[200];
double price[200];
for(int i=0; i<200;++i){
    qty[i]=atoi(columnVecs[2]->getString(i).c_str());
    price[i]=atof(columnVecs[3]->getString(i).c_str());
    sum[i]=qty[i]*price[i];
}

for(int i = 0; i < 200; ++i){
    cout<<columnVecs[0]->getString(i)<<", "<<columnVecs[1]->getString(i)<<", "<<sum[i]<<endl;
}
``` 

#### 7.5.3 `getRow()`方法按照行获取表的内容

例如，打印table的第一行，返回的结果是一个字典。

```C++
cout<<table->getRow(0)->getString()<<endl; 

// output
price->37.811678
qty->410
sym->IBM
timestamp->13:45:15
``` 

如果取某一行中的某一列数据可以通过先调用`getRow`，再调用`getMember`的方法，如下例所示。其中，`getMember()`函数的参数不是C++内置的string类型对象，而是DolphinDB C++ API的string类型Constant对象。

```C++
cout<<table->getRow(0)->getMember(Util::createString("price"))->getDouble()<<endl;

// output
37.811678
```

需要注意的是，按行访问table并逐一进行计算非常低效。为了达到更好的性能，建议参考[6.5.2小节](#652-getcolumn方法按列获取表的内容)的方式按列访问table并批量计算。

#### 7.5.4 使用`BlockReaderSP`对象分段读取表数据

对于大数据量的表，API提供了分段读取方法。(此方法仅适用于DolphinDB 1.20.5, 1.10.16及其以上版本)

在C++客户端中执行以下脚本创建一个大数据量的表：
```C++
string script; 
script.append("n=20000\n"); 
script.append("syms= `IBM`C`MS`MSFT`JPM`ORCL`BIDU`SOHU`GE`EBAY`GOOG`FORD`GS`PEP`USO`GLD`GDX`EEM`FXI`SLV`SINA`BAC`AAPL`PALL`YHOO`KOH`TSLA`CS`CISO`SUN\n"); 
script.append("mytrades=table(09:30:00+rand(18000, n) as timestamp, rand(syms, n) as sym, 10*(1+rand(100, n)) as qty, 5.0+rand(100.0, n) as price); \n"); 
conn.run(script); 
```

分段读取数据并用getString()方法获取表的内容, 需要注意的是fetchSize必须不小于8192。
```C++
string sb = "select * from mytrades";
int fetchSize = 8192;
BlockReaderSP reader = conn.run(sb,4,2,fetchSize);//priority=4, parallelism=2
ConstantSP table;
int total = 0;
while(reader->hasNext()){
    table=reader->read();
    total += table->size();
    cout<< "read" <<table->size()<<endl;
    cout<<table->getString()<<endl; 
}
```

### 7.6 AnyVector

AnyVector是DolphinDB中一种特殊的数据形式，与常规的向量不同，它的每个元素可以是不同的数据类型或数据形式。

```C++
ConstantSP result = conn.run("[1, 2, [1,3,5], [0.9, 0.8]]");
cout<<result->getString()<<endl;
```

使用 `get` 方法获取第三个元素：

```C++
VectorSP v = result->get(2); 
cout<<v->getString()<<endl; 
``` 

结果是一个Int类型的向量[1,3,5]。

## 8. 保存数据到DolphinDB数据表

DolphinDB数据表按存储方式分为三种:

* 内存表: 数据仅保存在内存中，存取速度最快，但是节点关闭后数据就不存在了。
* 分布式表（DFS表）：数据可保存在不同的节点，亦可保存在同一节点，由分布式文件系统统一管理。路径以"dfs://"开头。
* 本地磁盘表：数据仅保存在本地磁盘。不建议生产环境中使用。

### 8.1 保存数据到DolphinDB内存表

DolphinDB提供多种方式来保存数据到内存表：

* 通过[insert into](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/i/insertInto.html)语句保存单条数据
* 通过[tableInsert](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/t/tableInsert.html)函数批量保存多条数据
* 通过[tableInsert](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/t/tableInsert.html)函数保存数据表

下面分别介绍三种方式保存数据的实例，在例子中使用到的数据表有3列，分别是STRING, DATE, DOUBLE类型，列名分别为name, date和price。
在DolphinDB中执行以下脚本创建内存表：

```
t = table(100:0, `name` date`price, [STRING, DATE, DOUBLE]); 
share t as tglobal; 
``` 

上面的例子中，我们通过[`table`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/t/table.html)函数来创建表，指定了表的容量和初始大小、列名和数据类型。由于内存表是会话隔离的，所以普通内存表只有当前会话可见。为了让多个客户端可以同时访问t，我们使用[`share`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/CommandsReferences/share.html)在会话间共享内存表。

#### 8.1.1 使用insert into语句保存数据

可以采用如下方式保存单条数据。

```C++
char script[100];
sprintf(script, "insert into tglobal values(%s, date(timestamp(%ld)), %lf)", "`a", 1546300800000, 1.5);
conn.run(script);
```

也可以使用[insert into](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/i/insertInto.html) 语句保存多条数据:

```C++
string script; 
int rowNum=10000, indexCapacity=10000; 
VectorSP names = Util::createVector(DT_STRING, rowNum, indexCapacity); 
VectorSP dates = Util::createVector(DT_DATE, rowNum, indexCapacity); 
VectorSP prices = Util::createVector(DT_DOUBLE, rowNum, indexCapacity); 

int array_dt_buf[Util:: BUF_SIZE]; //定义date列缓冲区数组
double array_db_buf[Util:: BUF_SIZE]; //定义price列缓冲区数组

int start = 0; 
int no=0; 
while (start < rowNum) {

    size_t len = std::min(Util::BUF_SIZE, rowNum - start);
    int *dtp = dates->getIntBuffer(start, len, array_dt_buf); //dtp指向每次通过 `getIntBuffer` 得到的缓冲区的头部
    double *dbp = prices->getDoubleBuffer(start, len, array_db_buf); //dbp指向每次通过 `getDoubleBuffer` 得到的缓冲区的头部
    for (int i = 0; i < len; i++) {
        names->setString(i+start, "name_"+std::to_string(++no)); //对string类型的name列直接进行赋值，不采用getbuffer的方式
        dtp[i] = 17898+i;
        dbp[i] = (rand()%100)/3.0;
    }
    dates->setInt(start, len, dtp); //写完后使用 `setInt` 将缓冲区写回数组
    prices->setDouble(start, len, dbp); //写完后使用 `setDouble` 将缓冲区写回数组
    start += len;

}
vector<string> allnames = {"names", "dates", "prices"}; 
vector<ConstantSP> allcols = {names, dates, prices}; 
conn.upload(allnames, allcols); 

script += "insert into tglobal values(names, dates, prices); tglobal"; 
TableSP table = conn.run(script); 
``` 

#### 8.1.2 使用tableInsert函数批量保存多条数据

在这个例子中，我们利用索引指定TableSP对象的多行数据，将它们批量保存到DolphinDB server上。

```C++
vector<ConstantSP> args;
TableSP table = createDemoTable();
VectorSP range = Util::createPair(DT_INDEX);
range->setIndex(0, 0);
range->setIndex(1, 10);
cout<<range->getString()<<endl;
args.push_back(table->get(range));
conn.run("tableInsert{tglobal}", args);
```

#### 8.1.3 使用tableInsert函数保存TableSP对象

```C++
vector<ConstantSP> args; 
TableSP table = createDemoTable(); 
args.push_back(table); 
conn.run("tableInsert{tglobal}", args); 
``` 

把数据保存到内存表，还可以使用[append!](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/a/append!.html)函数，它可以把一张表追加到另一张表。但是，一般不建议通过[append!](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/a/append!.html)函数保存数据，因为[append!](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/a/append!.html)函数会返回一个空表，不必要地增加通信量。

```C++
vector<ConstantSP> args;
TableSP table = createDemoTable();
args.push_back(table);
conn.run("append!(tglobal);", args);
```

### 8.2 保存数据到分布式表

分布式表是DolphinDB推荐在生产环境下使用的数据存储方式，它支持快照级别的事务隔离，保证数据一致性。分布式表支持多副本机制，既提供了数据容错能力，又能作为数据访问的负载均衡。下面的例子通过C++ API把数据保存至分布式表。

#### 8.2.1 使用tableInsert函数保存TableSP对象

在DolphinDB中使用以下脚本创建分布式表。[`database`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/d/database.html)函数用于创建数据库。分布式数据库地路径必须以"dfs://"
开头。[`createPartitionedTable`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/c/createPartitionedTable.html)函数用于创建分区表。
``` 
login( `admin, ` 123456)
dbPath = "dfs://SAMPLE_TRDDB";
tableName = `demoTable
db = database(dbPath, VALUE, 2010.01.01..2010.01.30)
pt=db.createPartitionedTable(table(1000000:0, `name` date `price, [STRING,DATE,DOUBLE]), tableName, ` date)
```

使用[`loadTable`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/l/loadTable.html)方法加载分布式表，通过[`tableInsert`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/t/tableInsert.html)方式追加数据：

```C++
TableSP table = createDemoTable(); 
vector<ConstantSP> args; 
args.push_back(table); 
conn.run("tableInsert{loadTable('dfs://SAMPLE_TRDDB', `demoTable)}", args); 
``` 

[`append!`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/a/append!.html)函数也能向分布式表追加数据，但是性能与[`tableInsert`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/t/tableInsert.html)相比要差，建议不要轻易使用：

```C++
TableSP table = createDemoTable();
conn.upload("mt", table);
conn.run("loadTable('dfs://SAMPLE_TRDDB', `demoTable).append!(mt);");
conn.run(script);
```

#### 8.2.2 分布式表的并发写入

DolphinDB的分布式表支持并发读写，下面展示如何在C++客户端中将数据并发写入DolphinDB的分布式表。

首先，在DolphinDB服务端执行以下脚本，创建分布式数据库"dfs://natlog"和分布式表"natlogrecords"。其中，数据库按照VALUE-HASH-HASH的组合进行三级分区。

```
dbName="dfs://natlog"
tableName="natlogrecords"
db1 = database("", VALUE, datehour(2019.09.11T00:00:00)..datehour(2019.12.30T00:00:00) )//starttime,  newValuePartitionPolicy=add
db2 = database("", HASH, [IPADDR, 50]) //source_address 
db3 = database("", HASH,  [IPADDR, 50]) //destination_address
db = database(dbName, COMPO, [db1,db2,db3])
data = table(1:0, ["fwname","filename","source_address","source_port","destination_address","destination_port","nat_source_address","nat_source_port","starttime","stoptime","elapsed_time"], [SYMBOL,STRING,IPADDR,INT,IPADDR,INT,IPADDR,INT,DATETIME,DATETIME,INT])
db.createPartitionedTable(data,tableName,`starttime`source_address`destination_address)
```

DolphinDB不允许多个writer同时将数据写入到同一个分区，因此在客户端多线程并行写入数据时，需要确保每个线程分别写入不同的分区。

对于按哈希值进行分区的分布式表， DolphinDB C++ API 提供了`getHash`函数来数据的hash值。在客户端设计多线程并发写入分布式表时，可根据哈希分区字段数据的哈希值分组，每组指定一个写线程。这样就能保证每个线程同时将数据写到不同的哈希分区。

```C++
ConstantSP spIP = Util::createConstant(DT_IP);
int key = spIP->getHash(BUCKETS);
```

开启生产数据和消费数据的线程，下面的`genData`用于生成模拟数据，`writeData`用于写数据。

```C++
for (int i = 0; i < tLong; ++i) {
    arg[i].index = i;
    arg[i].count = tLong;
    arg[i].nLong = nLong;
    arg[i].cLong = cLong;
    arg[i].nTime = 0;
    arg[i].nStarttime = sLong;
    genThreads[i] = std::thread(genData, (void *)&arg[i]);
    writeThreads[i] = std::thread(writeData, (void *)&arg[i]);
}
```

每个生产线程首先生成数据，其中`createDemoTable`函数用于产生模拟数据，并返回一个TableSP对象。

```C++
void *genData(void *arg) {
  struct parameter *pParam;
  pParam = (struct parameter *)arg;
  long partitionCount = BUCKETS / pParam->count;

  for (unsigned int i = 0; i < pParam->nLong; i++) {
    TableSP table =
        createDemoTable(pParam->cLong, partitionCount * pParam->index, partitionCount, pParam->nStarttime, i * 5);
    tableQueue[pParam->index]->push(table);
  }
  return NULL;
}
```

每个消费线程开始向DolphinDB并行写入数据。

```C++
void *writeData(void *arg) {
    struct parameter *pParam;
    pParam = (struct parameter *)arg;

    TableSP table;
    for (unsigned int i = 0; i < pParam->nLong; i++) {
        tableQueue[pParam->index]->pop(table);
        long long startTime = Util::getEpochTime();
        vector<ConstantSP> args;
        args.push_back(table);
        conn[pParam->index].run("tableInsert{loadTable('dfs://natlog', `natlogrecords)}", args);
        pParam->nTime += Util::getEpochTime() - startTime;
    }
    printf("Thread %d,insert %ld rows %ld times, used %ld ms.\n", pParam->index, pParam->cLong, pParam->nLong, pParam->nTime);
    return NULL;
}
```
更多分布式表的并发写入案例可以参考样例[MultiThreadDFSWriting.cpp](./example/DFSWritingWithMultiThread/MultiThreadDfsWriting.cpp)。

#### 8.2.3 利用PartitionedTableAppender并发写入分布式表

上述方法较为复杂，C++ API提供了更简便地自动按分区分流数据并行写入的方法:

```C++
PartitionedTableAppender(string dbUrl, string tableName, string partitionColName, DBConnectionPool& pool);
```

- dbUrl: 分布式数据库地址，若为内存表可设为“”
- tableName: 分布式表名
- partitionColName: 分区字段
- DBConnectionPool: 连接池

使用最新的1.30版本及以上的server，可以使用C++ API中的 PartitionedTableAppender对象来写入分布式表。其基本原理是设计一个连接池，然后获取分布式表的分区信息，将分区分配给连接池来并行写入，一个分区在同一时间只能由一个连接写入。

先在服务器端创建一个数据库 "dfs://SAMPLE_TRDDB" 以及一个分布式表 "demoTable"：

```
login( `admin, `123456)
dbPath = "dfs://SAMPLE_TRDDB";
tableName = `demoTable
if(existsDatabase(dbPath)){
	dropDatabase(dbPath)
}
db = database(dbPath, VALUE, 2010.01.01..2010.01.30)
pt=db.createPartitionedTable(table(1000000:0, `name`date `price, [STRING,DATE,DOUBLE]), tableName, `date)
```

然后在C++客户端创建连接池pool并传入PartitionedTableAppender，使用append方法往分布式表并发写入本地数据:

```C++
DBConnectionPool pool("localhost", 8848, 20, "admin", "123456");
PartitionedTableAppender appender("dfs://SAMPLE_TRDDB", "demoTable", "date", pool);
TableSP table = createDemoTable();
appender.append(table);
ConstantSP result = conn.run("select * from loadTable('dfs://SAMPLE_TRDDB', `demoTable)");
cout <<  result->getString() << endl;
```


### 8.3 保存数据到本地磁盘表

本地磁盘表通用用于静态数据集的计算分析。它不支持事务，也不持支并发读写。

在DolphinDB中使用以下脚本创建一个本地磁盘表，使用[`database`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/d/database.html)函数创建数据库，调用[`saveTable`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/CommandsReferences/saveTable.html)命令将内存表保存到磁盘中：


``` 
t = table(100:0, `name` date`price, [STRING,DATE,DOUBLE]);
db=database("/home/dolphindb/demoDB");
saveTable(db, t, `dt);
share t as tDiskGlobal;
```

使用[`tableInsert`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/t/tableInsert.html)函数是向本地磁盘表追加数据最为常用的方式。这个例子中，我们使用[`tableInsert`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/t/tableInsert.html)向共享的内存表tDiskGlobal中插入数据，接着调用[`saveTable`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/CommandsReferences/saveTable.html)把插入的数据保存到磁盘上。

```C++
TableSP table = createDemoTable(); 
vector<ConstantSP> args; 
args.push_back(table); 
conn.run("tableInsert{tDiskGlobal}", args); 
conn.run("saveTable(db, tDiskGlobal, `dt); "); 
``` 

本地磁盘表支持使用[`append!`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/a/append!.html)函数把数据追加到表中：

```C++
TableSP table = createDemoTable();
conn.upload("mt", table);
string script;
script += "db=database(\"/home/demoTable1\");";
script += "tDiskGlobal.append!(mt);";
script += "saveTable(db,tDiskGlobal,`dt);";
conn.run(script);
```

注意：

1. 对于本地磁盘表，[`append!`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/a/append!.html)函数只把数据追加到内存，如果要保存到磁盘上，必须再次执行[`saveTable`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/CommandsReferences/saveTable.html)函数。
2. 除了使用[`share`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/CommandsReferences/share.html)让表在其他会话中可见，也可以在C++ API中使用[`loadTable`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/l/loadTable.html)来加载磁盘表，使用[`append!`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/a/append!.html)来追加数据。但是，我们不推荐这种方法，因为[`loadTable`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/l/loadTable.html)函数从磁盘加载数据，会消耗大量时间。如果有多个客户端都使用[`loadTable`](https://www.dolphindb.cn/cn/help/FunctionsandCommands/FunctionReferences/l/loadTable.html) ，内存中会有多个表的副本，造成数据不一致。

关于C++ API的更多信息，可以参考C++ API 头文件[dolphindb.h](./include/DolphinDB.h)。

### 8.4 批量异步写入数据

在高频的，实时单行插入数据的场景下，使用API的批量异步写入可以有效提升服务器的I/O效率，同时提升客户端写入的吞吐量。DolphinDB C++ API提供batchTableWriter对象，在客户端设置一个数据缓冲队列。当服务器端忙于网络I/O时，客户端写线程仍然可以将数据持续写入缓冲队列（该队列由客户端维护）。写入队列后即可返回，避免了写线程的忙等。同时，batchTableWriter对象在客户端打开一个消费线程负责消费缓冲队列中的数据，批量打包传输给服务器端，从而提升服务器的I/O效率。目前，只支持批量写入数据到磁盘表和内存表。异步方式提交有如下几个特点：

- API客户端提交任务到缓冲队列，缓冲队列接到任务后，客户端即认为任务已完成。
- 提供getStatus等接口查看状态。

**请注意：**

* 消费线程消费缓冲队列中的数据具有即时性，即不存在队列占满后消费或定时消费。
* 批量异步写入不保证数据最终成功写入数据库。如果后台线程写入过程中出现错误或者服务端异常，后台线程将退出，并在下一次写入数据时清空队列，抛出异常。但其报错不具备实时性。可以通过getUnwrittenData获取抛出异常的insert之前所有写入缓冲队列但是没有成功写入服务器的数据，返回形式是TableSP.

batchTableWriter对象及主要方法介绍如下：

```C++
BatchTableWriter(const std::string& hostName, int port, const std::string& userId, const std::string& password, bool acquireLock=true)
```
* hostName 连接服务器的IP地址。
* port 连接服务器的端口号。
* userId 是字符串，表示连接服务器的用户名。
* password 是字符串，表示连接服务器的密码。
* acquireLock 是布尔值，表示在使用过程中，API内部是否需要加锁。默认为true, 表示需要加锁。在并发调用API的场景下，建议加锁。

以下是BatchTableWriter对象包含的函数方法介绍：
```C++
addTable(const string& dbName, const string& tableName="", bool partitioned=true);
```
- dbName: 磁盘表时，需填写数据库名。内存表时填写表名。
- tableName: 需要写入的磁盘表的表名。内存表时该值为空。
- partitioned: 表示添加的表是否为分区表。设置为true表示是分区表。如果添加的表是磁盘未分区表，必需设置partitioned为false.

**请注意:**

* 如果添加的是内存表，需要share该表。
* 表名不可重复添加，需要先移除之前添加的表，否则会抛出异常。

```C++
insert(const string& dbName, const string& tableName, Fargs)
```

- Fargs：是变长参数，代表插入的一行数据。写入的数据可以使用dolphindb的数据类型，也可以使用C++原生数据类型。数据类型和表中列的类型需要一一对应。数据类型对应关系见下表：

C++ 原生数据类型与DolphinDB数据类型对应关系表

| DolphinDB类型 | C++类型         |
| ------------- | --------------- |
| BOOL          | char            |
| CHAR          | char            |
| SHORT         | short           |
| STRING        | const char*     |
| STRING        | string          |
| SYMBOL        | const char*     |
| SYMBOL        | string          |
| LONG          | long long       |
| NANOTIME      | long long       |
| NANOTIMESTAMP | long long       |
| TIMESTAMP     | long long       |
| FLOAT         | float           |
| DOUBLE        | double          |
| DATE          | int             |
| MONTH         | int             |
| TIME          | int             |
| MINUTE        | int             |
| DATETIME      | int             |
| DATEHOUR      | int             |
| UUID          | unsigned char*  |
| UUID          | unsigned char[] |
| IPADDR        | unsigned char*  |
| IPADDR        | unsigned char[] |
| INT128        | unsigned char*  |
| INT128        | unsigned char[] |

**请注意:**

* 调用insert前需先调用addTable添加表，否则会抛出异常。
* 变长参数个数和数据类型需要与insert表的列数及类型匹配。
* 如果插入过程出现异常导致后台线程退出，再次调用insert会抛出异常，可以调用getUnwrittenData来获取之前所有写入缓冲队列但是没有成功写入服务器的数据（不包括本次insert的数据），然后再removeTable。如果需要再次插入数据，需要重新调用 `addTable`.
* 在移除该表的过程中调用本函数，仍然能够插入成功，但这些插入的数据并不会发送到服务器。移除该表的时候调用insert算是未定义行为，不建议这样写程序。

```C++
removeTable(const string& dbName, const string& tableName="")
```

释放由addTable添加的表所占用的资源。第一次调用该函数，该函数返回即表示后台线程已退出。

```C++
getUnwrittenData(const string& dbName, const string& tableName="")
```
获取还未写入的数据，主要是用于的时候获取写入出现错误时，剩下未写入的数据。该函数会取出剩下未写入的数据，这些数据将不会被继续写入，如若需要重新写入，需要再次调用插入函数。

```C++
getStatus(const string& dbName, const string& tableName="")
```

返回值是由一个整型和两个布尔型组合的元组，分别表示当前写入队列的深度、当前表是否被移除（true: 表示正在被移除），以及后台写入线程是否因为出错而退出。

```C++
getAllStatus()
```

获取所有当前存在的表的信息，不包含被移除的表。

返回值是一个表，共有六列，对应列的说明如下：

| 列名        | 详情          |
|:------------- |:-------------|
|DatabaseName|数据库名称/内存表名称|
|TableName|表名称/空字符串|
|WriteQueueDepth|当前写入队列深度|
|SendedRows|已成功发送到服务器的行数|
|Removing|表是否正在被移除|
|Finished|后台线程是否因为出错退出|

示例：

```C++
#include "BatchTableWriter.h"
using namespace dolphindb;
using namespace std;
int main(){
  shared_ptr<BatchTableWriter> btw = make_shared<BatchTableWriter>(host, port, userId, password, true);
  btw->addTable("dfs://demoDB", "demoTable");
  for(int i = 0; i < 1000; i+=3)
    btw->insert("dfs://demoDB", "demoTable", i,i+1,i+2);
  btw->removeTable("dfs://demoDB", "demoTable");
}
```

更多批量异步写入案例，请参考[BatchTableWriterDemo.cpp](./example/BatchTableWriter/BatchTableWriterDemo.cpp)。

## 9. C++ Streaming API

C++ API处理流数据的方式有三种：ThreadedClient, ThreadPooledClient 和 PollingClient。这三种实现方式的细节请见[test/StreamingThreadedClientTester.cpp](./test/StreamingThreadedClientTester.cpp), [test/StreamingThreadPooledClientTester.cpp](./test/StreamingThreadPooledClientTester.cpp) 和 [test/StreamingPollingClientTester.cpp](./test/StreamingPollingClientTester.cpp)。

### 9.1 编译

#### 9.1.1 Linux 64位

安装cmake：

``` bash
sudo apt-get install cmake
```

编译：

``` bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ../path_to_api-cplusplus/
make -j `nproc` 
```

编译成功后，会生成三个可执行文件。

#### 9.1.2 在Windows中使用MinGW编译

安装[MinGW](http://www.mingw.org/)和[cmake](https://cmake.org/):

``` bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release `path_to_api-cplusplus` -G "MinGW Makefiles"
mingw32-make -j `nproc` 
```

编译成功后，会生成三个可执行文件。

注意：
- 1. 编译前，需要把libDolphinDBAPI.dll复制到编译目录。
- 2. 执行例子前，需要把libDolphinDBAPI.dll和libgcc_s_seh-1.dll复制到可执行文件的相同目录下。

### 9.2 API

#### 9.2.1 ThreadedClient

ThreadedClient 产生一个线程。每次新数据从流数据表发布时，该线程去获取和处理数据。

##### 9.2.1.1 定义线程客户端

``` 
ThreadedClient::ThreadClient(int listeningPort);
```

* listeningPort 是单线程客户端的订阅端口号。

##### 9.2.1.2 调用订阅函数

``` 
ThreadSP ThreadedClient::subscribe(string host, int port, MessageHandler handler, string tableName, string actionName = DEFAULT_ACTION_NAME, int64_t offset = -1, bool resub = true, VectorSP filter = nullptr);
```

* host是发布端节点的主机名。

* port是发布端节点的端口号。

* handler是用户自定义的回调函数，用于处理每次流入的消息。函数的参数是流入的消息，每条消息就是流数据表的一行。函数的结果必须是void。

* tableName是字符串，表示发布端上共享流数据表的名称。

* actionName是字符串，表示订阅任务的名称。它可以包含字母、数字和下划线。

* offset是整数，表示订阅任务开始后的第一条消息所在的位置。消息是流数据表中的行。如果没有指定offset，或它为负数或超过了流数据表的记录行数，订阅将会从流数据表的当前行开始。offset与流数据表创建时的第一行对应。如果某些行因为内存限制被删除，在决定订阅开始的位置时，这些行仍然考虑在内。

* resub是布尔值，表示订阅中断后，是否会自动重订阅。

* filter是一个向量，表示过滤条件。流数据表过滤列在filter中的数据才会发布到订阅端，不在filter中的数据不会发布。

ThreadSP 指向循环调用handler的线程的指针。该线程在此topic被取消订阅后会退出。

示例：

``` 
auto t = client.subscribe(host, port, [](Message msg) {
    // user-defined routine
    }, tableName);
t->join();
```

##### 9.2.1.3 取消订阅

``` 
void ThreadClient::unsubscribe(string host, int port, string tableName, string actionName = DEFAULT_ACTION_NAME);
```

* host 是发布端节点的主机名。

* port 是发布端节点的端口号。

* tableName 是字符串，表示发布端上共享流数据表的名称。

* actionName 是字符串，表示订阅任务的名称。它可以包含字母、数字和下划线。

该函数用于停止向发布者订阅数据。

#### 9.2.2 ThreadPooledClient

ThreadPooledClient 产生用户指定数量的多个线程。每次新数据从流数据表发布时，这些线程同时去获取和处理数据。当数据到达速度超过单个线程所能处理的限度时，ThreadPooledClient 比 ThreadedClient 有优势。

##### 9.2.2.1 定义多线程客户端

``` 
ThreadPooledClient::ThreadPooledClient(int listeningPort, int threadCount);
```
* listeningPort 是多线程客户端节点的订阅端口号。
* threadCount 是线程池的大小。

##### 9.2.2.2 调用订阅函数

``` 
vector<ThreadSP> ThreadPooledClient::subscribe(string host, int port, MessageHandler handler, string tableName, string actionName = DEFAULT_ACTION_NAME, int64_t offset = -1, bool resub = true, VectorSP filter = nullptr);
```

参数参见9.2.1.2节。

返回一个指针向量，每个指针指向循环调用handler的线程。这些线程在此topic被取消订阅后会退出。

示例：

``` 
auto vec = client.subscribe(host, port, [](Message msg) {
    // user-defined routine
    }, tableName);
for(auto& t : vec) {
    t->join();
}
```

##### 9.2.2.3 取消订阅

``` 
void ThreadPooledClient::unsubscribe(string host, int port, string tableName, string actionName = DEFAULT_ACTION_NAME);
```

参数参见9.2.1.3节。

#### 9.2.3 PollingClient

订阅数据时，会返回一个消息队列。用户可以从其中获取和处理数据。

##### 9.2.3.1 定义客户端

``` 
PollingClient::PollingClient(int listeningPort);
```

* listeningPort 是客户端节点的订阅端口号。

##### 9.2.3.2 订阅

``` 
MessageQueueSP PollingClient::subscribe(string host, int port, string tableName, string actionName = DEFAULT_ACTION_NAME, int64_t offset = -1);
```

参数参见9.2.1.2节。

该函数返回指向消息队列的指针。

示例：

``` 
auto queue = client.subscribe(host, port, handler, tableName);
Message msg;
while(true) {
    if(queue->poll(msg, 1000)) {
        if(msg.isNull()) break;
        // handle msg
    }
}
```

##### 9.2.3.3 取消订阅

``` 
void PollingClient::unsubscribe(string host, int port, string tableName, string actionName = DEFAULT_ACTION_NAME);
```

参数参见9.2.1.3节。

注意，对于这种订阅模式，若返回一个空指针，说明已取消订阅。

## 10. openssl 1.0.2版本源码安装
这部分主要是介绍下没有1.0.2版本openssl的，从源码编译安装的过程。已有的话忽略本节。


首先创建一个自定义目录，给自己编译的openssl使用。

这个示例里，我们使用/newssl目录，实际可以自己修改。
```console
demo@ddb:~# mkdir /newssl
```

下载openssl源码
```console
demo@ddb:~# wget https://www.openssl.org/source/old/1.0.2/openssl-1.0.2u.tar.gz
```

解压缩后，进入源码目录，配置编译结果存放目录为刚才创建的newssl目录
```console
demo@ddb:~/openssl-1.0.2u# ./config shared --prefix=/newssl
```


编译安装
```console
demo@ddb:~/openssl-1.0.2u# make install
```

因为这里编译结果存放的目录是/newssl，在实际编译时，链接的头文件和库文件目录也需要添加上我们存放的目录/newssl

比如[1.1.4编译](#114-编译)这个例子里，原先的g++编译命令是:

```
g++ main.cpp -std=c++11 -DLINUX -D_GLIBCXX_USE_CXX11_ABI=1 -DLOGGING_LEVEL_2 -O2 -I../include   -lDolphinDBAPI -lpthread -lssl -L../bin/linux_x64/ABI1  -Wl,-rpath,.:../bin/linux_x64/ABI1 -o main
```
要使用我们自己编译的openssl库，需要改成
```
g++ main.cpp -std=c++11 -DLINUX -D_GLIBCXX_USE_CXX11_ABI=1 -DLOGGING_LEVEL_2 -O2 -I../include   -lDolphinDBAPI -lpthread -L../bin/linux_x64/ABI1  -Wl,-rpath,.:../bin/linux_x64/ABI1 -L /newssl/lib/ -I /newssl/include -lssl -lcrypto -luuid -o main
```
本例中，编译文件后，直接运行main会报错，是由于我们的/newssl不在系统路径里，所以在运行main前，可以设置变量
```
export LD_LIBRARY_PATH=/newssl/lib
```
然后再运行./main就可以运行了

