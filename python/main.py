import os
import subprocess
import lens
import tkinter as tk
from PIL import Image, ImageTk

CANVAS_WIDTH = 1200
CANVAS_HEIGHT = 800

IMAGE_WIDTH = 1000
IMAGE_HEIGHT = 600
IMAGE_SCALE = 1 / 100
IMAGE_NAME = 'program.png'

dirname = os.path.dirname(__file__)
r2d_path = os.path.join(dirname, 'program.r2d')
exe_path = os.path.join(dirname, '..', 'raytracer2d', 'main.exe')
png_path = os.path.join(dirname, IMAGE_NAME)

root = tk.Tk()
root.geometry('{}x{}'.format(CANVAS_WIDTH, CANVAS_HEIGHT))
root.title('Raytracer2D')
root.pack_propagate(False)


lens_options = list(lens.get_lens_keys())
selected_lens = tk.StringVar()
selected_lens.set('LB1761')
drop = tk.OptionMenu(root, selected_lens, *lens_options)
drop.pack(side='top', anchor='ne')

png = ImageTk.PhotoImage(Image.open(png_path))

render = tk.Label(root, image=png)
render.pack(side='top', anchor='nw')

def raytrace():
    l = lens.Lens(selected_lens.get(), [0,0])
    print(l.lens)
    print(l)

    r2d_file = open(r2d_path, 'w')
    r2d_file.write('IMAGE {} {} {} {}\n'.format(IMAGE_WIDTH, IMAGE_HEIGHT, IMAGE_SCALE, png_path))
    r2d_file.write('RAY -100 -10 1 0 600 1\n')
    r2d_file.write('RAY -100 0 1 0 600 1\n')
    r2d_file.write('RAY -100 10 1 0 600 1\n')
    r2d_file.write(str(l))
    r2d_file.write('RAYTRACE\n')
    r2d_file.close()
    subprocess.call([exe_path, r2d_path])
    png = ImageTk.PhotoImage(Image.open(png_path))
    render.configure(image=png)
    render.image = png
    render.pack(side='top', anchor='nw')

button = tk.Button(root, text='Raytrace', command=raytrace)
button.pack(side='top', anchor='ne')

    
raytrace()

root.mainloop()