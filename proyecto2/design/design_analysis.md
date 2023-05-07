1. Descripcion del MPI
Cada proceso de MPI va a realizar el trabajo de procesar un mapa, excepto el primero,
el cual se va a encargar de distribuir los mapas de manera dinamica a los demas procesos y medir el tiempo.

2. Descripcion del OMP
La creacion de hilos de OMP se da al empezar a avanzar los mapas cada noche, toma una fila de manera dinamica y la procesa.
Estos hilos procesan cada uno una fila de la matriz para cada noche adentro de un ciclo del tamano de las filas.

3. Una ventaja y desventaja de los puntos anteriores
-Ventaja de OMP: Al ser declarativo es mas simple de controlar, ajustar y programar. Ademas de no haber
 esperas activas entre procesos. Tiene distribucion mas equitativa de trabajo en general, evita el que hayan hilos ociosos.
 Desventaja: En caso de que haya mas columnas que filas el programa baja su eficiencia ya que la distribucion de trabajo es menos
 granular y por lo tanto mas irregular.
 
-Ventaja de MPI: Al distribuir de manera dinamica la carga no hay tanto problema de disparidad de trabajo como en el caso de
que si hay un mapa gigante los demas hilos van a poder trabajar en los suyos y no sobrecargar a este de trabajo.
Desventaja de MPI: Tiene posibilidad de tener hilos o procesos ociosos en espera de que los ultimos terminen cuando hay menos mapas que procesos.
En caso de no tener el nodo proncipal disponible

4. Separacion de asuntos
El primer tipo de proceso es el proceso 0, el cual determina los mapas a procesar y los envia a los
procesos trabajadores para que estos se encarguen de procesarlos.
El segundo tipo son los trabajadores, estos reciben los datos necesarios para procesar un mapa y
proceden a avanzarlo para luego exportar su resultado.

5. Entrada y salida de los procesos
El proceso 0 recibe como entrada el job o mapa deseado a procesar, tambien manda a salida el archivo
`time.txt` que contiene el tiempo tomado por todos los procesos en procesar el trabajo.
Los procesos trabajadores por su lado no reciben el mapa por parametro, sino que lo reciben por MPI
de parte del proceso 0; en cuanto a salida se encargan de exportar el resultado de las operciones pedidas a la carpeta `results`.
