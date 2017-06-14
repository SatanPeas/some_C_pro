#include <stdio.h>
#include <pthread.h>
#define BUFFER_SIZE 16

struct prodcons
{
    int buffer[BUFFER_SIZE];/*实际存放数据的数组*/
    pthread_mutex_t  lock;/*互斥体lock，用于对缓冲区的互斥操作*/
    int readpos,writepos; /*读写指针*/
    pthread_cond_t notempty;/*缓冲区非空的条件变量*/
    pthread_cond_t notfull;/*缓冲区未满 的条件变量*/
};

/*初始化缓冲区*/
void pthread_init(struct prodcons *p)
{
    pthread_mutex_init(&p->lock,NULL);
    pthread_cond_init(&p->notempty,NULL);
    pthread_cond_init(&p->notfull,NULL);
    p->readpos  = 0;
    p->writepos = 0;
}

/*将产品放入缓冲区，这里是存入一个整数*/
void put(struct prodcons *p,int data)
{
    pthread_mutex_lock(&p->lock);
    /*等待缓冲区未满*/
    if((p->writepos +1)%BUFFER_SIZE ==p->readpos)
    {
        pthread_cond_wait(&p->notfull,&p->lock);
    }
    p->buffer[p->writepos] =data;
    p->writepos++;
    if(p->writepos >= BUFFER_SIZE)
    p->writepos = 0;
    pthread_cond_signal(&p->notempty);
    pthread_mutex_unlock(&p->lock);
}

/*从缓冲区取出整数*/
int get(struct prodcons *p)
{
    int data;
    pthread_mutex_lock(&p->lock);
    /*等待缓冲区非空*/
    if(p->writepos == p->readpos)
    {
        pthread_cond_wait(&p->notempty ,&p->lock);//非空就设置条件变量notempty
    }
    /*读书据，移动读指针*/
    data = p->buffer[p->readpos];
    p->readpos++;
    if(p->readpos == BUFFER_SIZE)
        p->readpos = 0;
    /*设置缓冲区未满的条件变量*/
    pthread_cond_signal(&p->notfull);
    pthread_mutex_unlock(&p->lock);
    return data;
}

 /*测试：生产站线程将1 到1000的整数送入缓冲区，消费者线程从缓冲区中获取整数，两者都打印信息*/
#define OVER (-1)
struct prodcons buffer;
void *producer(void *data)
{
    int n;
    for( n=0;n<1000;n++)
    {
        printf("%d ------>\n",n);
        put(&buffer,n);
    }
    put(&buffer,OVER);
    return NULL;
}

void *consumer(void *data)
{
    int d;
    while(1)
    {
        d = get(&buffer);
        if(d == OVER)
            break;
        else
            printf("----->%d\n",d);
    }
    return NULL;
}

int main()
{
    pthread_t th_p,th_c;
    void *retval;
    pthread_init(&buffer);
    pthread_create(&th_p,NULL,producer,0);
    pthread_create(&th_c,NULL,consumer,0);
    /*等待两个线程结束*/
    pthread_join(th_p, &retval);
    pthread_join(th_c,&retval);
    return 0;
}