# Proyecto 2 - Bosque Encantado Concurrente

## Descripción del problema

Este proyecto consiste en la creación e implementación un programa el cual transforma matrices siguiendo un set específico de reglas, las especifícaciones del proyecto según su enunciado son las siguientes:

Un rey generoso de una isla muy lejana, estaba preocupado porque sus ciudadanos estaban aburridos de la monotonía de la pequeña isla. Tuvo la idea de encantar el bosque que está alrededor de su castillo, de tal forma que cada medianoche el bosque cambie y al día siguiente los ciudadanos encuentren un lugar diferente para recrearse. A su mago le pareció genial la idea, pero no sabe qué reglas incluir en el hechizo para que el bosque se mantenga en equilibrio y no llegue a convertirse en un desierto o una selva impenetrable. Si tuvieran alguna forma de ver el efecto de las reglas a futuro, podrían decidir el hechizo con mayor seguridad.

El rey tiene un mapa del bosque como el extracto que puede verse más adelante en el ejemplo de entrada (` map001.txt`). El mapa ilustra lo que hay en cada metro cuadrado de la isla, a los que les llaman celdas. Una celda puede contener un árbol mágico ( ` 'a' ` ), un trozo de lago encantado ( ` 'l' ` ), o pradera donde pueden transitar los ciudadanos ( ` '-' ` ). Las dimensiones del mapa se encuentran en la primera línea de entrada, indicadas como el número de filas y columnas.

Las reglas que quieren probarse en cada medianoche trabajan en una celda a la vez y consideran las 8 celdas vecinas que tiene alrededor. Las reglas son las siguientes.

    1. *Inundación*: Si la celda tiene un árbol y al menos 4 vecinos que son lago encantado, entonces el lago ahoga el árbol, y pasa a ser lago encantado.
    2. *Sequía*: Si la celda es lago encantado y tiene menos de 3 vecinos que sean lago encantado, entonces el lago se seca y pasa a ser pradera.
    3. *Reforestación*: Si la celda es pradera y tiene al menos 3 vecinos árboles, las semillas tendrán espacio para crecer y la celda se convierte en árbol.
    4. *Hacinamiento*: Si la celda es un árbol y tiene más de 4 vecinos árbol, el exceso de sombra evita que crezca y entonces pasa a ser pradera.
    5. *Estabilidad*: Cualquier otra situación, la celda permanece como está.

El programa ayuda a probar las reglas mágicas anteriores en varios mapas. El programa recibe una orden de trabajo que el mago quiere probar. La orden de trabajo es un archivo cuyo nombre se envía por argumento de línea de comandos. En su contenido, la orden de trabajo lista algunos mapas iniciales de la isla, seguida de un número que indica la cantidad de medianoches en las que quiere probarse el efecto de las reglas del potencial hechizo.

### Ejemplo de un orden de trabajo `job001.txt`
```
map001.txt 2
map002.txt -100
map003.txt -20000
```
En la orden de trabajo, job001.txt el número que sucede al nombre de archivo indica la cantidad de noches en las que se debe probar el efecto de las reglas. Si el número es positivo, el programa debe producir un archivo con el estado del mapa en cada amanecer. Por ejemplo, para el primer mapa `map001.txt`, se solicita probar el efecto en 2 noches. En tal caso, el programa produce un archivo de salida con el mapa resultante a la primera medianoche con nombre `map001-1.txt`, y el resultado de la segunda medianoche en el archivo `map001-2.txt`. Estos números positivos permiten al mago rastrear el efecto de las reglas paso a paso.

Cuando el número de noches es negativo, como -100 para el `map002.txt`, el programa debe aplicar las reglas mágicas esa cantidad de noches, y producir como salida un único archivo con el mapa resultante después de la última medianoche, que para el segundo mapa sería `map002-100.txt`. Estos números negativos permiten al mago conocer si a largo plazo sus reglas producen una topografía razonable o si debe cambiarlas.

