# -*- coding: utf-8 -*-
"""
Created on Thu Feb 22 20:06:06 2024

@author: Maxwell
"""

import serial
import time

# Configura el puerto serie
puerto_serie = serial.Serial('COM10', 115200, timeout=1)

# Nombre del archivo de texto
nombre_archivo = "datos_sensor.csv"

try:
    # Abre el archivo en modo escritura
    with open(nombre_archivo, 'w') as archivo:
        # texto fijo para asegurarte de que el archivo se crea correctamente
        archivo.write("Inicio de datos:\n")

    while True:
        # Lee los datos del puerto
        datos = puerto_serie.readline().decode().strip()
        
        # Verifica que haya datos antes de guardarlos en el archivo
        if datos:
            print(datos)
            # Abre el archivo en modo adición para agregar nuevos datos
            with open(nombre_archivo, 'a') as archivo:
                # Escribe los datos en el archivo, precedidos por una etiqueta que indica el tipo de sensor
                archivo.write("Acelerometro: " + datos + "\n")
        
        # Lee los siguientes datos (giroscopo)
        datos = puerto_serie.readline().decode().strip()
        
        # Verifica que haya datos antes de guardarlos en el archivo
        if datos:
            print(datos)
            # Abre el archivo en modo adición para agregar nuevos datos
            with open(nombre_archivo, 'a') as archivo:
                # Escribe los datos en el archivo, precedidos por una etiqueta que indica el tipo de sensor
                archivo.write("Giroscopo: " + datos + "\n")
        
        # Espera 50ms
        time.sleep(0.05)

except KeyboardInterrupt:
    # Ctrl+C
    print("Interrupción del teclado detectada.")
    puerto_serie.close()