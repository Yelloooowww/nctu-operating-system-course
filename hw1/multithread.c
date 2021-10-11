// compile: $ gcc multithread.c -lpthread
// run: $ ./a.out datasize process_num
// (datasize<99999999, process_num<256)
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define INF 99999999


u_int64_t ans = 0;
u_int8_t buffer[INF];
u_int8_t integer = 13;

// 加入 Mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// 子執行緒函數
void* child(void *arg) {
  u_int64_t *input = (u_int64_t *) arg; // 取得資料
  for(u_int64_t j=input[0];j<input[1];j++){
    if (buffer[j]==integer){
      pthread_mutex_lock( &mutex ); // 上鎖
      ans++;
      pthread_mutex_unlock( &mutex ); // 解鎖
    }
  }
  pthread_exit(NULL);
}

u_int64_t char2int(char *c_input){
  u_int64_t tmp = 0;
  for(long unsigned int i=0;i<strlen(c_input);i++){
    u_int64_t decimal = 1;
    u_int64_t each_digi = (int)(c_input[i] - '0');
    for(long unsigned int j=0;j<strlen(c_input)-i-1;j++) decimal*=10;
    tmp += each_digi*decimal;
  }
  return tmp;
}

// 主程式
int main(int argc, char *argv[]) {
  //prepare data
  u_int64_t datasize = char2int(argv[1]);
  u_int8_t process_num = char2int(argv[2]);
  printf("test setting: datasize=%lu, process_num=%d \n",datasize,process_num);

  for(u_int64_t i=0;i<datasize;i++) buffer[i] = rand();
  u_int64_t input[process_num][2];
  for(u_int8_t i=0;i<process_num;i++){
    input[i][0] = (datasize/process_num)*i; //index_start
    if (!(i == process_num-1)) input[i][1] = (datasize/process_num)*(i+1); //index_start
    else input[i][1] = datasize;
  }

  long long int time_sum = 0;
  for(int test_time=0;test_time<10;test_time++){
    ans = 0;

    // calculate time cost
    struct  timeval start;
    struct  timeval end;
    long long int diff;
    gettimeofday(&start,NULL); //start time



    pthread_t t[process_num];
    for(u_int8_t i=0;i<process_num;i++){
      pthread_create(&t[i], NULL, child, (void*) input[i]);
    }


    for(u_int8_t i=0;i<process_num;i++) pthread_join(t[0], NULL);

    // calculate time cost
    gettimeofday(&end,NULL);
    diff = 1e6*(end.tv_sec-start.tv_sec) + (end.tv_usec-start.tv_usec);
    printf("time cost is %lld ms  ,",diff);

    time_sum += diff;
    printf("Integer 13 occurs %ld times in the array\n", ans );
  }


  long long int time_avg = time_sum/10;
  printf("average time cost is %lld ms\n",time_avg);

  return 0;
}
