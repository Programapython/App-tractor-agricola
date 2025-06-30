from PySide6.QtCore import QSize, QPoint, QDateTime, QTimer, Qt
from PySide6.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QHBoxLayout,QPushButton, QWidget, QLabel, QDial, QFrame, QToolBar
from PySide6.QtGui import QPainter, QColor, QPen, QBrush, QFont

import sys


class TankWidget(QWidget):
    def paintEvent(self, event):
        painter = QPainter(self)
        # Definimos el color azul
        painter.setBrush(QColor(100, 147, 237))
        # Colocamos el rectángulo del tanque
        painter.drawRect(40, 30, 80, 200)


class mainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("GUI autopropulasado")
        # self.setFixedSize(QSize(400, 300))  # Tamaño de pantalla
        # self.showMaximized()  # Pantalla completa
        self.showFullScreen()  # Pantalla total

        # button = QPushButton("Iniciar mapeo")
        # self.setCentralWidget(button)

        self.comWidget()
        self.comLayout()
        self.fecha_hora()
        self.columnas()

    def comWidget(self):
        # Widget principal
        self.widget1 = QWidget()
        # self.widget1.setFixedSize(100, 500)
        self.setCentralWidget(self.widget1)

    def comLayout(self):
        # Layout principal
        self.layout1 = QVBoxLayout()
        self.widget1.setLayout(self.layout1)
        # Layout columnas
        self.layoutColumnas = QHBoxLayout()
        self.layout1.addLayout(self.layoutColumnas)

    def fecha_hora(self):
        # Fecha y hora
        self.barraHerramientas = QToolBar("Fecha y hora")
        self.barraHerramientas.setMovable(False)
        self.lblFechaHora = QLabel()
        self.lblFechaHora.setFont(QFont("Verdana", 15))
        self.barraHerramientas.addWidget(self.lblFechaHora)
        self.addToolBar(Qt.TopToolBarArea, self.barraHerramientas)

        # Actualizacion de la fecha y hora
        self.update_datetime()
        timer = QTimer(self)
        timer.timeout.connect(self.update_datetime)
        timer.start(1000)

    def columnas(self):
        # ====== COLUMNA IZQUIERDA ======
        self.columnaIzq = QVBoxLayout()
        # Grafica del tanque
        self.tanque = TankWidget()
        self.tanque.setFixedSize(160, 260)
        self.columnaIzq.addWidget(self.tanque, alignment=Qt.AlignCenter)
        # Boton para regular manualmente
        btn_manual = QPushButton("Regular manualmente")
        self.columnaIzq.addWidget(btn_manual, alignment=Qt.AlignCenter)

        # Potenciometros
        layoutPot = QHBoxLayout()
        potDer = QVBoxLayout()
        potIzq = QVBoxLayout()

        self.pot1 = QDial()
        self.pot2 = QDial()
        for dial in (self.pot1, self.pot2):
            dial.setRange(0, 100)

        potDer.addWidget(QLabel("Derecha", alignment=Qt.AlignCenter))
        potDer.addWidget(self.pot1)

        potIzq.addWidget(QLabel("Izquierda", alignment=Qt.AlignCenter))
        potIzq.addWidget(self.pot2)

        layoutPot.addLayout(potIzq)
        layoutPot.addLayout(potDer)

        self.columnaIzq.addLayout(layoutPot)

        # ====== COLUMNA DERECHA ======
        self.columnaDer = QVBoxLayout()

        lblEstado = QLabel("Estado del auto")
        lblEstado.setAlignment(Qt.AlignLeft)
        self.columnaDer.addWidget(lblEstado)

        lblVelocidad = QLabel("Velocidad aproximada")
        lblVelocidad.setAlignment(Qt.AlignLeft)
        self.columnaDer.addWidget(lblVelocidad)

        # Envolvemos columnas
        frameIzq = QFrame()
        frameIzq.setLayout(self.columnaIzq)
        frameDer = QFrame()
        frameDer.setLayout(self.columnaDer)

        self.layoutColumnas.addWidget(frameIzq)
        self.layoutColumnas.addWidget(frameDer)

    def update_datetime(self):
        actFechaHora = QDateTime.currentDateTime().toString("dddd dd MMMM yyyy - hh:mm:ss")
        self.lblFechaHora.setText(actFechaHora)

'''
    def botoneras(self):
        self.valvula1_Label = QLabel("Valvula 1:")
        self.valvula1_button = QPushButton("Abrir/cerrar")
        self.valvula1_button.setFixedSize(100, 50)
        self.layout1.addWidget(self.valvula1_Label)
        self.layout1.addWidget(self.valvula1_button)
'''

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = mainWindow()
    window.show()
    sys.exit(app.exec())
