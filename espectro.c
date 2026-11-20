#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_DATOS   1000
#define ARCHIVO_DAT "espectro.dat"
#define ARCHIVO_REP "reporte.txt"

int  crear_archivo(const char *nombre);
int  generar_datos(FILE *fp);
int  capturar_datos(FILE *fp);
int  cargar_datos(const char *nombre, double wl[], double inten[], int max);
void suavizar(const double inten[], double inten_suave[], int n, int k);
int  detectar_picos(const double wl[], const double inten_suave[], int n,
                    double umbral, double picos[], int idx_picos[]);
int  generar_reporte(const char *nombre, const double wl[], const double inten[],
                     const double inten_suave[], int n, int k, double umbral,
                     const double picos[], const int idx_picos[], int num_picos);
double aleatorio(double a, double b);
void limpiar_buffer(void);

int main(void)
{
    double wl[MAX_DATOS], inten[MAX_DATOS], inten_suave[MAX_DATOS];
    double picos[MAX_DATOS];
    int    idx_picos[MAX_DATOS];
    int    n, k, num_picos, i;
    double umbral;
    FILE  *fp;
    char   resp;

    srand((unsigned) time(NULL));

    printf("=== Analisis de espectro I(lambda) ===\n\n");

    fp = fopen(ARCHIVO_DAT, "r");
    if (fp == NULL) {
        printf("El archivo '%s' no existe.\n", ARCHIVO_DAT);
        printf("Desea crearlo ahora? (s/n): ");
        if (scanf(" %c", &resp) != 1 || (resp != 's' && resp != 'S')) {
            printf("No se puede continuar sin datos. Fin del programa.\n");
            return 1;
        }
        if (!crear_archivo(ARCHIVO_DAT)) {
            printf("Error: no se pudo crear el archivo.\n");
            return 1;
        }
    } else {
        fclose(fp);
    }

    n = cargar_datos(ARCHIVO_DAT, wl, inten, MAX_DATOS);
    if (n < 0) {
        printf("Error: no se pudo abrir '%s'.\n", ARCHIVO_DAT);
        return 1;
    }
    if (n == 0) {
        printf("Error: el archivo '%s' esta vacio o no tiene datos validos.\n",
               ARCHIVO_DAT);
        return 1;
    }
    printf("\nSe leyeron %d puntos de '%s'.\n", n, ARCHIVO_DAT);

    do {
        printf("Ingrese k (vecinos a cada lado, ventana w = 2k+1): ");
        if (scanf("%d", &k) != 1) { limpiar_buffer(); k = -1; }
        if (k < 1 || 2 * k + 1 > n)
            printf("  k debe ser >= 1 y la ventana no puede exceder %d puntos.\n", n);
    } while (k < 1 || 2 * k + 1 > n);

    printf("Ingrese el umbral de intensidad: ");
    while (scanf("%lf", &umbral) != 1) {
        limpiar_buffer();
        printf("  Valor invalido. Ingrese el umbral: ");
    }

    suavizar(inten, inten_suave, n, k);
    num_picos = detectar_picos(wl, inten_suave, n, umbral, picos, idx_picos);

    printf("\nVentana w = %d (k = %d), umbral = %.2f\n", 2 * k + 1, k, umbral);
    printf("\n%10s %18s %18s\n", "lambda", "intensidad_raw", "intensidad_suave");
    for (i = 0; i < n; i++)
        printf("%10.2f %18.4f %18.4f\n", wl[i], inten[i], inten_suave[i]);

    printf("\nPicos encontrados: %d\n", num_picos);
    for (i = 0; i < num_picos; i++)
        printf("  Indice %d -> lambda = %.2f nm, I_suave = %.4f\n",
               idx_picos[i], picos[i], inten_suave[idx_picos[i]]);

    if (generar_reporte(ARCHIVO_REP, wl, inten, inten_suave, n, k, umbral,
                        picos, idx_picos, num_picos))
        printf("\nReporte guardado en '%s'.\n", ARCHIVO_REP);
    else
        printf("\nError: no se pudo escribir '%s'.\n", ARCHIVO_REP);

    return 0;
}

int crear_archivo(const char *nombre)
{
    FILE *fp;
    int   opcion, ok;

    printf("\n--- Crear %s ---\n", nombre);
    printf("1) Generar un espectro simulado (picos + ruido)\n");
    printf("2) Capturar datos manualmente\n");
    printf("Opcion: ");
    if (scanf("%d", &opcion) != 1) { limpiar_buffer(); opcion = 0; }

    if (opcion != 1 && opcion != 2) {
        printf("Opcion invalida.\n");
        return 0;
    }

    fp = fopen(nombre, "w");
    if (fp == NULL)
        return 0;

    if (opcion == 1)
        ok = generar_datos(fp);
    else
        ok = capturar_datos(fp);

    fclose(fp);
    return ok;
}

