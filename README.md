可能有用的知识，可以使用typora来打开以下文字
# 基础内容

CONFIG_MSIZE 内存大小 128MB 

pmem[CONFIG_MSIZE ]  

 `MUXDEF(CONFIG_TRACE, "ON", "OFF")`则表示, 如果定义了`CONFIG_TRACE`, 则预处理结果为`"ON"`(`"OFF"`在预处理后会消失), 否则预处理结果为`"OFF"` 

uint8_t pmem[CONFIG_MSIZE] 对应内存地址（相对地址）所存储的值 一个地址对应一个8位

CPU_state 就是riscv32_CPU_state 保存着寄存器跟pc 变量名为cpu



 一个地址代表一个字节，32为机器表示用32个二进制位表示地址，那么寻址空间就是2^32 



 `%x` 格式说明符用于以十六进制格式输出无符号32位整数 

 `%u` 格式说明符用于无符号32位整数的输出 

*0x80000000



enum 定义一系列常量 enum的变量只能从enum中找值

 typedef  定义别名



所有都尽量保存成word_t类型（也就是uint32_t） 输出的时候改成16进制



 `paddr_t` 一般用于表示物理内存地址 



elf中header是整个elf的基础信息，程序头表信息 通过每一个表项可以定位到对应的段，.data和.bss为数据段，.bss为未初始化的。







config指令

```text
make menuconfig
```



# PA2

rd:  目的操作数的寄存器 

rs1:  源操作数 

rs2:  源操作数 

imm:  立即数 

src1: 寄存器的值



vaddr_write --->paddr_write-->pmem_write(paddr_t addr, int len, word_t data) 

把data写入 addr中（len为传入的字节长度） addr 为真实地址 需要后续处理变成虚拟地址







cd ics2022/am-kernels/tests/cpu-tests

指令:  make ARCH=riscv32-nemu ALL=dummy run -j4

make ARCH=riscv32-nemu mainargs=v run -j32



src1R() 读对应寄存器的值 保存到src1

rs1R()  读对应的rs1的值 也就是寄存器下标的值保存到src1



ELF文件

直接读取传入的**可执行文件**，文件名即`argv[0]`

NEMUFLAGS += -e $(IMAGE).elf  直接在args后面加参数





 如果 `NATIVE_USE_KLIB` 被注释了的话，在 native 下运行就不会有手写的库函数，native 会使用 glibc 的库函数。如果 `NATIVE_USE_KLIB` 没有被注释，在 native 下运行有手写的库函数，当手写函数与 glibc 库函数名称冲突时，优先链接用户手写的函数，这样就达到了使用 klib 的效果。 



 `abstract-machine`中有一个特殊的架构叫`native`, 是用GNU/Linux默认的运行时环境来实现的AM API.  



typedef struct {

 const char *name;

 // we treat ioaddr_t as paddr_t here

 paddr_t low;

 paddr_t high;

 void *space;

 io_callback_t callback;

} IOMap;



low 和high都是物理内存地址（逻辑上），space是实际开辟的地址

时钟{

​	读的时候才进行时钟变化，即时间变化

}



堆栈的值对应char*是因为内存保存的值为8位，该变量指向的是内存地址的值。



CONFIG_VGA_CTL_MMIO 0xa0000100 为vga的控制器信息

CONFIG_FB_ADDR 0xa1000000 为vga图像信息

先调用__am_gpu_config  再调用vga_update_screen

画图需要把二维的画图信息（也就是自己定义的颜色的图） 转换成一维存储于内存中。

显示器会将该内存中一维的信息变成显示器上的像素





io_write(AM_GPU_FBDRAW,其他参数)  -->  ioe_write(AM_GPU_FBDRAW,其他参数) -->

调用(handler_t) lut[reg] (其他参数) --> 调用__am_gpu_fbdraw(其他参数) , 

其他参数构成一个结构体AM_GPU_FBDRAW_T

io_read的时候会读取当前值，然后在回调里面修改值（也就是改变状态)



isa_reg_str2val() 调用的时候前面+$



gdb:

printf "0x%x\n",s->pc









# PA3

ecall 自陷指令

 mtvec寄存器来存放异常入口地址 

- mepc寄存器 - 存放触发异常的PC
- mstatus寄存器 - 存放处理器的状态
- mcause寄存器 - 存放触发异常的原因

riscv32触发异常后硬件的响应过程如下:

1. 将当前PC值保存到mepc寄存器
2. 在mcause寄存器中设置异常号
3. 从mtvec寄存器中取出异常入口地址
4. 跳转到异常入口地址

创建了 

typedef struct {

 word_t mstatus;

 word_t mcause;

 word_t mepc;

 paddr_t mtvec;

} riscv32_CPU_CSRs;  csr





在difftest.cc中添加寄存器，并且对寄存器的写和读进行函数修改

在duc.c中 没有进行对mstatus的校验

在duc.c中 没有进行对mcause的校验





 定义了两个全局变量`ramdisk_start`和`ramdisk_end`，将`build/ramdisk.img`文件的内容插入到这两个变量之间 



栈区：

​	栈区的内存由系统自动分配，用于存放函数的参数值、局部变量等。

​	存储的是指针一样的东西，具体的值分配在其他地址段

堆区：

​	堆区的内存是通过new、malloc、realloc等方式分配的，编译器不会负责它们的释放工作，需要程序员手动释放。

​	堆区存储的是对象的实际内存位置，new出来的对象的内存地址。

全局变量（和静态局部变量）有专门数据段存储，初始值是0，具体位置是固定的 ,已经初始化的存放到data段，没有初始化的保存到BSS段。

局部变量保存到栈上，分配的具体对象在堆上。



 可以使用`_end`符号来确定堆的起始位置。在许多C和C++的运行时环境中，堆是紧跟着BSS段之后的内存区域，因此`_end`符号就标志着堆的起始位置

 `_end`符号并不是一个变量，而是一个地址



 `make ARCH=$ISA-nemu update`会自动编译Navy中的程序 , 把`navy-apps/fsimg/`目录下的所有内容整合成ramdisk镜像`navy-apps/build/ramdisk.img`, 同时生成这个ramdisk镜像的文件记录表`navy-apps/build/ramdisk.h` 



把串口抽象化，也就是说在系统调用中，调用的都是对文件操作，也就是fs_write，具体的区分在fs_write中，也就是对文件的读写操作有个类似多态化的操作（实际上是用函数指针来改变函数指向）





file_table的下标值，也就是对应文件位置的值，通过该值来进行对文件的区分







