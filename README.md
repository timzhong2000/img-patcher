# img-patcher
lyb操作系统课程，软盘镜像生成器

## 基本用法

### 编译

`gcc img-patcher.cpp -o bootimg-patcher.exe`

### 参数列表：

1. 输出镜像(string)
2. 输出镜像大小(int)
3. 是否自动写入有效位(0或1)
4. 插入位置(字节)(int)
5. 插入程序文件(string)

其中4,5可以重复

### 基本用例

生成大小为`1474560`字节的标准软盘镜像`./out.img`，并且自动添加引导扇区有效位，并在`0字节`位置(0扇区)插入`boot.com`，在`512字节`位置(1扇区)插入`A.com`，在`1024字节`位置(2扇区)插入`B.com`。

对应命令：`bootimg-patcher.exe ./out.img 1474560 1 0 ./boot.com 512 ./A.com 1024 ./B.com`

**早日搞文件系统，早日脱离坑爹的手写软盘。如果觉得好用请右上角点个star**
