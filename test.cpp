#include<iostream>

using namespace std;

void test(char *a){
  cout << (char*)a << endl;
}

int main(){
  //char a[3] = {'a', 'b', 'c'};
  char b = 'k';
  char* c = &b;
  test(c);
  return 0;
}


