// https://github.com/SukkoPera/PsxNewLib
// https://github.com/dmadison/ArduinoXInput

#define XINPUT

#ifdef XINPUT
#include <XInput.h>
#endif

#ifndef XINPUT
#define DEBUG
#endif

#include <DigitalIO.h>
#include <PsxControllerHwSpi.h>

#include <avr/pgmspace.h>
typedef const __FlashStringHelper * FlashStr;
typedef const byte* PGM_BYTES_P;
#define PSTR_TO_F(s) reinterpret_cast<const __FlashStringHelper *> (s)

// This can be changed freely but please see above
const byte PIN_PS2_ATT = 10;

const byte PIN_BUTTONPRESS = A0;
const byte PIN_HAVECONTROLLER = A1;

bool dirty = true;

const char buttonSelectName[] PROGMEM = "Select";
const char buttonL3Name[] PROGMEM = "L3";
const char buttonR3Name[] PROGMEM = "R3";
const char buttonStartName[] PROGMEM = "Start";
const char buttonUpName[] PROGMEM = "Up";
const char buttonRightName[] PROGMEM = "Right";
const char buttonDownName[] PROGMEM = "Down";
const char buttonLeftName[] PROGMEM = "Left";
const char buttonL2Name[] PROGMEM = "L2";
const char buttonR2Name[] PROGMEM = "R2";
const char buttonL1Name[] PROGMEM = "L1";
const char buttonR1Name[] PROGMEM = "R1";
const char buttonTriangleName[] PROGMEM = "Triangle";
const char buttonCircleName[] PROGMEM = "Circle";
const char buttonCrossName[] PROGMEM = "Cross";
const char buttonSquareName[] PROGMEM = "Square";


#define PS_select (psxButtons & ( 1 << 0 ))
#define PS_L3 (psxButtons & ( 1 << 1 ))
#define PS_R3 (psxButtons & ( 1 << 2 ))
#define PS_start (psxButtons & ( 1 << 3 ))
#define PS_up (psxButtons & ( 1 << 4 ))
#define PS_right (psxButtons & ( 1 << 5 ))
#define PS_down (psxButtons & ( 1 << 6 ))
#define PS_left (psxButtons & ( 1 << 7 ))
#define PS_L2 (psxButtons & ( 1 << 8 ))
#define PS_R2 (psxButtons & ( 1 << 9 ))
#define PS_L1 (psxButtons & ( 1 << 10 ))
#define PS_R1 (psxButtons & ( 1 << 11 ))
#define PS_T (psxButtons & ( 1 << 12 ))
#define PS_O (psxButtons & ( 1 << 13 ))
#define PS_X (psxButtons & ( 1 << 14 ))
#define PS_S (psxButtons & ( 1 << 15 ))

#define PS_LX (((uint16_t)lx*257)-32768) // 0..255 -> -32768..32767
#define PS_LY (((255-(uint16_t)ly)*257)-32768) // 0..255 -> 32767..-32768
#define PS_RX (((uint16_t)rx*257)-32768) // 0..255 -> -32768..32767
#define PS_RY (((255-(uint16_t)ry)*257)-32768) // 0..255 -> 32767..-32768
//psxnelib: left-right: 0...255 down-up: 255...0, xinput: left-right: -32768..32767, down-up: 32767..-32768

const char* const psxButtonNames[PSX_BUTTONS_NO] PROGMEM = {
  buttonSelectName,
  buttonL3Name,
  buttonR3Name,
  buttonStartName,
  buttonUpName,
  buttonRightName,
  buttonDownName,
  buttonLeftName,
  buttonL2Name,
  buttonR2Name,
  buttonL1Name,
  buttonR1Name,
  buttonTriangleName,
  buttonCircleName,
  buttonCrossName,
  buttonSquareName
};

byte psxButtonToIndex (PsxButtons psxButtons) {
  byte i;

  for (i = 0; i < PSX_BUTTONS_NO; ++i) {
    if (psxButtons & 0x01) {
      break;
    }

    psxButtons >>= 1U;
  }

  return i;
}

