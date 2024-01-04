#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
//#include <unistd.h>

#define MAX 150
/*cambios :
-LIBERART: MAL PASAJE DE PARAMETROS, SE ESTABA PASANDO LA CABEZA DE LA LISTA CON DOS ** EN VEZ DE SOLO UNO
-LIBERARD: TENIA EXACTAMENTE EL MISMO ERROR 

ESTAS DOS FUNCIONES PROVOCABAN QUE EL PROGRAMA SE COLGARA AL ELEGIR LA OPCION 5 O 6 .
-EN LA ACCION LISTARFALLECIDO ASI COMO EN LISTAR LOCALIDAD, AHORA SE CREAN LOS FICTICIOS ANTES DE TODO, YA QUE LAS ACCIONES ANTERIORES ESTABAN PENSADAS
PARA LIBERAR LISTAS CON FICTICIOS INCLUIDOS, SI ESTOS NO SE CREABAN Y SE PASABA COMO PARAMETRO UN PUNTERO comun OCASIONABA UN SEGMENTATION FAULT()
INTENTABA OPERAR SOBRE UNA DIRECION DE MEMORIA INVALIDA.

-EN LA ACCION InsertarAlFinalT:  SE REALIZA INCORRECTAMENTE LA INSERCION DEL ELEMENTO POR PRIMERA VEZ CUANDO LA LISTA ESTA VACIA
Y TAMBIEN CUANDO LA LISTA YA TIENE AL MENOS UN ELEMENTO.

-EN LISTAR LOCALIDAD SE  INCIALIZO LA VARIABLE "i".
 
-


*/

// PEREZ MAXIMO 45241164
// LAUTARO PELLISERO 44244764
// COMISION 1 AMBOS
struct Tfecha
{
    int dia;
    int mes;
    int anio;
};

struct TPersona
{
    int DNI;
    char nombre[20];
    char apellido[20];
    struct Tfecha nacimiento; // Campo de tipo Tfecha para la fecha de nacimiento
    int edad;
    char ciudad[20];
    char DP[30];                 // Dirección postal
    char CE[20];                 // Correo electrónico
    char tel[15];                // Teléfono
    int VF;                      // Vive/falleció: 1 si está vivo, 0 si ha fallecido
    struct Tfecha fallecimiento; // Campo de tipo Tfecha para la fecha de fallecimiento
};

typedef struct
{
    struct TPersona a[MAX];
    int cant;
} TData;

typedef struct TDoblete
{
    struct TPersona info;
    struct TDoblete *next;
} TDoblete;

typedef struct TTriplete
{
    struct TPersona info;
    struct TTriplete *next;
    struct TTriplete *back;
} TTriplete;

void swap(struct TPersona *x, struct TPersona *y); // accion que hace un  intercambio de elementos, en este caso en el arreglo.
int vacia(TData array);                            // retorna 1 si el arreglo esta vacio.
int llena(TData array);                            // retorna 1 si el arreglo esta lleno.

// Acciones/Funciones para tratar LSE:
TDoblete *crearNodoFicticioD();                                             // crea un doblete (info y next solamente).
void insertarAlFinalD(struct TDoblete **cabeza, struct TPersona nuevoDato); // se inserta al final para conservar el orden del arreglo
void mostrarListaD(struct TDoblete *cabeza);                                // es diferente a mostrar una LDE por consigna, en LDE se debe dar a elegir como ver los datos.
void liberarD(struct TDoblete *cabeza);                                    // esta accion libera/borra toda una LSE

// Acciones/Funciones para tratar LDE:
TTriplete *crearNodoFicticioT();                                                                     // Crea un Triplete(info, next y back).
void insertarAlFinalT(struct TTriplete **cabeza, struct TTriplete **fin, struct TPersona nuevoDato); // se inserta al final para conservar el orden del arreglo
void mostrarListaT(struct TTriplete *cabeza, struct TTriplete *fin);                                 // Da a elegir al usuario el orden en el que quiere ver los datos de la LDE
void liberarT(struct TTriplete *cabeza);                                                            // esta accion libera/borra toda una LDE

// Acciones/Funciones que actuan sobre el arreglo
void insertarPersona(TData *registro, struct TPersona nuevaPer); //
void suprimir(TData *array, int dni);
void mostrarArreglo(TData array);
void modificar(TData *array, int dni);
void listarFallecidos(TData array, TDoblete **sec);
void listarLocalidad(TData array, char ciudad[20], TTriplete **p, TTriplete **u);
void ordenarDni(TData array);
void myflush(FILE *in);
int buscarDni(TData array, int dni);
void ordenarPorApellido(TData *array);
void ordenarPorNombre(TData *array);
void ordenarPorDNI(TData *array);

