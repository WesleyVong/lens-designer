import os
import json
import math

dirname = os.path.dirname(__file__)
lens_directory = os.path.join(dirname, 'lib_lens')
lens_library = {}
lens_json = os.listdir(lens_directory)
for l in lens_json:
    with open(os.path.join(lens_directory, l), 'r') as f:
        lens_library.update(json.load(f))

material_directory = os.path.join(dirname, 'lib_material')
material_library = {}
material_json = os.listdir(material_directory)
for i in material_json:
    with open(os.path.join(material_directory, i), 'r') as f:
        material_library.update(json.load(f))

def get_lens_keys():
    return lens_library.keys()

def get_lens(name):
    if name in lens_library:
        return lens_library[name]
    return None

class Lens:
    def __init__(self, name, position):
        self.name = name
        self.position = position
        self.lens = get_lens(name)
        if self.lens is None:
            raise ValueError('Lens {} not found'.format(name))
        
    def __str__(self):
        front_type = 'ARC'
        front_position = [self.position[0] + self.lens['r0'] - self.lens['thickness'] / 2, self.position[1]]
        front_radius = self.lens['r0']
        front_start_angle = -math.asin((self.lens['diameter'] / 2) / front_radius) + math.pi
        front_end_angle = math.asin((self.lens['diameter'] / 2) / front_radius) + math.pi

        rear_type = 'ARC'
        rear_position = [self.position[0] - self.lens['r0'] + self.lens['thickness'] / 2, self.position[1]]
        rear_radius = self.lens['r0']
        rear_start_angle = -math.asin((self.lens['diameter'] / 2) / rear_radius)
        rear_end_angle = math.asin((self.lens['diameter'] / 2) / rear_radius)

        material = material_library[self.lens['material']]
        return  ('IOR 3 {} {} {} {} {} {}\n'.format(material['B1'], material['B2'], material['B3'], material['C1'], material['C2'], material['C3']) +
                 'MATERIAL 0.5 0.5 0.5\n' +
                '{} {} {} {} {} {}\n'.format(front_type, front_position[0], front_position[1], front_radius, front_start_angle, front_end_angle) +
                'IOR 0\n' +
                'MATERIAL 0.5 0.5 0.5\n' +
                '{} {} {} {} {} {}\n'.format(rear_type, rear_position[0], rear_position[1], rear_radius, rear_start_angle, rear_end_angle))
                           
