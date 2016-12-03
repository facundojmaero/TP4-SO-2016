/**
 * @file   libreria.h
 * @author Facundo Maero, Gustavo Gonzalez
 * @date   Diciembre 2016
 * @version 0.1
 * @brief Prototipos de funciones, includes y definiciones de tipos de datos a usar
 *
*/

# include <sys/types.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# define align4(x) (((((x) -1) >>2) <<2)+4) /*!< Macro para alinear cantidades de memoria a reservar */
# define BLOCK_SIZE 40 /*!< Tamano del bloque de control */

/*!< Estructura de control para cada bloque a asignar */
typedef struct s_block * t_block ;
struct s_block {
	size_t size;
	struct s_block *next;
	struct s_block *prev;
	int free;
	void *ptr;
	char data[1];
};

//Prototipos de funciones
void myFree(void *p);
t_block myGetBlock (void *p);
int myValidAddr (void *p);
void mySplitBlock ( t_block b, size_t s);
t_block myExtendHeap ( t_block last , size_t s);
t_block myFindBlock ( t_block *last , size_t size );
void * myMalloc ( size_t size );
t_block myFusion (t_block b);
void myCopyBlock ( t_block src , t_block dst);

/*!< Puntero a la base de la heap, o primer segmento asignado */
void *base = NULL;