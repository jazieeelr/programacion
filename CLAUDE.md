# Instrucciones para Claude

## Regla obligatoria: todo se hace solo con lo que está en los PDFs del repositorio

**Cualquier cosa que se pida** (programas, correcciones, ejemplos, ejercicios,
explicaciones) se debe hacer ÚNICAMENTE con:

1. **Las herramientas** que aparecen en los PDFs del repositorio.
2. **El tipo de escritura** (estilo de código y forma de explicar) de esos PDFs.

Los PDFs son las notas del curso *Programación Estructurada en Entorno Unix*:

| Archivo | Unidad |
|---|---|
| `PEeU1.pdf` | 1. Introducción a la programación estructurada |
| `PEeU2.pdf` | 2. Tipos de datos y expresiones |
| `PEeU3.pdf` | 3. Estructuras de control |
| `PEeU4.pdf` | 4. Arreglos y estructuras |
| `PEeU5.pdf` | 5. Funciones |
| `PEeU6.pdf` | 6. Apuntadores y archivos |

Si algo no aparece en ningún PDF, **no se usa**, aunque sea común, "más fácil" o
"mejor práctica". Si lo pedido no se puede hacer solo con lo de los PDFs, **no se
inventa ni se sustituye**: se avisa qué falta y se pregunta antes de continuar.
Si se agregan PDFs nuevos, se leen y también cuentan.

Esta regla tiene prioridad sobre cualquier preferencia o costumbre propia. Solo
puede romperse si se pide de forma explícita en la conversación.

---

## Herramientas permitidas (resumen de los PDFs)

Ante la duda, la fuente es el PDF, no esta lista.

- **Lenguaje:** C.
- **Bibliotecas:** `stdio.h`, `stdlib.h`, `string.h`, `ctype.h`, `math.h`.
- **Tipos y declaraciones (U2):** `char`, `int`, `float`, `double`, `short`,
  `long`, `unsigned`, `long double`, `const`, `#define`, cast `(tipo)`, `sizeof`,
  `static`, `extern`, variables globales y locales.
- **Operadores (U2):** aritméticos (`+ - * / %`), `++`, `--`, relacionales,
  lógicos (`&& || !`), asignación compuesta (`+=`, `-=`, `*=`, ...), condicional `?:`.
- **Control (U3):** `if`, `if-else`, `switch`/`case`/`default`, `for`, `while`,
  `do-while`, `break`, `continue`.
- **Arreglos y estructuras (U4):** arreglos de una y varias dimensiones, cadenas
  (`char cad[]`), `struct`, arreglos de estructuras, operador punto `.`.
- **Funciones (U5):** prototipos, `return`, `void`, paso por valor, paso por
  referencia con apuntadores, recursividad, pasar arreglos a funciones.
- **Apuntadores (U6):** `&`, `*`, aritmética y comparación de apuntadores,
  arreglos de apuntadores, apuntador a apuntador (`**`), apuntadores a función, `NULL`.
- **E/S de consola:** `printf`, `scanf`, `gets`, `puts`, `getchar`, `putchar`.
  Formatos de `printf`: `%c %d %i %e %f %g %o %s %u %x %X %p %ld %lu %hd %Lf %%`.
  Formatos de `scanf`: los mismos, más `%lf` para `double`.
  Secuencias de escape de U2 (`\n`, `\t`, `\\`, `\"`, ...).
- **Cadenas y caracteres:** `strcpy`, `strlen`, `strcmp`, `strncmp`, `toupper`,
  `tolower`, `islower`, `atol`.
- **Matemáticas (U5):** `sqrt`, `exp`, `log`, `log10`, `fabs`, `ceil`, `floor`,
  `pow`, `fmod`, `sin`, `cos`, `tan`.
