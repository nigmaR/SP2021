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

pthread_mutex_t mutex;
pthread_barrier_t barrier;

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
  pthread_mutex_lock(&mutex);
  
  if (index != -1)
  {
    std::ofstream out("output.txt", std::ios::app);
    if (out.is_open())
    {
       out << "Номер строки с найденной подстрокой = " << loc_id;
       out << " начало подстроки с элемента  " << index;
       pthread_t my_tid = pthread_self();
       pid_t tid_linux = gettid();
       out << " tid(POSIX)=  " << my_tid;
       out << " tid(LINUX)=  " << tid_linux;
       out << " Строка: " << str << std::endl;

    }
    out.close();  


  }

  pthread_mutex_unlock(&mutex);
  pthread_barrier_wait (&barrier);

  sleep(2);

  return 0;
}


int main(int argc, char *argv[])
{
  pthread_mutex_init(&mutex, NULL);
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
  
  if ((errno = pthread_barrier_init(&barrier, NULL, n+1)) != 0)
  {
      perror ("PTHREAD_BARRIER_INIT");
      return (errno);
  }

  int id[n];
  for (i = 0; i < n; i++) {

    id[i]= i;
    pthread_create(&thread[i], NULL, thread_func, &id[i]);
  }



  pthread_barrier_wait (&barrier);
  
  std::fstream  fs1("output.txt", std::fstream::in);
  if(!fs1.is_open())
        return 0;
  while( !(fs1.getline(buf, BUF).eof())  ){
        printf("%s \n", buf);
        memset(buf, '\0', BUF);  // обнуляем на следующую итерацию
  }
  printf("%s \n", buf);
  fs1.close(); 

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