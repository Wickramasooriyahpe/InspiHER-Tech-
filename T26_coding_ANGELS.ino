/*
* IEEE
*************************************************
Author: Hasini Punsara E.Wickramasooriya
        A.P Tashini Hansika Mendis
        Samathi Kisalka Sapumana
                
First written: 14/09/2021
Last updated: 20/09/2021
Board: Arduino Mega 2560
Platform: Arduino IDE
PURPOSE:
Smart door lock system with sanitizer despenser unit.
************************************************/

#include <LiquidCrystal.h>
#include <Servo.h>
#include <Keypad.h>


//-------servo motors--------------- 
Servo myservo;
Servo myservo2; //sanitizer unit
Servo myservo3;
int pos= 0; // position of servo motor

//--------LCD------------------------
LiquidCrystal lcd(24,25,26,27 ,28, 29);

//------------keypad-----------------
const byte rows=4;
const byte cols=4;
char key[rows][cols]={
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};
byte rowPins[rows]={3,2,1,0};
byte colPins[cols]={5,6,7};
Keypad myKeypad= Keypad(makeKeymap(key), rowPins,colPins,rows,cols);
char keypressed;

//---------ID-------------------------
int code = 001;  //#001 + *
int code2 = 002; //#002 + *
int code3 = 003; //#003 + *

//--------password---------------------
int password= 145278;
int password2 = 354691;
int password3 = 789541;

//-------ultrasonic sensor-------------
long time_duration;
long distance_to_object;

//-------variables to store id & pw----
int total1,i1,i2,i3,i4;// ID
int total2,j1,j2,j3,j4,j5,j6;//pasword
char c1,c2,c3,c4; 
char p1,p2,p3,p4,p5,p6,p7;
int state = 0; // 0-not in use
int led,led2; // led- red LDEs and led2-green LEDs

void setup() 
{  
    myservo.attach(9); //Servo motor 1  
    myservo2.attach(49); // servo motor 2
    myservo3.attach(8);//servo motor 3
    lcd.begin(16,2);//specify the width and height of the LCD
    pinMode (21 , OUTPUT); // buzzer
    pinMode(10,INPUT);//pir
    pinMode (13,OUTPUT); // trig pin
    pinMode (12 , INPUT); // echo pin
    pinMode(48,OUTPUT);// yellow LED --> indicate sanitizer
    // led for
      for(led=31;led<40;led+=2)
        {
          pinMode(led,OUTPUT);
        }
      for(led2=30;led2<39;led2+=2)
        {
          pinMode(led2,OUTPUT);
        }  
}

