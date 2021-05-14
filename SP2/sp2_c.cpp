#include <stdio.h>
int main()
{
	unsigned int array[10] = {1,2,3,4,5,6,7,8,9,10};
	unsigned int sum = 0;

	for (int i = 0; i < 10; i++)
	{
        printf("array[%u]= %u",i, array[i]);
    	if (i % 2 == 0)
        	//четное
    	{
       		array[i] = array[i] << 1;

    	}
    	else
    	{
        	//нечетное
       		array[i] = array[i] >> 1;

    	}
        printf(" => %u\n", array[i]);
    	sum += array[i];
	}
    printf("sum = %u\n",sum);
	return 0;  
}
