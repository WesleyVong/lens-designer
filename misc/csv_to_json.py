import csv
import os
import json

def csv_to_json(csv_filepath, json_filepath):
    with open(csv_filepath, 'r') as csv_file:
        csv_reader = csv.reader(csv_file)
        headers = next(csv_reader)
        
        # Create the JSON object
        json_data = {}
        for row in csv_reader:
            key = row[0]
            json_data[key] = {headers[i]: row[i] for i in range(1, len(headers))}
    
    # Write the JSON object to a file
    with open(json_filepath, 'w') as json_file:
        json.dump(json_data, json_file, indent=4)

# File paths
dirname = os.path.dirname(__file__)
csv_filepath = os.path.join(dirname, 'optical_constants_fixed.csv')
json_filepath = os.path.join(dirname, 'optical_constants.json')

# Convert CSV to JSON
csv_to_json(csv_filepath, json_filepath)