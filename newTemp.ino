#include <Wire.h>                    // include the Wire library
#include <LiquidCrystal_I2C.h>       // include LiquidCrystal_I2C library.
#include <DS3231.h>
#include <PinButton.h>

#define LCD_ADDR        0x3F
#define Button_1        5                // Macro for the button 1 pin on the arduino
#define Button_2        8                // Macro for the button 2 pin on the arduino
#define Button_3        7                // Macro for the button 3 pin on the arduino
#define Button_4        4                // Macro for the button 4 pin on the arduino
#define Backlight_Pin   3
#define auger_Pin       6
#define distFan_Pin     9
#define combFan_Pin     10
PinButton Button2 (Button_2);
PinButton Button3 (Button_3);
PinButton Button4 (Button_4);
PinButton Button1 (Button_1);
DS3231 Clock;
LiquidCrystal_I2C lcd(LCD_ADDR, 20,4);  // Set the LCD I2C address 

int brightness = 100;
int mode = 12;
int setting = 3;
bool manualMode = true;
bool displayClock = true;
bool On = false;
bool displayOn;
int temperature_Setting = 21;
int temperature;
int temp_Set;
byte smily[8] = { B00000, B01010, B00000, B00000, B10001, B01110, B00000, B00000 };         // smily character not used
byte heart_h[8] = { B00000, B00000, B01010, B10101, B10001, B01010, B00100, B00000 };       // heart character not used
byte heart_f[8] = { B00000, B00000, B01010, B11111, B11111, B01110, B00100, B00000 };       // heart chatacter
byte temp[8] = { B00100, B01010, B01010, B01110, B01110, B11111, B11111, B01110 };          // icon for thermometer
byte fan[8] = { B00111, B01011, B00000, B01111, B01101, B01000,  B00100, B00000 };          // 1/2 icon for fan
byte fan2[8] = { B00100, B00010, B10110,  B11110,  B00000,  B11010,  B11100,  B00000 };     // 1/2 icon for fan
byte degree[8] = { B01100, B10010, B10010, B01100, B00000, B00000, B00000, B00000 };        // icon for degree symbol
byte arrow_u[8] = { B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100 };       // arrow up

void setup() {

  Serial.begin(9600);
  Wire.begin();                     // Initialize wire library
  pinMode(auger_Pin, OUTPUT);       // configure pin on plc
  pinMode(distFan_Pin, OUTPUT);
  pinMode(combFan_Pin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);       // configure pin on plc
  pinMode (Backlight_Pin, OUTPUT);
  analogWrite (Backlight_Pin, brightness);
  lcd.begin(16,2);                  // Initialize LCD 2x16
  lcd.backlight();                  // Turn on back light.  
  LoadChars();
  Startup_Text ();
}

void loop() {

  temperature = Clock.GET_TEMPERATURE() - 3;
  if (On == true) {
    if (manualMode) RunManualMode ();
    if (!manualMode) RunAutoMode ();
  }
  if (mode == 0) DisplayManualMode ();
  if (mode == 1) DisplayAutoMode ();
  if (mode == 2) SelectOperatingMode ();
  if (mode == 3) ClockMenu ();
  if (mode == 4) HourSet ();
  if (mode == 5) MinuteSet ();
  if (mode == 6) MonthSet ();
  if (mode == 7) DaySet ();
  if (mode == 8) WeekdaySet ();
  if (mode == 9) YearSet ();
  if (mode == 10) SecondSet ();
  if (mode == 11) OpMenu ();
  if (mode == 12) StartupMode();
  if (mode == 13) ShutdownMode ();
  if (mode == 14) SelectSettingsMode ();
  if (mode == 15) OnOffMenu ();
  if (mode == 16) SelectOnOff ();
  if (mode == 17) BacklightAdjustMenu ();
  if (mode == 18) BacklightAdjust ();
  if (mode == 19) SetTemp ();
  
} // loop

// *********************************** Display/Menu Functions ************************************

void DisplayAutoMode () {

bool dClockState;
bool prevdClockState;
  
  dClockState = displayClock;
  CheckButtons ();
  if (displayClock) {
    DisplayClock ();
  }
  if (!displayClock) {
    DisplayTemperature ();
  }
  if (Button1.isReleased ()) {
    if((dClockState != prevdClockState) && (dClockState == HIGH)){
      // button has been pressed
      displayClock = !displayClock;
    }
    else if( dClockState == LOW ){
        displayClock = HIGH;
      }
    lcd.clear();
  }
  if (Button2.isReleased ()) {
    if((dClockState != prevdClockState) && (dClockState == HIGH)){
      // button has been pressed
      displayClock = !displayClock;
    }
    else if( dClockState == LOW ){
        displayClock = HIGH;
      }
    lcd.clear ();
  }
  prevdClockState = dClockState;
  if (Button4.isReleased ()) {
    lcd.clear ();
    mode = 3;
  }
}

