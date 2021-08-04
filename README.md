# foundation
foundation 是用C++11编写的一个基础库，封装了一些开发中常用的基础组件，包括：
+ 内存池分配器
+ 缓存器
+ 日志
+ 时间轮实现的定时器
+ 位图
+ 跨平台的网络IO接口
+ 线程队列
+ 双向链表
+ ...

使用方式可参考unit test。

## 编译
通过cmake编译 foundation。请确保您安装的cmake 版本大于3.10。    
之后执行以下命令：
```shell
mkdir build && cd build
cmake ..
make
```
   
如果需要同时编译unit test, 执行：
```shell
mkdir build && cd build
cmake -DWITH_TEST=ON ..
make
```
这会下载google-test依赖，请确保网络畅通。   

## 协议

foundation 使用BSD 3-Clause使用条款，详情请看[BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)。