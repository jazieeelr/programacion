/*
 * Registro de personas usando un archivo de texto como "base de datos"
 *
 * Cada persona se guarda en una línea de "personas.txt" con los campos
 * separados por '|':
 *
 *   id|nombre|edad|fecha_nacimiento|direccion|estado_civil|sexo
 *   1|Juan Perez|30|15/03/1996|Av. Siempre Viva 742|Soltero|M
 *
 * Compilar:  gcc -Wall -o personas personas.c
 * Ejecutar:  ./personas
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define ARCHIVO     "personas.txt"
#define ARCHIVO_TMP "personas.tmp"
#define MAX_TEXTO   100
#define MAX_LINEA   512

typedef struct {
    int  id;
    char nombre[MAX_TEXTO];
    int  edad;
    char fecha_nacimiento[11];   /* dd/mm/aaaa */
    char direccion[MAX_TEXTO];
    char estado_civil[20];
    char sexo;                   /* 'M' o 'F' */
} Persona;

/* ---------- Utilidades de entrada ---------- */

/* Lee una línea del teclado y le quita el salto de línea */
void leer_texto(const char *mensaje, char *destino, int tam) {
    for (;;) {
        printf("%s", mensaje);
        if (fgets(destino, tam, stdin) == NULL) {
            destino[0] = '\0';
            return;
        }
        /* Si la línea no cabía, descartar lo que sobró */
        if (strchr(destino, '\n') == NULL) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
        }
        destino[strcspn(destino, "\n")] = '\0';

        /* El separador '|' rompería el formato del archivo */
        if (strchr(destino, '|') != NULL) {
            printf("  No se permite el caracter '|'.\n");
        } else if (destino[0] == '\0') {
            printf("  El campo no puede estar vacio.\n");
        } else {
            return;
        }
    }
}

int leer_entero(const char *mensaje) {
    char buf[32];
    char *fin;
    for (;;) {
        leer_texto(mensaje, buf, sizeof buf);
        long n = strtol(buf, &fin, 10);
        if (*fin == '\0') return (int)n;
        printf("  Escribe un numero valido.\n");
    }
}

