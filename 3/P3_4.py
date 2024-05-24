# -*- coding: utf-8 -*-
"""
Created on Thu Feb 22 20:06:06 2024

@author: Maxwell
"""

import serial
import time
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Configura el puerto serie, asegúrate de cambiar el puerto y la velocidad adecuadamente
puerto_serie = serial.Serial('COM15', 115200, timeout=1)

# Listas para almacenar los datos
datos_tiempo = []
datos_acelerometro_x = []
datos_acelerometro_y = []
datos_acelerometro_z = []

# Variables para el tiempo de acumulación
tiempo_acumulacion = 5  # 5 segundos
tiempo_inicio_acumulacion = time.time()

# Configuración de Matplotlib para actualización automática
plt.ion()
fig, ax = plt.subplots()
linea, = ax.plot([], [], label='Acelerómetro X')
ax.set_xlabel('Tiempo (s)')
ax.set_ylabel('Valor')
ax.legend()

# Función para inicializar la gráfica
def init():
    linea.set_data([], [])
    return linea,

# Función de actualización de la gráfica
def update(i):
    global tiempo_inicio_acumulacion

    # Lee los datos del puerto serie
    datos = puerto_serie.readline().decode().strip()

    # Verifica que haya datos
    if datos:
        # Divide los datos en una lista de valores
        valores = datos.split(',')

        # Almacena los datos
        datos_tiempo.append(time.time() - tiempo_inicio_acumulacion)
        datos_acelerometro_x.append(float(valores[0]))
        datos_acelerometro_y.append(float(valores[1]))
        datos_acelerometro_z.append(float(valores[2]))

        # Verifica si ha pasado el tiempo de acumulación
        if time.time() - tiempo_inicio_acumulacion >= tiempo_acumulacion:
            # Calcula el promedio y la desviación estándar
            promedio_x = np.mean(datos_acelerometro_x)
            desviacion_std_x = np.std(datos_acelerometro_x)
            print("Promedio de Acelerómetro X:", promedio_x)
            print("Desviación estándar de Acelerómetro X:", desviacion_std_x)

            # Actualiza la gráfica
            linea.set_data(datos_tiempo, datos_acelerometro_x)
            ax.relim()
            ax.autoscale_view()

            # Reinicia las listas de datos y el tiempo de inicio de acumulación
            datos_tiempo.clear()
            datos_acelerometro_x.clear()
            datos_acelerometro_y.clear()
            datos_acelerometro_z.clear()
            tiempo_inicio_acumulacion = time.time()

    return linea,

# Configura la animación
ani = animation.FuncAnimation(fig, update, init_func=init, interval=50, blit=True)

# Muestra la gráfica
plt.show()

        





