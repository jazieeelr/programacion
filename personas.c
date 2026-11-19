#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define nnom 50
#define ndir 80

struct persona{
   char nombre[nnom];
   int edad;
   int dia;
   int mes;
   int anio;
   char direccion[ndir];
   char civil[15];
   char sexo;
}reg;

int menu(void);
void agregar(void);
void listar(void);
void buscar(void);
void mostrar(void);
void leercad(char cad[],int n);
int leerent(void);
char leercar(void);

int main(){
   int op;
   do{
      op=menu();
      switch(op){
         case 1:
            agregar();
            break;
         case 2:
            listar();
            break;
         case 3:
            buscar();
            break;
      }
   }while(op!=4);
   return 0;
}

int menu(void){
   int c;
   do{
      printf("\n\nREGISTRO DE PERSONAS\n\n");
      printf("1. Agregar persona\n");
      printf("2. Listar personas\n");
      printf("3. Buscar por nombre\n");
      printf("4. Terminar\n");
      printf("\nSu opcion... ");
      c=leerent();
   }while(c<1 || c>4);
   return c;
}

//lee los datos de una persona y los agrega al final del archivo
void agregar(void){
   FILE *fp;
   char archivo[]="personas.bin";
   int op;
   if((fp=fopen(archivo,"ab"))==NULL){
      printf("ERROR: No se puede abrir el archivo\n");
      exit(1);
   }
   printf("\nNombre: ");
   leercad(reg.nombre,nnom);
   do{
      printf("Edad: ");
      reg.edad=leerent();
   }while(reg.edad<0);
   printf("Fecha de nacimiento\n");
   do{
      printf("   Dia (1-31): ");
      reg.dia=leerent();
   }while(reg.dia<1 || reg.dia>31);
   do{
      printf("   Mes (1-12): ");
      reg.mes=leerent();
   }while(reg.mes<1 || reg.mes>12);
   do{
      printf("   Anio: ");
      reg.anio=leerent();
   }while(reg.anio<0);
   printf("Direccion: ");
   leercad(reg.direccion,ndir);
   do{
      printf("Estado civil\n");
      printf("   1. Soltero\n   2. Casado\n   3. Divorciado\n");
      printf("   4. Viudo\n   5. Union libre\n");
      printf("   Su opcion... ");
      op=leerent();
   }while(op<1 || op>5);
   switch(op){
      case 1:
         strcpy(reg.civil,"Soltero");
         break;
      case 2:
         strcpy(reg.civil,"Casado");
         break;
      case 3:
         strcpy(reg.civil,"Divorciado");
         break;
      case 4:
         strcpy(reg.civil,"Viudo");
         break;
      case 5:
         strcpy(reg.civil,"Union libre");
         break;
   }
   do{
      printf("Sexo (M/F): ");
      reg.sexo=toupper(leercar());
   }while(reg.sexo!='M' && reg.sexo!='F');
   fwrite(&reg,sizeof(reg),1,fp);
   if(ferror(fp)){
      perror("Error durante la escritura");
      exit(2);
   }
   fclose(fp);
   printf("\nPersona guardada en %s\n",archivo);
}

//despliega todos los registros del archivo
void listar(void){
   FILE *fp;
   char archivo[]="personas.bin";
   int cuenta=0;
   if((fp=fopen(archivo,"rb"))==NULL){
      printf("\nNo hay personas registradas\n");
      return;
   }
   fread(&reg,sizeof(reg),1,fp);
   while(!feof(fp)){
      cuenta++;
      printf("\n%d.",cuenta);
      mostrar();
      fread(&reg,sizeof(reg),1,fp);
   }
   fclose(fp);
   printf("\n\nTotal: %d persona(s)\n",cuenta);
}

//despliega los registros cuyo nombre es igual al buscado
void buscar(void){
   FILE *fp;
   char archivo[]="personas.bin";
   char nom[nnom];
   int cuenta=0;
   if((fp=fopen(archivo,"rb"))==NULL){
      printf("\nNo hay personas registradas\n");
      return;
   }
   printf("\nNombre a buscar: ");
   leercad(nom,nnom);
   fread(&reg,sizeof(reg),1,fp);
   while(!feof(fp)){
      if(!strcmp(nom,reg.nombre)){
         cuenta++;
         mostrar();
      }
      fread(&reg,sizeof(reg),1,fp);
   }
   fclose(fp);
   if(cuenta==0){
      printf("\nNo se encontro a %s\n",nom);
   }
}

//despliega la persona guardada en la variable global reg
void mostrar(void){
   printf("\nNombre: %s",reg.nombre);
   printf("\nEdad: %d",reg.edad);
   printf("\nFecha de nacimiento: %d/%d/%d",reg.dia,reg.mes,reg.anio);
   printf("\nDireccion: %s",reg.direccion);
   printf("\nEstado civil: %s",reg.civil);
   printf("\nSexo: %c\n",reg.sexo);
}

//lee una linea del teclado y le quita el salto de linea
void leercad(char cad[],int n){
   int c;
   do{
      if(fgets(cad,n,stdin)==NULL){
         exit(0);   //se termino la entrada
      }
      if(cad[strlen(cad)-1]=='\n'){
         cad[strlen(cad)-1]='\0';
      }else{
         //la linea era mas larga que la cadena, se ignora el resto
         c=getchar();
         while(c!='\n' && c!=EOF){
            c=getchar();
         }
      }
      if(strlen(cad)==0){
         printf("No puede quedar vacio, ingrese de nuevo: ");
      }
   }while(strlen(cad)==0);
}

//lee un entero; si no se escribio un numero regresa -1
int leerent(void){
   int n,c,car;
   c=scanf("%d",&n);
   //ignorando el resto de la linea, incluido el enter
   car=getchar();
   while(car!='\n' && car!=EOF){
      car=getchar();
   }
   if(car==EOF){
      exit(0);   //se termino la entrada
   }
   if(c!=1){
      return -1;
   }
   return n;
}

//lee el primer caracter de la linea e ignora el resto
char leercar(void){
   int c,car;
   c=getchar();
   car=c;
   while(car!='\n' && car!=EOF){
      car=getchar();
   }
   return c;
}
