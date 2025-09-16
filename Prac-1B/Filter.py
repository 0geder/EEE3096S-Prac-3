import pandas as pd

# Load the file
#df = pd.read_excel("WEOApr2025all.xlsx")
df = pd.read_excel(r"C:\Users\0geda\OneDrive - University of Cape Town\Desktop\Third Year\Semester 2\EEE3096S\Week 1\EEE3096S-Prac-1\Prac-1B\WEOApr2025all.xlsx")
# Filter for Nigeria
nigeria_df = df[df['Country'] == 'Nigeria']
nigeria_df.to_excel("Nigeria_data.xlsx", index=False)

# Filter for South Africa
south_africa_df = df[df['Country'] == 'South Africa']
south_africa_df.to_excel("South_Africa_data.xlsx", index=False)

print("Files saved: Nigeria_data.xlsx and South_Africa_data.xlsx")
