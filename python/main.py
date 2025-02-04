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
IMAGE_ORIGIN_X = 500
IMAGE_ORIGIN_Y = 200
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

image_scale = IMAGE_SCALE
image_origin_x = IMAGE_ORIGIN_X
image_origin_y = IMAGE_ORIGIN_Y

def raytrace():
    r2d_file = open(r2d_path, 'w')
    r2d_file.write('IMAGE {} {} {} {} {} {}\n'.format(IMAGE_WIDTH, IMAGE_HEIGHT, image_scale, image_origin_x, image_origin_y, png_path))
    for i in np.linspace(450, 750, 5):
        for j in np.linspace(-10, 10, 10):
            r2d_file.write('RAY -100 {} 1 0 {} 1\n'.format(j, i))

    num_lenses = len(lens_sel)
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
    global lens_controls
    num_lenses = len(lens_sel) + 1
    row_idx = num_lenses
    lens_sel.append(tk.StringVar())
    lens_sel[-1].set('LB1761')
    lens_dd.append(tk.Entry(lens_controls, textvariable=lens_sel[-1]))
    lens_dd[-1].grid(row=row_idx, column=0)
    e1_data.append(tk.DoubleVar())
    e1_data[-1].set(0)
    e1.append(tk.Entry(lens_controls, textvariable=e1_data[-1]))
    e1[-1].grid(row=row_idx, column=1)
    e2_data.append(tk.DoubleVar())
    e2_data[-1].set(0)
    e2.append(tk.Entry(lens_controls, textvariable=e2_data[-1]))
    e2[-1].grid(row=row_idx, column=2)
    cb_data.append(tk.BooleanVar())
    cb_data[-1].set(False)
    cb.append(tk.Checkbutton(lens_controls, text='Reverse', variable=cb_data[-1]))
    cb[-1].grid(row=row_idx, column=3, sticky='w')

def remove_lens():
    num_lenses = len(lens_sel)
    if (num_lenses == 0):
        return
    lens_sel.pop()
    lens_dd[-1].grid_remove()
    lens_dd[-1].destroy()
    lens_dd.pop()
    e1_data.pop()
    e1[-1].destroy()
    e1.pop()
    e2_data.pop()
    e2[-1].destroy()
    e2.pop()
    cb_data.pop()
    cb[-1].destroy()
    cb.pop()

def save():
    file_path = filedialog.asksaveasfilename(defaultextension=".json")
    if (file_path):
        with open(file_path, 'w') as f:
            lens_sel_arr = [x.get() for x in lens_sel]
            e1_data_arr = [x.get() for x in e1_data]
            e2_data_arr = [x.get() for x in e2_data]
            cb_data_arr = [x.get() for x in cb_data]
            f.write(json.dumps([lens_sel_arr, e1_data_arr, e2_data_arr, cb_data_arr]))

def load():
    file_path = filedialog.askopenfilename(defaultextension=".json")
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

def render_zoom_in():
    global image_scale
    image_scale *= 1.25
    raytrace()

def render_zoom_out():
    global image_scale
    image_scale /= 1.25
    raytrace()

def render_move_left():
    global image_origin_x
    image_origin_x += 50
    raytrace()

def render_move_right():
    global image_origin_x
    image_origin_x -= 50
    raytrace()

def render_move_up():
    global image_origin_y
    image_origin_y += 50
    raytrace()

def render_move_down():
    global image_origin_y
    image_origin_y -= 50
    raytrace()

def on_render_scroll(e):
    global image_scale
    if (e.delta > 0):
        image_scale *= e.delta / 120 * 1.25
    else:
        image_scale /= -e.delta / 120 * 1.25
    raytrace()

def on_render_click(e):
    global image_origin_x, image_origin_y
    dx = e.x - IMAGE_ORIGIN_X
    dy = e.y - IMAGE_ORIGIN_Y
    image_origin_x -= dx
    image_origin_y -= dy
    raytrace()

def render_reset():
    global image_scale, image_origin_x, image_origin_y
    image_scale = IMAGE_SCALE
    image_origin_x = IMAGE_ORIGIN_X
    image_origin_y = IMAGE_ORIGIN_Y
    raytrace()

num_lenses = 0
lens_sel = []
lens_dd = []
e1_data = []
e1 = []
e2_data = []
e2 = []
cb_data = []
cb = []

lens_options = list(lens.get_lens_keys())

header_controls = tk.Frame(root)
header_controls.grid(row=0, column=0, sticky='w')

save_btn = tk.Button(header_controls, text='Save', command=save)
save_btn.grid(row=0, column=0, sticky='w')

load_btn = tk.Button(header_controls, text='Load', command=load)
load_btn.grid(row=0, column=1, sticky='w')

image_controls = tk.Frame(root)
image_controls.grid(row=2, column=0, sticky='w')

image_zoom_in_btn = tk.Button(image_controls, text='Zoom In', command=render_zoom_in)
image_zoom_in_btn.grid(row=0, column=0, sticky='w')
image_zoom_out_btn = tk.Button(image_controls, text='Zoom Out', command=render_zoom_out)
image_zoom_out_btn.grid(row=0, column=1, sticky='w')
image_move_left_btn = tk.Button(image_controls, text='Move Left', command=render_move_left)
image_move_left_btn.grid(row=0, column=2, sticky='w')
image_move_right_btn = tk.Button(image_controls, text='Move Right', command=render_move_right)
image_move_right_btn.grid(row=0, column=3, sticky='w')
image_move_up_btn = tk.Button(image_controls, text='Move Up', command=render_move_up)
image_move_up_btn.grid(row=0, column=4, sticky='w')
image_move_down_btn = tk.Button(image_controls, text='Move Down', command=render_move_down)
image_move_down_btn.grid(row=0, column=5, sticky='e')

image_reset_btn = tk.Button(root, text='Reset Image', command=render_reset)
image_reset_btn.grid(row=2, column=10, sticky='w')

render = tk.Label(root, image=png)
render.config(bg='black')
render.grid(row=1, column=0, sticky='nw', columnspan=50)
render.bind('<MouseWheel>', on_render_scroll)
render.bind('<Button-1>', on_render_click)

lens_controls = tk.Frame(root)
lens_controls.grid(row=3, column=0, sticky='w')

l1 = tk.Label(lens_controls, text='Lens')
l1.grid(row=0, column=0, sticky='w')
l2 = tk.Label(lens_controls, text='PosX')
l2.grid(row=0, column=1, sticky='w')
l3 = tk.Label(lens_controls, text='PosY')
l3.grid(row=0, column=2, sticky='w')
l4 = tk.Label(lens_controls, text='Direction')
l4.grid(row=0, column=3, sticky='w')

rt = tk.Button(lens_controls, text='Raytrace', command=raytrace)
rt.grid(row=100, column=0, sticky='w')

add = tk.Button(lens_controls, text='Add Lens', command=add_lens)
add.grid(row=100, column=1)

sub = tk.Button(lens_controls, text='Remove Lens', command=remove_lens)
sub.grid(row=100, column=2)


add_lens()
    
raytrace()

root.mainloop()