int main()
{
    TDoblete *cabeza=NULL;     // cabeza de LSE de fallecidos.
    TTriplete *cab, *fin; // cabeza y final de LDE de la localidad.
    TData arreglo;        // arreglo principal.
    struct TPersona persona;
    FILE *archivo;
    int fechaActual, dni, opc = 0; // fechaActual ayuda como auxiliar para poder calcular los años de diferenecia .// dni  almacena el dni de la persona a buscar.
    // opc almacena la opcion del usuario.
    char localidad[50]; // auxiliar para la localidad a buscar
    // pasar los datos del archivo "personas.dat" a el arreglo.
    archivo = fopen("personas.dat", "rb"); // se abre en modo lectura para binarios.
    if (archivo == NULL)                   // si esta vacio
    {
        printf("El archivo especificado no existe. Será creado automáticamente.\n");
    }
    else // si el archivo contiene elementos..
    {
        int i = 0;
        while (i < MAX && fread(&arreglo.a[i], sizeof(struct TPersona), 1, archivo) == 1) // se copian los datos del archivo en el arreglo.
        {
            i++;
            arreglo.cant++;
        }

        if (i >= MAX) // Esto por si el archivo contiene mas de 150 elementos.
        {
            printf("Se ha alcanzado la capacidad máxima del arreglo.\n");
        }

        fclose(archivo); // se cierra el archivo.
    }

    do
    { // Menu de opciones principal.
        printf("\n");
        printf("Menú de opciones:\n");
        printf("1. Alta de Persona\n");
        printf("2. Baja de Persona\n");
        printf("3. Modificar datos de personas\n");
        printf("4. Listar todas las personas \n");
        printf("5. Listar personas fallecidas\n");
        printf("6. Listar personas de una localidad\n");
        printf("7. Ordenar por DNI\n");
        printf("8. Salir\n");
        printf("Ingrese la opción deseada: ");
        scanf("%d", &opc);

        switch (opc)
        {
        case 1: // Alta de Persona

            printf("Ingrese el estado actual de la persona( 0 si ha muerto, 1 si esta viva): ");
            scanf("%d", &persona.VF);
            if (persona.VF == 0)
            { // Saber si la persona aun vive o no.
              //  si no vive.. pedimos ingresar la fecha de fallecimiento.

                printf("Ingrese la fecha de Fallecimiento: ");
                printf(" Ingrese el dia: ");
                scanf("%d", &persona.fallecimiento.dia);

                printf(" ingrese el mes: ");
                scanf("%d", &persona.fallecimiento.mes);

                printf(" ingrese el año: ");
                scanf("%d", &persona.fallecimiento.anio);
                if( persona.fallecimiento.dia < 1 || 31 < persona.fallecimiento.dia || persona.fallecimiento.mes < 1 || 12 < persona.fallecimiento.mes || persona.fallecimiento.anio < 1980 || 2023 < persona.fallecimiento.anio ){
                    printf("\nError la fecha ingresada es incorrecta.\n");
                    break;
                }
            }
            myflush(stdin);
            printf("\nIngrese el nombre de la persona: "); // nombre de la persona.
            scanf("%[^\n]s", persona.nombre);

            myflush(stdin);

            printf("Ingrese el apellido de la persona: "); // apellido de la persona.
            scanf("%[^\n]s", persona.apellido);

            myflush(stdin);

            printf("Ingrese el Dni de la persona: "); // dni de la persona.
            scanf("%d", &persona.DNI);
            if (persona.DNI < 100)
            {
                printf("\nError el DNI ingresado es incorrecto.\n");
                break;
            }
            
            printf("Ingrese la fecha de nacimiento de la persona: ");
            printf(" Ingrese el dia: ");
            scanf("%d", &persona.nacimiento.dia);

            printf(" ingrese el mes: ");
            scanf("%d", &persona.nacimiento.mes);

            printf(" ingrese el año: ");
            scanf("%d", &persona.nacimiento.anio);
            if( persona.nacimiento.dia < 1 || 31 < persona.nacimiento.dia || persona.nacimiento.mes < 1 || 12 < persona.nacimiento.mes ||persona.nacimiento.anio < 1900 || 2023 < persona.nacimiento.anio ){
                    printf("\nError la fecha ingresada es incorrecta.\n");
                    break;
            }
            // edad actual de la persona:
            if (persona.VF == 0)
            {
                // si la persona fallecio, entonces su edad actual es la edad con la que murio.
                persona.edad = persona.fallecimiento.anio - persona.nacimiento.anio;
            }
            else
            {

                printf("ingrese el año actual para calcular la edad de la persona: "); // de lo contrario( si sigue con vida) la calculamos de esta manera.
                scanf("%d", &fechaActual);
                persona.edad = fechaActual - persona.nacimiento.anio;
            }

            myflush(stdin);

            printf("Ingrese la ciudad de residencia de la persona: ");
            scanf("%[^\n]s", persona.ciudad);

            myflush(stdin);

            printf("Ingrese la direccion postal de la persona: ");
            scanf("%[^\n]s", persona.DP);

            myflush(stdin);

            printf("Ingrese el correo electronico de la persona: ");
            scanf("%s", persona.CE);

            printf("Ingrese el telefono  de la persona: ");
            scanf("%s", persona.tel);
            insertarPersona(&arreglo, persona);
            break;
        case 2: // Baja de Persona
            printf("Ingrese el dni de la persona a eliminar:");
            scanf("%d", &dni);
            suprimir(&arreglo, dni);
            break;
        case 3: // Modificar una persona.
            printf("Ingrese el dni de la persona a modificar: ");
            scanf("%d", &dni);
            modificar(&arreglo, dni);
            break;
        case 4: // Listado de Personas
            mostrarArreglo(arreglo);
            break;
        case 5: // Listar fallecidos con LSE
            listarFallecidos(arreglo, &cabeza);
            liberarD(cabeza);
            break;
        case 6: // Listar misma localidad con LDE.
            myflush(stdin);
            printf("Ingrese una localidad: ");
            scanf("%[^\n]s", localidad);
            myflush(stdin);
            listarLocalidad(arreglo, localidad, &cab, &fin);
            liberarT(cab);
            break;

        case 7: // Ordenamiento por metodo de Burbuja tomando como criterio el DNI.
            ordenarDni(arreglo);
            break;
        case 8:
            printf("Cerrando el programa y guardando datos..");
            archivo = fopen("personas.dat", "wb"); // Se abre de nuevo el archivo pero esta vez para escribir.
            int k = 0;
            while (k < arreglo.cant)
            {
                fwrite(&arreglo.a[k], sizeof(struct TPersona), 1, archivo); // Se escribe en el archivo.
                k++;
            }
            fclose(archivo); // Por ultimo se vuelve  a cerrar el archivo definitivamente.
            break;
        default:

            printf("Opción inválida. Por favor, elija una opción válida.\n");
        }

    } while (opc != 8);

    return 0;
};

