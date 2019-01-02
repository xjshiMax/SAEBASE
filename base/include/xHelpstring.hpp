//2018-12-13
/*
封装常用的字符串辅助接口
1. int转string
2. string 转int
3. byte 转HEX
4. HEX 转bypt
5. MD5 摘要加密
6. sha1摘要加密
7. 字符串分割
8. 字符串替换

*/
#pragma once
#include <stdio.h>
#include <string>
#include <algorithm>
using namespace std;
//将字节流（二进制）转换成16机制数据。如果小于16，则在前面补‘0’
namespace SAEBASE{
hex:  4位2进制
字节： 8为位2进制 （窄字符）
字：双字节，16位	（宽字符）
双字：3字节，32位
*/
inline static void bytesToHex(const void* pSrcBinary,unsigned int nSize,std::string &strRetHex)
{

}
inline static void HexToBytes(const void pstrHex,unsigned int nSize,OUT std::string &strRetBytes)
{

}
//大写转小写
inline static string xToLower(string srcstr)
{
	std::transform(srcstr.begin(),srcstr.end(),srcstr.begin(),::tolower);
	return srcstr;
}
//小写转大写
inline static string xToUpper(string srcstr)
{
	std::transform(srcstr.begin(),srcstr.end(),srcstr.begin(),::toupper);
	return srcstr;
}
//去掉头尾空格
inline static string term(string s)
{
	s.erase(0,s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ")+1);
	return s;
}
};
