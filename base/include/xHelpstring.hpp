//2018-12-13
/*
��װ���õ��ַ��������ӿ�
1. intתstring
2. string תint
3. byte תHEX
4. HEX תbypt
5. MD5 ժҪ����
6. sha1ժҪ����
7. �ַ����ָ�
8. �ַ����滻

*/
#pragma once
#include <stdio.h>
#include <string>
#include <algorithm>
using namespace std;
//���ֽ����������ƣ�ת����16�������ݡ����С��16������ǰ�油��0��
namespace SAEBASE{
hex:  4λ2����
�ֽڣ� 8Ϊλ2���� ��խ�ַ���
�֣�˫�ֽڣ�16λ	�����ַ���
˫�֣�3�ֽڣ�32λ
*/
inline static void bytesToHex(const void* pSrcBinary,unsigned int nSize,std::string &strRetHex)
{

}
inline static void HexToBytes(const void pstrHex,unsigned int nSize,OUT std::string &strRetBytes)
{

}
//��дתСд
inline static string xToLower(string srcstr)
{
	std::transform(srcstr.begin(),srcstr.end(),srcstr.begin(),::tolower);
	return srcstr;
}
//Сдת��д
inline static string xToUpper(string srcstr)
{
	std::transform(srcstr.begin(),srcstr.end(),srcstr.begin(),::toupper);
	return srcstr;
}
//ȥ��ͷβ�ո�
inline static string term(string s)
{
	s.erase(0,s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ")+1);
	return s;
}
};