void loop() {
    if (state == 0) {displayscreen();}
   // ultrasonicSensor();
    int y=0;
    //killSwitch(); 
          keypressed = myKeypad.getKey();  //The getKey fucntion keeps the program runing, as long you didn't press "*" the whole thing bellow wouldn't be triggered???
          
          if (keypressed == '#')             // and you can use the rest of you're code simply
             {
                state = 1 ;
                enterID();               
                if ((total1 == code) || (total1 == code2) || (total1 == code3) ) //if the code is correct you trigger whatever you want here it just print a message on the screen
                   {   
                      startPassword();   
                      enterPassword();
                      if (((total2 == password ) && (total1 == code)) || ((total2 == password2 ) && (total1 == code2)) || ((total2 == password3 ) && (total1 == code3)))
                      {
                        
                        doorUnlock();
                      } 
                      else
                      {
                        incorrectPW();
                        redLED();
                        enterPWcommand();
                        delay(500);
                          int x = 0;
                          while(x<2){
                            enterPassword();
                                 if (((total2 == password ) && (total1 == code)) || ((total2 == password2 ) && (total1 == code2)) || ((total2 == password3 ) && (total1 == code3)))
                                    {
                                      doorUnlock();
                                      x=2;
                                    } 
                                    else
                                     {
                                      
                                      incorrectPW();
                                      redLED();
                                      enterPWcommand();
                                      delay(500);
                                                                                   
                                     }                                    
                              x++;                            
                            }
                           
                            if(x == 2)
                            {
                              accesDenied();
                            }                              
                       }                               
                   }
          else // Work if the entered id is wrong
                 {
                  incorrectID(); 
                  redLED();
                  delay(100);
                  while(y<2){
                    keypressed = myKeypad.getKey();
                    if (keypressed == '#'){
                    enterID();
                    if ((total1 == code) || (total1 == code2) || (total1 == code3))
                                     {
                                     startPassword();
                                     enterPassword();
                                     if (((total2 == password ) && (total1 == code)) || ((total2 == password2 ) && (total1 == code2)) || ((total2 == password3 ) && (total1 == code3)))
                                      {
                                      doorUnlock();
                                      y = 2;
                                      }
                                    // }//-----??? 
                                     else //-------------------change------------------------
                      {
                        incorrectPW();
                        redLED();
                        enterPWcommand();
                        delay(500);
                          int x = 0;
                          while(x<2){
                            enterPassword();
                                 if (((total2 == password ) && (total1 == code)) || ((total2 == password2 ) && (total1 == code2)) || ((total2 == password3 ) && (total1 == code3)))
                                    {
                                      doorUnlock();
                                      x=2;
                                    } 
                                    else
                                     {
                                      
                                      incorrectPW();
                                      redLED();
                                      enterPWcommand();
                                      delay(500);
                                                                                   
                                     }                                    
                              x++;                            
                            }
                           
                            if(x == 2)
                            {
                              accesDenied();
                            }                              
                       }                            
                       // y++ was here                
                    } //????????????????????????????????????
                    else
                                     {
                                      incorrectID();
                                      redLED();
                                      delay(500);
                                                                                   
                                     }  
                                      y++; 
                   if(y == 2)
                      {
                        accesDenied();
                             
                      }    
                   }         
                 } 
                               
             }
             } //?????????
            
      killSwitch();                             
}

//----------------Display Function ------------------------
void displayscreen()
{ 
  
lcd.setCursor(0,0);
lcd.println(" Welcome!.Enter ");
lcd.setCursor(1 ,1); 
lcd.println(" ID and Press * ");

}

//--------------Function Password--------------------------
void enterPassword(){
          
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Password");
                    keypressed = myKeypad.waitForKey();  // here all programs are stopped until you enter the four digits then it gets compared to the code above
                    if (keypressed != NO_KEY)
                      {
                       p1 = keypressed;
                       lcd.setCursor(0, 1);
                       lcd.print("*");
                       }
                    keypressed = myKeypad.waitForKey();
                    if (keypressed != NO_KEY)
                      {
                       p2 = keypressed;
                       lcd.setCursor(1, 1);
                       lcd.print("*");
                       }
                     keypressed = myKeypad.waitForKey();
                      if (keypressed != NO_KEY)
                      {
                       p3 = keypressed;
                       lcd.setCursor(2, 1);
                       lcd.print("*");
                       }
                       keypressed = myKeypad.waitForKey();
                       if (keypressed != NO_KEY)
                      {
                       p4 = keypressed;
                       lcd.setCursor(3, 1);
                       lcd.print("*");
                       }
                       keypressed = myKeypad.waitForKey();
                    if (keypressed != NO_KEY)
                      {
                       p5 = keypressed;
                       lcd.setCursor(4, 1);
                       lcd.print("*");
                       }
                       keypressed = myKeypad.waitForKey();
                    if (keypressed != NO_KEY)
                      {
                       p6 = keypressed;
                       lcd.setCursor(5, 1);
                       lcd.print("*");
                       }
                      keypressed = myKeypad.waitForKey();
                   if (keypressed =='*')
                      {
                       p7 = keypressed;
                       lcd.setCursor(6, 1);
                       lcd.print("*");
                       
                       j1=(p1-48)*100000;        //the keys pressed are stored into chars I convert them to int then i did some multiplication to get the code as an int of xxxx ???
                       j2=(p2-48)*10000;
                       j3=(p3-48)*1000;
                       j4=(p4-48)*100;
                       j5=(p5-48)*10;
                       j6=(p6-48)*1;
                       total2=j1+j2+j3+j4+j5+j6;
                       
                      }
  }
 //------------function to enter ID--------------------------- 
