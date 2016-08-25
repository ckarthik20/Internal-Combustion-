#include <LiquidCrystal.h> //import lcd library
#include <Keypad.h> //import keypad library
int relayPin=22;
int DAQ=33;
boolean Cycle = false;                           
unsigned long PulseStartTime; 
unsigned long PulseEndTime;    
unsigned long PulseTime;   
unsigned long timer1; 
unsigned long timer2; 
unsigned long timer;  
unsigned long RPM = 0;  
LiquidCrystal lcd(8,9,10,11,12,13); //lcd pins
const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns
//define the keymap
char keys [ROWS] [COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {0,1,2,3}; //connect to row pinouts 
byte colPins[COLS] = {4,5,6,7}; //connect to column pinouts
//create the keypad
Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
char currentValue[4];
//variables declaration
boolean valOnePresent = false;
boolean next = false;
boolean final = false;
String num1, num2,num3,num4;
int initialdelay,timelimit;
int ans,rpm1;
char op;
int time1,mech,start,sdelay,speed1,speedsignal,actua,basis;
int count=0;
void(*resetFunc) (void) = 0; 
void setup()
{
  pinMode(relayPin,OUTPUT);
  pinMode(DAQ,OUTPUT);
  lcd.begin(20,4);
  lcd.setCursor(2,2);
  lcd.print("Welcome");
  delay(2500);
  lcd.clear();
   lcd.setCursor(2,2);
  lcd.print("Time Limit:");
 lcd.setCursor(17,3);
            lcd.print("sec");
             currentValue[0]='0';
 currentValue[1]='0';
 currentValue[2]='0';
  showEnteredTime();
}

void loop()
{ char tempVal[3];
    char key = myKeypad.getKey();
    if (key != NO_KEY && (key=='1'||key=='2'||key=='3'||key=='4'||key=='5'||key=='6'||key=='7'||key=='8'||key=='9'||key=='0'))
      {
          if (valOnePresent != true)
          {
            
           currentValue[0] = currentValue[1];
 currentValue[1] = currentValue[2];
 currentValue[2] = key;
 showEnteredTime();
 final=true;
               /*num1 = num1 + key;
              int numLength = num1.length();
              lcd.setCursor(14, 3); 
              lcd.print(num1);
              final=true;*/
          }
     
      }

    else if (final == true && key != NO_KEY && key == 'D')
     {
       ans= atol(currentValue);
        if(count==0)
         {
           time1=ans;
           count++;
           lcd.clear();
           lcd.print("Speed Limit:");
            lcd.setCursor(17,3);
            lcd.print("rpm");
 currentValue[0]='0';
 currentValue[1]='0';
 currentValue[2]='0';
  showEnteredTime();
          }
        else if(count==1)
          {
            speed1=ans;
            count++;
            lcd.clear();
            lcd.print("Actuator:");
            lcd.setCursor(0,1);
            lcd.print("1:Solenoid");
            lcd.setCursor(0,2);
            lcd.print("2:Stepper Motor");
            lcd.setCursor(15,2);
             currentValue[0]='0';
 currentValue[1]='0';
 currentValue[2]='0';
showEnteredValue();
          } 
        else if(count==2)
          {   count++;
            mech=ans;
            if(mech==1)
             {
               num2="Solenoid";
             }
            else if(mech==2)
              {
                num2="Setpper Motor";
                lcd.clear();
               
              }
         
 
          lcd.clear();
            lcd.print("Encoder Signal");
            lcd.setCursor(0,1);
            lcd.print("1:Analog");
            lcd.setCursor(0,2);
            lcd.print("2:Digital");
            lcd.setCursor(15,2);
            currentValue[0]='0';
 currentValue[1]='0';
 currentValue[2]='0';
showEnteredValue();
             
          }  
        else if(count==3)
          { count++;
             speedsignal=ans;
             if(speedsignal==1)
             {
               num3="Analog Signal";
             }
            else if(speedsignal==2)
              {
                num3="Digital Signal";
               
              }
              lcd.clear();
          lcd.print("Actuator Contition");
            lcd.setCursor(0,1);
            lcd.print("1:Cycle Basis");
            lcd.setCursor(0,2);
            lcd.print("2:Time Basis");
            lcd.setCursor(15,2);
            currentValue[0]='0';
            currentValue[1]='0';
            currentValue[2]='0';
            showEnteredValue();
             }
        else  if(count==4)
          {
           count++;
             basis=ans;
             if(basis==1)
             {
               num4="Cycle Basis";
             }
            else if(basis==2)
              {
                num4="Time Basis";
              
              }
              lcd.clear();
            lcd.print("Initial Delay:");
          lcd.setCursor(17,3);
            lcd.print("sec");
            currentValue[0]='0';
 currentValue[1]='0';
 currentValue[2]='0';
 showEnteredTime();
          }
        else  if(count==5)
          {
            count++;
            sdelay=ans;            
          }
          
        else if(count==6)
          {
              count++;
            lcd.clear();
            delay(1000);
            lcd.print("Starter Motor");
             lcd.setCursor(0,1);
            lcd.print("Time Limit:"); 
            lcd.setCursor(0,2);
            lcd.print(time1);
            lcd.print("sec");
            delay(1000);
            lcd.clear();
              lcd.print("Speed Limit");
            lcd.setCursor(0,1);
            lcd.print(speed1);
             lcd.print("rpm");
            delay(1000); 
            lcd.clear();
            lcd.print("Actuator Used:");
            lcd.setCursor(0,1);
            lcd.print(num2);
            delay(1000);
            lcd.clear();
              lcd.print("Encoder Signal Read");
            lcd.setCursor(0,1);
            lcd.print(num3);
            delay(1000);
            lcd.clear();
            lcd.print("Actuator Condition");
            lcd.setCursor(0,1);
            lcd.print(num4);
            delay(1000);
            lcd.clear();
            lcd.print("Do you want to");
            lcd.setCursor(0,1);
            lcd.print( "Countinue?");
          }
          else if(count==7)
          {  
            lcd.clear();           
            lcd.print("Processing..");
            count++;
            initialdelay=sdelay*1000;
            timelimit=time1*1000;
          timer1=millis();
            delay(initialdelay);
            timer2=millis();
            timer=(timer2-timer1)-initialdelay;
            attachInterrupt(3, RPMPulse, RISING);
            digitalWrite(DAQ,HIGH);
            digitalWrite(relayPin,HIGH);
             delay(timelimit);
             digitalWrite(relayPin,LOW);
          }
     }
    else if (key != NO_KEY && key == 'C')
     {
        valOnePresent = false;
        final = false;
         currentValue[0]='0';
 currentValue[1]='0';
 currentValue[2]='0';
 showEnteredTime();
        ans = 0;
        op = ' ';
      }
     
    else if (key != NO_KEY && key == 'A')
     {
         digitalWrite(relayPin,LOW);
            digitalWrite(DAQ,LOW);
       resetFunc();
     }
  
 
}
void RPMPulse()
{
  if (Cycle == false)               
  {
    PulseStartTime = millis();  
    Cycle=true;           
    return;                     
  }
  if (Cycle == true)       
  {
    detachInterrupt(3);       
    PulseEndTime = millis();    
    Cycle = false;               
    calcRPM();                
  }
}

void calcRPM()
{
  PulseTime = PulseEndTime - PulseStartTime; 
    lcd.setCursor(0,0); 
  lcd.print("PulseTime =");               
  lcd.print(PulseTime);                  
                
  RPM = 60000/PulseTime;             
  attachInterrupt(3, RPMPulse, RISING);
  if(RPM>long(speed1)||timer>timelimit)
{
  
 digitalWrite(22,LOW);
}
loop1();  
}

void loop1()
{ 
 lcd.setCursor(0,1);
lcd.print("RPM = ");  
 lcd.print(float(RPM)); 
delay(1000); 

}

void showEnteredTime()
{
 lcd.setCursor(14,3);
 lcd.print(currentValue[0]);
 lcd.print(currentValue[1]);
 lcd.print(":");
 lcd.print(currentValue[2]);
 lcd.print(currentValue[3]);
}
void showEnteredValue()
{
 lcd.setCursor(14,3);
 lcd.print(currentValue[0]);
}
