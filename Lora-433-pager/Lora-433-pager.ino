/*
 * This Code is modded by Jim Park (jim(^dOt^)buzz(^aT^)gmail(^dOt^)com )from the original 
 * http://playground.arduino.cc/Code/PCD8544
*/


#include <avr/sleep.h>

#define PIN_CE    10  // LCD CS  .... 
#define PIN_RST   9   // LCD RST .... 
#define PIN_DC    11  // LCD Dat/Com. 
#define PIN_DIN   12  // LCD SPIDat . 
#define PIN_CLK   13  // LCD SPIClk . 
#define PIN_BG    8   // LCD Background LED
                      // LCD Gnd .... 
                      // LCD Vcc .... 

#define PIN_BUR   17   // the right Button
#define PIN_BUL   15   // the left Button
#define PIN_BEP   6   // the beeper

#define LCD_C     LOW
#define LCD_D     HIGH

#define LCD_X     84
#define LCD_Y     48
#define LCD_CMD   0

#define MAXMSG 10

int wakePin = 2;                 // pin used for waking up
int sleepStatus = 0;             // variable to store a request for sleep
int count = 0;                   // counter

int a = 0;
int buttonMode = 0;
int scrollPosition = -10;
int beeped = 0;
char m[256];  //message char buffer 
bool scroll = false;
bool newMessage = false;
int mcounter=0; //message counter
int totalcounter=0; //message counter
int dispcounter=0;  //displayed massage
int old_dispcounter=-1;
String messagequeue[MAXMSG];//holds at max 10 messages

static const byte ASCII[][5] =
{
 {0x00, 0x00, 0x00, 0x00, 0x00} // 20  
,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c ¥
,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j 
,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ←
,{0x00, 0x06, 0x09, 0x09, 0x06} // 7f →
};

void beep(unsigned char delayms){
  analogWrite(PIN_BEP, 20); // Almost any value can be used except 0 and 255
  delay(delayms);           // wait for a delayms ms
  analogWrite(PIN_BEP, 0);  // 0 turns it off
  delay(delayms);           // wait for a delayms ms
} 

void LcdCharacter(char character){
  LcdWrite(LCD_D, 0x00);
  for (int index = 0; index < 5; index++){
    LcdWrite(LCD_D, ASCII[character - 0x20][index]);
  }
  LcdWrite(LCD_D, 0x00);
}

void LcdClear(void){
  for (int index = 0; index < LCD_X * LCD_Y / 8; index++){
    LcdWrite(LCD_D, 0x00);
  }
}

void LcdInitialise(void){
  pinMode(PIN_CE,   OUTPUT);
  pinMode(PIN_RST,  OUTPUT);
  pinMode(PIN_DC,   OUTPUT);
  pinMode(PIN_DIN,  OUTPUT);
  pinMode(PIN_CLK,  OUTPUT);
  pinMode(PIN_BG,   OUTPUT);

//Restart the display
  digitalWrite(PIN_RST, LOW);
  delay(1);
  digitalWrite(PIN_RST, HIGH);

  LcdWrite( LCD_CMD, 0x21 );   // LCD Extended Commands.
  LcdWrite( LCD_CMD, 0xBf );   // Set LCD Vop (Contrast). //B1
  LcdWrite( LCD_CMD, 0x04 );   // Set Temp coefficent. //0x04
  LcdWrite( LCD_CMD, 0x14 );   // LCD bias mode 1:48. //0x13
  //LcdWrite( LCD_CMD, 0x0C ); // LCD in normal mode. 0x0d for inverse
  LcdWrite( LCD_CMD, 0x0d );   // LCD in inverse mode. 0x0C for normal
  LcdWrite( LCD_C, 0x20);
  LcdWrite( LCD_C, 0x0C);
}

void LcdString(char *characters){
  while (*characters)  {
    LcdCharacter(*characters++);
  }
}

