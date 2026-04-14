import sys
import os
from PySide6.QtWidgets import QApplication, QWidget, QPushButton, QMainWindow, QStyle
from PySide6.QtCore import QSize, Qt, QUrl
from PySide6.QtWebEngineWidgets import QWebEngineView
from PySide6.QtGui import QIcon
class MainWindow(QMainWindow):
        def __init__(self):
            super().__init__()
            self.setWindowTitle("Shikgoo Settings")
            self.resize(700,700)
            button = QPushButton("System starten")
            self.setCentralWidget(button)
            
def main():
    app = QApplication(sys.argv)
    path_name = r"C:\Netzwerkprogrammierung\GUI_Programming\Shikgoo_Logo.png"
    if os.path.exists(path_name):
         app.setWindowIcon(QIcon(path_name))
    else:
         app.setWindowIcon(app.style().standardIcon(QStyle.SP_ComputerIcon))
         
    window = MainWindow()
    window.show()
    view = QWebEngineView()
    view.setURl(QUrl("fheihf"))
    view.show()
    sys.exit(app.exec())
if __name__ == "__main__":
    main()
