// stdafx.cpp : 只包括标准包含文件的源文件
// OpenglHook.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#pragma comment(lib, "legacy_stdio_definitions.lib") 

extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }
// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