/* Valida dd/mm/aaaa y que la fecha exista y no sea futura */
int fecha_valida(const char *f, int *d, int *m, int *a) {
    static const int dias_mes[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    char extra;
    if (strlen(f) != 10 || sscanf(f, "%2d/%2d/%4d%c", d, m, a, &extra) != 3)
        return 0;
    if (*m < 1 || *m > 12 || *a < 1900) return 0;

    int max = dias_mes[*m - 1];
    int bisiesto = (*a % 4 == 0 && *a % 100 != 0) || (*a % 400 == 0);
    if (*m == 2 && bisiesto) max = 29;
    if (*d < 1 || *d > max) return 0;

    time_t t = time(NULL);
    struct tm *hoy = localtime(&t);
    int ha = hoy->tm_year + 1900, hm = hoy->tm_mon + 1, hd = hoy->tm_mday;
    if (*a > ha || (*a == ha && (*m > hm || (*m == hm && *d > hd))))
        return 0;
    return 1;
}

/* Calcula la edad a partir de la fecha de nacimiento */
int calcular_edad(int d, int m, int a) {
    time_t t = time(NULL);
    struct tm *hoy = localtime(&t);
    int edad = (hoy->tm_year + 1900) - a;
    if (hoy->tm_mon + 1 < m || (hoy->tm_mon + 1 == m && hoy->tm_mday < d))
        edad--;
    return edad;
}

/* ---------- Lectura / escritura del archivo ---------- */

/* Convierte una línea del archivo en una Persona. Devuelve 1 si tuvo éxito */
int linea_a_persona(const char *linea, Persona *p) {
    return sscanf(linea, "%d|%99[^|]|%d|%10[^|]|%99[^|]|%19[^|]|%c",
                  &p->id, p->nombre, &p->edad, p->fecha_nacimiento,
                  p->direccion, p->estado_civil, &p->sexo) == 7;
}

void escribir_persona(FILE *f, const Persona *p) {
    fprintf(f, "%d|%s|%d|%s|%s|%s|%c\n",
            p->id, p->nombre, p->edad, p->fecha_nacimiento,
            p->direccion, p->estado_civil, p->sexo);
}

/* Busca el id más alto guardado para asignar el siguiente */
int siguiente_id(void) {
    FILE *f = fopen(ARCHIVO, "r");
    char linea[MAX_LINEA];
    Persona p;
    int max = 0;
    if (f == NULL) return 1;   /* el archivo aún no existe */
    while (fgets(linea, sizeof linea, f))
        if (linea_a_persona(linea, &p) && p.id > max) max = p.id;
    fclose(f);
    return max + 1;
}

void mostrar_encabezado(void) {
    printf("\n%-4s %-25s %-5s %-11s %-30s %-12s %-4s\n",
           "ID", "Nombre", "Edad", "Nacimiento", "Direccion", "Edo. civil", "Sexo");
    printf("--------------------------------------------------------------------------------------------------\n");
}

void mostrar_persona(const Persona *p) {
    printf("%-4d %-25.25s %-5d %-11s %-30.30s %-12s %-4c\n",
           p->id, p->nombre, p->edad, p->fecha_nacimiento,
           p->direccion, p->estado_civil, p->sexo);
}

/* ---------- Operaciones del menú ---------- */

void agregar_persona(void) {
    Persona p;
    int d, m, a, op;
    char buf[MAX_TEXTO];
    const char *estados[] = {"Soltero", "Casado", "Divorciado", "Viudo", "Union libre"};

    printf("\n--- Nueva persona ---\n");
    leer_texto("Nombre: ", p.nombre, sizeof p.nombre);

    for (;;) {
        leer_texto("Fecha de nacimiento (dd/mm/aaaa): ",
                   p.fecha_nacimiento, sizeof p.fecha_nacimiento);
        if (fecha_valida(p.fecha_nacimiento, &d, &m, &a)) break;
        printf("  Fecha invalida.\n");
    }
    p.edad = calcular_edad(d, m, a);
    printf("  Edad calculada: %d anios\n", p.edad);

    leer_texto("Direccion: ", p.direccion, sizeof p.direccion);

    printf("Estado civil:\n");
    for (int i = 0; i < 5; i++) printf("  %d) %s\n", i + 1, estados[i]);
    do {
        op = leer_entero("Opcion: ");
    } while (op < 1 || op > 5);
    strcpy(p.estado_civil, estados[op - 1]);

    for (;;) {
        leer_texto("Sexo (M/F): ", buf, sizeof buf);
        p.sexo = (char)toupper((unsigned char)buf[0]);
        if (buf[1] == '\0' && (p.sexo == 'M' || p.sexo == 'F')) break;
        printf("  Escribe M o F.\n");
    }

    p.id = siguiente_id();

    FILE *f = fopen(ARCHIVO, "a");   /* "a" = agregar al final */
    if (f == NULL) {
        perror("No se pudo abrir el archivo");
        return;
    }
    escribir_persona(f, &p);
    fclose(f);
    printf("Persona guardada con ID %d.\n", p.id);
}

void listar_personas(void) {
    FILE *f = fopen(ARCHIVO, "r");
    char linea[MAX_LINEA];
    Persona p;
    int total = 0;

    if (f == NULL) {
        printf("\nNo hay personas registradas.\n");
        return;
    }
    mostrar_encabezado();
    while (fgets(linea, sizeof linea, f)) {
        if (linea_a_persona(linea, &p)) {
            mostrar_persona(&p);
            total++;
        }
    }
    fclose(f);
    printf("\nTotal: %d persona(s)\n", total);
}

/* Compara sin distinguir mayúsculas: ¿'texto' contiene a 'buscado'? */
int contiene(const char *texto, const char *buscado) {
    size_t n = strlen(buscado);
    for (; *texto; texto++) {
        size_t i = 0;
        while (i < n && tolower((unsigned char)texto[i]) ==
                        tolower((unsigned char)buscado[i]))
            i++;
        if (i == n) return 1;
    }
    return 0;
}

void buscar_persona(void) {
    char buscado[MAX_TEXTO];
    char linea[MAX_LINEA];
    Persona p;
    int encontrados = 0;

    FILE *f = fopen(ARCHIVO, "r");
    if (f == NULL) {
        printf("\nNo hay personas registradas.\n");
        return;
    }
    leer_texto("\nNombre (o parte del nombre) a buscar: ", buscado, sizeof buscado);

    while (fgets(linea, sizeof linea, f)) {
        if (linea_a_persona(linea, &p) && contiene(p.nombre, buscado)) {
            if (encontrados == 0) mostrar_encabezado();
            mostrar_persona(&p);
            encontrados++;
        }
    }
    fclose(f);
    if (encontrados == 0) printf("No se encontro a nadie con ese nombre.\n");
}

/* Para borrar se copia todo a un archivo temporal excepto la persona elegida */
void eliminar_persona(void) {
    char linea[MAX_LINEA];
    Persona p;
    int borrado = 0;

    FILE *f = fopen(ARCHIVO, "r");
    if (f == NULL) {
        printf("\nNo hay personas registradas.\n");
        return;
    }
    int id = leer_entero("\nID de la persona a eliminar: ");

    FILE *tmp = fopen(ARCHIVO_TMP, "w");
    if (tmp == NULL) {
        perror("No se pudo crear el archivo temporal");
        fclose(f);
        return;
    }
    while (fgets(linea, sizeof linea, f)) {
        if (linea_a_persona(linea, &p) && p.id == id) {
            borrado = 1;          /* esta línea no se copia */
            continue;
        }
        fputs(linea, tmp);
    }
    fclose(f);
    fclose(tmp);

    if (borrado) {
        remove(ARCHIVO);
        rename(ARCHIVO_TMP, ARCHIVO);
        printf("Persona con ID %d eliminada.\n", id);
    } else {
        remove(ARCHIVO_TMP);
        printf("No existe una persona con ID %d.\n", id);
    }
}

int main(void) {
    int opcion;
    do {
        printf("\n===== REGISTRO DE PERSONAS =====\n");
        printf("1) Agregar persona\n");
        printf("2) Listar personas\n");
        printf("3) Buscar por nombre\n");
        printf("4) Eliminar persona\n");
        printf("0) Salir\n");
        opcion = leer_entero("Opcion: ");

        switch (opcion) {
            case 1: agregar_persona();  break;
            case 2: listar_personas();  break;
            case 3: buscar_persona();   break;
            case 4: eliminar_persona(); break;
            case 0: printf("Hasta luego.\n"); break;
            default: printf("Opcion no valida.\n");
        }
    } while (opcion != 0);
    return 0;
}