void DisplayManualMode () {

bool dClockState;
bool prevdClockState;
  
  dClockState = displayClock;
  CheckButtons ();
  if (displayClock) {
    DisplayClock ();
  }
  if (!displayClock) {
    DisplayTemperature ();
  }
  if (Button1.isReleased ()) {
    if((dClockState != prevdClockState) && (dClockState == HIGH)){
      // button has been pressed
      displayClock = !displayClock;
    }
    else if( dClockState == LOW ){
        displayClock = HIGH;
      }
    lcd.clear();
  }
  if (Button2.isReleased ()) {
    if((dClockState != prevdClockState) && (dClockState == HIGH)){
      // button has been pressed
      displayClock = !displayClock;
    }
    else if( dClockState == LOW ){
        displayClock = HIGH;
      }
    lcd.clear ();
  }
  prevdClockState = dClockState;
  if (Button4.isReleased ()) {
    lcd.clear ();
    mode = 3;
  }
}

void ClockMenu () {

  lcd.setCursor (0,0);
  lcd.print ("Set Clock");
  CheckButtons ();
  if (Button1.isReleased ()) {
    lcd.clear();
    mode = 11;
  }
  if (Button2.isReleased ()) {
    lcd.clear();
    mode = 17;
  }
  if (Button3.isReleased ()) {
    lcd.clear ();
    mode = 10;
  }
  if (Button4.isReleased ()) {
    mode = 0;
  }
}

void OpMenu () {

  lcd.setCursor (0,0);
  lcd.print ("Set Mode");
  CheckButtons ();
  if (Button1.isReleased ()) {
    mode = 15;
    lcd.clear();
  }
  if (Button2.isReleased ()) {
    mode = 3;
    lcd.clear();
  }
  if (Button3.isReleased ()) {
    lcd.clear ();
    mode = 2;
  }
  if (Button4.isReleased ()) {
    lcd.clear ();
    mode = 0;
  }
}

void OnOffMenu () {

  lcd.setCursor (0,0);
  lcd.print ("Switch On or Off");
  CheckButtons ();
  if (Button1.isReleased ()) {
    lcd.clear();
    mode = 17;
  }
  if (Button2.isReleased ()) {
    lcd.clear();
    mode = 11;
  }
  if (Button3.isReleased ()) {
    displayOn = On;
    mode = 16;
  }
  if (Button4.isReleased ()) {
    lcd.clear ();
    mode = 0;
  }  
}

void BacklightAdjustMenu () {

  lcd.setCursor (0,0);
  lcd.print ("Adjust Backlight");
  CheckButtons ();
  if (Button1.isReleased ()) {
    lcd.clear();
    mode = 3;
  }
  if (Button2.isReleased ()) {
    lcd.clear();
    mode = 15;
  }
  if (Button3.isReleased ()) {
    mode = 18;
  }
  if (Button4.isReleased ()) {
    lcd.clear ();
    mode = 0;
  }
}

void SetTemp () {
  
  lcd.setCursor (0,0);
  lcd.print ("Set Temperature");
  lcd.setCursor (0,1);
  lcd.print (temp_Set);
  CheckButtons ();
  if (Button1.isReleased ()) {
    temp_Set = temp_Set + 1;
    lcd.clear();
  }
  if (Button2.isReleased ()) {
    temp_Set = temp_Set - 1;
    lcd.clear();
  }
  if (Button3.isReleased ()) {
    lcd.clear();
    temperature_Setting = temp_Set;
    mode = 11;
  }
  if (Button4.isReleased ()) {
    lcd.clear ();
    temperature_Setting = temp_Set;
    mode = 11;
  }  
}

void BacklightAdjust () {

  lcd.setCursor (0,0);
  lcd.print ("Adjust Backlight");
  lcd.setCursor (0,1);
  lcd.print (brightness);
  CheckButtons ();
  if (Button1.isReleased ()) {
    if (brightness < 250) brightness = brightness + 25;
    analogWrite (Backlight_Pin, brightness);
    lcd.clear();
  }
  if (Button2.isReleased ()) {
    if (brightness > 25) brightness = brightness - 25;
    analogWrite (Backlight_Pin, brightness);
    lcd.clear();
  }
  if (Button3.isReleased ()) {
    lcd.clear();
    mode = 17;
  }
  if (Button4.isReleased ()) {
    lcd.clear ();
    mode = 17;
  }
}

