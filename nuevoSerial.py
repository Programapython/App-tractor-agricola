import serial, time
import anvil.server

data = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
dataAnt = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

def recibirData(orden):
    if orden == "s":
        data[0] = True
    elif orden == "r":
        data[0] = False

    if orden == "ac":
        data[4] = True
    elif orden == "de":
        data[4] = False
    
    if orden == "a1":
        data[5] = True
    elif orden == "d1":
        data[5] = False

    if orden == "a2":
        data[6] = True
    elif orden == "d2":
        data[6] = False
    
    if orden == "a3":
        data[7] = True
    elif orden == "d3":
        data[7] = False
    
    if orden == "a4":
        data[8] = True
    elif orden == "d4":
        data[8] = False

    if orden == "a5":
        data[9] = True
    elif orden == "d5":
        data[9] = False

    if orden == "a6":
        data[10] = True
    elif orden == "d6":
        data[10] = False

@anvil.server.callable
def emitirData(orden):
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
    
    print(data)

    cadenaOut = ",".join(str(bit) for bit in data)
    cadenaOut =cadenaOut + '\n'
    arduino.write(cadenaOut.encode())

@anvil.server.callable
def dataTanque():
    return 111

try:
    arduino = serial.Serial('COM11', baudrate=9600, timeout=1)
    while True:
        cadenaIn = str(arduino.readline().decode().strip())
        recibirData(cadenaIn)

        anvil.server.connect("server_DA7ZJDVK5LUMUWLUCAC6TXLO-KRGY4GP76FPLYQTH")
        anvil.server.wait_forever()
        
except Exception as e:
    print(f"Error: {e}")
finally:
    arduino.close()