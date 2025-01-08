import pdfplumber
import pandas as pd
import re

class OpticsDataExtractor:
    def __init__(self, pdf_path):
        self.pdf_path = pdf_path
        
    def extract_constants(self):
        results = []
        
        with pdfplumber.open(self.pdf_path) as pdf:
            for page in pdf.pages:
                # Extract text from the page
                text = page.extract_text()
                
                # Split text into lines and check if "Data Sheet" is in the first few lines
                lines = text.split('\n')
                first_lines = ' '.join(lines[:3]).lower()  # Check first 3 lines
                
                # Only process pages that have "Data Sheet" in the title area
                if 'data sheet' in first_lines:
                    # Find tables in the page
                    tables = page.extract_tables()
                    
                    for table in tables:
                        # Convert table to pandas DataFrame for easier handling
                        df = pd.DataFrame(table)
                        
                        # Check if this is the Constants of Dispersion Formula table
                        if any('Constants of Dispersion Formula' in str(cell) for cell in df.iloc[:, 0]):
                            # Process the constants
                            constants = self._process_constants_table(df)
                            
                            # Try to extract the optic's name from the first few lines
                            optic_name = self._extract_optic_name(lines[:5])
                            constants['optic_name'] = optic_name
                            
                            results.append(constants)
        
        return results
    
    def _extract_optic_name(self, lines):
        """Extract optic name from the first few lines of the data sheet"""
        for line in lines:
            # Skip lines with "Data Sheet" or empty lines
            if 'Data Sheet' in line or not line.strip():
                continue
            return line.strip()
        return "Unknown"
    
    def _process_constants_table(self, df):
        constants = {}
        
        # Find the start of the Constants of Dispersion Formula table
        start_idx = None
        for idx, row in df.iterrows():
            if any('Constants of Dispersion Formula' in str(cell) for cell in row):
                start_idx = idx
                break
                
        if start_idx is None:
            return constants
            
        # Process only the rows after the header until we hit an empty row or different table
        for idx in range(start_idx + 1, len(df)):
            row = df.iloc[idx]
            
            # Stop if we hit an empty row or a row that looks like a different table header
            if pd.isna(row[0]) or pd.isna(row[1]) or ('table' in str(row[0]).lower()):
                break
                
            # Clean the constant name and value
            constant_name = str(row[0]).strip()
            constant_value = str(row[1]).strip()
            
            # Only process B1, B2, B3, C1, C2, C3 constants
            if not constant_name.startswith(('B', 'C')):
                continue
                
            # Convert value to float if possible
            try:
                constant_value = float(constant_value)
                constants[constant_name] = constant_value
            except ValueError:
                continue
                
        return constants

def main():
    # Example usage
    pdf_path = "resources/schott-optical-glass-collection-datasheets-english-us-march2018.pdf"
    extractor = OpticsDataExtractor(pdf_path)
    results = extractor.extract_constants()
    
    # Convert results to DataFrame
    df_results = pd.DataFrame(results)
    
    # Reorder columns to put optic_name first
    cols = ['optic_name'] + [col for col in df_results.columns if col != 'optic_name']
    df_results = df_results[cols]
    
    # Print the table
    pd.set_option('display.max_rows', None)
    pd.set_option('display.max_columns', None)
    pd.set_option('display.width', None)
    print("\nExtracted Constants:")
    print(df_results)
    
    # Optionally save to CSV
    df_results.to_csv('misc/optical_constants.csv', index=False)
    print("\nResults have been saved to 'optical_constants.csv'")

if __name__ == "__main__":
    main() 