void LcdWrite(byte dc, byte data){
  digitalWrite(PIN_DC, dc);
  digitalWrite(PIN_CE, LOW);
  shiftOut(PIN_DIN, PIN_CLK, MSBFIRST, data);
  digitalWrite(PIN_CE, HIGH);
}

// gotoXY routine to position cursor 
// x - range: 0 to 84
// y - range: 0 to 5

void gotoXY(int x, int y){
  LcdWrite( 0, 0x80 | x);  // Column.
  LcdWrite( 0, 0x40 | y);  // Row.  
}

void drawBox(void){
  unsigned char  j;  
   for(j=0; j<84; j++){
    gotoXY (j,0);       // top
    LcdWrite (1,0x01);
    gotoXY (j,5);       //Bottom
    LcdWrite (1,0x80);
  }

  for(j=0; j<6; j++){
    gotoXY (83,j);    // Right
    LcdWrite (1,0xff);
    gotoXY (0,j);     // Left
    LcdWrite (1,0xff);
  }
}

void Scroll(String message){
  for (int i = scrollPosition; i < scrollPosition + 48; i++){ //+61 was +11
    if ((i >= message.length()) || (i < 0))
      LcdCharacter(' ');
    else
      LcdCharacter(message.charAt(i));
  }
  scrollPosition++;
  if ((scrollPosition >= message.length()) && (scrollPosition > 0))
    scrollPosition = -10;
}

void read_serial(){
  if (Serial.available()) {
    String message="";

    while(Serial.available()){
      message+=(char)Serial.read();
    }
    Serial.print("received from Computer:");
    Serial.println(message);  
    newMessage = true;
    //put the new message in the queue
    messagequeue[mcounter]=message;
    mcounter=(mcounter+1)%MAXMSG;
    totalcounter++;
  }

  if (Serial1.available()) {
    //read serial as a character
    //NOTE because the serial is read as "char" and not "int", the read value must be compared to character numbers
    //hence the quotes around the numbers in the case statement
    String message="";
    
    while(Serial1.available()){
      message+=(char)Serial1.read();
    }
    Serial.print("received from LORA:");
    Serial.println(message);
    //Serial1.print("received from you:");
    //Serial1.println(message);
    newMessage = true;
    //put the new message in the queue
    messagequeue[mcounter]=message;
    mcounter=(mcounter+1)%MAXMSG;
    totalcounter++;
  }
}

void reset(){
  buttonMode = 0;
  scrollPosition = -10;
  beeped = 0;
  scroll = false;
  newMessage = false;
  mcounter=0; //message counter
  totalcounter=0; //message counter
  dispcounter=0;  //displayed massage
  old_dispcounter=-1;
  digitalWrite(PIN_BG, LOW);
  for(int i=0; i<MAXMSG; i++)
    messagequeue[i]="";
}

void switch_light(){
  bool l = digitalRead(PIN_BG);
  digitalWrite(PIN_BG, !l);
}

void wakeUpNow(){        // here the interrupt is handled after wakeup
  // execute code here after wake-up before returning to the loop() function
  // timers and code using timers (serial.print and more...) will not work here.
  // don't really need to execute any special functions here,
  // just want the thing to wake up
}


void sleepNow(){      // put the arduino to sleep
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
    sleep_enable();          // enables the sleep bit in the mcucr register
                             // so sleep is possible. just a safety pin
 
    /* enable an interrupt. 
     * In the function call attachInterrupt(A, B, C)
     * A   can be either 0 or 1 for interrupts on pin 2 or 3.  
     * B   Name of a function you want to execute at interrupt for A.
     * C   Trigger mode of the interrupt pin. can be:
     *             LOW        a low level triggers
     *             CHANGE     a change in level triggers
     *             RISING     a rising edge of a level triggers
     *             FALLING    a falling edge of a level triggers
     * In all but the IDLE sleep modes only LOW can be used.
     */
    attachInterrupt(0,wakeUpNow, LOW); // use interrupt 0 (pin 2) and run function, wakeUpNow when pin 2 gets LOW
    sleep_mode();            // here the device is actually put to sleep, THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
    sleep_disable();         // first thing after waking from sleep: disable sleep...
    detachInterrupt(0);      // disables interrupt 0 on pin 2 so the wakeUpNow code will not be executed during normal running time.
}

