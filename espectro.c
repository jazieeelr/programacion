#include <stdio.h>
#include <stdlib.h>
int cargar(char *archivo,float wl[],float inten[]);
void suavizar(float inten[],float suave[],int n,int k);
int detectar(float wl[],float suave[],int n,float umbral,
             float picos[],int ipico[]);
void reporte(char *archivo,float wl[],float inten[],float suave[],int n,
             float picos[],int ipico[],int np);

int main(){
   float wl[1000],inten[1000],inten_suave[1000],picos[1000],umbral;
   int ipico[1000],n,k,np,i;
   n=cargar("espectro.dat",wl,inten);
   printf("\nSe leyeron %d puntos de espectro.dat",n);
   printf("\nTamano de la ventana w=2k+1, ingrese k: ");
   scanf("%d",&k);
   printf("Ingrese el umbral: ");
   scanf("%f",&umbral);
   suavizar(inten,inten_suave,n,k);
   np=detectar(wl,inten_suave,n,umbral,picos,ipico);
   printf("\nVentana w = %d (k = %d), umbral = %.1f",2*k+1,k,umbral);
   printf("\nPicos encontrados: %d",np);
   for(i=0;i<np;i++){
      printf("\n   Indice %d   lambda = %.1f nm   I = %.2f",
             ipico[i],picos[i],inten_suave[ipico[i]]);
   }
   reporte("reporte.txt",wl,inten,inten_suave,n,picos,ipico,np);
   printf("\nSe genero el archivo reporte.txt\n");
   return 0;
}

int cargar(char *archivo,float wl[],float inten[]){
   FILE *fp;
   int n=0;
   if((fp=fopen(archivo,"r"))==NULL){
      printf("ERROR: No se puede abrir el archivo %s\n",archivo);
      exit(1);
   }
   fseek(fp,0L,SEEK_END);
   if(ftell(fp)==0){
      printf("ERROR: El archivo %s esta vacio\n",archivo);
      exit(1);
   }
   rewind(fp);
   while(!feof(fp)&&n<1000){
      fscanf(fp,"%f %f\n",&wl[n],&inten[n]);
      n++;
   }
   fclose(fp);
   return n;
}

void suavizar(float inten[],float suave[],int n,int k){
   int i,m;
   float suma;
   for(i=0;i<n;i++){
      if(i<k||i>=n-k){
         suave[i]=inten[i];
      }else{
         suma=0.0;
         for(m=-k;m<=k;m++)
            suma+=inten[i+m];
         suave[i]=suma/(2*k+1);
      }
   }
}

int detectar(float wl[],float suave[],int n,float umbral,
             float picos[],int ipico[]){
   int i,np=0;
   for(i=1;i<n-1;i++){
      if(suave[i]>suave[i-1]&&suave[i]>suave[i+1]&&suave[i]>umbral){
         picos[np]=wl[i];
         ipico[np]=i;
         np++;
      }
   }
   return np;
}

void reporte(char *archivo,float wl[],float inten[],float suave[],int n,
             float picos[],int ipico[],int np){
   FILE *fp;
   int i;
   if((fp=fopen(archivo,"w"))==NULL){
      printf("ERROR: No se puede abrir el archivo %s\n",archivo);
      exit(1);
   }
   fprintf(fp,"RESUMEN DE PICOS\n");
   fprintf(fp,"Picos encontrados: %d\n",np);
   fprintf(fp,"%8s %10s %18s\n","indice","lambda","intensidad_suave");
   for(i=0;i<np;i++)
      fprintf(fp,"%8d %10.1f %18.2f\n",ipico[i],picos[i],suave[ipico[i]]);
   fprintf(fp,"\nSERIE COMPLETA\n");
   fprintf(fp,"%10s %16s %18s\n","lambda","intensidad_raw",
           "intensidad_suave");
   for(i=0;i<n;i++)
      fprintf(fp,"%10.1f %16.2f %18.2f\n",wl[i],inten[i],suave[i]);
   fclose(fp);
}
