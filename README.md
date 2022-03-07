# hexdump2bin

用于将vscode hexdump生成的文件转换为原文件。

使用mingw编译生成的程序，执行到fwrite会程序崩溃，暂时还没解决这个问题。
使用GCC编译程序在Linux运行，使用MSVC编译程序在Windows上运行均无问题。