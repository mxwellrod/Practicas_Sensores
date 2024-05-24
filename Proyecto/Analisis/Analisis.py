# -*- coding: utf-8 -*-
"""
Created on Wed May 22 23:15:55 2024

@author: Maxwell
"""

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Nombre del archivo de Excel
excel_file = 'sensor_data.xlsx'

# Parámetros de la ventana
window_size = 50  # 200 muestras para un periodo de 1 segundo (200 Hz)

def detect_position(data):
    position_accZ = []
    position_gyrY = []
    for i in range(len(data) - window_size + 1):
        window = data.iloc[i:i + window_size]

        # Promedios de los datos en la ventana actual
        mean_acc_z = np.mean(window['accZ (g)'])
        mean_acc_x = np.mean(window['accX (g)'])
        mean_gyr_y = np.mean(window['gyrY (dps)'])

        # Criterio para detectar subiendo basado en accZ
        if (window['accZ (g)'].iloc[-1] <= 0.87 and
            window['accZ (g)'].iloc[0] <= 1.1):
            position_accZ.append('Subiendo_accZ')
        else:
            position_accZ.append('NoSubiendo_accZ')
            
            
            # Condiciones para detectar bajadas basadas en accZ
        if (window['accZ (g)'].iloc[-1] >= 1.12 and
            window['accZ (g)'].iloc[0] >= 0.9 and
            window['accZ (g)'].iloc[0] <= 1.05):
            position_accZ.append('Bajando_accZ')
        else:
            position_accZ.append('NoBajando_accZ')

        # Criterio para detectar subiendo basado en gyrY
        if (window['gyrY (dps)'].iloc[-1] <= -11.5 and
            window['gyrY (dps)'].iloc[0] >= -3.5 and
            window['gyrY (dps)'].iloc[0] <= 4):
            position_gyrY.append('Subiendo_gyrY')
        else:
            position_gyrY.append('NoSubiendo_gyrY')

        # Condiciones para detectar bajadas basadas en gyrY
        if (window['gyrY (dps)'].iloc[-1] >= 15 and
            window['gyrY (dps)'].iloc[0] <= 12 and
            window['gyrY (dps)'].iloc[0] >= 2):
            position_gyrY.append('Bajando_gyrY')
        else:
            position_gyrY.append('NoBajando_gyrY')            

    return position_accZ, position_gyrY
# en principio las bajadas tendrian que ser despues de la posicion de top(reposo no creo?) pero hay que cambiar el tamaño de ventana


def plot_detected_positions(data, position_accZ, position_gyrY):
    fig, axs = plt.subplots(2, 1, figsize=(10, 12))

    # Gráfico para accZ
    axs[0].plot(data['Tiempo (s)'], data['accZ (g)'], label='accZ (g)')
    for i, pos in enumerate(position_accZ):
        if i + window_size - 1 < len(data):  # Verifica el rango del índice
            start_time = data['Tiempo (s)'][i]
            end_time = data['Tiempo (s)'][i + window_size - 1]
            if pos == "Subiendo_accZ":
                axs[0].axvspan(start_time, end_time, color='green', alpha=0.3, label='Subiendo_accZ' if i == 0 else "")
            elif pos == "Bajando_accZ":
                axs[0].axvspan(start_time, end_time, color='red', alpha=0.3, label='Bajando_accZ' if i == 0 else "")
    
    axs[0].set_xlabel('Tiempo (s)')
    axs[0].set_ylabel('accZ (g)')
    axs[0].legend()
    axs[0].grid(True)

    # Gráfico para gyrY
    axs[1].plot(data['Tiempo (s)'], data['gyrY (dps)'], label='gyrY (dps)')
    for i, pos in enumerate(position_gyrY):
        if i + window_size - 1 < len(data):  # Verifica el rango del índice
            start_time = data['Tiempo (s)'][i]
            end_time = data['Tiempo (s)'][i + window_size - 1]
            if pos == "Subiendo_gyrY":
                axs[1].axvspan(start_time, end_time, color='blue', alpha=0.3, label='Subiendo_gyrY' if i == 0 else "")
            elif pos == "Bajando_gyrY":
                axs[1].axvspan(start_time, end_time, color='red', alpha=0.3, label='Bajando_gyrY' if i == 0 else "")
    
    axs[1].set_xlabel('Tiempo (s)')
    axs[1].set_ylabel('gyrY (dps)')
    axs[1].legend()
    axs[1].grid(True)

    plt.tight_layout()
    plt.show()



def plot_detected_positions2(data, position_accZ, position_gyrY):
    fig, axs = plt.subplots(2, 1, figsize=(10, 12))

    # Gráfico para accZ
    axs[0].plot(data['Tiempo (s)'], data['accZ (g)'], label='accZ (g)')
    axs[0].plot(data['Tiempo (s)'], data['accZ (g)'].rolling(window=window_size).mean(), label='Smoothed accZ (g)')
    for i, pos in enumerate(position_accZ):
        if pos == "Subiendo_accZ":
            start_time = data['Tiempo (s)'][i]
            end_time = data['Tiempo (s)'][i + window_size - 1]
            axs[0].axvspan(start_time, end_time, color='green', alpha=0.3, label='Subiendo_accZ' if i == 0 else "")
        elif pos == "Bajando_accZ":
            start_time = data['Tiempo (s)'][i]
            end_time = data['Tiempo (s)'][i + window_size - 1]
            axs[0].axvspan(start_time, end_time, color='red', alpha=0.3, label='Bajando_accZ' if i == 0 else "")
    axs[0].set_xlabel('Tiempo (s)')
    axs[0].set_ylabel('accZ (g)')
    axs[0].legend()
    axs[0].grid(True)

    # Gráfico para gyrY
    axs[1].plot(data['Tiempo (s)'], data['gyrY (dps)'], label='gyrY (dps)')
    axs[1].plot(data['Tiempo (s)'], data['gyrY (dps)'].rolling(window=window_size).mean(), label='Smoothed gyrY (dps)')
    for i, pos in enumerate(position_gyrY):
        if pos == "Subiendo_gyrY":
            start_time = data['Tiempo (s)'][i]
            end_time = data['Tiempo (s)'][i + window_size - 1]
            axs[1].axvspan(start_time, end_time, color='blue', alpha=0.3, label='Subiendo_gyrY' if i == 0 else "")
        elif pos == "Bajando_gyrY":
            start_time = data['Tiempo (s)'][i]
            end_time = data['Tiempo (s)'][i + window_size - 1]
            axs[1].axvspan(start_time, end_time, color='orange', alpha=0.3, label='Bajando_gyrY' if i == 0 else "")
    axs[1].set_xlabel('Tiempo (s)')
    axs[1].set_ylabel('gyrY (dps)')
    axs[1].legend()
    axs[1].grid(True)

    plt.tight_layout()
    plt.show()




def main():
    # Leer los datos de la hoja 'Datos Sensores' del archivo Excel
    df_sensors = pd.read_excel(excel_file, sheet_name='Datos Sensores')
    
    # Detectar posición y máximos
    position_accZ, position_gyrY = detect_position(df_sensors)
    
    # Graficar los datos y las posiciones detectadas
    plot_detected_positions(df_sensors, position_accZ, position_gyrY)

if __name__ == '__main__':
    main()