void enterID(){
    lcd.clear();
               lcd.setCursor(0, 0);
               lcd.print("Enter ID");                  //when the "*" key is pressed you can enter the passcode
                    keypressed = myKeypad.waitForKey();// here all programs are stopped until you enter the four digits then it gets compared to the code above
                    if (keypressed != NO_KEY)
                      {
                      
                       lcd.setCursor(0, 1);
                       lcd.print("*");
                       delay(5);
                       }
                    if (keypressed != NO_KEY)
                      {
                       c1 = keypressed;
                       lcd.setCursor(1, 1);
                       lcd.print("*");
                       }
                    keypressed = myKeypad.waitForKey();
                    if (keypressed != NO_KEY)
                      {
                       c2 = keypressed;
                       lcd.setCursor(2, 1);
                       lcd.print("*");
                       }
                     keypressed = myKeypad.waitForKey();
                   if (keypressed != NO_KEY)
                      {
                       c3 = keypressed;
                       lcd.setCursor(3, 1);
                       lcd.print("*");
                       }
                      keypressed = myKeypad.waitForKey();                  
                   if (keypressed =='*')
                      {
                       c4 = keypressed;
                       lcd.setCursor(4, 1);
                       lcd.print("*");
                       
                       i1=(c1-48)*100;        //the keys pressed are stored into chars I convert them to int then i did some multiplication to get the code as an int of xxxx
                       i2=(c2-48)*10;
                       i3=(c3-48)*1;
                       total1=i1+i2+i3;
                      }
        
  }

  //---------for wrong/incorrect ID ----------------
  void incorrectID(){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Wrong ID !!!");          
          delay(500);
          lcd.clear();
          lcd.print("Enter ID");
          lcd.setCursor(0, 1);
          lcd.print("Again !!");
    }

    //----------function for wrong password-----------

  void incorrectPW(){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Wrong passowrd !!!");          
          delay(500);
          
    } 
  //---------function for wrong ID and password--------
  void accesDenied(){
         redLED();
         buzzerOn();
         lcd.clear();
         countDown();
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("Acces Denied");          
         lcd.setCursor(0, 1);
         lcd.print("Try Again !!");
         state = 0 ;
    }  
  //--------Enter pw function2------------------
  void enterPWcommand(){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Enter password");
          lcd.setCursor(0, 1);
          lcd.print("Again !!");
    } 
  //------------Function for count down----------
 
 void countDown(){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("After countdown");
          delay(500);
          lcd.clear();
           for(int k=30;k>0;k--){
                lcd.println("Wait for 30s");
                lcd.setCursor(4,1);
                lcd.print(k);
                delay(100);
                lcd.clear();
      }
    }
 //------------function to buzzer beep ---------------

