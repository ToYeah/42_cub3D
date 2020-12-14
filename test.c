#include <stdio.h>
#include <stdlib.h>

void freeptr(char *ptr)
{
	//free(ptr);
	//ptr = NULL;
	printf("ptr = %p\n", ptr); //1つめ
	printf("&ptr = %p\n", &ptr); //1つめ
}

int main()
{
	char *test = (char *)malloc(sizeof(char) * (10 + 1));
	char *tmp = "1234567890";
	int i = 0;
	while (i < 10)
	{
		test[i] = tmp[i];
		i++;
	}
	printf("test = %p\n", test); //1つめ
	printf("&test = %p\n", &test); //1つめ
	printf("test(str) = %s\n",test);
	printf("\n\n");

	freeptr(test); 
	printf("\n\n");
	printf("test = %p\n", test); //1つめ
	printf("&test = %p\n", &test); //1つめ
	printf("test(str) = %s\n",test);

	return 0;
}