void SelectOnOff () {

bool OnState;
bool prevOn;

  lcd.setCursor (0,0);
  lcd.print ("Switch On or Off");
  OnState = displayOn;
  if (displayOn == false){
    lcd.setCursor (0,1);
    lcd.print ("Off");
  }
  if (displayOn == true){
    lcd.setCursor (0,1);
    lcd.print ("On");
  }
  CheckButtons ();
  if (Button1.isReleased ()) {
    if((OnState != prevOn) && (OnState == HIGH))
    {
      // button has been pressed
      displayOn = !displayOn;
    }
    else if( OnState == LOW )
      {
        displayOn = HIGH;
      }
    lcd.clear ();
  }
  if (Button2.isReleased ()) {
    if((OnState != prevOn) && (OnState == HIGH))
    {
      // button has been pressed
      displayOn = !displayOn;
    }
    else if( OnState == LOW )
      {
        displayOn = HIGH;
      }
    lcd.clear ();
  }
  prevOn = On;
  if (Button3.isReleased ()) {
    lcd.clear ();
    if (displayOn == HIGH){
      On = LOW;
      if (digitalRead (auger_Pin) == HIGH) digitalWrite (auger_Pin, LOW);
      if (digitalRead (distFan_Pin) == HIGH) digitalWrite (distFan_Pin, LOW);
      if (digitalRead (combFan_Pin) == HIGH) digitalWrite (combFan_Pin, LOW);
      mode = 12;
    }
    if (displayOn == LOW){
      On = LOW;
      mode = 13;
    }
  }
  if (Button4.isReleased ()) {
    lcd.clear ();
    mode = 15;
  }  
}

void SelectSettingsMode () {
  lcd.setCursor (0,0);
  lcd.print ("Select Setting");
  lcd.setCursor (0,1);
  if (setting == 1) lcd.print ("Setting 1");
  if (setting == 2) lcd.print ("Setting 2");
  if (setting == 3) lcd.print ("Setting 3");
  if (setting == 4) lcd.print ("Setting 4");
  if (setting == 5) lcd.print ("Setting 5");
  if (setting == 6) lcd.print ("Setting 6");    
  CheckButtons ();
  if ((Button1.isReleased ()) && (setting <= 5)) {
    ++setting;
    lcd.clear();
  }
  if ((Button2.isReleased ()) && (setting >= 2)) {
    --setting;
    lcd.clear();
  }
  if (Button3.isReleased ()) {
    lcd.clear ();
    mode = 11;
  }
  if (Button4.isReleased ()) {
    lcd.clear ();
    mode = 11;
  }  
}

void SelectOperatingMode () {

bool manualState;
bool prevManual;

  manualState = manualMode;
  lcd.setCursor (0,0);
  lcd.print ("Set Mode");
  if (manualMode) {
    lcd.setCursor (0,1);
    lcd.print ("Manual");
  } else {
      lcd.setCursor (0,1);
      lcd.print ("Auto");
    }
  CheckButtons ();
  if (Button1.isReleased ()) {
    if((manualState != prevManual) && (manualState == HIGH)){
      // button has been pressed
      manualMode = !manualMode;
    }
    else if( manualState == LOW ){
        manualMode = HIGH;
      }
    lcd.clear();
  }
  if (Button2.isReleased ()) {
    if((manualState != prevManual) && (manualState == HIGH))
    {
      // button has been pressed
      manualMode = !manualMode;
    }
    else if( manualState == LOW )
      {
        manualMode = HIGH;
      }
    lcd.clear();
  }
  prevManual = manualMode;
  if (Button3.isReleased ()) {
    if (manualMode) {mode = 14;}
    if (!manualMode) {
      temp_Set = temperature_Setting;
      mode = 19;
    }
    lcd.clear ();
  }
  if (Button4.isReleased ()) {
    lcd.clear ();
    mode = 11;
  }
}

