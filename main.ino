
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - G P I O   D E F I N I T I O N S - - - - - - - - - - - - - - - - - - - - - - - - - - - -


const uint8_t RFmod = 5;  // RF modulation pin for 433 MHz transmitter

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - C O N S T   D E F I N I T I O N S - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const uint8_t numberOfChars = 40;  // Р’ СЃС‚СЂРѕРєРµ(РјР°СЃСЃРёРІРµ СЃРёРјРІРѕР»РѕРІ) РґРѕСЃС‚СѓРїРЅРѕ 29 СЌР»РµРјРµРЅС‚РѕРІ СЃ 0 РїРѕ 29. 30Р№ СЌР»РµРјРµРЅС‚ - СЌС‚Рѕ СЃРёРјРІРѕР» РєРѕРЅС†Р° СЃС‚СЂРѕРєРё 0x00

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - V A R   D E F I N I T I O N S - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// 2nd channel
char onCommand[26] = {'O','0','1','1','0','0','0','1','1','1','1','0','0','1','0','1','0','1','0','1','1','1','1','0','0','0'};
char offCommand[26] = {'O','0','1','1','0','0','0','1','1','1','1','0','0','1','0','1','0','1','0','1','1','0','0','1','0','0'};

//char onCommand[26] = {'O','0','0','1','0','1','0','1','0','0','1','0','0','1','0','1','0','1','0','1','1','1','1','0','0','1'};
//char offCommand[26] = {'O','0','1','1','1','0','1','1','0','1','0','0','0','0','0','1','0','1','1','1','1','0','0','1','0','1'};

char message[numberOfChars];

uint8_t shortPulse;
uint8_t shortPause;
uint16_t longPulse;
uint16_t longPause;
uint16_t syncpause;



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - S E T U P   F U N C T I O N - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void setup() 
{
  O(RFmod);  L(RFmod);
  Serial.begin(115200);
  for(uint8_t i = 0; i < numberOfChars-1; i++)  message[i] = 50; // РёРЅРёС†РёР°Р»РёР·РёСЂСѓРµРј РјР°СЃСЃРёРІ. = 50 СЂР°РІРЅРѕСЃРёР»СЊРЅРѕ = '2' ASCII
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - M A I N   L O O P - - - - - - - - - - - - - - - - - - - - - - -

void loop()
{
  unlockDoor(); 
  delay(2000);
  
}


// - - - - - - - - - - - - - - - - - - - S O C K E T   R F   C O N T R O L   F U N C T I O N S - - - - - - - - - - - - - - - - - - - - -

// work with custom socket commands
void switchSocket()
{
  initPulseLength(110 + (message[0] - 65)*10); // linear 'A' = 110 ... 'T' = 300;);
  for(uint8_t k = 1; k<=6; k++)
  {                  // 6 РїР°С‡РµРє РїРѕРґСЂСЏРґ РґР°СЋС‚ РЅР°РґРµР¶РЅРѕРµ СЂР°СЃРїРѕР·РЅР°РІР°РЅРёРµ РєРѕРјР°РЅРґС‹
    for(uint8_t counter = 1; counter <= 25; counter++)
    {
      if(message[counter] == '0')
      {
        pulseL(); 
      };  // РµСЃР»Рё '0', С‚Рѕ РІС‹РґР°С‚СЊ РєРѕСЂРѕС‚РєРёР№ РёРјРїСѓР»СЊСЃ
      if(message[counter] == '1')
      {
        pulseH();
      }  // РµСЃР»Рё '1', С‚Рѕ РІС‹РґР°С‚СЊ РґР»РёРЅРЅС‹Р№ РёРјРїСѓР»СЊСЃ
//      if(message[counter] == 'F') 
//      {
//        pulseL(); 
//        pulseH();
//      }  // РµСЃР»Рё '2' = "F", С‚Рѕ РІС‹РґР°С‚СЊ 1 РєРѕСЂРѕС‚РєРёР№ Рё РїРѕС‚РѕРј 1 РґР»РёРЅРЅС‹Р№ РёРјРїСѓР»СЊСЃ
    };
//    pulseL();
    delayMicroseconds(syncpause);      // РїРѕРґ РєРѕРЅРµС† РїР°С‡РєРё РІС‹РґР°С‚СЊ 1 РєРѕСЂРѕС‚РєРёР№ РёРјРїСѓР»СЊСЃ Рё РІС‹РґРµСЂР¶Р°С‚СЊ СЃРёРЅС…СЂРѕРїР°СѓР·Сѓ
  };
  //  for(counter = 0; counter <= 13; counter++) socketCommand[counter] = 0; // РѕС‡РёСЃС‚РёС‚СЊ РјР°СЃСЃРёРІ Р±Р°Р№С‚
  initPulseLength(250); // default value
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - 

// overloaded method. 
void switchSocket(char *command)
{
  char tmp[26];
  for(uint8_t i = 0; i < 26; i++) tmp[i] = message[i];
  for(uint8_t i = 0; i < 26; i++) message[i] = command[i];
  switchSocket();
  for(uint8_t i = 0; i < 26; i++) message[i] = tmp[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - 

void pulseH()
{
  H(RFmod);
  delayMicroseconds(longPulse);
  L(RFmod);
  delayMicroseconds(shortPause);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - 

void pulseL()
{
  H(RFmod);
  delayMicroseconds(shortPulse);
  L(RFmod); 
  delayMicroseconds(longPause);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - 

void initPulseLength(uint8_t lenght)
{
  shortPulse = lenght; // 215;  // real 221 - СЂРµР°Р»СЊРЅРѕ РЅР°С‡РёРЅР°РµС‚ СЂР°Р±РѕС‚Р°С‚СЊ СЃ 160 Рё РґРѕ 330 
  shortPause = shortPulse; // 220; // 222;  // real 228
  longPulse = shortPulse * 3; // 650;   // real 672
  longPause = shortPause * 3; // 665;   // real 679
  syncpause = shortPulse * 28; // 6245;  // real 6990 (but 6990+longPause = 7669. correction is ~(-65..-75))  
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - 


void unlockDoor()
{
  switchSocket(onCommand); delay(100); switchSocket(onCommand); delay(100); switchSocket(onCommand); delay(600);
  switchSocket(offCommand); delay(100); switchSocket(offCommand); delay(100); switchSocket(offCommand);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - L O W   L E V E L   F U N C T I O N S - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void H(uint8_t pinN)
{
  digitalWrite(pinN, HIGH);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - 
void L(uint8_t pinN)
{
  digitalWrite(pinN, LOW);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - 
void I(uint8_t pinN)
{
  pinMode(pinN, INPUT);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - 
void O(uint8_t pinN)
{
  pinMode(pinN, OUTPUT);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - 
boolean dR(uint8_t pinN)
{
  return(digitalRead(pinN));
}
