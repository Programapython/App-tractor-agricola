import sys
from PySide6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QLabel, QPushButton,
    QVBoxLayout, QHBoxLayout, QDial, QFrame
)
from PySide6.QtCore import QTimer, QDateTime, Qt
from PySide6.QtGui import QPainter, QColor


class TankWidget(QWidget):
    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setBrush(QColor(100, 149, 237))  # Azul tanque
        painter.drawRect(40, 30, 80, 200)  # Rectángulo del tanque


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Control de Auto")

        # Widget central
        central_widget = QWidget()
        self.setCentralWidget(central_widget)

        # Layout principal
        main_layout = QVBoxLayout()
        central_widget.setLayout(main_layout)

        # Fecha y hora
        self.datetime_label = QLabel()
        self.datetime_label.setAlignment(Qt.AlignCenter)
        main_layout.addWidget(self.datetime_label)
        self.update_datetime()

        timer = QTimer(self)
        timer.timeout.connect(self.update_datetime)
        timer.start(1000)

        # Contenedor de columnas
        columns_layout = QHBoxLayout()
        main_layout.addLayout(columns_layout)

        # ===== Columna Izquierda =====
        left_column = QVBoxLayout()

        # Gráfica del tanque
        self.tank = TankWidget()
        self.tank.setFixedSize(160, 260)
        left_column.addWidget(self.tank, alignment=Qt.AlignCenter)

        # Botón regular manualmente
        btn_manual = QPushButton("Regular manualmente")
        left_column.addWidget(btn_manual, alignment=Qt.AlignCenter)

        # Potenciómetros
        dial_layout = QHBoxLayout()
        dial_right = QVBoxLayout()
        dial_left = QVBoxLayout()

        self.dial_der = QDial()
        self.dial_izq = QDial()
        for dial in (self.dial_der, self.dial_izq):
            dial.setRange(0, 100)

        dial_right.addWidget(QLabel("Derecha", alignment=Qt.AlignCenter))
        dial_right.addWidget(self.dial_der)

        dial_left.addWidget(QLabel("Izquierda", alignment=Qt.AlignCenter))
        dial_left.addWidget(self.dial_izq)

        dial_layout.addLayout(dial_left)
        dial_layout.addLayout(dial_right)

        left_column.addLayout(dial_layout)

        # ===== Columna Derecha =====
        right_column = QVBoxLayout()

        lbl_estado = QLabel("Estado del auto")
        lbl_estado.setAlignment(Qt.AlignLeft)
        right_column.addWidget(lbl_estado)

        lbl_velocidad = QLabel("Velocidad aproximada")
        lbl_velocidad.setAlignment(Qt.AlignLeft)
        right_column.addWidget(lbl_velocidad)

        btn_bajar_aguilones = QPushButton("Bajar aguilones")
        right_column.addWidget(btn_bajar_aguilones)

        # Envolver columnas
        frame_left = QFrame()
        frame_left.setLayout(left_column)
        frame_right = QFrame()
        frame_right.setLayout(right_column)

        columns_layout.addWidget(frame_left)
        columns_layout.addWidget(frame_right)

    def update_datetime(self):
        current_datetime = QDateTime.currentDateTime().toString("dddd dd MMMM yyyy - hh:mm:ss")
        self.datetime_label.setText(current_datetime)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.resize(700, 450)
    window.show()
    sys.exit(app.exec())

