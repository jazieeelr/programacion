#include <stdio.h>
void inter(int *x, int *y);

int main(){
   int numero=10,x=3,y=7,i;
   int *ap,*p,**q,*nulo;
   int a[]={1,2,3,4,5};

   //1. Declarar una variable y un apuntador a ella
   ap=&numero;
   printf("\n1. Conceptos basicos\n");
   printf("\nContenido de numero: %d",numero);
   printf("\nDireccion de numero: %p",&numero);
   printf("\nContenido de ap: %p",ap);
   printf("\nContenido de la variable a la que apunta ap: %d",*ap);

   //2. Modificar la variable a traves del apuntador
   *ap=25;
   printf("\n\n2. Modificar mediante el apuntador\n");
   printf("\nDespues de *ap=25, numero vale: %d",numero);

   //3. Paso por referencia a una funcion
   printf("\n\n3. Paso por referencia\n");
   printf("\nAntes de inter: x = %d, y = %d",x,y);
   inter(&x,&y);
   printf("\nDespues de inter: x = %d, y = %d",x,y);

   //4. Apuntadores y arreglos
   p=a;
   printf("\n\n4. Apuntadores y arreglos\n");
   for(i=0;i<5;i++){
      printf("\na[%d] = %d   *(p + %d) = %d",i,a[i],i,*(p+i));
      printf("   direccion: %p",p+i);
   }

   //5. Apuntador a apuntador
   q=&ap;
   printf("\n\n5. Apuntador a apuntador\n");
   printf("\n**q = %d (mismo valor que numero)",**q);

   //6. Apuntador nulo
   nulo=NULL;
   printf("\n\n6. Apuntador NULL\n");
   if(nulo==NULL){
      printf("\nEl apuntador es NULL, no se debe usar *nulo\n");
   }
   return 0;
}

void inter(int *x, int *y){
   int aux;
   aux=*x;
   *x=*y;
   *y=aux;
}