void HourSet () {

bool h12;
bool PM;

  lcd.setCursor (0,1);
  if (Clock.GET_HOUR(h12, PM) >= 1 && Clock.GET_HOUR(h12, PM) < 10) {
    lcd.setCursor (0, 1);
    lcd.print (" ");
    lcd.print (Clock.GET_HOUR(h12, PM));
  }  else if (Clock.GET_HOUR(h12, PM) >= 10 && Clock.GET_HOUR(h12, PM) <= 12) {
       lcd.setCursor (0, 1);
       lcd.print (Clock.GET_HOUR(h12, PM));
  }  else if (Clock.GET_HOUR(h12, PM) >= 13 && Clock.GET_HOUR(h12, PM) < 22) {
       lcd.setCursor (0, 1);
       lcd.print (" ");
       lcd.print (Clock.GET_HOUR(h12, PM) - 12);
  }  else if (Clock.GET_HOUR(h12, PM) >= 22 && Clock.GET_HOUR(h12, PM) < 24) {
       lcd.setCursor (0, 1);
       lcd.print (Clock.GET_HOUR(h12, PM) - 12);
  }  else if (Clock.GET_HOUR(h12, PM) == 0) {
       lcd.setCursor (0, 1);
       lcd.print ("12");
  }
  lcd.print (" ");
  if (Clock.GET_HOUR(h12, PM) >= 0 && Clock.GET_HOUR(h12, PM) <= 11) {
    lcd.print ("AM");
  } else if (Clock.GET_HOUR(h12, PM) >= 12 && Clock.GET_HOUR(h12, PM) < 24) {
      lcd.print ("PM");
  }
  lcd.setCursor (0,0);
  lcd.print ("Set Hour");
  CheckButtons();
  if (Button1.isReleased() && (Clock.GET_HOUR(h12, PM) >= 23)) {
    Clock.SET_HOUR (0);
  } else if (Button1.isReleased()) {
      Clock.SET_HOUR(Clock.GET_HOUR (h12, PM) + 1);
    }
  if (Button2.isReleased() && Clock.GET_HOUR(h12, PM) == 0) {
    Clock.SET_HOUR(23);
  } else if (Button2.isReleased()) {
      Clock.SET_HOUR(Clock.GET_HOUR (h12, PM) - 1);
    }
  if (Button3.isReleased()) {
    lcd.clear ();
    mode = 6;
  }
  if (Button4.isReleased ()) {
    lcd.clear ();
    mode = 3;
  }
}

void MinuteSet () {

  CheckButtons ();
  lcd.setCursor (0,0);
  lcd.print ("Set Minute");
  lcd.setCursor (0,1);
  if (Clock.GET_MINUTE () < 10) {
    lcd.print ("0");
    lcd.print (Clock.GET_MINUTE ());
  } else {
    lcd.print (Clock.GET_MINUTE ());
  }
  if (Button1.isReleased() && (Clock.GET_MINUTE () >= 59)) {
    Clock.SET_MINUTE (0);
  } else if (Button1.isReleased()) {
      Clock.SET_MINUTE (Clock.GET_MINUTE () + 1);
    }
  if (Button2.isReleased() && (Clock.GET_MINUTE () == 0)) {
    Clock.SET_MINUTE (59);
  } else if (Button2.isReleased()) {
      Clock.SET_MINUTE (Clock.GET_MINUTE () - 1);
    }
  if (Button3.isReleased()) {
    lcd.clear ();
    mode = 4;
  }    
  if (Button4.isReleased ()) {
    lcd.clear ();
    mode = 3;
  }
}

void MonthSet () {

bool Century;

  lcd.setCursor (0,0);
  lcd.print ("Set Month");
  lcd.setCursor (0,1);
  if (Clock.GET_MONTH (Century) == 1)  lcd.print ("Jan ");
  if (Clock.GET_MONTH (Century) == 2)  lcd.print ("Feb ");
  if (Clock.GET_MONTH (Century) == 3)  lcd.print ("Mar ");
  if (Clock.GET_MONTH (Century) == 4)  lcd.print ("Apr ");
  if (Clock.GET_MONTH (Century) == 5)  lcd.print ("May ");
  if (Clock.GET_MONTH (Century) == 6)  lcd.print ("Jun ");
  if (Clock.GET_MONTH (Century) == 7)  lcd.print ("Jul ");
  if (Clock.GET_MONTH (Century) == 8)  lcd.print ("Aug ");
  if (Clock.GET_MONTH (Century) == 9)  lcd.print ("Sept ");
  if (Clock.GET_MONTH (Century) == 10) lcd.print ("Oct ");
  if (Clock.GET_MONTH (Century) == 11) lcd.print ("Nov ");
  if (Clock.GET_MONTH (Century) == 12) lcd.print ("Dec ");
  CheckButtons ();
  if (Button1.isReleased() && (Clock.GET_MONTH (Century) >= 12)) {
    Clock.SET_MONTH (1);
  } else if (Button1.isReleased()) {
     Clock.SET_MONTH (Clock.GET_MONTH (Century) + 1);
    }
  if (Button2.isReleased() && (Clock.GET_MONTH (Century) == 1)) {
    Clock.SET_MONTH (12);
  } else if (Button2.isReleased()) {
     Clock.SET_MONTH (Clock.GET_MONTH (Century) - 1);
    }
  if (Button3.isReleased()) {
    lcd.clear ();
    mode = 7;
  }
  if (Button4.isReleased ()) {
    lcd.clear ();
    mode = 3;
  }
}

