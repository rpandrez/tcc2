import json
from pickle import FLOAT
import nElementos
import convert_time
import save_XLS

json_name = 'gateway-utfpr_live_data_1683563433304.json'

with open(json_name, 'r') as f:
    data = json.load(f)
i=0
maximo =  nElementos.nElemup(json_name)

pnome : list[str] = []
pGWid : list[str] = []
pEUI : list[str] = []
pNPKG : list[int] = []
pBW : list[int] = []
pSF : list[int] = []
pCR : list[float] = []
pFREQ : list[int] = []
pRSSI : list[int] = []
pSNR : list[int] = []
pTEMPO1 : list[str] = []
pTEMPO2 : list[str] = []
pTEMPO3 : list[str] = []
pTEMPO4 : list[str] = []
pTEMPO5 : list[str] = []

while (i < maximo):
    '''if i%2 == 0:
        if verfdw == 'type.googleapis.com/ttn.lorawan.v3.DownlinkMessage':
            #downlink
            nomedw = data[i]['name']
            tempo = data[i]['time']
            GWid = data[i]['identifiers'][i]['gateway_ids']['gateway_id']
            BWdw = data[i]['data']['scheduled']['data_rate']['lora']['bandwidth']
            SFdw = data[i]['data']['scheduled']['data_rate']['lora']['spreading_factor']
            CRdw = data[i]['data']['scheduled']['data_rate']['lora']['coding_rate']
            freqdw = data[i]['data']['scheduled']['frequency']
            PotenciaTrasmissao = data[i]['data']['scheduled']['downlink']['tx_power']
        else:
            print('erro')
    else:'''
    if i == 273  or i == 275  or i == 292:
        i = i + 1
    if 'data' in data[i]:
        verf = data[i]['data']['@type']
        if verf == 'type.googleapis.com/ttn.lorawan.v3.GatewayUplinkMessage':
            metadata = data[i]['data']['message']['rx_metadata'][0]
            nome = data[i]['name']
            tempo1 = convert_time.convert_time(data[i]['time'])
            tempo2 = convert_time.convert_time(data[i]['data']['message']['settings']['time'])
            tempo3 = convert_time.convert_time(metadata['time'])
            tempo4 = convert_time.convert_time(metadata['received_at'])
            tempo5 = convert_time.convert_time(data[i]['data']['message']['received_at'])
            GWid = data[i]['identifiers'][0]['gateway_ids']['gateway_id']
            EUI = data[i]['identifiers'][0]['gateway_ids']['eui']
            
            #tipo = função tipo.py
            
            #npkg = int(data[i]['data']['message']['payload']['mac_payload']['f_hdr']['f_cnt']) #numero do pacote
            BW = int(data[i]['data']['message']['settings']['data_rate']['lora']['bandwidth'])
            SF = int(data[i]['data']['message']['settings']['data_rate']['lora']['spreading_factor'])
            CR = data[i]['data']['message']['settings']['data_rate']['lora']['coding_rate']
            freq = int(data[i]['data']['message']['settings']['frequency'])
            RSSI = int(metadata['rssi'])
            SNR  = float(metadata['snr'])
            #linha [i] = [nome, tmp1, tmp2, tmp3, tmp4, tmp5, GWid, EUI, npkg, BW, SF, CR, freq, RSSI, SNR]

            pnome.append(nome)
            pGWid.append(GWid)
            pEUI.append(EUI)
            #pNPKG.append(npkg)
            pBW.append(BW)
            pSF.append(SF)
            pCR.append(CR)
            pFREQ.append(freq)
            pRSSI.append(RSSI)
            pSNR.append(SNR)
            pTEMPO1.append(tempo1)
            pTEMPO2.append(tempo2)
            pTEMPO3.append(tempo3)
            pTEMPO4.append(tempo4)
            pTEMPO5.append(tempo5)
            print("iteração: ", i)
            i = i + 1
        else:
            print('erro')
            i = i + 1

save_XLS.criaXLS (pnome, pGWid, pEUI, pBW, pCR,  pFREQ, pRSSI, pSNR, pSF, pTEMPO1, pTEMPO2, pTEMPO3, pTEMPO4, pTEMPO5)