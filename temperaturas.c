#include <stdio.h>
#include <stdlib.h>

int main(){
   FILE *archivo;
   float temperatura[6][5];
   float promedio_sensor[5],promedio_intervalo[6],incremento[5];
   float suma,temp_maxima,temp_minima;
   int intervalo,sensor;
   int sensor_maxima,intervalo_maxima,sensor_minima,intervalo_minima;
   int sensor_mayor_incremento,todos_iguales;

   if((archivo=fopen("temperaturas.txt","r"))==NULL){
      printf("ERROR: No se puede abrir el archivo temperaturas.txt\n");
      exit(1);
   }
   for(intervalo=0;intervalo<6;intervalo++){
      for(sensor=0;sensor<5;sensor++){
         fscanf(archivo,"%f",&temperatura[intervalo][sensor]);
      }
   }
   fclose(archivo);

   printf("\nMatriz de temperaturas:");
   printf("\nTiempo\tS1\tS2\tS3\tS4\tS5");
   for(intervalo=0;intervalo<6;intervalo++){
      printf("\n%d",intervalo+1);
      for(sensor=0;sensor<5;sensor++){
         printf("\t%.1f",temperatura[intervalo][sensor]);
      }
   }

   for(sensor=0;sensor<5;sensor++){
      suma=0;
      for(intervalo=0;intervalo<6;intervalo++){
         suma=suma+temperatura[intervalo][sensor];
      }
      promedio_sensor[sensor]=suma/6;
   }

   for(intervalo=0;intervalo<6;intervalo++){
      suma=0;
      for(sensor=0;sensor<5;sensor++){
         suma=suma+temperatura[intervalo][sensor];
      }
      promedio_intervalo[intervalo]=suma/5;
   }

   temp_maxima=temperatura[0][0];
   sensor_maxima=0;
   intervalo_maxima=0;
   temp_minima=temperatura[0][0];
   sensor_minima=0;
   intervalo_minima=0;
   for(intervalo=0;intervalo<6;intervalo++){
      for(sensor=0;sensor<5;sensor++){
         if(temperatura[intervalo][sensor]>temp_maxima){
            temp_maxima=temperatura[intervalo][sensor];
            sensor_maxima=sensor;
            intervalo_maxima=intervalo;
         }
         if(temperatura[intervalo][sensor]<temp_minima){
            temp_minima=temperatura[intervalo][sensor];
            sensor_minima=sensor;
            intervalo_minima=intervalo;
         }
      }
   }

   for(sensor=0;sensor<5;sensor++){
      incremento[sensor]=temperatura[5][sensor]-temperatura[0][sensor];
   }

   sensor_mayor_incremento=0;
   todos_iguales=1;
   for(sensor=1;sensor<5;sensor++){
      if(incremento[sensor]>incremento[sensor_mayor_incremento]){
         sensor_mayor_incremento=sensor;
      }
      if(incremento[sensor]!=incremento[0]){
         todos_iguales=0;
      }
   }

   printf("\n\nTemperatura promedio por sensor:");
   for(sensor=0;sensor<5;sensor++){
      printf("\nS%d\t%.2f",sensor+1,promedio_sensor[sensor]);
   }
   printf("\n\nTemperatura promedio por intervalo:");
   for(intervalo=0;intervalo<6;intervalo++){
      printf("\n%d\t%.2f",intervalo+1,promedio_intervalo[intervalo]);
   }
   printf("\n\nTemperatura maxima: %.1f C",temp_maxima);
   printf("\nSensor de temperatura maxima: S%d",sensor_maxima+1);
   printf("\nIntervalo de temperatura maxima: %d",intervalo_maxima+1);
   printf("\nTemperatura minima: %.1f C",temp_minima);
   printf("\nSensor de temperatura minima: S%d",sensor_minima+1);
   printf("\nIntervalo de temperatura minima: %d",intervalo_minima+1);
   printf("\nIncremento de temperatura de cada sensor:");
   for(sensor=0;sensor<5;sensor++){
      printf("\nDT%d = %.1f C",sensor+1,incremento[sensor]);
   }
   if(todos_iguales==1){
      printf("\nTodos los sensores presentan el mismo incremento");
   }else{
      printf("\nSensor con mayor incremento: S%d",sensor_mayor_incremento+1);
   }

   if((archivo=fopen("reporte.txt","w"))==NULL){
      printf("ERROR: No se puede abrir el archivo reporte.txt\n");
      exit(1);
   }
   fprintf(archivo,"Matriz de temperaturas:\n");
   fprintf(archivo,"Tiempo\tS1\tS2\tS3\tS4\tS5\n");
   for(intervalo=0;intervalo<6;intervalo++){
      fprintf(archivo,"%d",intervalo+1);
      for(sensor=0;sensor<5;sensor++){
         fprintf(archivo,"\t%.1f",temperatura[intervalo][sensor]);
      }
      fprintf(archivo,"\n");
   }
   fprintf(archivo,"\nTemperatura promedio por sensor:\n");
   for(sensor=0;sensor<5;sensor++){
      fprintf(archivo,"S%d\t%.2f\n",sensor+1,promedio_sensor[sensor]);
   }
   fprintf(archivo,"\nTemperatura promedio por intervalo:\n");
   for(intervalo=0;intervalo<6;intervalo++){
      fprintf(archivo,"%d\t%.2f\n",intervalo+1,promedio_intervalo[intervalo]);
   }
   fprintf(archivo,"\nResumen estadistico:\n");
   fprintf(archivo,"Temperatura maxima: %.1f C\n",temp_maxima);
   fprintf(archivo,"Sensor de temperatura maxima: S%d\n",sensor_maxima+1);
   fprintf(archivo,"Intervalo de temperatura maxima: %d\n",intervalo_maxima+1);
   fprintf(archivo,"Temperatura minima: %.1f C\n",temp_minima);
   fprintf(archivo,"Sensor de temperatura minima: S%d\n",sensor_minima+1);
   fprintf(archivo,"Intervalo de temperatura minima: %d\n",intervalo_minima+1);
   fprintf(archivo,"Incremento de temperatura de cada sensor:\n");
   for(sensor=0;sensor<5;sensor++){
      fprintf(archivo,"DT%d = %.1f C\n",sensor+1,incremento[sensor]);
   }
   if(todos_iguales==1){
      fprintf(archivo,"Todos los sensores presentan el mismo incremento\n");
   }else{
      fprintf(archivo,"Sensor con mayor incremento: S%d\n",
              sensor_mayor_incremento+1);
   }
   fclose(archivo);
   printf("\nSe genero el archivo reporte.txt\n");
   return 0;
}