void DaySet () {

  lcd.setCursor (0,0);
  lcd.print ("Set Day");
  lcd.setCursor (0,1);
  if (Clock.GET_DAY () < 10) {
    lcd.print (Clock.GET_DAY ());    
    lcd.print (" ");    
  } else lcd.print (Clock.GET_DAY ());
  CheckButtons ();
  if (Button1.isReleased() && (Clock.GET_DAY () >= 31)) {
    Clock.SET_DAY (1);
  } else if (Button1.isReleased()) {
      Clock.SET_DAY (Clock.GET_DAY () + 1);
    }
  if (Button2.isReleased() && (Clock.GET_DAY () == 1)) {
    Clock.SET_DAY (31);
  } else if (Button2.isReleased()) {
      Clock.SET_DAY (Clock.GET_DAY () - 1);
    }
  if (Button3.isReleased()) {
    lcd.clear ();
    mode = 8;
  }    
  if (Button4.isReleased ()) {
    lcd.clear ();
    mode = 3;
  }
}

void WeekdaySet () {

  lcd.setCursor (0,0);
  lcd.print ("Set Weekday");
  lcd.setCursor (0,1);
  if (Clock.GET_WEEKDAY () == 1) lcd.print ("Saturday ");
  if (Clock.GET_WEEKDAY () == 2) lcd.print ("Sunday   ");
  if (Clock.GET_WEEKDAY () == 3) lcd.print ("Monday   ");
  if (Clock.GET_WEEKDAY () == 4) lcd.print ("Tuesday  ");
  if (Clock.GET_WEEKDAY () == 5) lcd.print ("Wednesday");
  if (Clock.GET_WEEKDAY () == 6) lcd.print ("Thursday ");
  if (Clock.GET_WEEKDAY () == 7) lcd.print ("Friday   ");
  CheckButtons ();
  if (Button1.isReleased() && (Clock.GET_WEEKDAY () >= 7)) {
    Clock.SET_WEEKDAY (1);
  } else if (Button1.isReleased()) {
      Clock.SET_WEEKDAY (Clock.GET_WEEKDAY () + 1);
    }
  if (Button2.isReleased() && (Clock.GET_WEEKDAY () == 1)) {
    Clock.SET_WEEKDAY (7);
  } else if (Button2.isReleased()) {
      Clock.SET_WEEKDAY (Clock.GET_WEEKDAY () - 1);
    }
  if (Button3.isReleased()) {
    lcd.clear ();
    mode = 9;
  }
  if (Button4.isReleased ()) {
    lcd.clear ();
    mode = 3;
  }
}

void YearSet () {

  lcd.setCursor (0,0);
  lcd.print ("Set Year");      
  lcd.setCursor (0,1);
  if (Clock.GET_YEAR () < 10) {
    lcd.print ("200");
    lcd.print (Clock.GET_YEAR ());
  } else {lcd.print ("20"); lcd.print (Clock.GET_YEAR ());}
  CheckButtons ();
  if (Button1.isReleased() && (Clock.GET_YEAR () >= 99)) {
      Clock.SET_YEAR (0);
  } else if (Button1.isReleased()) {
      Clock.SET_YEAR (Clock.GET_YEAR () + 1);
    }
  if (Button2.isReleased() && (Clock.GET_YEAR () == 0)) {
      Clock.SET_YEAR (99);
  } else if (Button2.isReleased()) {
      Clock.SET_YEAR (Clock.GET_YEAR () - 1);
    }
  if (Button3.isReleased()) {
    lcd.clear ();
    mode = 3;
  }
  if (Button4.isReleased ()) {
    lcd.clear ();
    mode = 3;
  }
}

void SecondSet () {

  lcd.setCursor (0,0);
  lcd.print ("Second ");
  lcd.print (char(7));
  lcd.print (" to 0");
  lcd.setCursor (0,1);
  if (Clock.GET_SECOND () < 10) {
    lcd.print ("0");
    lcd.print (Clock.GET_SECOND ());
  } else {
      lcd.print (Clock.GET_SECOND ());
    }
  CheckButtons ();
  if (Button1.isReleased()) {
    Clock.SET_SECOND (0);
  }
  if (Button3.isReleased()) {
    lcd.clear ();
     mode = 5;
  }
  if (Button4.isReleased ()) {
    lcd.clear ();
    mode = 3;
  }
}

void DisplayTemperature () {

  int temp;
  float tempf;
  static char outstr[18];

  lcd.setCursor (0, 0);
  tempf = ((Clock.GET_TEMPERATURE() - 3) * 9/5) + 32;
  dtostrf(tempf,5, 2, outstr);
  lcd.print (char(3));
  lcd.print (Clock.GET_TEMPERATURE() - 3);
  lcd.print (char(6));
  lcd.print ("C ");
  lcd.print (outstr);
  lcd.print (char(6));
  lcd.print ("F");
  lcd.setCursor (2,1);
  lcd.print ("D");
  lcd.setCursor (8,1);
    lcd.print ("C");
  if (digitalRead (auger_Pin) == HIGH) {
    lcd.setCursor (0,1);
    lcd.print (char (2));
  }
  if (digitalRead (distFan_Pin) == HIGH) {
    lcd.setCursor (3,1);
    lcd.print (char (4));
    lcd.print (char (5));
  }
  if (digitalRead (combFan_Pin) == HIGH) {    
    lcd.setCursor (9,1);
    lcd.print (char (4));
    lcd.print (char (5));
  }
  if (digitalRead (auger_Pin) == LOW) {
    lcd.setCursor (0,1);
    lcd.print (" ");
  }
  if (digitalRead (distFan_Pin) == LOW) {
    lcd.setCursor (3,1);
    lcd.print ("  ");
  }
  if (digitalRead (combFan_Pin) == LOW) {
    lcd.setCursor (9,1);
    lcd.print ("  ");
  }
}

