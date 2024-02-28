#include "threadpool.h"

void Job_running(threadpool* pool){
	while(pool->flag){
	  pthread_mutex_lock(&pool->mutexpool);
	  while(pool->poolhead==NULL && pool->flag==1){
	    pthread_cond_wait(&pool->notempty,&pool->mutexpool);
	  }
	  if(pool->flag==0){
	    pthread_mutex_unlock(&pool->mutexpool);
	    pthread_exit(0);
	  }
	  //threadjob *t = (threadjob*)malloc(sizeof(threadjob));
          threadjob *t=pool->poolhead;
	  pool->poolhead=pool->poolhead->next;
	  pool->jobnum--;
	  pthread_mutex_unlock(&pool->mutexpool);
	  t->data.pf(t->data.arg);
	  free(t);
	  t=NULL;
	}
	pthread_exit(0);
}

void *worker(void *arg){
      threadpool *pool = (threadpool *)arg;
      Job_running(pool);
      return NULL;
}

threadpool* Pool_init(int Maxthread){
	threadpool* pool;
	pool = (threadpool*)malloc(sizeof(threadpool));
	
	if(pool==NULL){
	  printf("malloc error\n");
	  return NULL;
	}
	
	pool->flag = 1;
	pool->Maxthread=Maxthread;
	pool->jobnum=0;
	pool->poolhead=NULL;
	pool->tail=NULL;
	pool->threads=NULL;
	pthread_mutex_init(&pool->mutexpool,NULL);
	pthread_cond_init(&pool->notempty,NULL);
	
	for(int i=0;i<Maxthread;i++){
	  pthread_t tid;
	  pthread_create(&tid,NULL,worker,pool);
	}
	return pool;
}

int Add_job(threadpool* pool , function_t pf , void* arg){
        if(pool->flag==0||pool->jobnum==pool->Maxthread){
          return -1;
        }
        threadjob* t=(threadjob*)malloc(sizeof(threadjob));
        t->data.arg=arg;
        t->data.pf=pf;
        t->next=NULL;
        pthread_mutex_lock(&pool->mutexpool);
        if(pool->poolhead==NULL){
           pool->poolhead=t;
           pool->tail=t;
        }
        else{
           pool->tail->next=t;
           pool->tail=t;
        }
        pool->jobnum++;
        pthread_cond_signal(&pool->notempty);
        pthread_mutex_unlock(&pool->mutexpool);
        return 1;
}

int Push(threadpool* pool , Jobnode data ){
	threadjob* t=(threadjob*)malloc(sizeof(threadjob));
	t->data=data;
	t->next=NULL;
	pthread_mutex_lock(&pool->mutexpool);
        if(pool->poolhead==NULL){
           pool->poolhead=t;
           pool->tail=t;
        }
        else{
           pool->tail->next=t;
           pool->tail=t;
        }
        pool->jobnum++;
        pthread_cond_signal(&pool->notempty);
        pthread_mutex_unlock(&pool->mutexpool);
        return 1;
}

Jobnode Pop(threadpool* pool){
        if(pool->flag==0||pool->jobnum==0){
          return pool->poolhead->data;
        }
	threadjob* t=pool->poolhead;
	pool->poolhead=pool->poolhead->next;
	free(t);
	t=NULL;
	return pool->poolhead->data;
}

int Delete_pool(threadpool* pool){
	pool->flag = 0;
	for(int i=0;i<pool->jobnum;i++){
	   pthread_cond_signal(&pool->notempty);
	}
	if(pool->threads){
	  free(pool->threads);
	}
	
	pthread_mutex_destroy(&pool->mutexpool);
	pthread_cond_destroy(&pool->notempty);
	
	free(pool);
	pool=NULL;
	return 0;	
}