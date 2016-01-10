#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int ** leerListas(char* entrada){
	FILE* archivoEntrada;
	printf("Se inicia la lectura del archivo\n");
	if((archivoEntrada = fopen(entrada,"r"))==NULL){
		printf("Error: Verifica la existencia del archivo. \n");
	} else{
		int lineas = contarLineas(archivoEntrada);
		fopen(entrada,"r");
		int ** conjuntoDeListas = malloc(sizeof(int*)*lineas);
    int j=0;
		printf("Reserva memoria para las listas\n");
		while(j<lineas){
			printf("Línea: %d\n",j);
			int cantidad;
			fscanf(archivoEntrada,"%d",&cantidad);
			printf("%d\n",cantidad );
			int i;
			int* lista = malloc(sizeof(int)*(cantidad+1));
			lista[0]=cantidad;
			for(i=1; i<=cantidad;i++){
				fscanf(archivoEntrada,"%d",&lista[i]);
				conjuntoDeListas[j] = lista;
				printf(" %d\n",conjuntoDeListas[j][i]);
			}
			j++;
		}
		return conjuntoDeListas;
  }
}

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
  printf("Cantidad de líneas: %d\n", i);
  return i;
}

int main(int argc,char*argv[]){
  int ** listas = leerListas("prueba");
	int i,j;
	for(i=0;i<3;i++){
		for(j=0;j<4;j++){
			printf("(%d)\n",listas[i][j]);
		}
	}
  return 0;
}
