#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>
#define BUF (255)
#define MAX_SUB_LEN (10)

int main(int argc, char *argv[])
{
  char buf[BUF] = {'\0'};
  char substr[MAX_SUB_LEN]= {'\0'};
  char str[BUF] = {'\0'};
  int n = 0, i;

  std::cout << "Введите подстроку для поиска " << std::endl;
  std::cin.getline(substr, MAX_SUB_LEN);
  std::fstream  fs("data.txt", std::fstream::in);
  if(!fs.is_open())
        return 0;
  while( !(fs.getline(buf, BUF).eof())  ){
        n++;// получение количества строк в файле
        memset(buf, '\0', BUF);  // обнуляем на следующую итерацию
  }
  fs.close(); 

  ssize_t readlen;
  int pipefd[n][2];
  for (i = 0; i < n; i++) {
    pipe(pipefd[i]);

  }


  std::fstream  fs2("data.txt", std::fstream::in);
  int check = 0;

  for (i = 0; i < n; i++) 
  {
     if(!fs2.is_open())
        return 0;

     fs2.getline(buf, BUF);
     check++;
     strcpy (str, buf);
     memset(buf, '\0', BUF);  // обнуляем на следующую итерацию

     

     if (fork() == 0)
     {
        fs2.close();

        close(pipefd[i][0]);    // close reading end in the child

        dup2(pipefd[i][1], 1);  // send stdout to the pipe
        dup2(pipefd[i][1], 2);  // send stderr to the pipe

        close(pipefd[i][1]);    // this descriptor is no longer needed


        execl("sp4_child"," ",substr, str, NULL);
     }
     memset(str, '\0', BUF);
  } 
  fs2.close();
  
  char buff[BUF];
  for (i = 0; i < n; i++) {
        close(pipefd[i][1]);  // close the write end of the pipe in the parent

         while ((readlen=read(pipefd[i][0], buff, sizeof(buff))) != 0)
         {
             buff[readlen] = '\0';
         }
        if (buff[0] != '-')
        {
            printf("Номер строки с найденной подстрокой = %i, ", i);
            printf("начало подстроки с элемента %s\n", buff);
        } 
        
  }
  

  return 0;
}