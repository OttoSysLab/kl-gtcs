from PySide2.QtWidgets import *
app = QApplication([])                          # Start an application.
window = QWidget()                              # Create a window.
layout = QVBoxLayout()                          # Create a layout.
button = QPushButton("I'm just a Button man")   # Define a button
layout.addWidget(QLabel('Hello World!'))        # Add a label
layout.addWidget(button)                        # Add the button man
window.setLayout(layout)                        # Pass the layout to the window
window.show()                                   # Show window
app.exec_()                                     # Execute the App