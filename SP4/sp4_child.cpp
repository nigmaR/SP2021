#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <cctype>

int main(int argc, char *argv[])
{	
  int i = 0, index = -1, j = 0;
  char substr[10];
  char str[255];
 
  strcpy (substr, argv[1]);
  strcpy (str, argv[2]);
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

  printf("%i ",index);
  int pid = getpid();
  printf("pid=%i", pid);
  printf(" Строка: %s",str);
  return 0;
}