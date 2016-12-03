# TP4-SO-2016 

### Laboratorio 4 - Implementación de malloc y free- 2016 
![Icono](http://icons.iconarchive.com/icons/elegantthemes/beautiful-flat/128/stack-icon.png)
___
|Autores                               |
|------------------------------------|
|Gonzalez Somerstein Gustavo -7721064  |
|Maero Facundo - 38479441              |

## 1. Descripción del trabajo
---
Este práctico consiste en la implementación de las funciones de reserva y liberación  de memoria en C, llamadas `myMalloc(1)` y `myFree(1)`.

## 2. Instrucciones de instalación y de uso
---
 - Para instalar los módulos, descargue el repositorio en su computadora.
 - Luego, mediante la terminal, ejecute los siguientes comandos para descomprimir el archivo .zip y situarse en la carpeta donde se halla el Makefile:
```
% cd Downloads
% unzip TP4-SO-2016-master.zip
% cd TP4-SO-2016-master/work/code
```
 - Para comenzar el proceso de compilación, ejecute el comando:
```
% make
```
 
- Luego, para ejecutar el programa y observar como trabajan las funciones implementadas, ingrese:
```
% ./malloc
```

- En consola observará la dirección de los bloques de control de diferentes variables, a medida que se solicita más memoria, las direcciones crecen. Pero luego de liberar variables y asignar otras nuevas, puede verse que los bloques libres intermedios se combinan para poder alojar variables de mayor tamaño.

- Al compilar y linkear, se genera un archivo donde se guardan los posibles errores y advertencias que encuentre el programa CppCheck al realizar el análisis estático del código. Este archivo se encuentra en:
```
work/code/err.txt
```
Si desea más información, remítase a la documentación proporcionada, que se encuentra en la ruta ```doc/html/index.html```