Note que el nombre del archivo de salida usa como prefijo el nombre del archivo mapa, seguido por un guión y el número de medianoche. Por ejemplo el archivo `map003.txt` representa el mapa de la isla en su estado actual, lo que equivale a la medianoche cero (`map003-0.txt`). El archivo `map003-20000.txt` representa esa misma isla 20,000 medianoches después (casi 55 años). Su programa podría partir de un mapa cualquiera y visualizar el efecto cierta cantidad de noches posteriores. Por ejemplo, si el mago quisiera conocer cómo se vería la isla 100 años después, podría escribir una orden de trabajo con cualquiera de las dos siguientes líneas equivalentes:
```
map003.txt -36500
map003-20000.txt -16500
```

La orden de trabajo anterior muestra que una salida del programa puede ser usada como entrada para el mismo. En las dos líneas equivalentes de la orden anterior, naturalmente la segunda es más eficiente, y le permitiría al mago obtener los esperados resultados en menor tiempo.

En cuanto a los archivos de mapa, como `map001.txt` y `map003-20000.txt`,  contienen la isla o un trozo de ella representado como una matriz de caracteres. En el primer renglón del archivo se indica la cantidad de filas y columnas de la matriz. Cada carácter en los renglones subsecuentes corresponden a una celda de la matriz (a excepción de los cambios de línea). Por ejemplo:

### Contenido del archivo `map001.txt`:
```
7 7
-------
-l--l--
-ll----
-l-----
---laa-
-aa-al-
a-a----
```
### Ejemplo de salida map001-1.txt:
```
-------
-------
-ll----
-------
----aa-
-aaaa--
aaaa---
```

## Compilación y manual de usuario. (Build)
1. Aceder la carpeta `concurrente21a-por-determinar/proyecto2` <br/><br/>
2. Una vez ahí, puede utilizar el comando `make` en la terminal para compilar todo el programa. <br/><br/>
3. Una vez que este termina la compilación, puede correr el programa tanto usando los comandos en terminal `./bin/proyecto2` seguido por sus parámetros, como lo serían un map, ejemplo: `./bin/proyecto2 map001.txt 200` o un job, ejemplo: `./bin/proyecto2 job002*`.
- También por comodidad puede usar el comando `make run` en la terminal seguido por sus parámetros a utilizar.
- El primer parámetro es el directorio donde se encuentra el mapa o trabajo a ejecutar.
- El segundo parámetro es el nombre del mapa o trabajo a ejecutar.
- El tercer parámetro es solo utilizado en el caso de que se quiera procesar un mapa y no un trabajo, y consiste de la cantidad de noches que se quiere avanzar en la simulación. Si este parámetro es un entero negativo solo se exportará el resultado de la última noche simulada, si fuera un entero positivo se exporta el resultado de cada noche iterada.<br/><br/>
3. Los resultados del llamado a la aplicación se encuentran en la carpeta del repositorio llamada */results*
-Para más información acerca del formato de respuestas y entradas ver la sección *Ejemplo de un orden de trabajo*
en la parte superior de este README.
<br/><br/>

## Análisis de rendimiento para OpenMP

  Es bastante evidente para este problema donde la seguridad condicional es perfecta y existen matrices muy grandes, que el incremento de velocidad será bastante elevado a la hora de utilizar concurrencia. Ahora vamos a analizar el porqué ciertos mapeos tuvieron un mejor rendimiento.

-*Bloque*: Este mapeo fue el peor en speedup con 5.31, es bastante lógico el porqué ocurre esto; si el número de filas se distribuye de modo que un hilo se quedó con una o más filas que los otros hilos, los demás tendrán que esperar para poder terminar la función. Una sección concurrente es tan rápida como el más lento de sus hilos. En este problema las búsquedas y comparaciones tienen una duración prácticamente igual para todas las celdas, por lo que es entendible el porque tener un hilo con más trabajo afecta el speedup.

-*Cíclico*: Este mapeo fue el segundo mejor detrás del dinámico, con 5.70 de speedup. A diferencia del mapeo por bloque, este no posee el problema de tener un hilo más lento que los demás. Todos los hilos hacen una distribución casi perfecta y es prácticamente innotable la espera entre hilos, ya que como dije anteriormente, la duración entre completar una fila u otra no varía en prácticamente nada.

