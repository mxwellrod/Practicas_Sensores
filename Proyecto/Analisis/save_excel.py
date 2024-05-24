# -*- coding: utf-8 -*-
"""
Created on Tue May 21 23:41:53 2024

@author: Maxwell
"""

import serial
import pandas as pd
import time

# Configura el puerto serie y la tasa de baudios
ser = serial.Serial('COM10', 115200)  # Reemplaza 'COM10' con el puerto correspondiente en tu sistema

# Lista para almacenar las filas de datos
data = []

# Nombre del archivo de Excel
excel_file = 'sensor_data1.xlsx'
csv_file = 'sensor_data1.csv'

# Parámetros adicionales
params = {
    'Fecha de nacimiento': '08/12/1999',
    'Género': 'Masculino',
    'Peso (kg)': 82,
    'Altura (cm)': 182,
    'Número de pie': 43,
    'Fumador': 'No',
    'Número de cigarrillos al día': 0,
    'Ingesta de alcohol': 'Sí',
    'Tipo y dosis de alcohol': 'Cerveza, 2 cañas',
    'Actividad física semanal': 'No',
    'Tipo de actividad física': '-',
    'Días de actividad física a la semana': 0,
    'Horas de actividad física semanales totales': 0
}

# Inicializa la variable de tiempo de inicio
start_time = None

# Duración de la colección de datos en segundos
collection_duration = 45  # Ajusta este valor según sea necesario

# Función para leer los datos del puerto serie y acumularlos
def read_data():
    global start_time
    if start_time is None:
        start_time = time.time()  # Establece el tiempo de inicio

    while True:
        if ser.in_waiting > 0:
            # Lee una línea del puerto serie
            line = ser.readline().decode('utf-8').strip()
            # Divide la línea en componentes
            values = line.split(', ')
            
            if len(values) == 6:  # Asegúrate de que la línea tenga seis valores (accX, accY, accZ, gyrX, gyrY, gyrZ)
                try:
                    # Convertir los valores a float
                    values = list(map(float, values))
                except ValueError:
                    continue  # Si hay un error en la conversión, saltar a la siguiente línea

                if start_time is None:
                    start_time = time.time()  # Establece el tiempo de inicio al recibir el primer dato

                # Calcula el tiempo transcurrido en segundos desde el inicio
                elapsed_time = time.time() - start_time
                
                # Verifica si se ha alcanzado la duración de la colección
                if elapsed_time > collection_duration:
                    break
                
                # Agrega el tiempo transcurrido al principio de los valores
                values.insert(0, elapsed_time)
                
                # Agrega los valores a la lista de datos
                data.append(values)
                print(f"Datos recibidos: {values}")  # Imprime los datos recibidos para verificación

def save_data_to_excel():
    # Crea un DataFrame para los datos de los sensores
    df_sensors = pd.DataFrame(data, columns=['Tiempo (s)', 'accX (g)', 'accY (g)', 'accZ (g)', 'gyrX (dps)', 'gyrY (dps)', 'gyrZ (dps)'])

    # Crea un DataFrame para los parámetros adicionales
    df_params = pd.DataFrame(list(params.items()), columns=['Parámetro', 'Valor'])

    # Guarda ambos DataFrames en el archivo Excel
    with pd.ExcelWriter(excel_file) as writer:
        df_params.to_excel(writer, sheet_name='Datos Paciente', index=False)
        df_sensors.to_excel(writer, sheet_name='Datos Sensores', index=False)
    
    df_sensors.to_csv(csv_file, index=False)
    
    print(f"Datos guardados en {excel_file} y {csv_file}")

if __name__ == '__main__':
    try:
        read_data()
        if data:  # Solo guarda si hay datos
            save_data_to_excel()
    except KeyboardInterrupt:
        print("Interrupción del usuario. Saliendo...")
        if data:  # Solo guarda si hay datos
            save_data_to_excel()
    finally:
        ser.close()








