# Proyecto 1 - Servidor Web que calcula sumas de Goldbach en paralelo

Este proyecto consiste en la creación e implementación un servidor funcional que interactúa con un cliente, el cual le brinda números al servidor para los cuales desea obtener sus sumas de Goldbach.

El servidor web maneja http y está esperando contínuamente solicitudes para procesarlas, en modelo cliente servidor.

La idea central de estre proyecto es en primera instancia implementar el método de calcular las sumas de Goldbach en el código del servidor inicialmente serial el cuál se nos fué brindado, junto con código útil para la tarea, por los profesores del curso. Luego incrementar el desempeño d al utilizar paralelismo y concurrencia junto con nuestro código para hacer los cálculos matemáticos.

## ¿Qué es una suma de Goldbach?
En 1742 Christian Goldbach de Prusia le escribió una carta al matemático suizo Leonhard Euler con una conjetura:

Todo número entero mayor que 5:

Si es par se puede escribir como suma de dos números primos (conjetura fuerte). Por ejemplo: 6=3+3, 8=3+5, 14=3+11=7+7.

Si es impar se puede escribir como suma de tres números primos (conjetura débil). Por ejemplo.: 7=2+2+3, 9=2+2+5, 9=3+3+3.

Euler no pudo demostrar la conjetura, ni tampoco cientos de matemáticos convirtiéndolo en el problema abierto más intrigante en la historia de la teoría de números. Algunas novelas y películas hacen alusión a la conjetura, y entre los años 2000 y 2002 se ofreció un premio de un millón de dólares a quien lograse demostrarla, premio que no fue reclamado. En el 2013, tras 271 años de su aparición, el matemático peruano Harald Helfgott propuso una demostración para la conjetura débil que está actualmente sometida a revisión de pares, y en caso de validarse convertiría la conjetura débil en teorema.

## La Aplicación: Cálculo de sumas de Goldbach
La aplicación a la que llama el servidor se encarga de recibir y procesar solicitudes, las cuales delega a hilos con el proceso de cálculo de sumas de Goldbach.

e.g:
```
* 42: 4 sums
* -42: 4 sums: 5 + 37, 11 + 31, 13 + 29, 19 + 23
```
En las siguientes iteraciones utilizaremos múltiples hilos para aprovechar la capacidad del procesador usado podemos obtener un mejor tiempo de ejecución al realizar varios de estos cálculos.

---

## Compilación y manual de usuario.
1. Aceder la carpeta *concurrente21a-por-determinar* <br/><br/>
2. Una vez ahí, puede utilizar el comando `make` en la terminal para compilar todo el programa. Una vez que este termina la compilación, puede pedir un puerto por parte del servidor web utilizando un llamado al servidor de la siguiente manera: `bin/webserv ####` . Donde #### es un valor de puerto entre 1024 y 49151.

<br/><br/>

3. Una vez que este mensaje se despliega, se puede acceder a la aplicación escribiendo en un navegador web: `localhost:####` . (Siendo #### el número de puerto asignado)

<br/>
- Para calcular las sumas de Goldbach de un solo número, se puede ingresar en la caja de texto en el navegador. <br/><br/>
- Si desea que el programa calcule las sumas de varios números deberá hacerlo via el URL de la siguiente manera: `localhost:####/numero1,numero2,numero3` . Siendo #### el número de puerto asignado y *numero#* los números a procesar. <br/>
Al presionar la tecla *enter* se le redirigirá a la página con los resultados.


<br/>

* Al utilizar el parámetro `-` antes de un número, por ejemplo: -42, de deplegarán las sumas como tal además de la cantidad de sumas posibles. <br/>

* En caso de introducir un número inválido se deberá volver a la página principal o intoducir los nuevos números deseados a través del URL nuevamente para proceder.



# Contribuyentes-Equipo Por Determinar
* Fabián Alonso González Rojas *B83493*<br/>
* Jose Alberto Solano Murillo *B87645*