void myflush(FILE *in) // esta accion la encontramos como solucion para insertar cadenas de caracteres con espacios.Junto con "%[^\n]s".
{
    int ch;

    do
        ch = fgetc(in);
    while (ch != EOF && ch != '\n');

    clearerr(in);
}

int vacia(TData array)
{

    if (array.cant == 0)
    {

        return 1;
    }
    else
    {
        return 0;
    }
};

int llena(TData array)
{

    if (array.cant == 150)
    {
        return 1;
    }
    else
    {
        return 0;
    }
};

void swap(struct TPersona *x, struct TPersona *y) // Esta accion sirve para intercambiar un elemento con otro dentro del arreglo.
{
    struct TPersona temp = *x;
    *x = *y;
    *y = temp;
};

void insertarPersona(TData *array, struct TPersona nuevaPer)
{
    if (llena(*array) == 1)
    {
        printf("Lista de personas llena! ");
    }
    else
    {
            int k = 0;
            while (k < array->cant && array->a[k].DNI != nuevaPer.DNI)
            { // analiza si no se repite el dni, puesto que no existen dos personas con el mismo dni.

                k++;
            }
            if (array->a[k].DNI == nuevaPer.DNI)
            {

                printf("\nError: Se encontro una persona con el mismo dni");
            }
            else
            {
                // Agregar la nueva persona al final del arreglo
                array->a[array->cant] = nuevaPer;
                array->cant++;

                // Ordenamiento por Apellido, Nombre y DNI
                for (int k = 0; k < array->cant - 1; k++)
                {
                    for (int j = 0; j < array->cant - k - 1; j++)
                    {
                        // Comparación por Apellido, luego por Nombre, y finalmente por DNI
                        if (strcmp(array->a[j].apellido, array->a[j + 1].apellido) > 0 ||
                            (strcmp(array->a[j].apellido, array->a[j + 1].apellido) == 0 &&
                             strcmp(array->a[j].nombre, array->a[j + 1].nombre) > 0) ||
                            (strcmp(array->a[j].apellido, array->a[j + 1].apellido) == 0 &&
                             strcmp(array->a[j].nombre, array->a[j + 1].nombre) == 0 &&
                             array->a[j].DNI > array->a[j + 1].DNI))
                        {
                            swap(&array->a[j], &array->a[j + 1]);
                        }
                    }
                }
            }
        }
    }