void DisplayClock () {

bool Century;
bool h12;
bool PM;

  lcd.setCursor (0,0);
  if (Clock.GET_HOUR(h12, PM) >= 1 && Clock.GET_HOUR(h12, PM) < 10) {
    lcd.setCursor (0, 0);
    lcd.print (" ");
    lcd.print (Clock.GET_HOUR(h12, PM));
  }  else if (Clock.GET_HOUR(h12, PM) >= 10 && Clock.GET_HOUR(h12, PM) <= 12) {
       lcd.setCursor (0, 0);
       lcd.print (Clock.GET_HOUR(h12, PM));
  }  else if (Clock.GET_HOUR(h12, PM) >= 13 && Clock.GET_HOUR(h12, PM) < 22) {
       lcd.setCursor (0, 0);
       lcd.print (" ");
       lcd.print (Clock.GET_HOUR(h12, PM) - 12);
  }  else if (Clock.GET_HOUR(h12, PM) >= 22 && Clock.GET_HOUR(h12, PM) < 24) {
       lcd.setCursor (0, 0);
       lcd.print (Clock.GET_HOUR(h12, PM) - 12);
  }  else if (Clock.GET_HOUR(h12, PM) == 0) {
    lcd.setCursor (0, 0);
    lcd.print ("12");
  }
  lcd.print (":");
  if (Clock.GET_MINUTE() < 10) {
    lcd.print ("0");
    lcd.print (Clock.GET_MINUTE());
  } else {
    lcd.print (Clock.GET_MINUTE());
  }
  if (Clock.GET_HOUR(h12, PM) >= 0 && Clock.GET_HOUR(h12, PM) <= 11) {
    lcd.print ("AM");
  } else if (Clock.GET_HOUR(h12, PM) >= 12 && Clock.GET_HOUR(h12, PM) < 24) {
    lcd.print ("PM");
  } 
  if (Clock.GET_MONTH(Century) == 1) lcd.print  ("   Jan ");
  if (Clock.GET_MONTH(Century) == 2) lcd.print  ("   Feb ");
  if (Clock.GET_MONTH(Century) == 3) lcd.print  ("   Mar ");
  if (Clock.GET_MONTH(Century) == 4) lcd.print  ("   Apr ");
  if (Clock.GET_MONTH(Century) == 5) lcd.print  ("   May ");
  if (Clock.GET_MONTH(Century) == 6) lcd.print  ("   Jun ");
  if (Clock.GET_MONTH(Century) == 7) lcd.print  ("   Jul ");
  if (Clock.GET_MONTH(Century) == 8) lcd.print  ("   Aug ");
  if (Clock.GET_MONTH(Century) == 9) lcd.print  ("  Sept ");
  if (Clock.GET_MONTH(Century) == 10) lcd.print ("   Oct ");
  if (Clock.GET_MONTH(Century) == 11) lcd.print ("   Nov ");
  if (Clock.GET_MONTH(Century) == 12) lcd.print ("   Dec ");
  if (Clock.GET_DAY() < 10) {
    lcd.print (" ");    
    lcd.print (Clock.GET_DAY());    
  } else lcd.print (Clock.GET_DAY());
  lcd.setCursor (0, 1);
  if (Clock.GET_WEEKDAY() == 1) lcd.print ("Saturday    ");
  if (Clock.GET_WEEKDAY() == 2) lcd.print ("Sunday      ");
  if (Clock.GET_WEEKDAY() == 3) lcd.print ("Monday      ");
  if (Clock.GET_WEEKDAY() == 4) lcd.print ("Tuesday     ");
  if (Clock.GET_WEEKDAY() == 5) lcd.print ("Wednesday   ");
  if (Clock.GET_WEEKDAY() == 6) lcd.print ("Thursday    ");
  if (Clock.GET_WEEKDAY() == 7) lcd.print ("Friday      ");
  lcd.print ("20");
  lcd.print (Clock.GET_YEAR());
}

