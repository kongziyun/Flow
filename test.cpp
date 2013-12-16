#include <pthread.h>
#include <iostream>
#include <map>
using namespace std;

void* func(void* something)
{
  pthread_exit(NULL);
}
int main(int argc, char* argv[]){
  cout<<sizeof(std::map<std::pair<int,int>, std::string>::iterator)<<endl;
  if (argc == 2){
  pthread_t a[8];
  for (int i = 0 ; i < 8; ++i)
    pthread_create(&a[i], NULL, func, NULL);
  pthread_exit(NULL);
  }
  else
{
  for (int i = 0; i < 8; ++i)
  func(NULL);
return 1;
}
}
