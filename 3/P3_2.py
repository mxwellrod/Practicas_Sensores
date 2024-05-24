# -*- coding: utf-8 -*-
"""
Created on Thu Feb 22 20:06:06 2024

@author: Maxwell
"""

import serial
import time

# Configura el puerto serie, asegúrate de cambiar el puerto y la velocidad adecuadamente
puerto_serie = serial.Serial('COM10', 115200, timeout=1)

try:
    while True:
        # Lee los datos del puerto serie
        datos = puerto_serie.readline().decode().strip()
        
        # Verifica que haya datos antes de mostrarlos
        if datos:
            print("Datos recibidos:", datos)
        
        # Espera 50ms
        time.sleep(0.05)

except KeyboardInterrupt:
    # Ctrl+C
    print("Interrupción del teclado detectada. Saliendo...")
    puerto_serie.close()