void buzzerOn(){
  for(int g=0; g<5 ;g++){
              digitalWrite (21 , HIGH);
              delay (50);
              digitalWrite (21 , LOW);
              delay (50);
                 }
  }   
 //**************** LOCK********************************************
  //-------- Starting servo clockwise (for door unlock)--------------
  void servoClockwiseLock(){    
          for(pos = 90; pos>=0; pos-=5) // open the door
                {
                myservo.write(pos); 
                delay(5); 
                }
       }

  //---------Starting servo anti-clockwise (for door lock)----------  
 void servoAntiClockwiseLock(){
          for(pos = -90 ; pos <= 90; pos +=5) // close the door
                { // in steps of 1 degree
                     myservo.write(pos); 
                     delay(15); 
                }
 }
 //********************** DOOR ***********************
  //-------- Starting servo clockwise (for the door opening)--------------
  void servoClockwiseDoor(){    
          for(pos = 90; pos>=0; pos-=5) // open the door
                {
                myservo3.write(pos); 
                delay(5); 
                }
       }

  //---------Starting servo anti-clockwise(for the door closing) ----------  
 void servoAntiClockwiseDoor(){
          for(pos = -90 ; pos <= 90; pos +=5) // close the door
                { // in steps of 1 degree
                     myservo3.write(pos); 
                     delay(15); 
                }
 }
 //-------Function for PIR sensor-------------------
 void pirSensor() {
 
    
    while(1)
      {
        int value = digitalRead(10);
        if(value == 1){
          servoAntiClockwiseDoor();
          servoAntiClockwiseLock();
        
        break;
       }
      }
 } 

 //------------Function for Kill switch---------------

  void killSwitch(){
    
    int  buttonvalue = digitalRead(11);
          if( buttonvalue == 1) 
            {
               servoClockwiseLock();
               servoClockwiseDoor();
               delay(800);
               servoAntiClockwiseDoor();
               servoAntiClockwiseLock();
               
              
            }                     
      
    }
  //--------------Function for red LED----------------------
void redLED(){
    //Go Foward  
    for(led=31;led<40;led+=2){
    digitalWrite(led,HIGH);
    delay(50);
    digitalWrite(led,LOW);
    }
    //Go Backword
   for(led=39;led>30;led-=2){
    digitalWrite(led,HIGH);
    delay(50);
    digitalWrite(led,LOW);
    } 
    
  }

//--------------Function for Green LED----------------------
void greenLED(){
     //Go Foward  
    for(led2=30;led2<39;led2+=2){
    digitalWrite(led2,HIGH);
    delay(50);
    digitalWrite(led2,LOW);
    }
    //Go Backword
   for(led2=38;led2>29;led2-=2){
    digitalWrite(led2,HIGH);
    delay(50);
    digitalWrite(led2,LOW);
    } 
    
  }
 //------------sanitizer unit servo Motor------------------    

  void sanitizerServo(){    
          for(pos = 90; pos>=0; pos-=5) 
                {
                myservo2.write(pos); 
                delay(5); 
                }
            for(pos = -90 ; pos <= 90; pos +=5) 
                { 
                 myservo2.write(pos); 
                 delay(15); 
                }    
       }

 //---------function for ultrasonic distance detect----------
  void Ultrasonic( long d){

  if (d < 20){
  
             
            for(int t =0;t <4;t++){
            digitalWrite(48,HIGH);
            delay(50);
            digitalWrite(48,LOW);
              } 
            sanitizerServo(); 
        
    }
  
 }

 //--------ultrasonic triger function-----------------------

 void ultrasonicSensor(){
          digitalWrite (13 , LOW);
          delayMicroseconds (2);
          
          digitalWrite (13 , HIGH);
          delayMicroseconds (10);
          
          digitalWrite (13 , LOW);
        
          time_duration = pulseIn (12 ,HIGH);
          distance_to_object =  (time_duration*0.034)/2;
        
          Serial.print ("Distance: ");
          Serial.println ( distance_to_object);
          delayMicroseconds (10);
          Ultrasonic(distance_to_object);
  
  }
 //---------door unlock full function----------------------
 void doorUnlock(){
            greenLED();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Use Sanitizer");
            delay(1000);
            ultrasonicSensor();
            lcd.clear();
            lcd.setCursor(0, 0);                                             
            lcd.print("Unlocked");
            servoClockwiseLock();
            servoClockwiseDoor();
            pirSensor();
            state = 0;
  } 
   //------------Start password function-----------
  void startPassword(){
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Enter Password");
                      lcd.setCursor(0, 1);
                      lcd.print("and press *");
                      delay(200);  
    }  
