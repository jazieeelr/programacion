#include <stdio.h>
#include <stdlib.h>

int main(){
   FILE *archivo;
   double matriz[10][10],cuadrada[10][10];
   double suma_fila[10],eigenvector[10];
   double dato,suma_total;
   int orden,fila,columna,k,caracter;

   printf("\nIngrese el orden de la matriz: ");
   scanf("%d",&orden);

   if((archivo=fopen("entrada.bin","rb"))==NULL){
      printf("\nNo existe el archivo entrada.bin, se va a crear\n");
      if((archivo=fopen("entrada.bin","wb"))==NULL){
         printf("ERROR: No se puede crear el archivo entrada.bin\n");
         exit(1);
      }
      for(fila=0;fila<orden;fila++){
         for(columna=0;columna<orden;columna++){
            printf("Ingrese el elemento [%d][%d]: ",fila,columna);
            scanf("%lf",&dato);
            fwrite(&dato,sizeof(double),1,archivo);
         }
      }
      fclose(archivo);
      printf("\nSe genero el archivo entrada.bin\n");
      if((archivo=fopen("entrada.bin","rb"))==NULL){
         printf("ERROR: No se puede abrir el archivo entrada.bin\n");
         exit(1);
      }
   }else{
      printf("\nSe abrio el archivo entrada.bin para lectura\n");
   }
   for(fila=0;fila<orden;fila++){
      for(columna=0;columna<orden;columna++){
         fread(&matriz[fila][columna],sizeof(double),1,archivo);
      }
   }
   fclose(archivo);

   for(fila=0;fila<orden;fila++){
      for(columna=0;columna<orden;columna++){
         cuadrada[fila][columna]=0;
         for(k=0;k<orden;k++){
            cuadrada[fila][columna]=cuadrada[fila][columna]+
                                    matriz[fila][k]*matriz[k][columna];
         }
      }
   }

   suma_total=0;
   for(fila=0;fila<orden;fila++){
      suma_fila[fila]=0;
      for(columna=0;columna<orden;columna++){
         suma_fila[fila]=suma_fila[fila]+cuadrada[fila][columna];
      }
      suma_total=suma_total+suma_fila[fila];
   }

   for(fila=0;fila<orden;fila++){
      eigenvector[fila]=suma_fila[fila]/suma_total;
   }

   printf("\nContenido de entrada.bin (matriz inicial):");
   for(fila=0;fila<orden;fila++){
      printf("\n");
      for(columna=0;columna<orden;columna++){
         printf("%.4f\t",matriz[fila][columna]);
      }
   }
   printf("\n\nMatriz al cuadrado:");
   for(fila=0;fila<orden;fila++){
      printf("\n");
      for(columna=0;columna<orden;columna++){
         printf("%.4f\t",cuadrada[fila][columna]);
      }
   }
   printf("\n\nSuma de elementos por fila:");
   for(fila=0;fila<orden;fila++){
      printf("\n%.4f",suma_fila[fila]);
   }
   printf("\n\nSuma total de elementos:");
   printf("\n%f",suma_total);
   printf("\n\nEigenvector:");
   for(fila=0;fila<orden;fila++){
      printf("\n%.4f",eigenvector[fila]);
   }

   if((archivo=fopen("salida.txt","w"))==NULL){
      printf("ERROR: No se puede abrir el archivo salida.txt\n");
      exit(1);
   }
   fprintf(archivo,"Eigenvector\n");
   for(fila=0;fila<orden;fila++){
      fprintf(archivo,"%.4f\n",eigenvector[fila]);
   }
   fclose(archivo);

   if((archivo=fopen("salida.txt","r"))==NULL){
      printf("ERROR: No se puede abrir el archivo salida.txt\n");
      exit(1);
   }
   printf("\n\nContenido de salida.txt:\n");
   caracter=getc(archivo);
   while(caracter!=EOF){
      putchar(caracter);
      caracter=getc(archivo);
   }
   fclose(archivo);
   return 0;
}
