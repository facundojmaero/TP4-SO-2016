/**
 * @file   main.c
 * @author Facundo Maero, Gustavo Gonzalez
 * @date   Diciembre 2016
 * @version 0.1
 * @brief Programa de prueba que utiliza funciones propias imitando la funcionalidad del malloc y free en C
 *
*/

#include "libreria.h"

  /**
  * @brief Funcion principal que realiza varios malloc y free para demostrar el funcionamiento del codigo
  *
  */
int main(){
	printf("Implementacion de malloc y free en C\n\n");

	char *str;

	str = (char *) myMalloc(15);
	strcpy(str, "tutorialspoint");
	printf("String str = %s\n", str);
	printf("%p -> direccion de str\n",myGetBlock(str));

	myFree(str);
	printf("Libero str\n\n");

	char *a;
	a = (char *) myMalloc(10);
	printf("malloc de a (10)\n");
	printf("%p -> direccion de a\n\n",myGetBlock(a));

	char *b,*c,*d;

	b = (char*) myMalloc(15);
	c = (char*) myMalloc(20);
	d = (char*) myMalloc(5);
	printf("malloc de b (15), c (20) y d (5)\n");
	printf("%p -> direccion de b\n",myGetBlock(b));
	printf("%p -> direccion de c\n",myGetBlock(c));
	printf("%p -> direccion de d\n",myGetBlock(d));

	myFree(b);
	myFree(c);
	printf("\nlibero b y c\n\n");

	char *e;
	e = (char*) myMalloc(30);
	printf("malloc de e (30)\n");
	printf("%p -> direccion de e\n",myGetBlock(e));

	myFree(d);
	printf("\nlibero d\n\n");

	char *f;
	f = (char*) myMalloc(10);
	printf("malloc de f (10)\n");
	printf("%p -> direccion de f\n",myGetBlock(f));

	return 0;
}

  /**
  * @brief Libera un bloque de memoria
  * @param p Puntero a la region de datos del bloque a liberar 
  */
void myFree(void *p){
	t_block b;
	if ( myValidAddr (p))
	{
		b = myGetBlock (p);
		b->free = 1;

		/* myFusion with previous if possible */
		if(b->prev && b->prev->free){	
			b = myFusion (b->prev );
		}
		/* then myFusion with next */
		if (b->next){
			myFusion (b);
		}
		else
		{
			/* myFree the end of the heap */
			if (b->prev)
				b->prev ->next = NULL;
			else
				/* No more block !*/
				base = NULL;
			brk(b);
		}
	}
}

  /**
  * @brief A partir de la direccion a la region de datos de un bloque, obtiene la direccion del bloque de control.
  * @param p Puntero a la region de datos del bloque de interes
  * @return la direccion de la estructura de control del bloque
  */
t_block myGetBlock (void *p){
	char *tmp;
	tmp = (char*)p;
	tmp = tmp - BLOCK_SIZE;
	return (t_block) tmp;
}

  /**
  * @brief Valida si el puntero es el resultado de un malloc previo
  * @param p Puntero a la region de datos del bloque de interes
  * @return 1 si es una direccion valida, o 0 si no lo es
  */
int myValidAddr (void *p){
	if (base)
	{
		if ( p>base && p< sbrk (0))
		{
			return (p == (myGetBlock (p))->ptr);
		}
	}
	return (0);
}

  /**
  * @brief Divide el bloque en dos
  * @param p Puntero a la region de datos del bloque de interes
  * @param s Tamano de uno de los bloques que se quiere obtener
  */
void mySplitBlock ( t_block b, size_t s){
	t_block newBlock;
	newBlock = (t_block) (b->data + s);
	newBlock ->size = b->size - s - BLOCK_SIZE ;
	newBlock ->next = b->next;
	newBlock ->prev = b;
	newBlock ->free = 1;
	newBlock ->ptr = newBlock->data;
	b->size = s;
	b->next = newBlock;
	if(newBlock->next)
		newBlock->next->prev = newBlock;
}

  /**
  * @brief Agranda la region de memoria del proceso desplazando la heap
  * @param p Puntero a la region de datos del ultimo bloque
  * @param s Tamano que se quiere expandir la heap
  * @return la direccion del ultimo bloque en la heap luego de expandirla
  */
t_block myExtendHeap ( t_block last , size_t s){
	t_block b;
	b = (t_block) sbrk (0);
	if(sbrk(BLOCK_SIZE + s) < 0)
		return (NULL );
	b->size = s;
	b->next = NULL;
	b->prev = last;
	b->ptr = b->data;
	if (last)
		last ->next = b;
	b->free = 0;
	return (b);
}

  /**
  * @brief Busca un bloque para reservar memoria, a partir del tamano buscado y el puntero al ultimo bloque encontrado, con el algoritmo de First Fit
  * @param last Puntero a la region de datos del ultimo bloque encontrado
  * @param size Tamano del bloque que se quiere buscar
  * @return la direccion del bloque encontrado
  */
t_block myFindBlock ( t_block *last , size_t size ){
	t_block b = (t_block) base;
	while (b && !(b->free && b->size >= size )) {
		*last = b;
		b = b->next;
	}
	return (b);
}

  /**
  * @brief Funcion principal que reserva bloques de memoria
  * @param size Tamano del bloque a reservar
  */
void * myMalloc ( size_t size ){
	t_block b,last;
	size_t s;
	s = align4 (size);
	if (base) {
		/* First find a block */
		last = (t_block) base;
		b = myFindBlock (&last ,s);
		if (b) {
			/* split the block */
			if ((b->size - s) >= ( BLOCK_SIZE + 4))
			mySplitBlock (b,s);
			b->free =0;
		} else {
			/* No fitting block , extend the heap */
			b = myExtendHeap (last ,s);
			if (!b)
			return (NULL );
		}
	} else {
		/* first time */
		b = myExtendHeap (NULL ,s);
		if (!b)
			return (NULL);
		base = b;
		b->ptr = &b->data;
	}
	return (b->data );
}

  /**
  * @brief Intenta fusionar bloques de memoria vacios para formar uno mas grande
  * @param b Puntero a la region de datos del bloque de interes
  * @return la direccion del bloque fusionado
  */
t_block myFusion ( t_block b){
	if (b->next && b->next ->free ){
		b->size += BLOCK_SIZE + b->next ->size;
		b->next = b->next ->next;
		if (b->next)
		b->next ->prev = b;
	}
	return (b);
}