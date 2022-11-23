#include "PS2X_lib.h"  //for v1.6
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo1,servo2;
PS2X ps2x; // create PS2 Controller Class

//Depan
#define FL_ENA 11
#define FR_ENB 10
#define F_IN1 7
#define F_IN2 6
#define F_IN3 8
#define F_IN4 9
//Belakang
#define BL_ENA 2
#define BR_ENB 3
#define B_IN1 5
#define B_IN2 25
#define B_IN3 30 
#define B_IN4 33

int maxspeed = 100;

int pil=0;
int FL,FR,BL,BR;
int maxim=6;
int pos=0;
int persen;

#define detektorBaterai A0
#define resistorPlus 10000L
#define resistorGround 1000L
#define teganganReferesiAnalog 2.56
double teganganPembagi;
double teganganBaterai;
int Btr;

int error = 0; 
byte type = 0;
byte vibrate = 0;
double x,spd;
void setup(){
  //Serial.begin(115200);
  servo1.attach(13);
  analogReference(INTERNAL2V56);
  analogRead(detektorBaterai);
  //servo2.attach(12);
  lcd.begin();
  
  error = ps2x.config_gamepad(52,51,53,50, false, false);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

  if(error == 0){
    Serial.println("Found Controller, configured successful");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Go to www.billporter.info for updates and to report bugs.");
  }
   
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
   
  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
   
  type = ps2x.readType(); 
    
  switch(type) {
    case 0:
      Serial.println("Unknown Controller type");
      break;
    case 1:
      Serial.println("DualShock Controller Found");
      break;
    case 2:
      Serial.println("GuitarHero Controller Found");
      break;
    default:
      break;
  }
}

