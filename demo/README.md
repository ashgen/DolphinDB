# Demo 编译方法

步骤如下：

1. 解压*demo.zip*到*src*目录下。
2. 拷贝编译好的 C++ API 动态库至 *demo/lib* 目录下。
3. 执行下面的命令完成编译。以 Visual Studio 2022 为例，编译脚本中应使用 `SSL_LIBS` 参数指定 openssl 库的路径。
```
cd api-cplusplus/demo
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64 -DSSL_LIBS=D:/openssl-1.0.2l-vs2017
cmake --build .
```
4. 将生成的可执行文件 *apiDemo.exe* 和依赖的动态库 *DolphinDBAPI.dll*, *libeay32MD.dll* 和 *ssleay32MD.dll* 放置在 *demo\bin\Debug* 目录下即可执行。

💡注意：该 demo 同样支持 mingw 和 Linux，只是 cmake 语句略有不同。
