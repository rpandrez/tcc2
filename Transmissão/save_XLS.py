import pandas as pd

def criaXLS(lista1, lista2, lista3, lista4, lista5, lista6, lista7, lista8, lista9, lista10, lista11, lista12, lista13, lista14):
    # Criando um DataFrame com as listas
    df = pd.DataFrame({'Nome': lista1, 'GW id': lista2, 'EUI': lista3, 'BW': lista4, 'CR': lista5, 'Frequencia': lista6, 'RSSI': lista7,
                       'SNR': lista8, 'SF': lista9, 'Tempo1': lista10, 'Tempo2': lista11, 'Tempo3': lista12, 'Tempo4': lista13, 'Tempo5': lista14})

    # Escrevendo o DataFrame em um arquivo .xls
    with pd.ExcelWriter('meu_arquivo_now2.xlsx') as writer:
            df.to_excel(writer, index=False)