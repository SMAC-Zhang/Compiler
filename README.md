测试方法
使用含testx.c 与 含testx.fmj的tests文件夹换掉原本的tests
使用make testc命令测试 testx.c， 输出为testx.output1
使用make testfmj命令进行测试 testx.fmj， 输出为testx.output2
使用make clean命令删除所有中间文件和结果