void Startup_Text () {

  lcd.setCursor (0,0);
  lcd.print ("Welcome To Your");
  lcd.setCursor (1,1);
  lcd.print (char(2));
  lcd.setCursor (4,1);
  lcd.print ("Harmon");
  lcd.setCursor (12,1);
  lcd.print (char(2));
  delay (3000);
  lcd.clear ();
}  //END startup_text

// ***************************************** Operating Functions************************************

void StartupMode () {

int end_Minute;
int current_Minute;
bool get_Mode = true;
bool setting_Select = true;
bool temp_Select = true;
bool start_Up = true;
bool manualState;
bool prevManual;

  //****************************setup

  while (get_Mode) {
  manualState = manualMode;
  lcd.setCursor (0,0);
  lcd.print ("Set Mode");
  if (manualMode) {
    lcd.setCursor (0,1);
    lcd.print ("Manual");
  } else {
      lcd.setCursor (0,1);
      lcd.print ("Auto");
    }
  CheckButtons ();
  if (Button1.isReleased ()) {
    if((manualState != prevManual) && (manualState == HIGH)){
      // button has been pressed
      manualMode = !manualMode;
    }
    else if( manualState == LOW ){
        manualMode = HIGH;
      }
    lcd.clear();
  }
  if (Button2.isReleased ()) {
    if((manualState != prevManual) && (manualState == HIGH))
      {
        manualMode = !manualMode;
      }
      else if( manualState == LOW )
        {
          manualMode = HIGH;
        }
      lcd.clear();
    }
    if (Button3.isReleased ()) {
      get_Mode = false;
      lcd.clear ();
    }
    if (Button4.isReleased ()) {
      lcd.clear ();
      mode = 11;
    }
  }
  if (!manualMode) {
    temp_Set = temperature_Setting;
    temp_Select = true;
    setting_Select = !setting_Select;
  }
  if (manualMode) temp_Select = !temp_Select;
  
  while (setting_Select) {
    lcd.setCursor (0,0);
    lcd.print ("Select Setting");
    lcd.setCursor (0,1);
    if (setting == 1) lcd.print ("Setting 1");
    if (setting == 2) lcd.print ("Setting 2");
    if (setting == 3) lcd.print ("Setting 3");
    if (setting == 4) lcd.print ("Setting 4");
    if (setting == 5) lcd.print ("Setting 5");
    if (setting == 6) lcd.print ("Setting 6");    
    CheckButtons ();
    if ((Button1.isReleased ()) && (setting <= 5)) {
      ++setting;
      lcd.clear();
    }
    if ((Button2.isReleased ()) && (setting >= 2)) {
      --setting;
      lcd.clear();
    }
    if (Button3.isReleased ()) {
      lcd.clear ();
      setting_Select = !setting_Select;
    }
  } 
  while (temp_Select) {
    lcd.setCursor (0,0);
    lcd.print ("Set Temperature");
    lcd.setCursor (0,1);
    lcd.print (temp_Set);
    CheckButtons ();
    if (Button1.isReleased ()) {
      temp_Set = temp_Set + 1;
      lcd.clear();
    }
    if (Button2.isReleased ()) {
      temp_Set = temp_Set - 1;
      lcd.clear();
    }
    if (Button3.isReleased ()) {
      lcd.clear();
      temp_Select = !temp_Select;
    }
  }
  end_Minute = CalculateEndMintute (Clock.GET_MINUTE ());
  digitalWrite (combFan_Pin, HIGH);

  //*****************************loop
  while (start_Up) {
    lcd.setCursor (0,0);
    lcd.print ("Starting Up");
    lcd.setCursor (0,1);
    lcd.print ("Press Pgm to end");
    current_Minute = Clock.GET_MINUTE ();
    CheckButtons ();
    if (Button4.isReleased ()) {
      start_Up = false;
      lcd.clear();
      On = true;
      if (manualMode) mode = 0;
      if (!manualMode) mode = 1;
    }
    if (current_Minute == end_Minute)
    {
      start_Up = false;
      lcd.clear();
      On = true;
      if (manualMode) mode = 0;
      if (!manualMode) mode = 1;
    }
  } 
} // end statup mode

void ShutdownMode () {

bool shut_Down = true;

int end_Minute;
int current_Minute;

  end_Minute = CalculateEndMintute (Clock.GET_MINUTE ());
  lcd.setCursor (0,0);
  On = LOW;
  lcd.print ("Shutting Down");
  lcd.setCursor (0,1);
  lcd.print ("Press Pgm to end");
  digitalWrite (auger_Pin, LOW);
  digitalWrite (distFan_Pin, LOW);
  while (shut_Down) {
    current_Minute = Clock.GET_MINUTE ();
    CheckButtons ();    
    if (current_Minute == end_Minute) {
      digitalWrite (combFan_Pin, LOW);
      lcd.clear ();
      if (manualMode) mode = 0;
      if (!manualMode) mode = 1;
    }
    if (Button4.isReleased ()) {
      shut_Down = false;
      lcd.clear();
      On = false;
      digitalWrite (combFan_Pin, LOW);
      if (manualMode) mode = 0;
      if (!manualMode) mode = 1;
    }
    if (current_Minute == end_Minute)
    {
      shut_Down = false;
      lcd.clear();
      On = false;
      digitalWrite (combFan_Pin, LOW);
      if (manualMode) mode = 0;
      if (!manualMode) mode = 1;
    }
  }
}