void suprimir(TData *array, int dni)
{
    int i = 0, pos = -1; // Inicializar i y pos

    if (vacia(*array) == 1)
    {
        printf("\nLa lista de personas está vacía. No hay elementos para eliminar.\n");
    }
    else
    {
        while (i < array->cant && dni != array->a[i].DNI)
        {
            i++;
        }

        if (dni == array->a[i].DNI) // se verifica si se encontró el DNI
        {
            pos = i; // Almacenar la posicion donde se encontro.
            while (pos < array->cant - 1)
            {
                array->a[pos] = array->a[pos + 1]; // se realiza un corrimiento a la izquierda para pisar a la persona con el mismo dni
                pos++;
            }
            array->cant--;
            printf("\nSe ha eliminado la persona con el DNI: %d\n", dni);
        }
        else
        {
            printf("\nPersona inexistente.\n");
        }
    }
}

void mostrarArreglo(TData array)
{
    if (vacia(array) == 0)
    {
        printf("Elementos del arreglo:\n");
        for (int i = 0; i < array.cant; i++)
        {
            printf("\n");
            printf("nombre: %s \n", array.a[i].nombre);
            printf("apellido: %s \n", array.a[i].apellido);
            printf("Dni: %d \n", array.a[i].DNI);
            printf("Fecha de nacimiento:\n");
            printf(" %d/", array.a[i].nacimiento.dia);
            printf(" %d/", array.a[i].nacimiento.mes);
            printf(" %d ", array.a[i].nacimiento.anio);
            printf("\n");
            printf("Edad: %d \n", array.a[i].edad);
            printf("ciudad de residencia: %s \n", array.a[i].ciudad);
            printf("direccion postal: %s \n", array.a[i].DP);
            printf("correo electronico: %s \n ", array.a[i].CE);
            printf("telefono: %s \n", array.a[i].tel);
            if (array.a[i].VF == 0)
            {
                // printf("\n");
                printf("Vive: No\n");
                printf("Fecha de fallecimiento:\n");
                printf(" %d/", array.a[i].fallecimiento.dia);
                printf(" %d/", array.a[i].fallecimiento.mes);
                printf(" %d ", array.a[i].fallecimiento.anio);
                printf("\n");
            }
            else
            {

                printf("Vive: Si\n");
            }
        }
    }
    else
    {
        printf("\n");
        printf("Lista de personas vacia! porfavor ingrese una.");
        printf("\n");
    }
}
// Función para crear un nodo ficticio para una lista doblemente enlazada
struct TTriplete *crearNodoFicticioT()
{
    // Asignar memoria para el nuevo nodo ficticio
    struct TTriplete *nodoFicticio = malloc(sizeof(struct TTriplete));
    if (nodoFicticio == NULL)
    {
        // Manejar el error si no se puede asignar memoria
        printf("Error: No se puede asignar memoria para el nodo ficticio\n");
        return NULL;
    }
    // Inicializar el nodo ficticio
    nodoFicticio->back = NULL;
    nodoFicticio->next = NULL;
    return nodoFicticio;
}

void insertarAlFinalT(struct TTriplete **cabeza, struct TTriplete **fin, struct TPersona nuevoDato)
{
    // Crear un nuevo nodo
    struct TTriplete *nuevoNodo = malloc(sizeof(struct TTriplete));
    if (nuevoNodo == NULL)
    {
        // Manejar el error si no se puede asignar memoria
        printf("Error: No se puede asignar memoria para el nuevo nodo\n");
        return;
    }

    // Inicializar el nuevo nodo
    nuevoNodo->info = nuevoDato;
    nuevoNodo->next = NULL;

    // Si la lista está vacía, el nuevo nodo se inserta entre cabeza y fin
    if ((*cabeza)->next == *fin)
    {
        (*cabeza)->next = nuevoNodo;
        nuevoNodo->next = *fin;
        nuevoNodo->back = *cabeza;
        (*fin)->back = nuevoNodo;
    }
    else
    {
        // Si la lista no está vacía, encontrar el último nodo y enlazarlo al nuevo nodo
        struct TTriplete *ultimo = (*cabeza)->next;
        while (ultimo->next != *fin)
        {
            ultimo = ultimo->next;
        }

        // Enlazar el nuevo nodo al final
        nuevoNodo->back = ultimo;
        nuevoNodo->next = *fin;
        ultimo->next = nuevoNodo;
        (*fin)->back = nuevoNodo;


      
    }
}

