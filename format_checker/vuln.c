#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <wchar.h>
#include <locale.h>

#define BUFSIZE 64
#define FLAGSIZE 64

void loadformato(char* buf, size_t len) {
	
	FILE *f = fopen("formato.txt","r");
	if (f == NULL) {
		printf("%s", "Parece que'formato.txt' no existe en tu ordenador (puedes crearlo para probar el programa).\n");
		fflush(stdin);
		fflush(stdout);
		exit(0);
	}

	fgets(buf,len,f); // size bound read
	fclose(f);
}

void vuln(){
	char formato[BUFSIZE];
	char buff[128];

	loadformato(formato, FLAGSIZE);

	printf("Dejame comprobar el formato de la contraseña >> ");
	scanf("%127s", buff);
	printf("Contraseña introducida: ");
	printf(buff);
	if (!strcmp(buff, formato)){
		printf("\nTu contraseña es muy segura! :)\n");
		printf("%s", getenv("FLAG"));
	}
	else{
		printf("\nDeberias cambiar la contraseña! >:(\n");
	}
	printf("\n");
}

int main(int argc, char **argv){

	setvbuf(stdout, NULL, _IONBF, 0);
	
	// Set the gid to the effective gid
	// this prevents /bin/sh from dropping the privileges
	gid_t gid = getegid();
	setresgid(gid, gid, gid);
	vuln();
	return 0;
}
