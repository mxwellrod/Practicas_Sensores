Extracción de datos:

En este código se implementa la lectura de datos de acelerómetro y giroscopio usando un Arduino BLE 33 y la librería `Arduino_LSM9DS1`. 
Los datos se envían a través del puerto serie con un intervalo de muestreo de 5ms, es decir, una frecuencia de 200 Hz.

El código requiere las siguientes librerías:
El código requiere las siguientes librerías:
- 'BBTimer.hpp': Para gestionar el temporizador.
- 'mbed.h': Librería del sistema operativo Mbed.
- 'Streaming.h': Para facilitar la impresión en el puerto serie.
- 'Arduino_LSM9DS1.h': Para la gestión de los sensores LSM9DS1.

Lo más destacable de este código es el cambio de referencia empleado con respecto a los ejes por defecto de los sensores(ard) y los establecidos para el proyecto(proj).
Los ejes se corresponden de la siguiente forma:
Acelerometro: ard_y = -proj_z ; ard_z = proj_y ; ard_x = proj_x
Giroscopo: ard_y = proj_z ; ard_z = proj_y ; ard_x = -proj_x 