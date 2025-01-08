import csv

def csv_to_markdown(csv_filepath, markdown_filepath):
    with open(csv_filepath, 'r') as csv_file:
        csv_reader = csv.reader(csv_file)
        headers = next(csv_reader)
        
        # Create the markdown table header
        markdown_lines = ['| ' + ' | '.join(headers) + ' |']
        markdown_lines.append('|' + '---|' * len(headers))
        
        # Add the rows to the markdown table
        for row in csv_reader:
            markdown_lines.append('| ' + ' | '.join(row) + ' |')
    
    # Write the markdown table to a file
    with open(markdown_filepath, 'w') as markdown_file:
        markdown_file.write('\n'.join(markdown_lines))

# File paths
csv_filepath = 'misc/optical_constants.csv'
markdown_filepath = 'misc/optical_constants.md'

# Convert CSV to Markdown
csv_to_markdown(csv_filepath, markdown_filepath)