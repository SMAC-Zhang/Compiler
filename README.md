# 运行方法

## RPI

使用`make RPI`命令生成RPI编译器<br>
使用`./RPI xx.fmj` 生成`xx.s`文件 <br>
使用 `arm-linux-gnueabihf-gcc -mcpu=cortex-a72 xx.s libsysy.a --static -o xx.out`链接外部库并生成`xx.out`的最终代码。<br>
使用`qemu-arm -B 0x1000 xx.out`命令运行代码。<br>

## LLVM IR

使用`make LLVM`命令生成LLVM编译器<br>
使用`./LLVM xx.fmj` 命令生成`xx.ll`文件。<br>
使用`lli xx.out`命令运行代码<br>

## 中间文件

AST树打印在`xx.ast`文件中<br>
IR+代码打印在`xx.irp`文件中<br>
基本块打印在`xx.stm`文件中<br>
数据流图打印在`xx.liv`文件中<br>

# 运行测试

使用带testx.fmj 和 libsys.a的tests文件替换tests文件<br>
然后使用`make testrpi`命令进行测试 将自动生成arm代码然后执行，并echo返回值。<br>
然后使用`make testllvm`命令进行测试 将自动生成llvm代码然后执行，并echo返回值。<br>
使用`make clean`命令删除所有生成的文件。<br>