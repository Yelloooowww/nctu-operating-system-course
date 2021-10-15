// compile: $ gcc multiprocess_string.c
// run: $ ./a.out datasize process_num
// (datasize<99999999, process_num<256)
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define INF 99999999+1


u_int64_t ans = 0;
char buffer[INF][6];
char string[6] = {'y','e','l','l','o','w'};

// 主程式
int main(int argc, char *argv[]) {
  FILE *F ;
  F = fopen("multiprocess_string.csv","a");
  if(F == NULL) printf("open file fail QQ\n");

  //prepare data
  u_int64_t datasize = atoi(argv[1]);
  u_int8_t process_num = atoi(argv[2]);
  printf("test setting: datasize=%lu, process_num=%d \n",datasize,process_num);

  for(u_int64_t i=0;i<datasize;i++){
    for(u_int64_t j=0;j<6;j++) buffer[i][j] = rand();//string[j];
  }

  long long int time_sum = 0;
  for(int test_time=0;test_time<10;test_time++){
    ans = 0;
    int status;
    pid_t pid[process_num];


    // calculate time cost
    struct  timeval start;
    struct  timeval end;
    long long int diff;
    gettimeofday(&start,NULL); //start time

    // child process
    for(u_int8_t i=0;i<process_num;i++){
      if ((pid[i] = fork()) == -1) {
        exit(EXIT_FAILURE);
      }
      else if (pid[i] == 0) {//start of child process
        u_int8_t count = 0;
        u_int64_t index_start = (datasize/process_num)*i;
        u_int64_t index_end;
        if (!(i == process_num-1)) index_end = (datasize/process_num)*(i+1);
        else index_end = datasize;

        for(u_int64_t j=index_start;j<index_end;j++){
          char flag = 1; //True
          for(u_int64_t k=0;k<6;k++){
            if(buffer[j][k]!=string[k]){
              flag = 0; // the string != "yellow"
              break;
            }
          }
          if (flag==1) count++ ;
          // printf("%d %d %d\n", index_start,index_end,j);
        }
        exit(count);
      }
    }

    for(u_int8_t i=0;i<process_num;i++){//start of parent process
      if ((pid[i] = wait(&status)) == -1) ; //Wait for child process.
        // perror("wait error");
      else if ((pid[i] = wait(&status)) != 0) { //Check status.
        if (WIFSIGNALED(status) != 0) ;
          // printf("Child process ended because of signal %d \n",WTERMSIG(status));

        else if (WIFEXITED(status) != 0){
          ans += WEXITSTATUS(status); //WEXITSTATUS 是一個巨集 (macro) , 他可以用來提取出指定的子程序結束狀態值
          // printf("Child process ended normally; status = %d \n",WEXITSTATUS(status));
        }
        // else printf("Child process did not end normally \n");
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
  fprintf(F, "%lu, %d, %lld \n",datasize, process_num, time_avg);
  fclose(F);

  return 0;
}
