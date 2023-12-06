/********************************************************************************
Este código é um exemplo de um dispositivo LoRaWAN baseado no framework LMIC. Ele define algumas configurações do dispositivo, como chaves de sessão 
de rede e de aplicativo, endereço do dispositivo e mapeamento dos pinos. Além disso, ele define uma função do_send para enviar dados para um gateway 
LoRaWAN e uma função onEvent que será chamada quando determinados eventos ocorrerem.



Este é um exemplo de código para enviar um pacote LoRaWAN válido usando configurações de frequência e criptografia compatíveis com a The Things 
Network (TTN). Neste exemplo, a ativação é feita por personalização (ABP), em que o endereço do dispositivo (DevAddr) e as chaves de sessão 
(NwkSKey e AppSKey) são pré-configurados.

O código define os pinos para a placa de desenvolvimento HELTEC LoRa 32. 

O pacote a ser enviado é um numero de 32 bits (1101 0110 0111 0010 0001 0011 0001 1011) e é enviado a cada 3 segundos. 

O código coloca o dispositivo em modo hibernação nos intervalos entre mensagens, caracteristica da Classe A no protocolo LoRaWAN

A função onEvent é usada para imprimir na porta serial eventos importantes que ocorrem durante a execução do código. A função send() é usada para 
enviar o pacote LoRaWAN.
*/

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <Wire.h>
#include "RTClib.h"

//
// LoRaWAN NwkSKey, chave de sessão de rede em MSB
static const PROGMEM u1_t NWKSKEY[16] = {0xF1, 0xC5, 0x52, 0xA2, 0xE6, 0xEA, 0xE1, 0x89, 0xF9, 0xC2, 0x6A, 0x19, 0xF9, 0xD8, 0xC8, 0x50};

// LoRaWAN AppSKey, chave de sessão do aplicativo em MSB
static const u1_t PROGMEM APPSKEY[16] = {0xBF, 0x0A, 0x6E, 0x5A, 0xBB, 0x46, 0x23, 0xB6, 0xFA, 0x7C, 0x31, 0x55, 0xD3, 0x6C, 0xCA, 0xAA};

// Endereço do dispositivo final LoRaWAN (DevAddr) em MSB
static const u4_t DEVADDR = 0x260CB260 ; // <-- Altere este endereço para cada nó!

void os_getArtEui (u1_t* buf) { } //armazena em um buffer de 8 bytes e passa como argumento 
void os_getDevEui (u1_t* buf) { } 
void os_getDevKey (u1_t* buf) { }

static uint8_t mydata[] = "11010110011100100001001100011011"; //<- numero de 32 bits
static osjob_t sendjob; // <- agenda tarefas e interrupções no sistema LoRaWAN

// Agendar TX a cada X segundos (pode se tornar mais longo devido às limitações do ciclo de trabalho).
const unsigned TX_INTERVAL = 3;

// mapeamento de pinos p/ HELTEC LoRa 32
const lmic_pinmap lmic_pins = {
    .nss = 18,                       
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 14,                       
    .dio = {/*dio0*/ 26,/*dio1*/ 35,/*dio2*/ 34} 
};

void do_sleep(osjob_t* j) {
  // Configura o tempo para o próximo envio
  os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
  
  // Calcula quanto tempo falta para o próximo envio
  uint32_t sleep_time = TX_INTERVAL*1000000 - micros();

  // Verifica se o tempo de sono é maior que zero
  if (sleep_time > 0) {
    // Configura o dispositivo para acordar após o tempo de sono
    esp_sleep_enable_timer_wakeup(sleep_time);

   // Configura o dispositivo para acordar após o tempo de sono
    esp_deep_sleep_start();
  }
}


/*Essa função de callback é usada para lidar com diferentes eventos da rede LoRaWAN e 
 executar ações apropriadas, como agendar transmissões e processar dados recebidos.
 
A função onEvent() recebe um parâmetro ev que é o tipo de evento que ocorreu.*/

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            break;
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (Incluindo janelas de espera RX)"));
            if (LMIC.txrxFlags & TXRX_ACK)
              Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
              Serial.println(F("Received "));
              Serial.println(LMIC.dataLen);
              Serial.println(F(" bytes of payload"));
            }
            // Agendar próxima transmissão ou colocar em modo de hibernação profunda
            do_sleep(&sendjob);
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            break;
        case EV_TXSTART:
            Serial.println(F("EV_TXSTART"));
            break;
        case EV_TXCANCELED:
            Serial.println(F("EV_TXCANCELED"));
            break;
        case EV_RXSTART:
            break;
        case EV_JOIN_TXCOMPLETE:
            Serial.println(F("EV_JOIN_TXCOMPLETE: no JoinAccept"));
            break;
        default:
            Serial.print(F("Unknown event: "));
            Serial.println((unsigned) ev);
            break;
    }
}

