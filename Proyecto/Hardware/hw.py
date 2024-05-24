# -*- coding: utf-8 -*-
"""
Created on Fri May 24 06:48:44 2024

@author: Maxwell
"""

from docx import Document

# Crear un nuevo documento de Word
doc = Document()
doc.add_heading('Comparativa de Sensores Inerciales', level=1)

# Agregar una tabla al documento
table = doc.add_table(rows=1, cols=7)
table.style = 'Table Grid'

# Definir los encabezados de la tabla
hdr_cells = table.rows[0].cells
hdr_cells[0].text = 'Parámetro'
hdr_cells[1].text = 'LSM9DS1'
hdr_cells[2].text = 'MPU-9250'
hdr_cells[3].text = 'BNO055'
hdr_cells[4].text = 'ICM-20948'
hdr_cells[5].text = 'BMI160'

# Datos de los sensores
data = [
    ['Fabricante', 'STMicroelectronics', 'InvenSense', 'Bosch Sensortec', 'InvenSense', 'Bosch Sensortec'],
    ['Acelerómetro', '', '', '', '', ''],
    ['Rango de Medición', '±2/±4/±8/±16 g', '±2/±4/±8/±16 g', '±2/±4/±8/±16 g', '±2/±4/±8/±16 g', '±2/±4/±8/±16 g'],
    ['Precisión', '0.244 mg/LSB (±2 g)', '16,384 LSB/g (±2 g)', '1 LSB = 0.00098 m/s² (±2 g)', '16,384 LSB/g (±2 g)', '16,384 LSB/g (±2 g)'],
    ['Giroscopio', '', '', '', '', ''],
    ['Rango de Medición', '±245/±500/±2000 dps', '±250/±500/±1000/±2000 dps', '±125/±250/±500/±1000/±2000 dps', '±250/±500/±1000/±2000 dps', '±125/±250/±500/±1000/±2000 dps'],
    ['Precisión', '8.75 mdps/LSB (±245 dps)', '131 LSB/°/s (±250 dps)', '1 LSB = 0.00875 °/s (±125 dps)', '131 LSB/°/s (±250 dps)', '16.4 LSB/°/s (±125 dps)'],
    ['Magnetómetro', '', '', '', '', ''],
    ['Rango de Medición', '±4/±8/±12/±16 gauss', '±4800 μT', '±1300/±2500 μT', '±4900 μT', 'No disponible'],
    ['Precisión', '0.00014 gauss/LSB (±4 gauss)', '0.15 µT/LSB', '1.16 µT/LSB (±1300 µT)', '0.15 µT/LSB', 'No disponible'],
    ['Consumo de Energía', '', '', '', '', ''],
    ['Acelerómetro', '2.0 mA (en modo alto rendimiento)', '450 µA', '200 µA', '68 µA', '150 µA'],
    ['Giroscopio', '2.0 mA', '3.2 mA', '12 mA', '1.23 mA', '900 µA'],
    ['Magnetómetro', '0.2 mA', '280 µA', '1.5 mA', '90 µA', 'No disponible'],
    ['Interfaz de Comunicación', 'I2C, SPI', 'I2C, SPI', 'I2C', 'I2C, SPI', 'I2C, SPI'],
    ['Características Adicionales', 'Sensor de temperatura', 'Sensor de temperatura', 'Fusión de sensores en el chip', 'Sensor de temperatura', 'Sensor de temperatura']
]

# Agregar los datos a la tabla
for row_data in data:
    row_cells = table.add_row().cells
    for i, cell_data in enumerate(row_data):
        row_cells[i].text = cell_data

# Guardar el documento
doc.save('Comparativa_Sensores.docx')

