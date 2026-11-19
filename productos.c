#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
char archivo[]="productos.bin";

struct producto{
   int clave;
   char nombre[30];
   float precio;
   int cantidad;
};

void agregar();
void mostrar();
void leecad(char *cad,int n);

int main(){
   char op;
   do{
      printf("\n\n   PRODUCTOS");
      printf("\na) Agregar productos");
      printf("\nb) Mostrar productos");
      printf("\nc) Salir");
      printf("\nOpcion: ");
      scanf(" %c",&op);
      getchar(); //quita el salto de linea
      op=tolower(op);
      switch(op){
         case 'a':
            agregar();
            break;
         case 'b':
            mostrar();
            break;
         case 'c':
            printf("\nFin del programa\n");
            break;
         default:
            printf("\nOpcion no valida");
      }
   }while(op!='c');
   return 0;
}

//lee los productos y los guarda al final del archivo
void agregar(){
   FILE *fp;
   struct producto reg;
   char op;
   if((fp=fopen(archivo,"ab"))==NULL){
      printf("ERROR: No se puede abrir el archivo\n");
      exit(1);
   }
   do{
      printf("\nClave: ");
      scanf("%d",&reg.clave);
      getchar();
      printf("Nombre: ");
      leecad(reg.nombre,30);
      printf("Precio: ");
      scanf("%f",&reg.precio);
      printf("Cantidad: ");
      scanf("%d",&reg.cantidad);
      fwrite(&reg,sizeof(reg),1,fp);
      printf("\nProducto guardado");
      do{
         printf("\nDesea agregar otro producto? (s/n) ");
         scanf(" %c",&op);
         getchar();
         op=tolower(op);
      }while(op!='s' && op!='n');
   }while(op=='s');
   fclose(fp);
}

//despliega todos los productos del archivo
void mostrar(){
   FILE *fp;
   struct producto reg;
   int total=0;
   if((fp=fopen(archivo,"rb"))==NULL){
      printf("ERROR: No se puede abrir el archivo\n");
      return;
   }
   printf("\n%-8s%-30s%10s%10s","Clave","Nombre","Precio","Cantidad");
   fread(&reg,sizeof(reg),1,fp);
   while(!feof(fp)){
      printf("\n%-8d%-30s%10.2f%10d",reg.clave,reg.nombre,reg.precio,
             reg.cantidad);
      total++;
      fread(&reg,sizeof(reg),1,fp);
   }
   printf("\n\nTotal de productos: %d",total);
   fclose(fp);
}

//lee una cadena del teclado y le quita el salto de linea
void leecad(char *cad,int n){
   int lon;
   fgets(cad,n,stdin);
   lon=strlen(cad);
   if(cad[lon-1]=='\n'){
      cad[lon-1]='\0';
   }
}
