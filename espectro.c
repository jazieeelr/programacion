#include <stdio.h>
#include <stdlib.h>

int main(){
   FILE *archivo;
   float wl[1000],inten[1000],inten_suave[1000],picos[1000];
   float umbral,suma;
   int indice_pico[1000];
   int total_puntos,vecinos,ventana,total_picos,caracter,i,m;

   if((archivo=fopen("espectro.dat","r"))==NULL){
      printf("ERROR: No se puede abrir el archivo espectro.dat\n");
      exit(1);
   }
   caracter=getc(archivo);
   if(caracter==EOF){
      printf("ERROR: El archivo espectro.dat esta vacio\n");
      exit(1);
   }
   rewind(archivo);
   total_puntos=0;
   while(!feof(archivo)){
      fscanf(archivo,"%f %f\n",&wl[total_puntos],&inten[total_puntos]);
      total_puntos=total_puntos+1;
   }
   fclose(archivo);
   printf("\nSe leyeron %d puntos del archivo espectro.dat",total_puntos);

   printf("\nIngrese el numero de vecinos k: ");
   scanf("%d",&vecinos);
   printf("Ingrese el umbral: ");
   scanf("%f",&umbral);
   ventana=2*vecinos+1;

   for(i=0;i<total_puntos;i++){
      if(i<vecinos){
         inten_suave[i]=inten[i];
      }else{
         if(i>=total_puntos-vecinos){
            inten_suave[i]=inten[i];
         }else{
            suma=0;
            for(m=-vecinos;m<=vecinos;m++){
               suma=suma+inten[i+m];
            }
            inten_suave[i]=suma/ventana;
         }
      }
   }

   total_picos=0;
   for(i=1;i<total_puntos-1;i++){
      if(inten_suave[i]>inten_suave[i-1]){
         if(inten_suave[i]>inten_suave[i+1]){
            if(inten_suave[i]>umbral){
               picos[total_picos]=wl[i];
               indice_pico[total_picos]=i;
               total_picos=total_picos+1;
            }
         }
      }
   }

   printf("\nVentana w = %d (k = %d), umbral = %.1f",ventana,vecinos,umbral);
   printf("\nPicos encontrados: %d",total_picos);
   for(i=0;i<total_picos;i++){
      printf("\nIndice: %d   Lambda: %.1f   Intensidad suave: %.2f",
             indice_pico[i],picos[i],inten_suave[indice_pico[i]]);
   }

   if((archivo=fopen("reporte.txt","w"))==NULL){
      printf("ERROR: No se puede abrir el archivo reporte.txt\n");
      exit(1);
   }
   fprintf(archivo,"RESUMEN DE PICOS\n");
   fprintf(archivo,"Picos encontrados: %d\n",total_picos);
   fprintf(archivo,"indice\tlambda\tintensidad_suave\n");
   for(i=0;i<total_picos;i++){
      fprintf(archivo,"%d\t%.1f\t%.2f\n",
              indice_pico[i],picos[i],inten_suave[indice_pico[i]]);
   }
   fprintf(archivo,"\nSERIE COMPLETA\n");
   fprintf(archivo,"lambda\tintensidad_raw\tintensidad_suave\n");
   for(i=0;i<total_puntos;i++){
      fprintf(archivo,"%.1f\t%.2f\t\t%.2f\n",wl[i],inten[i],inten_suave[i]);
   }
   fclose(archivo);
   printf("\nSe genero el archivo reporte.txt\n");
   return 0;
}
