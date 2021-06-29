#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long int ullint;

extern ullint *task (ullint x, ullint y);


int main()
{
    ullint x, y, i ;
    ullint *arr;
    printf("Введите размеры двух массивов\n");
    scanf("%llu%llu", &x, &y);
	arr = task(x,y);
    printf("Начало вывода Си\n");
    for (i = 0; i<(x+y); i++)
    {
        printf("%llu ", arr[i]);
    }
    printf("\n");
    
	return 0;  
}
