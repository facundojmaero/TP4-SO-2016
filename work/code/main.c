# include <sys/types.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# define align4(x) ((((( x) -1) >>2) <<2)+4)
# define BLOCK_SIZE 20

# define TAMANO sizeof ( struct s_block )

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
void * realloc (void *p, size_t size);
void free(void *p);
t_block get_block (void *p);
int valid_addr (void *p);
void split_block ( t_block b, size_t s);
t_block extend_heap ( t_block last , size_t s);
t_block find_block ( t_block *last , size_t size );
void * malloc ( size_t size );
void * calloc ( size_t number , size_t size );
t_block fusion (t_block b);
void copy_block ( t_block src , t_block dst);

void *base = NULL;

int main(){
	char *str;

   /* Initial memory allocation */
   str = (char *) malloc(15);
   strcpy(str, "tutorialspoint");
   printf("String = %s\n", str);

   free(str);
   printf("FLAAAAG\n");
	return 0;
}

void * realloc (void *p, size_t size){
	size_t s;
	t_block b, newBlock;
	void *newp;
	if (!p){
		return ( malloc (size));
	}
	if ( valid_addr (p))
	{
		s = align4 (size);
		b = get_block (p);
		if (b->size >= s)
		{
			if (b->size - s >= ( BLOCK_SIZE + 4))
				split_block (b,s);
		}
		else
		{
			/* Try fusion with next if possible */
			if (b->next && b->next ->free && (b->size + BLOCK_SIZE + b->next ->size) >= s)
			{
				fusion (b);
				if (b->size - s >= ( BLOCK_SIZE + 4))
					split_block (b,s);
			}
			else
			{
				/* good old realloc with a new block */
				newp = malloc (s);
				if (! newp)
					/* we ’re doomed ! */
					return (NULL );
				/* I assume this work ! */
				newBlock = get_block (newp );
				/* Copy data */
				copy_block (b,newBlock );
				/* free the old one */
				free(p);
				return (newp );
			}
		}
		return (p);
	}
	return (NULL );
}

void free(void *p){
	t_block b;
	if ( valid_addr (p))
	{

		b = get_block (p);
		b->free = 1;

		/* fusion with previous if possible */
		if(b->prev && b->prev->free){	
			b = fusion (b->prev );
		}
		/* then fusion with next */
		if (b->next){
			fusion (b);
		}
		else
		{
			/* free the end of the heap */
			if (b->prev)
				b->prev ->next = NULL;
			else
				/* No more block !*/
				base = NULL;
			brk(b);
		}
	}
}

/* Get the block from and addr*/
t_block get_block (void *p){
	char *tmp;
	tmp = (char*)p;
	return (t_block) (p = tmp -= BLOCK_SIZE );
}

/* Valid addr for free*/
int valid_addr (void *p){
	if (base)
	{
		if(p == (get_block (p))->ptr)
			printf("si\n");
		if ( p>base && p< sbrk (0))
		{
			return (p == (get_block (p))->ptr);
		}
	}
	return (0);
}

void split_block ( t_block b, size_t s){
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

t_block extend_heap ( t_block last , size_t s){
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

t_block find_block ( t_block *last , size_t size ){
	t_block b = (t_block) base;
	while (b && !(b->free && b->size >= size )) {
		*last = b;
		b = b->next;
	}
	return (b);
}

void * malloc ( size_t size ){
	t_block b,last;
	size_t s;
	s = align4 (size );
	if (base) {
		/* First find a block */
		last = (t_block) base;
		b = find_block (& last ,s);
		if (b) {
			/* can we split */
			if ((b->size - s) >= ( BLOCK_SIZE + 4))
			split_block (b,s);
			b->free =0;
		} else {
			/* No fitting block , extend the heap */
			b = extend_heap (last ,s);
			if (!b)
			return (NULL );
		}
	} else {
		/* first time */
		b = extend_heap (NULL ,s);
		if (!b)
		return (NULL );
		base = b;
	}
	return (b->data );
}

void * calloc ( size_t number , size_t size ){
	size_t *newBlock;
	size_t s4 ,i;
	newBlock = (size_t*) malloc ( number * size );
	if (newBlock) {
		s4 = align4 ( number * size) << 2;
		for (i=0; i<s4 ; i++)
			newBlock[i] = 0;
	}
	return (newBlock);
}

t_block fusion ( t_block b){
	if (b->next && b->next ->free ){
		b->size += BLOCK_SIZE + b->next ->size;
		b->next = b->next ->next;
		if (b->next)
		b->next ->prev = b;
	}
	return (b);
}

/* Copy data from block to block*/
void copy_block ( t_block src , t_block dst){
	int *sdata ,* ddata ;
	size_t i;
	sdata = (int*) src ->ptr;
	ddata = (int*) dst ->ptr;
	for (i=0; i*4<src ->size && i*4<dst ->size; i++)
		ddata [i] = sdata [i];
}