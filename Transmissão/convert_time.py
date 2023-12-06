from dateutil import parser

def convert_time(data_hora_str):
    data_hora = parser.isoparse(data_hora_str)
    data_hora_formatada = data_hora.strftime('%H:%M:%S')

    return data_hora_formatada