void mostrarListaT(struct TTriplete *cabeza, struct TTriplete *fin)
{

    int opc = 0;

    if (cabeza->next == fin)
    { // secuencia vacia.
        printf("\nNo hay personas que coincidan con esa localidad\n");
    }
    else
    {
        struct TTriplete *aux = NULL; // auxiliar para recorrer la lista.
        printf("Desea ver los datos de forma 1.ascendente o 2.descendente?: ");
        scanf("%d", &opc);
        if (opc == 1)
        { // forma ascendente.
            aux = cabeza->next;
            while (aux != fin)
            {
                printf("\n");
                printf("nombre: %s \n", (*aux).info.nombre);
                printf("apellido: %s \n", (*aux).info.apellido);
                printf("Dni: %d \n", (*aux).info.DNI);
                printf("Fecha de nacimiento \n");
                printf("Dia: %d/", (*aux).info.nacimiento.dia);
                printf("Mes: %d/", (*aux).info.nacimiento.mes);
                printf("Año: %d", (*aux).info.nacimiento.anio);
                printf("\n");
                printf("edad actual: %d \n", (*aux).info.edad);
                printf("ciudad de residencia: %s \n", (*aux).info.ciudad);
                printf("direccion postal: %s \n", (*aux).info.DP);
                printf("correo electronico: %s \n ", (*aux).info.CE);
                printf("telefono: %s \n", (*aux).info.tel);
                aux = aux->next;
            }
        }
        else
        { // forma descendente.

            aux = fin->back;
            while (aux != cabeza)
            {
                printf("\n");
                printf("nombre: %s \n", (*aux).info.nombre);
                printf("apellido: %s \n", (*aux).info.apellido);
                printf("Dni: %d \n", (*aux).info.DNI);
                printf("Fecha de nacimiento \n");
                printf("Dia: %d/", (*aux).info.nacimiento.dia);
                printf("Mes: %d/", (*aux).info.nacimiento.mes);
                printf("Año: %d", (*aux).info.nacimiento.anio);
                printf("\n");
                printf("edad actual: %d \n", (*aux).info.edad);
                printf("ciudad de residencia: %s \n", (*aux).info.ciudad);
                printf("direccion postal: %s \n", (*aux).info.DP);
                printf("correo electronico: %s \n ", (*aux).info.CE);
                printf("telefono: %s \n", (*aux).info.tel);
                aux = aux->back;
            }
        }
    }
}
void liberarT(struct TTriplete *cabeza)
{
    struct TTriplete *actual = cabeza;
    struct TTriplete *siguiente;
    while (actual != NULL)
    {
        siguiente = actual->next;
        free(actual); // Liberar memoria del nodo actual
        actual = siguiente;
    }

    cabeza = NULL; // Establecer la cabeza de la lista como NUL
}

TDoblete *crearNodoFicticioD()
{

    TDoblete *nodoFicticio = malloc(sizeof(TDoblete));
    if (nodoFicticio == NULL)
    {
        // Manejar el error si no se puede asignar memoria
        printf("Error: No se puede asignar memoria para el nodo ficticio\n");
    }
    // Inicializar el nodo ficticio
    nodoFicticio->next = NULL;

    return nodoFicticio;
}

