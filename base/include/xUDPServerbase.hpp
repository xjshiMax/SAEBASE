//2018-12-18
/*
封装udp svr
1) 结合xreactor 实现常规udp server的io复用
2）实现组播功能
3）实现广播功能
*/
#pragma once
#include "xbaseclass.hpp"
namespace SAEBASE{

class xUDPServreBase :public xEventHandler
{

};
}