#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
/*
Función que permite obtener la matriz de listas
que es contenido en el archivo de entrada
entrada: nombre del archivo que debe ser leído, se asume que está
				en la misma carpeta que el ejecutable
*/
int ** leerListas(char* entrada){
	FILE* archivoEntrada;
	if((archivoEntrada = fopen(entrada,"r"))==NULL){
		printf("Error: Verifica la existencia del archivo. \n");
	} else{
		int lineas = contarLineas(archivoEntrada);
		fopen(entrada,"r");
		int ** conjuntoDeListas = malloc(sizeof(int*)*lineas);
    int j=0;
		while(j<lineas){
			int cantidad;
			fscanf(archivoEntrada,"%d",&cantidad);
			int i;
			int* lista = malloc(sizeof(int)*(cantidad+1));
			lista[0]=cantidad;
			for(i=1; i<=cantidad;i++){
				fscanf(archivoEntrada,"%d",&lista[i]);
				conjuntoDeListas[j] = lista;
			}
			j++;
		}
		return conjuntoDeListas;
  }
}

/*
	Permite contar la cantidad de líneas que posee el archivo
	para así posteriormente leer las listas
	archivo = archivo al cual se le contarán las líneas
*/
int contarLineas(FILE* archivo){
  int i; //Cantidad de líneas
  char temporal;
  while((temporal=fgetc(archivo))!=EOF){
    if(temporal=='\n'){
      i++;
      }
  }
	i++;
  fclose(archivo);
  return i;
}

/*
	Función que permite eleminar los elementos repetidas de las listas leídas
	para así cumplir con la definición de conjuntos (no poseen repetidos)
	A = lista de elementos del conjunto
*/
int*  eliminarRepetidos(int * A){
	int * B = malloc(sizeof(int)*(A[0]+1)); //lista sin repetidos
	int i;
	int k;
	int flag = 0;
	B[0] = 0;
	for(i=1;i<=A[0];i++){
		for(k=1;k<=B[0];k++){
			if(B[k]==A[i]){
				flag = 0;
				break;
			}
			else{
				flag = 1;
			}
		}
		if(i==1){
			B[1] = A[1]; //Se agrega un elementos
			B[0] =B[0]+1; //Se aumenta el largo de la lista
		}
		if(flag == 1){
			B[k] = A[i]; //Si no se encontró un elemento repetido, el nuevo se agrega
			B[0] = B[0]+1; //a la lista sin repetidos;
			flag = 0;
		}
	}
	free(A); //Se elimina la lista de entrada
	A = NULL; //Se deja apuntando a null
	B = realloc(B,B[0]+1); //Se realoca 1313 la memoria, para la lista nueva
	return B;
}

/*Fimco+pm ŕomcoáñ*/
int main(int argc,char*argv[]){
  int ** listas = leerListas("prueba");
	int i,j;
	int cantListas = contarLineas(fopen("prueba","r"));

	printf("Cantidad de listas leídas %d\n",cantListas);
	//Se eliminan los elementos repetidos de cada una de las listas leídas
	for(i=0;i<cantListas;i++){
	listas[i] =eliminarRepetidos(listas[i]);
		for(j=0;j<=listas[i][0];j++){
			if(j==0) printf("Lista %d, con repetidos eliminados.\nCantidad de elementos %d\n",i,listas[i][j]);
			printf("%d \n",listas[i][j]);
		}
	}
  return 0;
}
