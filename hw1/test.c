#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// 計數器
int counter = 0;

// 加入 Mutex
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

// 子執行緒函數
void* child(void *arg) {
  int *input = (int *) arg; // 取得資料
  printf("%d,%d\n",input[0],input[1] );
  for(int i = 0;i < 3;++i) {
    pthread_mutex_lock( &mutex1 ); // 上鎖
    int tmp = counter;
    sleep(1);
    counter = tmp + 1;
    pthread_mutex_unlock( &mutex1 ); // 解鎖
    printf("Counter = %d\n", counter);
  }
  pthread_exit(NULL);
}

// 主程式
int main() {
  pthread_t t1, t2;
  int input[2][2] = {{1, 2},{3,4}}; // 輸入的資料
  pthread_create(&t1, NULL, child, (void*) input[0]);
  pthread_create(&t2, NULL, child, (void*) input[1]);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  return 0;
}
