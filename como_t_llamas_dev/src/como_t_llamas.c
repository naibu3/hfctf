#include <stdio.h>

void hacked()
{
	FILE *f;
	f = fopen("flag.txt", "r");
	char flag[100];
	fgets(flag, 100, f);
	printf("Esta funcion es secreta! Como has logrado ejecutarla?! :O %s", flag);
	fflush(stdout);
	fflush(stdin);
	fclose(f);
	return ;
}

void register_name()
{
    char buffer[16];

    printf("Como te llamas?: \n");
	fflush(stdout);
	fflush(stdin);
    scanf("%s", buffer);
	fflush(stdout);
	fflush(stdin);
    printf("Hola %s, lo siento, no conozco la FLAG :(\n", buffer);
	fflush(stdout);
	fflush(stdin);
}

int main()
{
    register_name();

    return 0;
}
