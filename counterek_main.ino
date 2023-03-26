#include <TM1637Display.h>
#define CLK 5
#define DIO 4
TM1637Display display(CLK, DIO);

#include <EEPROM.h>

const long goto_sleep_delay = 0.5*60*1000; //3 min //90 * 1000; //half a minute 30000
unsigned long last_pressed = 0;
unsigned long last_change = 0;
bool pressed = true;
bool reset_press = true;
const int interval = 500; 
int counter = 0;
int counter_archive; //difference between it and obsolete may be lost if power down and difference between them is lower then 100; currently increasing
int counter_archive_obsolete; //stored in eeprom currently
const int button = 2;
const int resetbuton = 3;
long eeepromcounter;

bool triggeredonce = true;

void digitalInterrupt() {  /*needed for the digital input interrupt*/ }

ISR(WDT_vect) { //DON'T FORGET THIS!  Needed for the watch dog timer.  This is called after a watch dog timer timeout - this is the interrupt function called after waking up
}

void go_to_sleep() {
  display.clear();
  last_change = millis();
  //BOD DISABLE - this must be called right before the __asm__ sleep instruction
  MCUCR |= (3 << 5); //set both BODS and BODSE at the same time
  MCUCR = (MCUCR & ~(1 << 5)) | (1 << 6); //then set the BODS bit and clear the BODSE bit at the same time
  Serial.println("went to sleep by go_to_sleep");
  delay(100);
  __asm__  __volatile__("sleep");//in line assembler to go to sleep
}

void setup() {
  display.setBrightness(5, true); //0-7(highest), true == on, false == off

  //Save Power by writing all Digital IO LOW - note that pins just need to be tied one way or another, do not damage devices!
  for (int i = 0; i < 20; i++) {
    if ( (i != button | i != resetbuton ) ) {//just because the buttons are hooked up to digital pin 2 and 3
      pinMode(i, OUTPUT);
    }
  }
  pinMode(button, INPUT_PULLUP);
  pinMode(resetbuton, INPUT_PULLUP);
  
  Serial.begin(115200);
  Serial.println("Setting up the display...");
  display.clear();
  attachInterrupt(0, digitalInterrupt, FALLING); //interrupts for waking up
  attachInterrupt(1, digitalInterrupt, FALLING);

 //Disable ADC - don't forget to flip back after waking up if using ADC in your application ADCSRA |= (1 << 7);
  ADCSRA &= ~(1 << 7);

  //ENABLE SLEEP - this enables the sleep mode
  SMCR |= (1 << 2); //power down mode
  SMCR |= 1;//enable sleep

  go_to_sleep();
}



void loop() {
  unsigned long currentMillis = millis();
  if ( currentMillis - last_change < goto_sleep_delay) {

    pressed = digitalRead(button);
    reset_press = digitalRead(resetbuton);
    if(reset_press == LOW){ //used to reset the counter to zero when button pressed;
      
      EEPROM.get(0, eeepromcounter);
      eeepromcounter = eeepromcounter + long(counter);
      EEPROM.put(0, eeepromcounter);
      delay(100);
      EEPROM.get(0, eeepromcounter);
      Serial.print("Value in EEPROM: ");Serial.println(eeepromcounter);
      delay(500);
      counter = counter - counter%10;
      display.showNumberDec(counter, false, 4, 0);
      while(counter > 0){
        counter = counter - 10;
        display.showNumberDec(counter, false, 4, 0);
        delay(10);
      }
      triggeredonce = false;
      Serial.print("counter has been resetted to: ");Serial.println(counter);
    }
    if ( !pressed && (currentMillis - last_pressed > interval) ) {
      last_change = millis();
      last_pressed = millis();
      
      if (triggeredonce == true) {
        counter = counter + 1;
        display.showNumberDec(counter, false, 4, 0); 
        triggeredonce = false;
        }
      else {triggeredonce = true;}
      
      Serial.print("Current value of counter: ");Serial.print(counter); Serial.print(" triggeredonce state: "); Serial.println(triggeredonce);

    }

  }
  else {
    Serial.print("Going to sleep... Current millis - last_change: ");Serial.print(currentMillis - last_change); Serial.print(" is compared to goto_sleep_dealy: ");Serial.println(goto_sleep_delay);
    go_to_sleep();
  }

}
