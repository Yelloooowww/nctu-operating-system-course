// compile: $ gcc multiprocess.c
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <inttypes.h>

#define INF 99999999


u_int64_t ans = 0;
u_int8_t buffer[INF];
u_int8_t integer = 13;


int main(){
  // prepare data
  u_int64_t datasize = 9999999+1;
  u_int8_t process_num = 1;

  for(u_int64_t i=0;i<datasize;i++) buffer[i] = rand();

  long long int time_sum = 0;
  for(int test_time=0;test_time<10;test_time++){
    ans = 0;
    int status;
    pid_t pid;


    // calculate time cost
    struct  timeval start;
    struct  timeval end;
    long long int diff;
    gettimeofday(&start,NULL); //start time

    // child process
    for(u_int8_t i=0;i<process_num;i++){
      if ((pid = fork()) == -1) {
        // perror("fork error");
        exit(EXIT_FAILURE);
      }
      else if (pid == 0) {//start of child process
        u_int8_t count = 0;
        u_int64_t index_start = (datasize/process_num)*i;
        u_int64_t index_end;
        if (!(i == process_num-1)) index_end = (datasize/process_num)*(i+1);
        else index_end = datasize;

        for(u_int64_t j=index_start;j<index_end;j++){
          if (buffer[j]==integer) count++;
        }
        exit(count);
      }


      else{//start of parent process
        if ((pid = wait(&status)) == -1) ; //Wait for child process.
          // perror("wait error");
        else { //Check status.
          if (WIFSIGNALED(status) != 0) ;
            // printf("Child process ended because of signal %d \n",WTERMSIG(status));

          else if (WIFEXITED(status) != 0){
            ans += WEXITSTATUS(status); //WEXITSTATUS 是一個巨集 (macro) , 他可以用來提取出指定的子程序結束狀態值
            // printf("Child process ended normally; status = %d \n",WEXITSTATUS(status));
            // printf("Integer 13 occurs %ld times in the array\n", ans );
          }

          // else printf("Child process did not end normally \n");
        }
      }
    }


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
