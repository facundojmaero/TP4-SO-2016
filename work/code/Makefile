# CC=gcc
# CFLAGS=-c 
# #-Wall -pedantic -O2
# OBJECTS=main.o funciones.o
# PATHOBJECTS=$(addprefix $(ODIR)/,$(OBJECTS))
# ODIR=obj
# LDIR=include
# SRCDIR=src

# all: malloc #cppcheck

# malloc: $(OBJECTS)
# 	@echo
# 	@echo Linkeando objetos
# 	$(CC) $(PATHOBJECTS) -o $@

# cppcheck:
# 	@echo
# 	@echo Realizando verificacion CppCheck
# 	@echo Los resultados de la misma se encuentran en err.txt
# 	cppcheck . 2> err.txt

# main.o: $(SRCDIR)/main.c $(LDIR)/ksamp.h
# 	@echo
# 	@echo Compilando codigos fuente
# 	$(CC) $(CFLAGS) $< -o $(ODIR)/$@

# funciones.o: $(SRCDIR)/funciones.c $(LDIR)/ksamp.h
# 	$(CC) $(CFLAGS) $< -o $(ODIR)/$@

# clean:
# 	@echo Borrando archivos objeto, codigo compilado y log de errores
# 	rm -rf *o $(ODIR)/*.o malloc $(LDIR)/*.gch err.txt

CC=gcc
FLAGS=-c -Wall -pedantic -O2

all: malloc cppcheck

malloc: main.o 
	gcc main.o -o malloc

main.o: main.c libreria.h
	$(CC) $(FLAGS) $< -o $@

# funciones.o: funciones.c libreria.h
# 	$(CC) $(FLAGS) $< -o $@

cppcheck:
	@echo
	@echo Realizando verificacion CppCheck
	@echo Los resultados de la misma se encuentran en err.txt
	cppcheck . 2> err.txt

clean:
	@echo Borrando archivos objeto, codigo compilado y log de errores
	rm -rf *o $(ODIR)/*.o malloc $(LDIR)/*.gch err.txt