void insertarAlFinalD(struct TDoblete **cabeza, struct TPersona nuevoDato)
{
    // Crear un nuevo nodo
    struct TDoblete *nuevoNodo = (struct TDoblete *)malloc(sizeof(struct TDoblete));
    if (nuevoNodo == NULL)
    {
        // Manejar la falta de memoria
        printf("Error: No se pudo asignar memoria para el nuevo nodo.\n");
        return;
    }
    nuevoNodo->info = nuevoDato;
    nuevoNodo->next = NULL;

    // Si está vacía la lista, el nuevo nodo se convierte en la cabeza de la lista
    if ((*cabeza)->next == NULL)
    {
        (*cabeza)->next = nuevoNodo;
    }
    else
    {
        // Si la lista no está vacía, encontrar el último nodo y enlazarlo al nuevo nodo  para asi lograr insertarlo al final
        // y conservar el orden del arreglo.
        struct TDoblete *ultimo = *cabeza;
        while (ultimo->next != NULL)
        {
            ultimo = ultimo->next;
        }
        ultimo->next = nuevoNodo;
    }
}
void mostrarListaD(TDoblete *cabeza)
{
    TDoblete *aux = cabeza;

    if (aux == NULL)
    {
        printf("\nNo hay personas fallecidas.\n");

        return;
    }
    else
    {
        printf("\nPersonas Fallecidas:");
        while (aux != NULL)
        {

            printf("\n");
            printf("nombre: %s \n", aux->info.nombre);
            printf("apellido: %s \n", aux->info.apellido);
            printf("Dni: %d \n", aux->info.DNI);
            printf("Fecha de nacimiento: ");
            printf("%d/", aux->info.nacimiento.dia);
            printf("%d/", aux->info.nacimiento.mes);
            printf("%d ", aux->info.nacimiento.anio);
            printf("\n");
            printf("Edad: %d \n", aux->info.edad);
            printf("ciudad de residencia: %s \n", aux->info.ciudad);
            printf("direccion postal: %s \n", aux->info.DP);
            printf("correo electronico: %s \n ", aux->info.CE);
            printf("telefono: %s \n", aux->info.tel);
            printf("Fecha de fallecimiento: ");
            printf("%d/", aux->info.fallecimiento.dia);
            printf("%d/", aux->info.fallecimiento.mes);
            printf("%d ", aux->info.fallecimiento.anio);
            printf("\n");

            aux = aux->next; // avanzo al siguiente nodo!
        }
    }
}
void liberarD(struct TDoblete *cabeza)
{
    struct TDoblete *actual = cabeza;
    struct TDoblete *siguiente = NULL;

    while (actual != NULL)
    {
        siguiente = actual->next;
        free(actual); // Liberar memoria del nodo actual
        actual = siguiente;
    }

    cabeza = NULL; // Establecer la cabeza de la lista como NUL
}
void listarFallecidos(TData array, TDoblete **sec)
{
    TDoblete *aux = NULL;

     // creacion de Ficticio.
    *sec = crearNodoFicticioD();

    if (vacia(array) == 1)
    {
        printf("\nLa lista de personas está vacía.\n");
    }
    else
    {
        int i = 0;
        // creacion de Ficticio.
        *sec = crearNodoFicticioD();

        while (i < array.cant)
        {
            if (array.a[i].VF == 0)
            {
                insertarAlFinalD(sec, array.a[i]);
            }
            i++;
        }
        aux = (*sec)->next; // aux se coloca despues del ficticio.

        mostrarListaD(aux);
    }
}
void listarLocalidad(TData array, char ciudad[30], TTriplete **p, TTriplete **u)
{
    TTriplete *aux, *aux2, *nuevo = NULL;
    int i = 0;

    *p = crearNodoFicticioT();
    *u = crearNodoFicticioT();
    (*p)->next = (*u);
    (*u)->back = (*p);

    if(vacia(array) == 0)
    {
        while (i < array.cant)
        {
            if (strcmp(array.a[i].ciudad, ciudad) == 0)
            {
                //printf("Se inseto una persona!");
                insertarAlFinalT(p, u, array.a[i]);
                
            }
            i++;
        }
        aux = *p;
        aux2 = *u;
        mostrarListaT(aux,aux2);
    }
    else
    {
        printf("\nLista vacia. no hay elementos para analizar\n.");
    }
}
void ordenarDni(TData array)
{
    int i, j;
    if (vacia(array) == 0)
    {
        // Método de burbuja usando como criterio el campo DNI.
        for (i = array.cant; i > 0; i--)
        {
            for (j = 0; j < i - 1; j++)
            {
                if (array.a[j].DNI > array.a[j + 1].DNI)
                {
                    swap(&array.a[j], &array.a[j + 1]);
                }
            }
        }
        printf("Elementos ordenados por DNI:\n");
        mostrarArreglo(array); // Se muestran los elementos ordenados por DNI.
    }
    else
    {
        printf("\nLista vacía, no hay elementos para analizar.\n");
    }
}
int buscarDni(TData array, int dni)
{
for (int i = 0; i < array.cant; i++) {
        if (array.a[i].DNI == dni) {
            return i;  // Se encontró el DNI en la posición i
        }
    }
    return -1;  
// No se encontró el DNI en el arreglo

    // Se utiliza busqueda binaria del elemento en el arreglo
    /*int inf = 0;              // limite inferior
    int sup = array.cant - 1; // limite superior.

    while (inf <= sup)
    {
        int medio = inf + (sup - inf) / 2;

        if (array.a[medio].DNI == dni)
        {
            // Encontramos el DNI, devuelve la posición
            return medio;
        }

        if (array.a[medio].DNI < dni)
        {
            // El DNI buscado está en la mitad derecha
            inf = medio + 1;
        }
        else
        {
            // El DNI buscado está en la mitad izquierda
            sup = medio - 1;
        }
    }

    // Si no se encontró el DNI, devuelve -1.
    return -1;*/
}
void ordenarPorApellido(TData *array)
{
    for (int i = 0; i < array->cant - 1; i++)
    {
        for (int j = 0; j < array->cant - i - 1; j++)
        {
            if (strcmp(array->a[j].apellido, array->a[j + 1].apellido) > 0)
            {
                swap(&array->a[j], &array->a[j + 1]);
            }
        }
    }
}
void ordenarPorNombre(TData *array)
{
    for (int i = 0; i < array->cant - 1; i++)
    {
        for (int j = 0; j < array->cant - i - 1; j++)
        {
            if (strcmp(array->a[j].nombre, array->a[j + 1].nombre) > 0)
            {
                swap(&array->a[j], &array->a[j + 1]);
            }
        }
    }
}
void ordenarPorDNI(TData *array){
     int i, j;

    for (i = 0; i <  array->cant- 1; i++) {
        for (j = 0; j < array->cant - i - 1; j++) {
            if (array->a[j].DNI > array->a[j + 1].DNI) {
                 swap(&array->a[j], &array->a[j + 1]);
            }
        }
    }
}
void modificar(TData *array, int dni)
{
    int opc = 0;
    if (vacia(*array))
    {
        printf("\nLa lista de personas está vacía. No hay elementos para modificar.\n");
        return;
    }

    // Búsqueda del DNI en el arreglo
    int posicion = buscarDni(*array, dni);

    if (posicion != -1)
    {
        struct TPersona pers = array->a[posicion]; // Guardar/copiar la persona original
        // se muestran los valores de los campos originales de la persona.
        printf("\nPersona a modificadar:\n");
        printf("Nombre: %s\n", array->a[posicion].nombre);
        printf("Apellido: %s\n", array->a[posicion].apellido);
        printf("DNI: %d\n", array->a[posicion].DNI);
        printf("Fecha de Nacimiento: %d/%d/%d\n", array->a[posicion].nacimiento.dia, array->a[posicion].nacimiento.mes, array->a[posicion].nacimiento.anio);
        printf("Ciudad de Residencia: %s\n", array->a[posicion].ciudad);
        printf("Dirección Postal: %s\n", array->a[posicion].DP);
        printf("Correo Electrónico: %s\n", array->a[posicion].CE);
        printf("Teléfono: %s\n", array->a[posicion].tel);
        if (array->a[posicion].VF == 0)
        {
            printf("Vive: No\n");
            printf("Fecha de fallecimiento:\n");
            printf(" %d/", array->a[posicion].fallecimiento.dia);
            printf(" %d/", array->a[posicion].fallecimiento.mes);
            printf(" %d ", array->a[posicion].fallecimiento.anio);
            printf("\n");
        }
        else
        {

            printf("Vive: Si\n");
        }

        do
        {
            printf("\n");
            printf("Menú de opciones para modificar:\n");
            printf("1. Modificar Nombre\n");
            printf("2. Modificar Apellido\n");
            printf("3. Modificar Fecha de Nacimiento\n");
            printf("4. Modificar Ciudad de Residencia\n");
            printf("5. Modificar Dirección Postal\n");
            printf("6. Modificar Correo Electrónico\n");
            printf("7. Modificar Teléfono\n");
            printf("8. Modificar la condicion de la persona\n");
            printf("9. Modificar Fecha de Fallecimiento\n");
            printf("10. Salir\n");
            printf("Ingrese la opción deseada: ");
            scanf("%d", &opc);
            switch (opc)
            {
            case 1:
                myflush(stdin);
                printf("Ingrese el nombre de la persona: "); // nombre de la persona.
                scanf("%[^\n]s", array->a[posicion].nombre);
                printf("\nSe modifico nombre de la persona!\n");
                
                break;
            case 2:
                myflush(stdin);
                printf("\nIngrese el apellido de la persona: "); // apellido de la persona.
                scanf("%[^\n]s", array->a[posicion].apellido);
                printf("\nSe modifico apellido de la persona!\n");

                break;
            case 3:
                
                printf("Ingrese la nueva fecha de nacimiento:\n");
                printf("Dia: ");
                scanf("%d", &array->a[posicion].nacimiento.dia);
                printf("Mes: ");
                scanf("%d", &array->a[posicion].nacimiento.mes);
                printf("Año: ");
                scanf("%d", &array->a[posicion].nacimiento.anio);
                
                if( array->a[posicion].nacimiento.dia < 1 || 31 < array->a[posicion].nacimiento.dia || array->a[posicion].nacimiento.mes < 1 || 12 < array->a[posicion].nacimiento.mes || array->a[posicion].nacimiento.anio < 1930 || 2023 < array->a[posicion].nacimiento.dia ){
                    printf("\nError la fecha ingresada es incorrecta.\n");
                }else{
                    printf("\nSe modifico fecha de nacimiento de la persona!\n");
                }
                break;
            case 4:
                myflush(stdin);
                printf("Ingrese la nueva ciudad de residencia: ");
                scanf("%[^\n]s", array->a[posicion].ciudad);
                printf("\nSe modifico ciudad de residencia de la persona!\n");

                break;
            case 5:
                myflush(stdin);
                printf("Ingrese la nueva dirección postal: ");
                scanf("%[^\n]s", array->a[posicion].DP);
                printf("\nSe modifico dirección postal de la persona!\n");

                break;
            case 6:
                myflush(stdin);
                printf("Ingrese el nuevo correo electrónico: ");
                scanf("%[^\n]s", array->a[posicion].CE);
                 printf("\nSe modifico el correo electrónico!\n");

                break;
            case 7:
                myflush(stdin);
                printf("Ingrese el nuevo teléfono: ");
                scanf("%[^\n]s", array->a[posicion].tel);
                printf("\nSe modifico el teléfono! \n");
                break;
            case 8:
                printf("Ingrese la condicion de la persona( 1 si vive o 0 si ha muerto):");
                scanf("%d", &array->a[posicion].VF);
                if (array->a[posicion].VF == 0){

                    printf("\nRecuerde modificar la fecha de fallecimiento!\n");
                }
                break;

            case 9:
                if (array->a[posicion].VF == 0)
                {
                    printf("Ingrese la nueva fecha de Fallecimiento:\n");
                    printf("Dia: ");
                    scanf("%d", &array->a[posicion].fallecimiento.dia);
                    printf("Mes: ");
                    scanf("%d", &array->a[posicion].fallecimiento.mes);
                    printf("Año: ");
                    scanf("%d", &array->a[posicion].fallecimiento.anio);
                    if( array->a[posicion].fallecimiento.dia < 1 || 31 < array->a[posicion].fallecimiento.dia || array->a[posicion].fallecimiento.mes < 1 || 12 < array->a[posicion].fallecimiento.mes || array->a[posicion].fallecimiento.anio < 1930 || 2023 < array->a[posicion].fallecimiento.dia ){
                    printf("\nError la fecha ingresada es incorrecta.\n");
                }else{
                    printf("\nSe modifico la fecha de fallecimiento de la persona!\n");
                }
                }
                else
                {
                    printf("\nPrimero debe modificar la condicion de la persona!\n");
                }
                break;
            case 10:
                break;
            default:
                printf("Opción inválida. Por favor, elija una opción válida.\n");
            }
        } while (opc != 10);
        
        // Mostrar la persona modificada
        printf("\nPersona modificada:\n");
        printf("Nombre: %s\n", array->a[posicion].nombre);
        printf("Apellido: %s\n", array->a[posicion].apellido);
        printf("DNI: %d\n", array->a[posicion].DNI);
        printf("Fecha de Nacimiento: %d/%d/%d\n", array->a[posicion].nacimiento.dia, array->a[posicion].nacimiento.mes, array->a[posicion].nacimiento.anio);
        printf("Ciudad de Residencia: %s\n", array->a[posicion].ciudad);
        printf("Dirección Postal: %s\n", array->a[posicion].DP);
        printf("Correo Electrónico: %s\n", array->a[posicion].CE);
        printf("Teléfono: %s\n", array->a[posicion].tel);
        if (array->a[posicion].VF == 0)
        {
            printf("Vive: No\n");
            printf("Fecha de fallecimiento:\n");
            printf(" %d/", array->a[posicion].fallecimiento.dia);
            printf(" %d/", array->a[posicion].fallecimiento.mes);
            printf(" %d ", array->a[posicion].fallecimiento.anio);
            printf("\n");
        }
        else
        {
            printf("Vive: Si\n");
        }
        // Ordenar por nombre
        ordenarPorNombre(array);
        // Ordenar por apellido
        ordenarPorApellido(array);
       
        printf("\nPersona modificada con éxito.\n");
    }
    else
    {
        printf("\nPersona inexistente.\n");
    }
}
