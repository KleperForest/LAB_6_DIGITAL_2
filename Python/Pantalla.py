import tkinter as tk
import serial
import threading
import time

# Tamaño de la cuadrícula y el cuadrado
GRID_WIDTH = 240  # Número de celdas en la cuadrícula (ancho)
GRID_HEIGHT = 240  # Número de celdas en la cuadrícula (alto)
CELL_SIZE = 1  # Tamaño de cada celda en píxeles
SQUARE_SIZE = 20  # Tamaño del cuadro en píxeles

# Configuración del puerto serial
SERIAL_PORT = "COM3"  # Puerto COM para la comunicación UART
BAUD_RATE = 9600  # Velocidad de la comunicación UART


class Juego8Bits:
    def __init__(self, root):
        self.root = root
        self.root.title("Juego de 8 bits con Tkinter")
        
        # Crear el lienzo donde se dibujará el cuadrado
        self.canvas = tk.Canvas(self.root, width=GRID_WIDTH*CELL_SIZE, height=GRID_HEIGHT*CELL_SIZE, bg='black')
        self.canvas.pack()

        # Posición inicial del cuadrado (en la cuadrícula)
        self.x, self.y = 0, 0

        # Dibujar el cuadrado inicial
        self.cuadrado = self.canvas.create_rectangle(self.x * CELL_SIZE, self.y * CELL_SIZE,
                                                     (self.x + SQUARE_SIZE) * CELL_SIZE, (self.y + SQUARE_SIZE) * CELL_SIZE,
                                                     fill="orange")

        # Bandera para saber si el cuadrado está "descuartizado" (es decir, ya no se puede mover)
        self.desactivado = False

        # Iniciar el hilo para leer desde UART
        self.iniciar_uart()

    def iniciar_uart(self):
        """Iniciar el hilo para leer datos desde UART"""
        self.serial = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        self.thread = threading.Thread(target=self.leer_uart, daemon=True)
        self.thread.start()

    def leer_uart(self):
        """Leer datos desde el puerto UART y mover el cuadrado"""
        while True:
            if self.serial.in_waiting > 0:
                comando = self.serial.readline().decode('utf-8').strip()  # Leer el comando del UART
                self.mover_cuadrado(comando)

    def mover_cuadrado(self, comando):
        """Actualizar la posición del cuadrado basado en el comando recibido"""
        if self.desactivado:  # Si el cuadrado está "descuartizado", no se puede mover
            return

        if comando == "arriba" and self.y >= 20:  # Mover hacia arriba
            self.y -= 20
        elif comando == "abajo" and self.y <= GRID_HEIGHT - SQUARE_SIZE - 20:  # Mover hacia abajo
            self.y += 20
        elif comando == "izquierda" and self.x >= 20:  # Mover hacia la izquierda
            self.x -= 20
        elif comando == "derecha" and self.x <= GRID_WIDTH - SQUARE_SIZE - 20:  # Mover hacia la derecha
            self.x += 20
        elif comando == "Saltar":  # Cambiar color a azul por 100 ms y luego volver a naranja
            self.cambiar_color_temporal("blue", 250)
        elif comando == "Descuartizar":  # Cambiar color a rojo y desactivar movimientos
            self.desactivar_cuadrado()

        # Mover el cuadrado a la nueva posición (solo si no está desactivado)
        if not self.desactivado:
            self.canvas.coords(self.cuadrado, self.x * CELL_SIZE, self.y * CELL_SIZE,
                               (self.x + SQUARE_SIZE) * CELL_SIZE, (self.y + SQUARE_SIZE) * CELL_SIZE)

    def cambiar_color_temporal(self, color, duracion_ms):
        """Cambiar el color del cuadrado temporalmente y luego volver al color original"""
        self.canvas.itemconfig(self.cuadrado, fill=color)
        self.root.after(duracion_ms, lambda: self.canvas.itemconfig(self.cuadrado, fill="orange"))

    def desactivar_cuadrado(self):
        """Cambiar el color del cuadrado a rojo y desactivar su movimiento"""
        self.canvas.itemconfig(self.cuadrado, fill="red")
        self.desactivado = True


# Configuración de la ventana principal
root = tk.Tk()
juego = Juego8Bits(root)
root.mainloop()
