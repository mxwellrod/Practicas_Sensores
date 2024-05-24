El codigo de la detección hace uso de las siguientes librerias:

-BBTimer.hpp: Librería utilizada para el manejo de temporizadores por hardware.
-mbed.h: Librería de Mbed OS para el desarrollo en ARM.
-Streaming.h: Librería que facilita la impresión en el puerto serie.
-ArduinoBLE.h: Librería para el manejo de BLE en Arduino.
-Arduino_LSM9DS1.h: Librería para la comunicación con el sensor IMU LSM9DS1.

El corazon del código se encuentra en la máquina de estados implementada mediante la instrucción switch,
lo que permite recorrer las distintas etapas de la detección de puntillas a la par que se miden distintos
parametros de interés como son:

-Tiempo que tarda en ponerse de puntillas (desde parado hasta puntillas máxima)
-Tiempo que permanece de puntillas (desde parado hasta parado)
-Ángulo máximo
-Ángulo mínimo (mientras aguanta de puntillas)
-Ángulo medio (mientras aguanta de puntillas)

