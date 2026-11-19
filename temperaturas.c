#include <stdio.h>
#include <stdlib.h>
#define F 6
#define S 5
void leer(char *archivo,float t[][S]);
void mostrar(float t[][S]);
void promedios(float t[][S],float psen[],float pint[]);
void reporte(char *archivo,float t[][S],float psen[],float pint[],
             float delta[],float maxi,int smax,int imax,
             float mini,int smin,int imin,int sinc,int iguales);

int main(){
   float t[F][S],psen[S],pint[F],delta[S],maxi,mini;
   int i,j,smax,imax,smin,imin,sinc,iguales;
   leer("temperaturas.txt",t);
   mostrar(t);
   promedios(t,psen,pint);
   maxi=mini=t[0][0];
   smax=smin=imax=imin=0;
   for(i=0;i<F;i++){
      for(j=0;j<S;j++){
         if(t[i][j]>maxi){
            maxi=t[i][j];
            imax=i;
            smax=j;
         }
         if(t[i][j]<mini){
            mini=t[i][j];
            imin=i;
            smin=j;
         }
      }
   }
   for(j=0;j<S;j++)
      delta[j]=t[F-1][j]-t[0][j];
   sinc=0;
   iguales=1;
   for(j=1;j<S;j++){
      if(delta[j]>delta[sinc])
         sinc=j;
      if(delta[j]!=delta[0])
         iguales=0;
   }
   printf("\nTemperatura promedio por sensor:");
   for(j=0;j<S;j++)
      printf("\n   S%d: %.2f",j+1,psen[j]);
   printf("\nTemperatura promedio por intervalo:");
   for(i=0;i<F;i++)
      printf("\n   %d: %.2f",i+1,pint[i]);
   printf("\nTemperatura maxima: %.1f C (sensor S%d, intervalo %d)",
          maxi,smax+1,imax+1);
   printf("\nTemperatura minima: %.1f C (sensor S%d, intervalo %d)",
          mini,smin+1,imin+1);
   printf("\nIncremento de temperatura de cada sensor:");
   for(j=0;j<S;j++)
      printf("\n   DT%d = %.1f C",j+1,delta[j]);
   if(iguales)
      printf("\nTodos los sensores presentan el mismo incremento");
   else
      printf("\nSensor con mayor incremento: S%d",sinc+1);
   reporte("reporte.txt",t,psen,pint,delta,maxi,smax,imax,
           mini,smin,imin,sinc,iguales);
   printf("\nSe genero el archivo reporte.txt\n");
   return 0;
}

void leer(char *archivo,float t[][S]){
   FILE *fp;
   int i,j;
   if((fp=fopen(archivo,"r"))==NULL){
      printf("ERROR: No se puede abrir el archivo %s\n",archivo);
      exit(1);
   }
   for(i=0;i<F;i++)
      for(j=0;j<S;j++)
         fscanf(fp,"%f",&t[i][j]);
   fclose(fp);
}

void mostrar(float t[][S]){
   int i,j;
   printf("\nMatriz de temperaturas:");
   printf("\nTiempo     S1     S2     S3     S4     S5");
   for(i=0;i<F;i++){
      printf("\n%6d",i+1);
      for(j=0;j<S;j++)
         printf(" %6.1f",t[i][j]);
   }
   printf("\n");
}

void promedios(float t[][S],float psen[],float pint[]){
   int i,j;
   for(j=0;j<S;j++){
      psen[j]=0.0;
      for(i=0;i<F;i++)
         psen[j]+=t[i][j];
      psen[j]/=F;
   }
   for(i=0;i<F;i++){
      pint[i]=0.0;
      for(j=0;j<S;j++)
         pint[i]+=t[i][j];
      pint[i]/=S;
   }
}

void reporte(char *archivo,float t[][S],float psen[],float pint[],
             float delta[],float maxi,int smax,int imax,
             float mini,int smin,int imin,int sinc,int iguales){
   FILE *fp;
   int i,j;
   if((fp=fopen(archivo,"w"))==NULL){
      printf("ERROR: No se puede abrir el archivo %s\n",archivo);
      exit(1);
   }
   fprintf(fp,"Matriz de temperaturas:\n");
   fprintf(fp,"Tiempo     S1     S2     S3     S4     S5\n");
   for(i=0;i<F;i++){
      fprintf(fp,"%6d",i+1);
      for(j=0;j<S;j++)
         fprintf(fp," %6.1f",t[i][j]);
      fprintf(fp,"\n");
   }
   fprintf(fp,"\nTemperatura promedio por sensor:\n");
   fprintf(fp,"Sensor   Temperatura promedio (C)\n");
   for(j=0;j<S;j++)
      fprintf(fp,"  S%d     %10.2f\n",j+1,psen[j]);
   fprintf(fp,"\nTemperatura promedio por intervalo:\n");
   fprintf(fp,"Intervalo   Temperatura promedio (C)\n");
   for(i=0;i<F;i++)
      fprintf(fp,"%5d       %10.2f\n",i+1,pint[i]);
   fprintf(fp,"\nResumen estadistico:\n");
   fprintf(fp,"Temperatura maxima: %.1f C\n",maxi);
   fprintf(fp,"Sensor de temperatura maxima: S%d\n",smax+1);
   fprintf(fp,"Intervalo de temperatura maxima: %d\n",imax+1);
   fprintf(fp,"Temperatura minima: %.1f C\n",mini);
   fprintf(fp,"Sensor de temperatura minima: S%d\n",smin+1);
   fprintf(fp,"Intervalo de temperatura minima: %d\n",imin+1);
   fprintf(fp,"Incremento de temperatura de cada sensor:\n");
   for(j=0;j<S;j++)
      fprintf(fp,"   DT%d = %.1f C\n",j+1,delta[j]);
   if(iguales)
      fprintf(fp,"Todos los sensores presentan el mismo incremento\n");
   else
      fprintf(fp,"Sensor con mayor incremento: S%d\n",sinc+1);
   fclose(fp);
}