/*do_send(osjob_t* j) é uma função que é chamada quando um trabalho agendado é executado pela biblioteca LMIC
  em um dispositivo LoRaWAN. É usada para enviar pacotes de dados e configurar o próximo trabalho.*/
//int pacote = 0;
void do_send(osjob_t* j){
    //pacote = pacote;
    // Verifica se não há um trabalho TX/RX atual em execução
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        
        // Prepare a transmissão de dados upstream no próximo momento possível.
        LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 0);
        //Serial.print("Enviando pacote número ");
        //Serial.println(pacote);
        
        unsigned long sentTime = millis();
        //Serial.print("Pacote: ");
        //Serial.println(pacote);
        //Serial.print("Momento do envio ");
        /*DateTime now = rtc.now();

        Serial.print(now.day(), DEC);  
        Serial.print('/');
        Serial.print(now.month(), DEC);
        Serial.print('/');
        Serial.print(now.year(), DEC);
        Serial.print(' ');
        Serial.print(now.hour(), DEC);
        Serial.print(':');
        Serial.print(now.minute(), DEC);
        Serial.print(':');
        Serial.print(now.second(), DEC);
        Serial.print('.');
        Serial.print(micros() % 1000);
        Serial.println();*/
        //pacote++;
    }
  // O próximo TX é agendado após o evento TX_COMPLETE.
}


void setup() {
    Wire.begin();
    /*if (! rtc.begin()) {
    Serial.println("Não foi possível encontrar o módulo RTC!");
    while (1);
    }

    if (! rtc.isrunning()) {
    Serial.println("O módulo RTC não está sendo executado! Configurando...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }*/
  
    while (!Serial); // espera a Serial ser inicializada
    Serial.begin(115200);
    delay(100); 
    Serial.println(F("Starting"));
    //Serial.println(pacote);
    //pacote++;

    /*Define VCC High e espera um segundo pra inicializar*/
    #ifdef VCC_ENABLE
    pinMode(VCC_ENABLE, OUTPUT);
    digitalWrite(VCC_ENABLE, HIGH);
    delay(1000);
    #endif

    // Inicialização do OS
    os_init();
    // Reset na biblioteca LMIC
    LMIC_reset();

    /* Verifica se a diretiva de pré-processamento PROGMEM está definida, para verificar se as chaves estão armazenadas na memória flash do 
    microcontrolador. Se estiver, os valores de APPSKEY e NWKSKEY são copiados para as variáveis appskey e nwkskey usando memcpy_P().
    Em seguida, a sessão LMIC é configurada usando LMIC_setSession(). Se a diretiva PROGMEM não estiver definida, a sessão é configurada 
    diretamente com NWKSKEY e APPSKEY. */

    #ifdef PROGMEM 
    uint8_t appskey[sizeof(APPSKEY)];
    uint8_t nwkskey[sizeof(NWKSKEY)];
    memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
    memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
    LMIC_setSession (0x13, DEVADDR, nwkskey, appskey);
    #else
    LMIC_setSession (0x13, DEVADDR, NWKSKEY, APPSKEY);
    #endif

    #if defined(CFG_us915) || defined(CFG_au915)
    // Os canais 0-71 NA-US e AU são configurados automaticamente
    LMIC_selectSubBand(1);
    #else
    # error Region not supported
    #endif

// Desativa a validação de verificação de link
    LMIC_setLinkCheckMode(0);

// TTN usa SF9 para sua janela RX2.
    LMIC.dn2Dr = DR_SF9;

// Definir taxa de dados e a potencia de transmissão para uplink
    LMIC_setDrTxpow(DR_SF7,27);

// Inicia o trabalho
    do_send(&sendjob);
}


void loop() {
    unsigned long now;
    now = millis();
    if ((now & 512) != 0) {
      digitalWrite(13, HIGH);
    }
    else {
      digitalWrite(13, LOW);
    }
    os_runloop_once();
}
