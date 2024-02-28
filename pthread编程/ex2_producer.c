//本代码将注释掉的代码跟其附近的代码交换注释就行 一个完成任务一一个完成任务二
//思路：采用互斥量来完成，设置两个锁，当其中一个执行其操作时，便给自己上锁，并给另外一个解锁
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define NUMS 100 //表示生产，消费的次数
#define CAPACITY 5 //定义缓冲区最大值
int capacity = 0; //当前缓冲区的产品个数
pthread_mutex_t my_lock1 = PTHREAD_MUTEX_INITIALIZER;//互斥量
//pthread_mutex_t my_lock2 = PTHREAD_MUTEX_INITIALIZER;//互斥量

void *produce(void *args)
{
   /*******************************************************************/
   int times=NUMS;
  // int err;
   while(times--){
     pthread_mutex_lock(&my_lock1);
     if(capacity<CAPACITY){ //缓冲区未满 可以继续装
       capacity+=1;
       printf("加入一个产品，当前产品数为：%d\n",capacity);
     }
     else{
       printf("操作失败，缓冲区已满。当前产品数为：%d\n",capacity);
     }
     //err=pthread_mutex_unlock(&my_lock2);
     pthread_mutex_unlock(&my_lock1);
   }
  return NULL;
   /******************************************************************/
}
void * consume(void *args)
{
   /*******************************************************************/
  int times=NUMS;
  while(times--){
    //pthread_mutex_lock(&my_lock2);
    pthread_mutex_lock(&my_lock1);
    if(capacity>0){ //缓冲区未满 可以继续装
      capacity-=1;
      printf("消费一个产品，当前产品数为：%d\n",capacity);
    }
    else{
      printf("操作失败，缓冲区已空。当前产品数为：%d\n",capacity);
    }
    pthread_mutex_unlock(&my_lock1);
  }
  return NULL;
  /******************************************************************/
}
int main(int argc, char** argv) {
    int err;
    pthread_t produce_tid, consume_tid;
    void *ret;
    err = pthread_create(&produce_tid, NULL, produce, NULL);//创建线程
    if (err != 0) {
        printf("线程创建失败:%s\n", strerror(err));
        exit(-1);
    }
    err = pthread_create(&consume_tid, NULL, consume, NULL);
    if (err != 0)  {
        printf("线程创建失败:%s\n", strerror(err));
        exit(-1);
    }
    err = pthread_join(produce_tid, &ret);//主线程等到子线程退出
    if (err != 0) {
        printf("生产着线程分解失败:%s\n", strerror(err));
        exit(-1);
    }
    err = pthread_join(consume_tid, &ret);
    if (err != 0) {
        printf("消费者线程分解失败:%s\n", strerror(err));
        exit(-1);
    }
    
    return (EXIT_SUCCESS);
}      