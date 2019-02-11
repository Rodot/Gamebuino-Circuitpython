## Currently implemented functions:

(usage is the same as the c++ equivalents)

Things in brackets `[]` are optional parameters

Base methods:
* `void gb.begin()`
* `void gb.loader()` loads loader, was very useful for testing & re-flashing
* `bool gb.update()`
* `void gb.waitForUpdate()` waits until next screen update
* `void gb.updateDisplay()`
* `void gb.setFrameRate(int framerate)`
* `int gb.getCpuLoad()`
* `int gb.getFreeRam()`
* `string gb.getDefaultName()` Pretty useless as SD functionality isn't implemented currently

Display methods:
* `void gb.display.clear([int color])`
* `void gb.display.fill([int color])`
* `void gb.display.print([int x, int y,] string|int text)`
* `void gb.display.println([int x, int y,] string|int text)`
* `void gb.display.drawLine(int x1, int y1, int x2, int y2)`
* `void gb.display.drawRect(int x, int y, int width, int height)`
* `void gb.display.fillRext(int x, int y, int width, int height)`
* `void gb.display.drawPixel(int x, int y [, int color])`
* `void gb.display.drawCircle(int x, int y, int radius)`
* `void gb.display.fillCircle(int x, int y, int radius)`
* `void gb.display.drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3)`
* `void gb.display.fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3)`
* `void gb.display.drawRoundRect(int x, int y, int width, int height, int radius)`
* `void gb.display.fillRoundRect(int x, int y, int width, int height, int radius)`
* `void gb.display.setColor(int color)`
* `void gb.display.drawBitmap(int x, int y, bytes buffer [, int scale])`
* `void gb.display.drawImage(int x, int y, buffer image)`

Lights methods:
* `void gb.lights.clear([int color])`
* `void gb.lights.fill([int color])`
* `void gb.lights.fillRect(int x, int y, int width, int height)`
* `void gb.lights.drawPixel(int x, int y [, int color])`
* `void gb.lights.setColor(int color)`

Buttons constants:
* `int gb.buttons.DOWN`
* `int gb.buttons.LEFT`
* `int gb.buttons.RIGHT`
* `int gb.buttons.UP`
* `int gb.buttons.A`
* `int gb.buttons.B`
* `int gb.buttons.MENU`
* `int gb.buttons.HOME`

Buttons methods:
* `bool gb.buttons.pressed(int button)`
* `bool gb.buttons.released(int button)`
* `bool gb.buttons.held(int button, int duration)`
* `bool gb.buttons.repeat(int button, int duration)`
* `int gb.buttons.timeHeld(int button)`

Color constants:
* `int gb.color.WHITE`
* `int gb.color.GRAY`
* `int gb.color.DARKGRAY`
* `int gb.color.BLACK`
* `int gb.color.PURPLE`
* `int gb.color.PINK`
* `int gb.color.RED`
* `int gb.color.ORANGE`
* `int gb.color.BROWN`
* `int gb.color.BEIGE`
* `int gb.color.YELLOW`
* `int gb.color.LIGHTGREEN`
* `int gb.color.GREEN`
* `int gb.color.DARKBLUE`
* `int gb.color.BLUE`
* `int gb.color.LIGHTBLUE`

Gui methods:
* `string gb.gui.keyboard(string titleText [, string defaultText])`
* `int gb.gui.menu(string titleText, [, array[string] entries])`
* `void gb.gui.popup(string text, int duration)`
