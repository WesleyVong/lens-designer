import json
import lens
import os
import subprocess
import tkinter as tk
import numpy as np
from tkinter import filedialog
from PIL import Image, ImageTk

CANVAS_WIDTH = 1000
CANVAS_HEIGHT = 800

IMAGE_WIDTH = 1000
IMAGE_HEIGHT = 400
IMAGE_SCALE = 1 / 80
IMAGE_NAME = 'program.png'

dirname = os.path.dirname(__file__)
r2d_path = os.path.join(dirname, 'program.r2d')
exe_path = os.path.join(dirname, '..', 'raytracer2d', 'main.exe')
png_path = os.path.join(dirname, IMAGE_NAME)

root = tk.Tk()
root.geometry('{}x{}'.format(CANVAS_WIDTH, CANVAS_HEIGHT))
root.title('Raytracer2D')
root.pack_propagate(False)

png = ImageTk.PhotoImage(Image.open(png_path))

def raytrace():
    r2d_file = open(r2d_path, 'w')
    r2d_file.write('IMAGE {} {} {} {}\n'.format(IMAGE_WIDTH, IMAGE_HEIGHT, IMAGE_SCALE, png_path))
    for i in np.linspace(450, 750, 5):
        for j in np.linspace(-10, 10, 100):
            r2d_file.write('RAY -100 {} 1 0 {} 1\n'.format(j, i))

    num_lenses = len(l1)
    for l in range(num_lenses):
        l = lens.Lens(lens_sel[l].get(), [e1_data[l].get(),e2_data[l].get()], cb_data[l].get())
        r2d_file.write(l.as_r2d())
    r2d_file.write('RAYTRACE\n')
    r2d_file.close()
    subprocess.call([exe_path, r2d_path])
    png = ImageTk.PhotoImage(Image.open(png_path))
    render.configure(image=png)
    render.image = png

def add_lens():
    num_lenses = len(l1) + 1
    row_idx = num_lenses+1
    l1.append(tk.Label(root, text='Lens:'))
    l1[-1].grid(row=row_idx, column=0, sticky='w')
    lens_sel.append(tk.StringVar())
    lens_sel[-1].set('LB1761')
    lens_dd.append(tk.Entry(root, textvariable=lens_sel[-1]))
    lens_dd[-1].grid(row=row_idx, column=1)
    l2.append(tk.Label(root, text='PosX:'))
    l2[-1].grid(row=row_idx, column=2, sticky='w')
    e1_data.append(tk.DoubleVar())
    e1_data[-1].set(0)
    e1.append(tk.Entry(root, textvariable=e1_data[-1]))
    e1[-1].grid(row=row_idx, column=3)
    l3.append(tk.Label(root, text='PosY:'))
    l3[-1].grid(row=row_idx, column=4, sticky='w')
    e2_data.append(tk.DoubleVar())
    e2_data[-1].set(0)
    e2.append(tk.Entry(root, textvariable=e2_data[-1]))
    e2[-1].grid(row=row_idx, column=5)
    cb_data.append(tk.BooleanVar())
    cb_data[-1].set(False)
    cb.append(tk.Checkbutton(root, text='Reverse', variable=cb_data[-1]))
    cb[-1].grid(row=row_idx, column=6, sticky='w')

def remove_lens():
    num_lenses = len(l1)
    if (num_lenses == 0):
        return
    l1[-1].grid_remove()
    l1[-1].destroy()
    l1.pop()
    lens_sel.pop()
    lens_dd[-1].grid_remove()
    lens_dd[-1].destroy()
    lens_dd.pop()
    l2[-1].grid_remove()
    l2[-1].destroy()
    l2.pop()
    e1_data.pop()
    e1[-1].destroy()
    e1.pop()
    l3[-1].grid_remove()
    l3[-1].destroy()
    l3.pop()
    e2_data.pop()
    e2[-1].destroy()
    e2.pop()
    cb_data.pop()
    cb[-1].destroy()
    cb.pop()

def save():
    file_path = filedialog.asksaveasfilename(defaultextension=".r2d")
    if (file_path):
        with open(file_path, 'w') as f:
            lens_sel_arr = [x.get() for x in lens_sel]
            e1_data_arr = [x.get() for x in e1_data]
            e2_data_arr = [x.get() for x in e2_data]
            cb_data_arr = [x.get() for x in cb_data]
            f.write(json.dumps([lens_sel_arr, e1_data_arr, e2_data_arr, cb_data_arr]))

def load():
    file_path = filedialog.askopenfilename(defaultextension=".r2d")
    if (file_path):
        with open(file_path, 'r') as f:
            data = json.load(f)
            lens_sel_arr = data[0]
            e1_data_arr = data[1]
            e2_data_arr = data[2]
            cb_data_arr = data[3]
            while (len(lens_sel) > len(lens_sel_arr)):
                remove_lens()
            while (len(lens_sel) < len(lens_sel_arr)):
                add_lens()
            for i in range(len(lens_sel_arr)):
                lens_sel[i].set(lens_sel_arr[i])
                e1_data[i].set(e1_data_arr[i])
                e2_data[i].set(e2_data_arr[i])
                cb_data[i].set(cb_data_arr[i])
    raytrace()

num_lenses = 0
l1 = []
l2 = []
l3 = []
lens_sel = []
lens_dd = []
e1_data = []
e1 = []
e2_data = []
e2 = []
cb_data = []
cb = []

lens_options = list(lens.get_lens_keys())

add_lens()

save_btn = tk.Button(root, text='Save', command=save)
save_btn.grid(row=0, column=0, sticky='w')

load_btn = tk.Button(root, text='Load', command=load)
load_btn.grid(row=0, column=1, sticky='w')

render = tk.Label(root, image=png)
render.config(bg='black')
render.grid(row=1, column=0, sticky='nw', columnspan=50)

rt = tk.Button(root, text='Raytrace', command=raytrace)
rt.grid(row=10, column=0, sticky='w')

add = tk.Button(root, text='Add Lens', command=add_lens)
add.grid(row=10, column=1)

sub = tk.Button(root, text='Remove Lens', command=remove_lens)
sub.grid(row=10, column=2)
    
raytrace()

root.mainloop()