-*Bloque cíclico*: Este mapeo se realizó con una distribución cíclica de bloque de 4, su speedup es de 5.67 por lo que se acerca ba-*stante al cíclico. El hilo más lento no atrasa prácticamente nada ya que la máxima cantidad de filas que puede hacer un hilo es uno más al resto, creando pocos imbalances. No obstante, esta pequeña diferencia lo sitúa detrás del mapeo cíclico para este problema.

-*Dinámico*: El mejor speedup con 5.76, la diferencia de velocidad no es muy grande comparada con el mapeo cíclico. Su incremento de rendimiento se debe a que, aunque las filas terminan en velocidades casi iguales, existen posibles filas que terminan ligeramente más rápido al tener menos comparaciones en su ruta. Esta sutil diferencia lo vuelve el mejor mapeo para este problema.

-*Guiado*: Su speed up fue de 5.41, el segundo peor. Este rendimiento se debe posiblemente a que al realizar todas las distribuciones de hilos en un problema que realmente no tiene mucha variación de tiempo entre cada iteración de una fila, se toma más tiempo organizando el trabajo del que se gana en rendimiento.

![Gráfico con el análisis de rendimiento con OMP](https://git.ucr.ac.cr/FABIAN.GONZALEZROJAS/concurrente21a-por-determinar/-/blob/master/proyecto2/design/an%C3%A1lisis_de_rendimiento.jpg)

## Análisis de rendimiento para MPI
  Para la segunda entrega de este proyecto estamos usando la tecnología MPI, que por sus siglas en inglés significa Message Passing Interface, la cual es un modo de compartir información entre procesos a través de la red local. En este proyecto se usa dicha tecnología para poder crear diferentes procesos en los nodos del cluster Arenal de la Universidad de Costa Rica, y que estos se puedan comunicar entre si y distribuírse la carga de trabajo asignada.
  Se realizaron dos pruebas de rendimiento utilizando la tecnología MPI, una en la que se crearon 24 instancias de procesos y a cada uno se le mandaba la información de un mapa en el cuál trabajar, y otra en la cual solo se crean 3 procesos pero se usaba en conjunto con la tecnología OMP para utilizar varios hilos al procesar los datos.

  Al final el mejor desempeño del calculo de los bosques se obtuvo con la versión Omp + Mpi. Esto es bastante lógico si tomamos en cuenta que en el proceso mas pesado, es decir, el mapa de mas tamaño o noches de un nodo esclavo, va a seguir ejecutándose mucho tiempo después de ya haber terminado los demás procesos. Esto es lo que causa que la versión MPI a pesar de tener 24 núcleos trabajando al mismo tiempo, no obtenga grandes mejoras, ya que la mayoría de esos núcleos estarán inactivos gran parte del proceso. Lo que no ocurre con la versión OMP o Omp + mpi. Claramente los 8 núcleos de un solo nodo no pueden competir con los 24 de 3 nodos distribuidos. La versión OMP + MPI es la clara ganadora y la que aprovecha mejor los recursos disponibles en las maquinas.

  Si algo se aprendió una y otra vez observando diferentes métodos de mapeo a través del curso, es que un programa paralelo es tan rápido como el mas lento de sus hilos o procesos. Este problema sirvió perfectamente para demostrar lo anterior y el gráfico lo hace aun mas evidente:

![Grafico de análisis de rendimiento con MPI](https://git.ucr.ac.cr/FABIAN.GONZALEZROJAS/concurrente21a-por-determinar/-/blob/master/proyecto2/design/perf_measure-proy02_2.png)<br/>
*La barra azul del gráfico representa el speedup, cuyo eje es el izquierdo; y la barra anaranjada del gráfico representa la eficiencia, con su eje siendo el derecho*

## Diseño: Diagrama de clases
![Diagrama UML de clases](https://git.ucr.ac.cr/FABIAN.GONZALEZROJAS/concurrente21a-por-determinar/-/blob/master/proyecto2/design/Proyecto2_UML.jpg?raw=true)

# Contribuyentes-Equipo Por Determinar
* Fabián Alonso González Rojas *B83493*<br/>
* Jose Alberto Solano Murillo *B87645*