void home_screen(){
    if(dispcounter!=old_dispcounter){
      old_dispcounter=dispcounter;

      LcdClear();

      if(totalcounter>0){
        char id[2];
        String tid = String(dispcounter);
        tid.toCharArray(id,2);
        gotoXY(0,0);
        LcdString("m id: ");
        gotoXY(35,0);
        LcdString(id);
      }else{
        gotoXY(5,0);
        LcdString("no messages");
      }
      
      //if message is shorter than x characters -> display, else scroll
      messagequeue[dispcounter].toCharArray(m,256);
      if(messagequeue[dispcounter].length()>48){
        scroll = true;
      }else{
        gotoXY(0,1);
        LcdString(m);
        scroll = false;
      }
      gotoXY(0,5);//was 24,3
      LcdString("bg/rst");

      gotoXY(55,5);//was 24,3
      LcdString("next");

      newMessage=false;
      buttonMode = 1;
      beeped=0;
  }
}

void newMessage_screen(){
    if(mcounter>0)
      dispcounter=mcounter-1;
    else
      dispcounter=0;

    if(beeped<4){
      beep(50);
      beeped++;
    }
    
    LcdClear();
    gotoXY(5,2);
    if(a==0)
      LcdString("new message");
    else
      LcdString("NEW MESSAGE");

    gotoXY(0,5);
    LcdString("bg/rst"); // the left button should be there
    gotoXY(55,5);
    LcdString("read"); // the right button should be there
    scroll = false;
    buttonMode=0;
}

void setup(void){
  //Background light off
  digitalWrite(PIN_BG, LOW);
  LcdInitialise();
  LcdClear();

  Serial.begin(115200); // communicate with PC
  Serial1.begin(9600);  // communicate with lora DRF1278DM
  //while (!Serial) { ; } //wait for serial to appear if connected to PC
  Serial.println("All your base are belong to us");

  //the right button
  pinMode(PIN_BUR, INPUT_PULLUP);
  //the left button
  pinMode(PIN_BUL, INPUT_PULLUP);

/*now save power somehow...*/
  pinMode(wakePin, INPUT);
  attachInterrupt(0, wakeUpNow, LOW); // use interrupt 0 (pin 2) and run function wakeUpNow when pin 2 gets LOW
/*now save power somehow...*/
}

void loop(void){
  //to count how long the arduino was running
  count++;
//  if(count > 40) //sleep after 10 seconds
//    sleepNow(); 
  
  // is there something new ?
  read_serial();
  //flicker variable
  a=(a+1)%2;

  if (digitalRead(PIN_BUL) == LOW) {
     count=0;
     int button_delay=0;
     while (digitalRead(PIN_BUL) == LOW){
      button_delay++;
      delay(100);
      //Serial.println("holding");
      //Serial.println(button_delay);
    }
    if(button_delay<10)
      switch_light();
    else
      reset();
  }

  if (digitalRead(PIN_BUR) == LOW) {
     count=0;
     switch(buttonMode){
      case 0: newMessage=false; old_dispcounter=-1; break; // read
      case 1: if(totalcounter>10)
                dispcounter=(dispcounter+1)%MAXMSG; 
              else if(mcounter>0){
                dispcounter=(dispcounter+1)%mcounter; 
              }else{
                dispcounter=0;
              }
              break; //prev
      default: //nothing
      break;
     }
  }

//a new message has arrived
  if(newMessage){
    newMessage_screen();
  }

  if(scroll){
    gotoXY(0,1);
    Scroll(m);
  }

//a new message has arrived and the button was pressed
  if(!newMessage){
    home_screen();
  }

  delay(250);
}
