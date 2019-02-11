# Basics

Load the CircuitPython sketch, plug the gamebuino into the computer via USB and drag-and-drop a python file called `code.py` into it. It will run automatically and will also update automatically.

# Example Sketch

```py
import gamebuino_meta as gb

gb.begin()

while True:
    gb.waitForUpdate()
	
    gb.display.clear()
    gb.display.setColor(gb.color.PINK)
    gb.display.fillRect(0, 0, 10, 20)

    gb.display.setColor(gb.color.RED)
    gb.display.print("CPU:")
    gb.display.println(gb.getCpuLoad())
    gb.display.setColor(gb.color.GREEN)
    gb.display.print("RAM:")
    gb.display.println(gb.getFreeRam())
    if gb.buttons.repeat(gb.buttons.A, 0):
        gb.display.println("Button A held down!")
        gb.lights.fill(gb.color.GREEN)
```

# example image stuff

```py
buffer = bytes([8, 8,
    1, 0,
    0,
    0xFF,
    1,
    
    0x05, 0x07, 0x07, 0x07,
    0x70, 0x70, 0x70, 0x70,
    0x07, 0x07, 0x07, 0x07,
    0x70, 0x70, 0x70, 0x70,
    0x07, 0x07, 0x07, 0x07,
    0x70, 0x70, 0x70, 0x70,
    0x07, 0x07, 0x07, 0x07,
    0x70, 0x70, 0x70, 0x70,
])

gb.display.drawImage(50, 20, buffer)
```