void RunAutoMode () {

  if (temperature <= temperature_Setting) setting5 ();
  else if (temperature >= temperature_Setting) setting2 ();
}

void RunManualMode () {

  digitalWrite (combFan_Pin, HIGH);
  if (setting == 1) setting1 ();
  if (setting == 2) setting2 ();
  if (setting == 3) setting3 ();
  if (setting == 4) setting4 ();
  if (setting == 5) setting5 ();
  if (setting == 6) setting6 ();  
}

void setting1 (){
  if (Clock.GET_SECOND () >=49 && Clock.GET_SECOND () <= 59){
    digitalWrite (auger_Pin, HIGH);
    digitalWrite (distFan_Pin, HIGH);    
  }
  if (Clock.GET_SECOND () >=0 && Clock.GET_SECOND () <= 48){
    digitalWrite (auger_Pin, LOW);
    digitalWrite (distFan_Pin, LOW);    
  }    
}

void setting2 (){
  if (Clock.GET_SECOND () >=40 && Clock.GET_SECOND () <= 59){
    digitalWrite (auger_Pin, HIGH);
    digitalWrite (distFan_Pin, HIGH);    
  }
  if (Clock.GET_SECOND () >=0 && Clock.GET_SECOND () <= 39){
    digitalWrite (auger_Pin, LOW);
    digitalWrite (distFan_Pin, LOW);    
  }
}

void setting3 (){
  if (Clock.GET_SECOND () >=30 && Clock.GET_SECOND () <= 59){
    digitalWrite (auger_Pin, HIGH);
    digitalWrite (distFan_Pin, HIGH);
  }
  if (Clock.GET_SECOND () >=0 && Clock.GET_SECOND () <= 29){
    digitalWrite (auger_Pin, LOW);
    digitalWrite (distFan_Pin, LOW);    
  }
}

void setting4 (){
  if (Clock.GET_SECOND () >=20 && Clock.GET_SECOND () <= 59){
    digitalWrite (auger_Pin, HIGH);
    digitalWrite (distFan_Pin, HIGH);    
  }
  if (Clock.GET_SECOND () >=0 && Clock.GET_SECOND () <= 19){
    digitalWrite (auger_Pin, LOW);
    digitalWrite (distFan_Pin, LOW);    
  }
}

void setting5 (){
  if (Clock.GET_SECOND () >=10 && Clock.GET_SECOND () <= 59){
    digitalWrite (auger_Pin, HIGH);
    digitalWrite (distFan_Pin, HIGH);    
  }
  if (Clock.GET_SECOND () >=0 && Clock.GET_SECOND () <= 9){
    digitalWrite (auger_Pin, LOW);
    digitalWrite (distFan_Pin, LOW);    
  }
}

void setting6 (){
  if (Clock.GET_SECOND () >=2 && Clock.GET_SECOND () <= 59){
    digitalWrite (auger_Pin, HIGH);
    digitalWrite (distFan_Pin, HIGH);    
  }
  if (Clock.GET_SECOND () >=0 && Clock.GET_SECOND () <= 1){
    digitalWrite (auger_Pin, LOW);
    digitalWrite (distFan_Pin, LOW);    
  }
}

int CalculateEndMintute (int minute) {

int minuteAdd;
float remainder;
int r;
  
  if (minute >= 40) {
    minuteAdd = minute + 20;
    remainder = minuteAdd % 60;
    r = (int) remainder;
    return r;
  } else {
      minute = minute + 20;
      return minute;
    }
}

void CheckButtons () {
  Button1.update();
  Button2.update();
  Button3.update();
  Button4.update();
}

void LoadChars() {
  lcd.createChar (0, smily);    // load smiley to memory 0
  lcd.createChar (1, heart_h);  // load heart_h to memory 1
  lcd.createChar (2, heart_f);  // load heart_f to memory 2
  lcd.createChar (3, temp);     // load temp to memory 3 
  lcd.createChar (4, fan);      // load 1/2 fan to memory 4 
  lcd.createChar (5, fan2);     // load 1/2 fan to memory 5 
  lcd.createChar (6, degree);   // load degree to memory 6 
  lcd.createChar (7, arrow_u);  // load arrow_u to memory 7
}  // END LoadChars
