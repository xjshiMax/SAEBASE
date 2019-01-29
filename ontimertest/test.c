#include <iostream>
using namespace std;

int main()
{
  char _char = 'a';
  short _short = 1;
  int _int = 1;
  float _float = 1.7;
  double _double = 3.4;
  long _long = 2;
  long long _llong = 2;


  int *p1 = &_int;
  char *p2 = &_char;

  cout<<"_char "<<sizeof _char<<endl;
  cout<<"_short "<<sizeof _short<<endl; 
  cout<<"_int "<<sizeof _int<<endl;
  cout<<"_flaot "<<sizeof _float<<endl;
  cout<<"_double "<<sizeof _double<<endl;
  cout<<"_long "<<sizeof _long<<endl;
  cout<<"_longlong "<<sizeof _llong<<endl;
  cout<<"char* "<<sizeof p2<<endl;
  cout<<"int * "<<sizeof p1<<endl;
  return 0;
}
