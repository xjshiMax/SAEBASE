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
#define OUT
#define IN
//将字节流（二进制）转换成16机制数据。如果小于16，则在前面补‘0’
namespace SAEBASE{
/*
hex:  4位2进制
字节： 8为位2进制 （窄字符）
字：双字节，16位	（宽字符）
双字：3字节，32位
*/
inline static void bytesToHex(const void* pSrcBinary,unsigned int nSize,std::string &strRetHex)
{

}
inline static void HexToBytes(const void* pstrHex,unsigned int nSize,OUT std::string &strRetBytes)
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
inline static string strterm(string s)
{
	s.erase(0,s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ")+1);
	return s;
}
static const char hex2[] = "000102030405060708090a0b0c0d0e0f"
	"101112131415161718191a1b1c1d1e1f"
	"202122232425262728292a2b2c2d2e2f"
	"303132333435363738393a3b3c3d3e3f"
	"404142434445464748494a4b4c4d4e4f"
	"505152535455565758595a5b5c5d5e5f"
	"606162636465666768696a6b6c6d6e6f"
	"707172737475767778797a7b7c7d7e7f"
	"808182838485868788898a8b8c8d8e8f"
	"909192939495969798999a9b9c9d9e9f"
	"a0a1a2a3a4a5a6a7a8a9aaabacadaeaf"
	"b0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
	"c0c1c2c3c4c5c6c7c8c9cacbcccdcecf"
	"d0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
	"e0e1e2e3e4e5e6e7e8e9eaebecedeeef"
	"f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff";

//将int数字转为16进制字符串
inline static string toHex16Bit(unsigned int x) {
	const unsigned int hi = (x >> 8) & 0xff;
	const unsigned int lo = x & 0xff;
	string result(4, ' ');
	result[0] = hex2[2 * hi];
	result[1] = hex2[2 * hi + 1];
	result[2] = hex2[2 * lo];
	result[3] = hex2[2 * lo + 1];
	return result;
}

};