void loop(){   
    ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed

  
    servo1.write(90);
    //servo2.write(90);
    
//    if(ps2x.Analog(PSS_LY)>128 && ps2x.Analog(PSS_RX)>128) // Maju Kanan
//    {
//        spd=map(ps2x.Analog(PSS_LY),0,255,-maxspeed,maxspeed);
//        x = map(ps2x.Analog(PSS_RX),0,255,-maxspeed,maxspeed);// RX
//        FL=spd+50;
//        FR=x;
//        BL=spd+50;
//        BR=x;
//        motor(FL,FR,BL,BR);
//    }
//    else if(ps2x.Analog(PSS_LY)>128 && ps2x.Analog(PSS_RX)<128) // Maju Kiri
//    {
//        spd=map(ps2x.Analog(PSS_LY),0,255,-maxspeed,maxspeed);
//        x = map(ps2x.Analog(PSS_RX),0,255,-maxspeed,maxspeed);// RX
//        FL=-x;
//        FR=spd+50;
//        BL=-x;
//        BR=spd+50;
//        motor(FL,FR,BL,BR);
//    }
//    else if(ps2x.Analog(PSS_LY)<128 && ps2x.Analog(PSS_RX)>128) // Mundur kanan
//    {
//        spd=map(ps2x.Analog(PSS_LY),0,255,-maxspeed,maxspeed);
//        x = map(ps2x.Analog(PSS_RX),0,255,-maxspeed,maxspeed);// RX
//        FL=spd-50;
//        FR=-x;
//        BL=spd-50;
//        BR=-x;
//        motor(FL,FR,BL,BR);
//    }
//    else if(ps2x.Analog(PSS_LY)<128 && ps2x.Analog(PSS_RX)<128) // Mundur kiri
//    {
//        spd= map(ps2x.Analog(PSS_LY),0,255,-maxspeed,maxspeed);
//        x = map(ps2x.Analog(PSS_RX),0,255,-maxspeed,maxspeed);// RX
//        FL=x;
//        FR=spd-50;
//        BL=x;
//        BR=spd-50;
//        motor(FL,FR,BL,BR);
//    }
//    else 
    if(ps2x.Analog(PSS_RX)>128){
      spd=map(ps2x.Analog(PSS_RX),0,255,-maxspeed,maxspeed);
      FL = spd+40;
      FR = -spd-100;
      BL = -spd-100;
      BR = spd+40;
      motor(FL,FR,BL,BR);    
    }
    else if(ps2x.Analog(PSS_RX)<128){
      spd=map(ps2x.Analog(PSS_RX),0,255,-maxspeed,maxspeed);
      FL = spd-40;
      FR = -spd+100;
      BL = -spd+100;
      BR = spd-40;
      motor(FL,FR,BL,BR);    
    }
    else if(ps2x.Analog(PSS_LY)>128){
      spd=map(ps2x.Analog(PSS_LY),0,255,-maxspeed,maxspeed);
      FL=spd;
      FR=spd;
      BL=spd;
      BR=spd;
      motor(FL,FR,BL,BR);    
    }
    else if(ps2x.Analog(PSS_LY)<128){
      spd=map(ps2x.Analog(PSS_LY),0,255,-maxspeed,maxspeed);
      FL=spd;
      FR=spd;
      BL=spd;
      BR=spd;
      motor(FL,FR,BL,BR);  
    } 
    else{
      FL = 0;
      FR = 0;
      BL = 0;
      BR = 0;
      motor(FL,FR,BL,BR);
    }
     
    if(ps2x.ButtonPressed(PSB_START)){
      if(pil==0){
        lcd.clear();
        pil=1;  
      }
      Serial.println("Start is being held");
    } 
    if(ps2x.Button(PSB_SELECT)){
      if(pil!=0){
        lcd.clear();
        pil=0;  
      }
      Serial.println("Select is being held");
    }
         
    if(ps2x.Button(PSB_PAD_UP)) {         //will be TRUE as long as button is pressed
      Serial.println("UP is being held");
    }
    if(ps2x.ButtonPressed(PSB_PAD_RIGHT)){
      if(pil!=0){
      lcd.clear();
        if(pil<=maxim){
         pil++; 
        }
      }
      Serial.println("Right is being held");
    }
    if(ps2x.ButtonPressed(PSB_PAD_LEFT)){
      if(pil>1){
      lcd.clear();
        pil--;
      }
      Serial.println("LEFT is being held");
    }
    if(ps2x.Button(PSB_PAD_DOWN)){
      Serial.println("DOWN is being held");
    }   
    
        if(ps2x.Button(PSB_L3))
          Serial.println("L3 pressed");
        if(ps2x.Button(PSB_R3))
          Serial.println("R3 pressed");
        if(ps2x.Button(PSB_L2))
        {
          maxspeed=70;
          Serial.println("L2 pressed"); 
        }
        if(ps2x.Button(PSB_L1)){
          maxspeed=100;
          Serial.println("L1 pressed");
        } 
        if(ps2x.Button(PSB_R2)){
          servo1.write(120);
          Serial.println("R2 pressed");
        }
        if(ps2x.Button(PSB_R1)){
          servo1.write(70);
          Serial.println("R1 pressed");          
        }
        if(ps2x.Button(PSB_L3)){
          maxspeed=120;
          Serial.println("L1 pressed");
        } 
        if(ps2x.Button(PSB_R3)){
          maxspeed=150;
          Serial.println("L1 pressed");
        }  
         
         
    if(ps2x.Button(PSB_RED)){
      Serial.println("Circle just pressed");
            servo2.write(105);
         
    }

    if(ps2x.Button(PSB_PINK)) {
      Serial.println("Square just pressed");
            servo2.write(75);
    }
                  
    if(ps2x.Button(PSB_GREEN)){
      servo2.write(70);
      Serial.println("Triangle just pressed"); 
    } 

    if(ps2x.Button(PSB_BLUE)){
      servo2.write(110);
      Serial.println("X just pressed");  
    } 
                  

    //if(ps2x.NewButtonState(PSB_BLUE))            //will be TRUE if button was JUST pressed OR released
    //     Serial.println("X just changed");    
//    Serial.print("Stick Values:");
//    Serial.print(ps2x.Analog(PSS_LY), DEC);
//    Serial.print(",");
//    Serial.print(ps2x.Analog(PSS_LX), DEC); // LX
//    Serial.print(",");
//    Serial.print(ps2x.Analog(PSS_RY), DEC); // RY
//    Serial.print(",");
//    Serial.println(ps2x.Analog(PSS_RX), DEC); // RX
//    Serial.print(",");
//    Serial.print(y); // RX
//    Serial.print(",");
//    Serial.println(x); // RX
  uint16_t rawBaterai = 0;
  for(int i=0;i<10;i++)
  {
    rawBaterai += analogRead(detektorBaterai); 
  }
  rawBaterai /= 10;
   
  teganganPembagi = teganganReferesiAnalog * rawBaterai / 1023;//Volt
  teganganBaterai = teganganPembagi * ((resistorPlus + resistorGround) / resistorGround);//Volt

  menu(pil);
 delay(50);
}

