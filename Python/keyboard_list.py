from pynput.keyboard import Key, Listener

pressed = []

def on_press(key):
    if key not in pressed:
        pressed.append(key)
    print(pressed)
def on_release(key):
    if key in pressed:
        pressed.remove(key)
    print(pressed)

# Collect events until released
with Listener(
        on_press=on_press,
        on_release=on_release) as listener:
    listener.join()


