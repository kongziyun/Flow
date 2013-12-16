#include <omp.h>
#include <iostream>
using namespace std;

void main()
{
omp_lock_t lock;
omp_init_lock(&lock);
  #pragma omp single nowait
  {
    int count = 3;
    for (int i = 0; i < 10090; i++)
    {
      ++count;
    }
  sleep(3);
    omp_set_lock(&lock);
    cout<<count<<"\t"<<omp_get_thread_num()<<endl;
    omp_unset_lock(&lock);
  }
  omp_set_lock(&lock);
  cout <<"oops\t"<<endl;
  omp_unset_lock(&lock);
  return;
}
