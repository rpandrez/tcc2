import json

def nElemdw(json_name):
    with open(json_name, 'r') as f:
        data = json.load(f)
    
    countdw = 0
    for element in data:
        if 'data' in element and '@type' in element['data']:
            if element['data']['@type'] == "type.googleapis.com/ttn.lorawan.v3.DownlinkMessage":
                countdw += 1
        return countdw-1
    #print("N Dw = ", countdw)
    #print("N Up = ", countup)
def nElemup(json_name):
    with open(json_name, 'r') as f:
        data = json.load(f)
    
        countup = 0
    for element in data:
        if 'data' in element and '@type' in element['data']:
            if element['data']['@type'] == "type.googleapis.com/ttn.lorawan.v3.GatewayUplinkMessage":
                countup += 1
            
    return countup-1

