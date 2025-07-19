import serial, time
import anvil.server
import threading

#Retraso de inicio
time.sleep(5)

#Data compartida:
#start, niv1, niv2, niv3, ac_val, val1, val2, val3, val4, val5, val6]
data = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
dataAnt = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

@anvil.server.callable
def recibirOrden(orden):
    if orden == "s":
        data[0] = 1
    elif orden == "r":
        data[0] = 0

    if orden == "ac":
        data[4] = 1
    elif orden == "de":
        data[4] = 0
    
    if orden == "a1":
        data[5] = 1
    elif orden == "d1":
        data[5] = 0

    if orden == "a2":
        data[6] = 1
    elif orden == "d2":
        data[6] = 0
    
    if orden == "a3":
        data[7] = 1
    elif orden == "d3":
        data[7] = 0
    
    if orden == "a4":
        data[8] = 1
    elif orden == "d4":
        data[8] = 0

    if orden == "a5":
        data[9] = 1
    elif orden == "d5":
        data[9] = 0

    if orden == "a6":
        data[10] = 1
    elif orden == "d6":
        data[10] = 0

def emitirData(arduino, n=1):
    if n==1:
        cadenaOut = ",".join(str(bit) for bit in data)
        cadenaOut =cadenaOut + '\n'
        arduino.write(cadenaOut.encode())
    elif n==2:
        cadenaOut = ",".join(str(bit) for bit in [data[1], data[6], data[7], data[8], data[9], data[10], data[11]])
        cadenaOut =cadenaOut + '\n'
        arduino.write(cadenaOut.encode())

def recibirData(cadenaIn):
    cadenaIn = cadenaIn.split(",")
    print(cadenaIn)
    data[1] = int(cadenaIn[0])
    data[2] = int(cadenaIn[1])
    data[3] = int(cadenaIn[2])
'''
    for i in range(4):
        if int(cadenaIn[i+3]) <= 400 and dataAnt[4] == 1:
            data[i+5] = 0
        else:
            data[i+5] = 1
'''
'''
def recibirAnvil(orden):
    if orden == "s":
        data[0] = 1
    elif orden == "r":
        data[0] = 0

    if orden == "ac":
        data[4] = 1
    elif orden == "de":
        data[4] = 0
    
    if orden == "a1":
        data[5] = 1
    elif orden == "d1":
        data[5] = 0

    if orden == "a2":
        data[6] = 1
    elif orden == "d2":
        data[6] = 0
    
    if orden == "a3":
        data[7] = 1
    elif orden == "d3":
        data[7] = 0
    
    if orden == "a4":
        data[8] = 1
    elif orden == "d4":
        data[8] = 0

    if orden == "a5":
        data[9] = 1
    elif orden == "d5":
        data[9] = 0

    if orden == "a6":
        data[10] = 1
    elif orden == "d6":
        data[10] = 0
'''
@anvil.server.callable
def actualizarTan():
    return [data[1], data[2], data[3], data[5], data[6], data[7], data[8], data[9], data[10]]

# Función para leer desde un Arduino específico
def conex_arduino(nombre, puerto, baudrate=9600):
    try:
        arduino = serial.Serial(puerto, baudrate, timeout=1)
        return arduino
    except Exception as e:
        print(f"Error en {nombre}: {e}")

def leer_arduino(nombre, arduino):
    while True:
        if arduino.in_waiting:
            dato = arduino.readline().decode().strip()
            print(f"[{nombre}] {dato}")

            time.sleep(0.1)

def leer_arduino_uno():
    while True:
        if ardUno.in_waiting:
            dato = ardUno.readline().decode().strip()
            #print(f"[{uno}] {dato}")
            recibirOrden(dato)
            time.sleep(0.1)
        emitirData(ardUno)
        time.sleep(0.1)

def leer_arduino_nano():
    while True:
        if ardNano.in_waiting:
            dato = ardNano.readline().decode().strip()
            #print(f"[{nano}] {dato}")
            recibirData(dato)
            time.sleep(0.1)
            emitirData(ardNano, 2)
            time.sleep(0.1)

def leer_anvil():
    anvil.server.connect("server_DA7ZJDVK5LUMUWLUCAC6TXLO-KRGY4GP76FPLYQTH")
    anvil.server.wait_forever()

# Conectar a ambos Arduino 
ardUno = conex_arduino("ArduinoUno", "/dev/ttyACM0") #/dev/ttyACM0 COM11
ardNano = conex_arduino("ArduinoNano", "/dev/ttyUSB0") #/dev/ttyUSB0 COM9

# Hilos para cada Arduino
hiloUno = threading.Thread(target=leer_arduino_uno)
hiloNano = threading.Thread(target=leer_arduino_nano)
hiloAnvil = threading.Thread(target=leer_anvil)

# Inicia los hilos
hiloUno.start()
hiloNano.start()
hiloAnvil.start()

# Funcion para terminar con los hilos
def terminar_hilos():
    hiloUno.join()
    hiloNano.join()
    hiloAnvil.join()