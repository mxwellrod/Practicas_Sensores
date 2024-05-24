# -*- coding: utf-8 -*-
"""
Created on Thu Feb 22 20:06:06 2024

@author: Maxwell
"""

import serial
import time
import numpy as np
import matplotlib.pyplot as plt

# Listas para almacenar los datos acumulados durante 5 segundos
acumulador_acelerometro_x = []
acumulador_acelerometro_y = []
acumulador_acelerometro_z = []

tiempo_acumulacion = 5
tiempo_inicio_acumulacion = time.time()

puerto_serie = serial.Serial('COM15', 115200, timeout=1)

plt.ion()
fig, ax = plt.subplots(figsize=(10, 8))

linea_x, = ax.plot([], [], label='X')
linea_y, = ax.plot([], [], label='Y')
linea_z, = ax.plot([], [], label='Z')

ax.set_title("Acelerómetro")
ax.set_xlabel('Tiempo')
ax.set_ylabel('Valor')
ax.legend()

tiempo_transcurrido = 0

while tiempo_transcurrido < tiempo_acumulacion:
    dato = puerto_serie.readline().decode().strip()
    datos = dato.split(",")
    
    if datos:
        print("Datos recibidos:", datos)
    
    acumulador_acelerometro_x.append(float(datos[0]))
    acumulador_acelerometro_y.append(float(datos[1]))
    acumulador_acelerometro_z.append(float(datos[2]))

    tiempo_transcurrido = time.time() - tiempo_inicio_acumulacion
    
    # El eje x es el numero de puntos

    # linea_x.set_data(range(len(acumulador_acelerometro_x)), acumulador_acelerometro_x)
    # linea_y.set_data(range(len(acumulador_acelerometro_y)), acumulador_acelerometro_y)
    # linea_z.set_data(range(len(acumulador_acelerometro_z)), acumulador_acelerometro_z)
    
    # Conversión a tiempo
    
    linea_x.set_data(np.arange(len(acumulador_acelerometro_x)) * 0.05, acumulador_acelerometro_x)
    linea_y.set_data(np.arange(len(acumulador_acelerometro_y)) * 0.05, acumulador_acelerometro_y)
    linea_z.set_data(np.arange(len(acumulador_acelerometro_z)) * 0.05, acumulador_acelerometro_z)

    ax.relim()
    ax.autoscale_view()
    plt.draw()
    plt.pause(0.01)  # pausa para actualizar el plot

# Calcula el promedio y la desviación estándar solo para el acelerómetro
promedio_acelerometro_x = np.mean(acumulador_acelerometro_x)
promedio_acelerometro_y = np.mean(acumulador_acelerometro_y)
promedio_acelerometro_z = np.mean(acumulador_acelerometro_z)

desviacion_std_acelerometro_x = np.std(acumulador_acelerometro_x)
desviacion_std_acelerometro_y = np.std(acumulador_acelerometro_y)
desviacion_std_acelerometro_z = np.std(acumulador_acelerometro_z)

print("Promedio del acelerómetro (x, y, z):", promedio_acelerometro_x, promedio_acelerometro_y, promedio_acelerometro_z)
print("Desviación estándar del acelerómetro (x, y, z):", desviacion_std_acelerometro_x, desviacion_std_acelerometro_y, desviacion_std_acelerometro_z)

plt.ioff()
plt.show()
puerto_serie.close()



        