void motor(int FL, int FR,int BL,int BR) {
  if (FL < 0)
  {
    digitalWrite(F_IN2, HIGH);
    digitalWrite(F_IN1, LOW);
    analogWrite(FL_ENA, -FL);
  }
  else
  {
    digitalWrite(F_IN2, LOW);
    digitalWrite(F_IN1, HIGH);
    analogWrite(FL_ENA, FL);
  }
  if (FR < 0)
  {
    digitalWrite(F_IN3, HIGH);
    digitalWrite(F_IN4, LOW);
    analogWrite(FR_ENB, -FR);
  }
  else
  {
    digitalWrite(F_IN3, LOW);
    digitalWrite(F_IN4, HIGH);
    analogWrite(FR_ENB, FR);
  }
  if (BL < 0)
  {
    digitalWrite(B_IN2 , HIGH);
    digitalWrite(B_IN1 , LOW);
    analogWrite(BL_ENA , -BL);
  }
  else
  {
    digitalWrite(B_IN2 , LOW);
    digitalWrite(B_IN1 , HIGH);
    analogWrite(BL_ENA , BL);
  }
  if (BR < 0)
  {
    digitalWrite(B_IN3 , HIGH);
    digitalWrite(B_IN4 , LOW);
    analogWrite(BR_ENB , -BR);
  }
  else
  {
    digitalWrite(B_IN3 , LOW);
    digitalWrite(B_IN4 , HIGH);
    analogWrite(BR_ENB , BR);
  }
}

void menu(int pil){
  switch(pil){
    case 0:
      lcd.setCursor(0,0);
      lcd.print("Robot Mamobot");
      lcd.setCursor(0,1);
      lcd.print("Press Start");
      break;
    case 1:
      lcd.setCursor(0,0);
      lcd.print("Robot Menyala");
      break;
    case 2:
      lcd.setCursor(0,0);
      lcd.print("Joystick");
      lcd.setCursor(0,1);
      lcd.print(ps2x.Analog(PSS_LY));
      lcd.print(",");
      lcd.print(ps2x.Analog(PSS_LX));
      lcd.print(",");
      lcd.print(ps2x.Analog(PSS_RY));
      lcd.print(",");
      lcd.print(ps2x.Analog(PSS_RX));
      lcd.print("         ");
      break;
    case 3:
      lcd.setCursor(0,0);
      lcd.print("Maxspeed=");
      lcd.print(maxspeed);
      lcd.print("    ");
      lcd.setCursor(0,1);
      lcd.print(FL);
      lcd.print(",");
      lcd.print(FR);
      lcd.print(",");
      lcd.print(BL);
      lcd.print(",");
      lcd.print(BR);
      lcd.print("           ");
      break;
    case 4:
      if(ps2x.Button(PSB_R1)){
      //int persen=pos/180;  
      lcd.setCursor(0,0);
      lcd.print("Lifter Naik"); 
      }
      else if(ps2x.Button(PSB_R2)){
      //int persen=pos/180;
      lcd.setCursor(0,0);
      lcd.print("Lifter Turun"); 
      }
      else{
      lcd.setCursor(0,0);
      lcd.print("Lifter Diam");  
      }
      break;
      case 5:
      lcd.setCursor(0,0);
      lcd.print("Btr : ");
      lcd.print(teganganBaterai);
      Btr=((teganganBaterai-10.5)/(12.6-10.5))*100;
      lcd.print(" ");
      lcd.print(Btr);
      lcd.print("%   ");
      lcd.setCursor(0,1);
      if(teganganBaterai>12){
        lcd.print("0|||||||||||100%");
      }
      else if(teganganBaterai>11.75){
        lcd.print("0|||||||||  100%");
      }
      else if(teganganBaterai>11.5){
        lcd.print("0|||||||    100%");
      }
      else if(teganganBaterai>11.25){
        lcd.print("0|||||      100%");
      }
      else if(teganganBaterai>11){
        lcd.print("0|||        100%");
      }
      else if(teganganBaterai>10.75){
        lcd.print("0|          100%");
      }
      else{
        lcd.print("Baterai Habis");
      }
      break;
    default:
      lcd.setCursor(0,0);
      lcd.print("Menu Max");
      lcd.print("  ");
  }
}