FlashStr getButtonName (PsxButtons psxButton) {
  FlashStr ret = F("");
  
  byte b = psxButtonToIndex (psxButton);
  if (b < PSX_BUTTONS_NO) {
    PGM_BYTES_P bName = reinterpret_cast<PGM_BYTES_P> (pgm_read_ptr (&(psxButtonNames[b])));
    ret = PSTR_TO_F (bName);
  }

  return ret;
}

void dumpButtons (PsxButtons psxButtons) {
  static PsxButtons lastB = 0;

  if (psxButtons != lastB) {
    lastB = psxButtons;     // Save it before we alter it

    #ifdef DEBUG
    Serial.print (F("Pressed: "));

    for (byte i = 0; i < PSX_BUTTONS_NO; ++i) {
      byte b = psxButtonToIndex (psxButtons);
      if (b < PSX_BUTTONS_NO) {
        PGM_BYTES_P bName = reinterpret_cast<PGM_BYTES_P> (pgm_read_ptr (&(psxButtonNames[b])));
        Serial.print (PSTR_TO_F (bName));
      }

      psxButtons &= ~(1 << b);

      if (psxButtons != 0) {
        Serial.print (F(", "));
      }
    }
    Serial.println ();
    #endif
        
    #ifdef XINPUT
    if (PS_select) XInput.press(BUTTON_BACK); else XInput.release(BUTTON_BACK); //btn_7
    if (PS_start) XInput.press(BUTTON_START); else XInput.release(BUTTON_START); //btn_8
    if (PS_L3) XInput.press(BUTTON_L3); else XInput.release(BUTTON_L3); //btn_9
    if (PS_R3) XInput.press(BUTTON_R3); else XInput.release(BUTTON_R3); //btn_10
    XInput.setDpad(PS_up, PS_down, PS_left, PS_right);
    if (PS_X) XInput.press(BUTTON_A); else XInput.release(BUTTON_A); //btn_1
    if (PS_O) XInput.press(BUTTON_B); else XInput.release(BUTTON_B); //btn_2
    if (PS_S) XInput.press(BUTTON_X); else XInput.release(BUTTON_X); //btn_3
    if (PS_T) XInput.press(BUTTON_Y); else XInput.release(BUTTON_Y); //btn_4
    if (PS_L1) XInput.press(BUTTON_LB); else XInput.release(BUTTON_LB); //btn_5
    if (PS_R1) XInput.press(BUTTON_RB); else XInput.release(BUTTON_RB); //btn_6
    if (PS_L2) XInput.setTrigger(TRIGGER_LEFT, 255); else XInput.setTrigger(TRIGGER_LEFT, 0);
    if (PS_R2) XInput.setTrigger(TRIGGER_RIGHT, 255); else XInput.setTrigger(TRIGGER_RIGHT, 0);
    dirty = true;
    #endif
  }
}

void dumpAnalog (const char *str, const byte x, const byte y) {
  #ifdef DEBUG
  Serial.print (str);
  Serial.print (F(" analog: x = "));
  Serial.print (x);
  Serial.print (F(", y = "));
  Serial.println (y);
  #endif
}



const char ctrlTypeUnknown[] PROGMEM = "Unknown";
const char ctrlTypeDualShock[] PROGMEM = "Dual Shock";
const char ctrlTypeDsWireless[] PROGMEM = "Dual Shock Wireless";
const char ctrlTypeGuitHero[] PROGMEM = "Guitar Hero";
const char ctrlTypeOutOfBounds[] PROGMEM = "(Out of bounds)";

const char* const controllerTypeStrings[PSCTRL_MAX + 1] PROGMEM = {
  ctrlTypeUnknown,
  ctrlTypeDualShock,
  ctrlTypeDsWireless,
  ctrlTypeGuitHero,
  ctrlTypeOutOfBounds
};







PsxControllerHwSpi<PIN_PS2_ATT> psx;

boolean haveController = false;
 