int generar_datos(FILE *fp)
{
    double l_ini, l_fin, paso, l, I, x;
    double centro[10], altura[10], ancho[10];
    double fondo, ruido;
    int    n, num_lineas, i, j;

    printf("Lambda inicial (nm): ");
    while (scanf("%lf", &l_ini) != 1) { limpiar_buffer(); printf("  Repita: "); }

    do {
        printf("Lambda final (nm, mayor que la inicial): ");
        if (scanf("%lf", &l_fin) != 1) { limpiar_buffer(); l_fin = l_ini; }
    } while (l_fin <= l_ini);

    do {
        printf("Paso entre puntos (nm): ");
        if (scanf("%lf", &paso) != 1) { limpiar_buffer(); paso = 0.0; }
        n = (paso > 0.0) ? (int) ((l_fin - l_ini) / paso) + 1 : 0;
        if (paso > 0.0 && (n < 3 || n > MAX_DATOS))
            printf("  Con ese paso habria %d puntos (permitido: 3 - %d).\n", n, MAX_DATOS);
    } while (paso <= 0.0 || n < 3 || n > MAX_DATOS);

    do {
        printf("Numero de lineas espectrales (1 - 10): ");
        if (scanf("%d", &num_lineas) != 1) { limpiar_buffer(); num_lineas = 0; }
    } while (num_lineas < 1 || num_lineas > 10);

    fondo = aleatorio(5.0, 15.0);
    ruido = aleatorio(2.0, 6.0);

    for (j = 0; j < num_lineas; j++) {
        centro[j] = aleatorio(l_ini + 0.1 * (l_fin - l_ini),
                              l_fin - 0.1 * (l_fin - l_ini));
        altura[j] = aleatorio(40.0, 100.0);
        ancho[j]  = aleatorio(1.5 * paso, 3.0 * paso);
    }

    for (i = 0; i < n; i++) {
        l = l_ini + i * paso;
        I = fondo;
        for (j = 0; j < num_lineas; j++) {
            x = (l - centro[j]) / ancho[j];
            I += altura[j] / (1.0 + x * x);
        }
        I += aleatorio(-ruido, ruido);
        if (I < 0.0)
            I = 0.0;
        fprintf(fp, "%.1f %.1f\n", l, I);
    }

    printf("Archivo creado con %d puntos simulados.\n", n);
    return 1;
}

int capturar_datos(FILE *fp)
{
    int    n, i;
    double l, I;

    do {
        printf("Numero de puntos a capturar (1 - %d): ", MAX_DATOS);
        if (scanf("%d", &n) != 1) { limpiar_buffer(); n = 0; }
    } while (n < 1 || n > MAX_DATOS);

    for (i = 0; i < n; i++) {
        printf("Punto %d -> lambda (nm) e intensidad: ", i + 1);
        while (scanf("%lf %lf", &l, &I) != 2) {
            limpiar_buffer();
            printf("  Entrada invalida, repita: ");
        }
        fprintf(fp, "%.4f %.4f\n", l, I);
    }

    printf("Archivo creado con %d puntos.\n", n);
    return 1;
}

int cargar_datos(const char *nombre, double wl[], double inten[], int max)
{
    FILE *fp = fopen(nombre, "r");
    int   n = 0;

    if (fp == NULL)
        return -1;

    while (n < max && fscanf(fp, "%lf %lf", &wl[n], &inten[n]) == 2)
        n++;

    if (n == max)
        printf("Aviso: se alcanzo el maximo de %d datos.\n", max);

    fclose(fp);
    return n;
}

void suavizar(const double inten[], double inten_suave[], int n, int k)
{
    int    i, m;
    int    w = 2 * k + 1;
    double suma;

    for (i = 0; i < n; i++) {
        if (i < k || i > n - 1 - k) {
            inten_suave[i] = inten[i];
        } else {
            suma = 0.0;
            for (m = -k; m <= k; m++)
                suma += inten[i + m];
            inten_suave[i] = suma / w;
        }
    }
}

int detectar_picos(const double wl[], const double inten_suave[], int n,
                   double umbral, double picos[], int idx_picos[])
{
    int i, cont = 0;

    for (i = 1; i < n - 1; i++) {
        if (inten_suave[i] > inten_suave[i - 1] &&
            inten_suave[i] > inten_suave[i + 1] &&
            inten_suave[i] > umbral) {
            picos[cont]     = wl[i];
            idx_picos[cont] = i;
            cont++;
        }
    }
    return cont;
}

int generar_reporte(const char *nombre, const double wl[], const double inten[],
                    const double inten_suave[], int n, int k, double umbral,
                    const double picos[], const int idx_picos[], int num_picos)
{
    FILE *fp = fopen(nombre, "w");
    int   i;

    if (fp == NULL)
        return 0;

    fprintf(fp, "REPORTE DE ANALISIS ESPECTRAL\n");
    fprintf(fp, "=============================\n");
    fprintf(fp, "Archivo de datos : %s\n", ARCHIVO_DAT);
    fprintf(fp, "Numero de puntos : %d\n", n);
    fprintf(fp, "Ventana          : w = %d (k = %d)\n", 2 * k + 1, k);
    fprintf(fp, "Umbral           : %.2f\n\n", umbral);

    fprintf(fp, "RESUMEN DE PICOS (%d encontrados)\n", num_picos);
    fprintf(fp, "%8s %12s %18s\n", "indice", "lambda", "intensidad_suave");
    for (i = 0; i < num_picos; i++)
        fprintf(fp, "%8d %12.2f %18.4f\n",
                idx_picos[i], picos[i], inten_suave[idx_picos[i]]);
    if (num_picos == 0)
        fprintf(fp, "  (no se detectaron picos sobre el umbral)\n");

    fprintf(fp, "\nSERIE COMPLETA\n");
    fprintf(fp, "%10s %18s %18s\n", "lambda", "intensidad_raw", "intensidad_suave");
    for (i = 0; i < n; i++)
        fprintf(fp, "%10.2f %18.4f %18.4f\n", wl[i], inten[i], inten_suave[i]);

    fclose(fp);
    return 1;
}

double aleatorio(double a, double b)
{
    return a + (b - a) * ((double) rand() / RAND_MAX);
}

void limpiar_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
