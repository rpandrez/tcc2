import pandas as pd

# Ler o arquivo de texto
with open(r'C:\Users\renan\Desktop\Sudo\TCC II\TCC2\consumo_rasp.txt', 'r') as f:
    lines = f.readlines()

# Extrair os valores de cada linha
values = {'Bus Voltage': [], 'Current': [], 'Power': []}
for i in range(0, len(lines), 4):
    values['Bus Voltage'].append(float(lines[i].split(':')[1].strip().replace('V', '')))
    values['Current'].append(float(lines[i+1].split(':')[1].strip().replace('mA', '')))
    values['Power'].append(float(lines[i+2].split(':')[1].strip().replace('mW', '')))

# Criar um DataFrame com os valores extraídos
df = pd.DataFrame(values)

# Salvar o DataFrame em um arquivo .xlsx
df.to_excel(r'C:\Users\renan\Desktop\Sudo\TCC II\TCC2\consumo_rasp.xlsx', index=False)