void setup () {
  fastPinMode (PIN_BUTTONPRESS, OUTPUT);
  fastPinMode (PIN_HAVECONTROLLER, OUTPUT);
  
  delay (300);

  #ifdef DEBUG
  Serial.begin (115200);
  while(!Serial);
  Serial.println (F("Ready!"));
  #endif

  #ifdef XINPUT
  XInput.setAutoSend(false);
  XInput.begin();
  #endif
}
 
void loop () {
  static byte slx, sly, srx, sry, sl2, sr2;
  fastDigitalWrite (PIN_HAVECONTROLLER, haveController);
  
  if (!haveController) {
    if (psx.begin ()) {
      #ifdef DEBUG
      Serial.println (F("Controller found!"));
      #endif
      delay (300);
      if (!psx.enterConfigMode ()) {
        #ifdef DEBUG
        Serial.println (F("Cannot enter config mode"));
        #endif
      } else {
        PsxControllerType ctype = psx.getControllerType ();
        PGM_BYTES_P cname = reinterpret_cast<PGM_BYTES_P> (pgm_read_ptr (&(controllerTypeStrings[ctype < PSCTRL_MAX ? static_cast<byte> (ctype) : PSCTRL_MAX])));
        #ifdef DEBUG
        Serial.print (F("Controller Type is: "));
        Serial.println (PSTR_TO_F (cname));
        #endif

        if (!psx.enableAnalogSticks ()) {
          #ifdef DEBUG
          Serial.println (F("Cannot enable analog sticks"));
          #endif
        }
        
        //~ if (!psx.setAnalogMode (false)) {
          //~ Serial.println (F("Cannot disable analog mode"));
        //~ }
        //~ delay (10);
        
        if (!psx.enableAnalogButtons ()) {
          #ifdef DEBUG
          Serial.println (F("Cannot enable analog buttons"));
          #endif
        }
        
        if (!psx.exitConfigMode ()) {
          #ifdef DEBUG
          Serial.println (F("Cannot exit config mode"));
          #endif
        }
      }
      
      haveController = true;
    }
  } else {
    if (!psx.read ()) {
      #ifdef DEBUG
      Serial.println (F("Controller lost :("));
      #endif
      haveController = false;
    } else {
      fastDigitalWrite (PIN_BUTTONPRESS, !!psx.getButtonWord ());
      dumpButtons (psx.getButtonWord ());

      byte lx, ly;
      psx.getLeftAnalog (lx, ly);
      if (lx != slx || ly != sly) {
        dumpAnalog ("Left", lx, ly);
        #ifdef XINPUT
        XInput.setJoystick(JOY_LEFT, PS_LX, PS_LY); //psxnelib: left-right: 0...255 down-up: 255...0, xinput: left-right: -32768..32767, down-up: 32767..-32768
        dirty = true;
        #endif
        slx = lx;
        sly = ly;
      }

      byte rx, ry;
      psx.getRightAnalog (rx, ry);
      if (rx != srx || ry != sry) {
        dumpAnalog ("Right", rx, ry);
        #ifdef XINPUT
        XInput.setJoystick(JOY_RIGHT, PS_RX, PS_RY); //psxnelib: left-right: 0...255 down-up: 255...0, xinput: left-right: -32768..32767, down-up: 32767..-32768
        dirty = true;
        #endif
        srx = rx;
        sry = ry;
      }

      byte l2 = psx.getAnalogButton(PSAB_L2);
      if (l2 != 0 && l2 != sl2) {
        #ifdef DEBUG
        Serial.println(l2);
        #endif
        XInput.setTrigger(TRIGGER_LEFT, l2);
        dirty = true;
        sl2 = l2;
      }

      byte r2 = psx.getAnalogButton(PSAB_R2);
      if (r2 != 0 && r2 != sr2) {
        #ifdef DEBUG
        Serial.println(r2);
        #endif
        XInput.setTrigger(TRIGGER_RIGHT, r2);
        dirty = true;
        sr2 = r2;
      }
      
    }
  }

  
  //delay (1000 / 60);
  #ifdef XINPUT
  if (dirty) {
    XInput.send();
    dirty = false;
  }
  #endif
  
  //delayMicroseconds(1000);
  delay(10);

}
