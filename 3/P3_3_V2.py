# -*- coding: utf-8 -*-
"""
Created on Thu Feb 22 20:06:06 2024

@author: Maxwell
"""

import serial
import time
import csv

# Configura el puerto serie
puerto_serie = serial.Serial('COM10', 115200, timeout=1)

# Nombre del archivo CSV
nombre_archivo = "datos_sensor.csv"

try:
    # Abre el archivo en modo escritura
    with open(nombre_archivo, 'w', newline='') as archivo_csv:
        # Crea un escritor CSV
        escritor_csv = csv.writer(archivo_csv)
        
        # Escribe la primera fila como encabezado
        escritor_csv.writerow(["X", "Y", "Z"])  # Modifica los nombres de las columnas según tus datos
        
        while True:
            # Lee los datos del puerto
            datos = puerto_serie.readline().decode().strip()
            
            # Verifica que haya datos antes de guardarlos en el archivo
            if datos:
                print(datos)
                # Divide los datos por espacios (o cualquier otro separador) según el formato y crea una lista
                datos_lista = datos.split(",")
                
                # Escribe los datos en el archivo CSV
                escritor_csv.writerow([time.time()] + datos_lista)
            # Espera 50ms
            time.sleep(0.05)

except KeyboardInterrupt:
    # Ctrl+C
    print("Interrupción del teclado detectada.")
    puerto_serie.close()