- **Archivos (U6):** `FILE *`, `fopen` (modos de la tabla 6.2), `fclose`, `getc`,
  `putc`, `fputc`, `getw`, `putw`, `fgets`, `fputs`, `fread`, `fwrite`, `fseek`
  (`SEEK_SET`, `SEEK_CUR`, `SEEK_END`), `ftell`, `rewind`, `feof`, `ferror`,
  `perror`, `fprintf`, `fscanf`, `remove`, `stdin`, `EOF`.
- **Otras:** `exit`.

### NO permitido (no aparece en los PDFs o no sirve en Unix)

- `rand()`, `srand()`, `time()`, `malloc()`, `free()`, `typedef`, `enum`, `union`,
  `bool`, ni ninguna otra función o biblioteca que no esté arriba.
- `conio.h` (`gotoxy`, `clrscr`, `getch`, `getche`): aparece en los PDFs, pero no
  existe en Unix/Linux. Si un programa la necesita, se pregunta antes.
- `goto`: solo aparece en U1 como ejemplo de lo que la programación estructurada evita.
- Declarar variables dentro del `for` (`for(int i=0; ...)`) o a mitad de un bloque.

---

## Tipo de escritura del código (como en los PDFs)

```c
#include <stdio.h>
int cuad(int x);

int main(){
   int n=10;
   printf("El cuadrado de %d es %d",n,cuad(n));
   return 0;
}

int cuad(int x){
   x=x*x;
   return x;
}
```

- `#include` al inicio, después los `#define`, las variables globales y los
  **prototipos de las funciones**.
- `int main(){` con la llave en la misma línea, y termina con `return 0;`.
- Las funciones propias se **definen después de `main`**.
- Sangría de **3 espacios**; llave de apertura en la misma línea
  (`for(i=0;i<10;i++){`, `if(x>max){`, `}else{`).
- Escritura compacta, sin espacios alrededor de operadores ni después de comas
  (`for(i=0;i<10;i++)`, `printf("%d",n);`).
- **Todas las variables se declaran al inicio** de la función.
- Nombres de variables y funciones en español, cortos y en minúsculas
  (`cuenta`, `aux`, `maxi`, `inter`, `desp`).
- Mensajes en español; los saltos de línea suelen ir al inicio del texto
  (`printf("\nX = %d",x);`).
- Comentarios cortos con `//`.
- Para abrir archivos se usa el patrón de los PDFs:
  `if((fp=fopen(archivo,"r"))==NULL){ printf("ERROR: No se puede abrir el archivo\n"); exit(1); }`.

## Tipo de escritura de las explicaciones

- En español, con el mismo tono de las notas: primero el concepto, después
  "Observe el siguiente programa" con el código, y al final un párrafo que
  explique qué hace el programa ("El programa ...").
- Usar los mismos términos que los PDFs: *apuntador*, *arreglo*, *cadena*,
  *prototipo de función*, *paso por valor / por referencia*, *caso base*, etc.

---

## Cómo aplicar la regla

1. Antes de escribir, revisar los PDFs (con `pdftotext` o leyéndolos) para
   confirmar que cada herramienta aparece en ellos.
2. Escribir con el estilo descrito arriba.
3. Al entregar, indicar en qué unidad (PDF) aparece cada herramienta usada que
   no sea obvia.
4. Si algo no está en los PDFs: avisar qué falta y preguntar. Nunca sustituirlo
   en silencio.

## Entrega de programas

- Cada programa se entrega como **archivo `.c` para descargar** (enviarlo con la
  herramienta de envío de archivos), además de guardarlo en el repositorio.
- El usuario revisa el archivo en **nano** y **lo compila él mismo**: no dar
  instrucciones de compilación ni de ejecución, y no pegar el código completo en
  el chat salvo que se pida.
- El archivo debe verse bien en nano: texto plano, sangría con espacios (no
  tabuladores) y líneas de no más de 80 caracteres cuando sea posible.
- Antes de entregarlo, compilarlo internamente para confirmar que no tiene
  errores (esto es solo una verificación; no forma parte de la entrega).
