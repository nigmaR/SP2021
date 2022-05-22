#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <errno.h>

#define BUF (255)
#define MAX_SUB_LEN (10)

#if __GLIBC__ == 2 && __GLIBC_MINOR__ < 30
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)
#endif

char data[8][255] = {'\0'};
char substri[10] = {'\0'};

void * thread_func(void *arg)
{   
  
  int i = 0, index = -1, j = 0, loc_id;
  char substr[10];
  char str[255];
  loc_id = * (int *) arg;
  
  strcpy (substr, substri);
  strcpy (str, data[loc_id]);
  size_t substr_len = strlen(substr);
  size_t str_len = strlen(str);
  int succes = 1;
  
  for (i = 0; i < (int)(str_len-substr_len+1); i++) {
    succes = 1;
    for (j = 0; j < (int)(substr_len); j++) {
      if (std::tolower(substr[j]) != std::tolower(str[i+j]))
      {
        succes = 0;
        break;
      }
    }
    if (succes == 1)
    {
      index = i;
      break;
    }

  }
  if (index != -1)
  {

    printf("Номер строки с найденной подстрокой = %i, ", loc_id);
    printf("начало подстроки с элемента %i ", index);
    pthread_t my_tid = pthread_self();
    pid_t tid_linux = gettid();
    printf("tid(POSIX)=%lu ", my_tid);
    printf("tid(LINUX)=%i", tid_linux);
    printf(" Строка: %s \n",str);
  }
  sleep(2);
  return 0;
}

int main(int argc, char *argv[])
{
  char buf[BUF] = {'\0'};
  char substr[MAX_SUB_LEN]= {'\0'};
  char str[BUF] = {'\0'};
  int n = 0, i = 0;

  std::cout << "Введите подстроку для поиска " << std::endl;
  std::cin.getline(substri, MAX_SUB_LEN);
  std::fstream  fs("data.txt", std::fstream::in);
  if(!fs.is_open())
        return 0;
  while( !(fs.getline(buf, BUF).eof())  ){
        strcpy (data[n], buf);
        memset(buf, '\0', BUF);  // обнуляем на следующую итерацию
        n++;// получение количества строк в файле
  }
  
  strcpy (data[n], buf);
  memset(buf, '\0', BUF);
  n++;
  fs.close(); 

  int result;
  pthread_t thread[n];
  
  int id[n];
  for (i = 0; i < n; i++) {

    id[i]= i;
    pthread_create(&thread[i], NULL, thread_func, &id[i]);
  }

  for (i = 0; i < n; i++) {
      
    result = pthread_join(thread[i], NULL);
    if (result != 0) {

      printf("Joining the thread %i ", i);
      perror(": ");
      return EXIT_FAILURE;
  }
        
  }
 
  return 0;
}