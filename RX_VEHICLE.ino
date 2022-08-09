#include <LiquidCrystal.h>
#include <TimerOne.h>

int   msec_cnt=0,cnt;

const int LEFT_IR_PIN=A0;
const int RIGHT_IR_PIN=A1;
const int OBJECT_IR_PIN=A2;

const int RF_PIN=7;

const int MOTOR1_PIN1=3;      // the number of the LED pin
const int MOTOR1_PIN2=4;      // the number of the LED pin
const int MOTOR2_PIN1=10;      // the number of the LED pin
const int MOTOR2_PIN2=11;      // the number of the LED pin


int LEFT_IR_PIN_STATE=0;
int RIGHT_IR_PIN_STATE=0;
int SW_PIN_STATE=0;

boolean OBJECT_IR_FLAG=0;
boolean OBJECT_IR_PIN_STATE=0;
boolean OB_FLG=0;
boolean LEFT_IR_FLAG=0;
boolean RIGHT_IR_FLAG=0;
boolean START_FLG=0,STOP_FLAG=0;
boolean S_flag=0,L_flag=0,F_flag=0,R_flag=0;

boolean FARWARD_FLAG=0,RIGHT_TURN_FLAG=0,LEFT_TURN_FLAG=0,BACKWARD_FLG=0;

char RX_DATA;
void READ_RIGHT_IR_SENSOR(void);
void READ_SW_SNS(void);
void READ_LEFT_IR_SENSOR(void);
void RecieveData(void);
boolean MODE_FLAG=0;
void setup(){
  
  pinMode(LEFT_IR_PIN,INPUT);
  pinMode(RIGHT_IR_PIN,INPUT);
  pinMode(OBJECT_IR_PIN,INPUT);
  pinMode(RF_PIN,INPUT);  
  
  pinMode(MOTOR1_PIN1,OUTPUT);
  pinMode(MOTOR1_PIN2,OUTPUT);
  pinMode(MOTOR2_PIN1,OUTPUT);
  pinMode(MOTOR2_PIN2,OUTPUT);
  
  pinMode(13, OUTPUT);    
  
  Timer1.initialize(1000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( timerIsr ); 
  Serial.begin(9600);
  Serial.println("Goodnight moon!");
  MODE_FLAG=0;
}


void loop() {
  
      RecieveData();
        
   if((START_FLG==0)||(OBJECT_IR_FLAG)){
         digitalWrite(MOTOR1_PIN1, LOW);
         digitalWrite(MOTOR1_PIN2, LOW);
         digitalWrite(MOTOR2_PIN1, LOW);
         digitalWrite(MOTOR2_PIN2, LOW);
   }else if(FARWARD_FLAG){
         digitalWrite(MOTOR1_PIN1, HIGH);
         digitalWrite(MOTOR1_PIN2, LOW);
         digitalWrite(MOTOR2_PIN1, HIGH);
         digitalWrite(MOTOR2_PIN2, LOW);
   }else if(RIGHT_TURN_FLAG){
         digitalWrite(MOTOR1_PIN1, HIGH);
         digitalWrite(MOTOR1_PIN2, LOW);
         digitalWrite(MOTOR2_PIN1, LOW);
         digitalWrite(MOTOR2_PIN2, HIGH);
   }else if(LEFT_TURN_FLAG){
         digitalWrite(MOTOR1_PIN1, LOW);
         digitalWrite(MOTOR1_PIN2, HIGH);
         digitalWrite(MOTOR2_PIN1, HIGH);
         digitalWrite(MOTOR2_PIN2, LOW);
  }else if(BACKWARD_FLG){
         digitalWrite(MOTOR1_PIN1, LOW);
         digitalWrite(MOTOR1_PIN2, HIGH);
         digitalWrite(MOTOR2_PIN1, LOW);
         digitalWrite(MOTOR2_PIN2, HIGH);
  }else{
         digitalWrite(MOTOR1_PIN1, LOW);
         digitalWrite(MOTOR1_PIN2, LOW);
         digitalWrite(MOTOR2_PIN1, LOW);
         digitalWrite(MOTOR2_PIN2, LOW);
  }
                  
}


void timerIsr()
{
   
    // Toggle LED
 msec_cnt++;if(msec_cnt>=500){
      if(MODE_FLAG==0){
                digitalWrite(13, digitalRead( 13 ) ^ 1 ); 
      }else{ 
                digitalWrite(13,0); 
      }     
          msec_cnt=0;
   }
    
   LEFT_IR_PIN_STATE=digitalRead(LEFT_IR_PIN);
   RIGHT_IR_PIN_STATE=digitalRead(RIGHT_IR_PIN);
   SW_PIN_STATE=digitalRead(RF_PIN);
   OBJECT_IR_PIN_STATE=digitalRead(OBJECT_IR_PIN);
   READ_OBJECT_IR_SENSOR();
  
if(MODE_FLAG==0){
   
    READ_RIGHT_IR_SENSOR();
    READ_SW_SNS();//rf sns
    READ_LEFT_IR_SENSOR();

   if((LEFT_IR_FLAG==1)&&(RIGHT_IR_FLAG==1)){
       FARWARD_FLAG=1;    RIGHT_TURN_FLAG=0;    LEFT_TURN_FLAG=0;  STOP_FLAG=0;  BACKWARD_FLG=0;
   }else if((LEFT_IR_FLAG==1)&&(RIGHT_IR_FLAG==0)){
       FARWARD_FLAG=0;    RIGHT_TURN_FLAG=1;    LEFT_TURN_FLAG=0;  STOP_FLAG=0;BACKWARD_FLG=0;
   }else if((LEFT_IR_FLAG==0)&&(RIGHT_IR_FLAG==1)){
       FARWARD_FLAG=0;    RIGHT_TURN_FLAG=0;    LEFT_TURN_FLAG=1;   STOP_FLAG=0;BACKWARD_FLG=0;
   }else{
        FARWARD_FLAG=0;    RIGHT_TURN_FLAG=0;   LEFT_TURN_FLAG=0;  BACKWARD_FLG=0;
   }   
       
}
      
}




void READ_LEFT_IR_SENSOR(void){
      
    if(LEFT_IR_PIN_STATE==LOW)
    {       
           LEFT_IR_FLAG=1;
    }else{ 
           LEFT_IR_FLAG=0;
    }
    
 }


void READ_OBJECT_IR_SENSOR(void){
      
    if(OBJECT_IR_PIN_STATE==HIGH)
    {        if(OB_FLG==0){ 
                 OBJECT_IR_FLAG=1;  OB_FLG=1;
              }
    }else{ 
           OBJECT_IR_FLAG=0; OB_FLG=0;
    }
    
 }



void READ_RIGHT_IR_SENSOR(void){
      
    if(RIGHT_IR_PIN_STATE==LOW)
    {        
           RIGHT_IR_FLAG=1;
    }else{ 
           RIGHT_IR_FLAG=0;  
    }
    
 }




void READ_SW_SNS(void){
        
    if(SW_PIN_STATE==HIGH)
    {        
         if(S_flag==0)
          { 
              START_FLG=!START_FLG;
              S_flag=1;
          }
    }else{ S_flag=0; }
    
 }


void RecieveData(void)
{ 
if(Serial.available()>0)
{
    RX_DATA = Serial.read();
   

 if(RX_DATA=='f'){
    MODE_FLAG=!MODE_FLAG; 
 }

 if(RX_DATA=='k'){
        FARWARD_FLAG=0;   BACKWARD_FLG=0;  RIGHT_TURN_FLAG=0;  LEFT_TURN_FLAG=0;  START_FLG=0; 
  }

if(MODE_FLAG){ Serial.write(RX_DATA );    Serial.println(RX_DATA);
   if(RX_DATA=='a'){
        FARWARD_FLAG=1;   BACKWARD_FLG=0;  RIGHT_TURN_FLAG=0;  LEFT_TURN_FLAG=0;  START_FLG=1;   OBJECT_IR_FLAG=0;
   }else if(RX_DATA=='b'){
        FARWARD_FLAG=0;   BACKWARD_FLG=1;  RIGHT_TURN_FLAG=0;  LEFT_TURN_FLAG=0;  START_FLG=1;   OBJECT_IR_FLAG=0;
   }else if(RX_DATA=='c'){
        FARWARD_FLAG=0;   BACKWARD_FLG=0;  RIGHT_TURN_FLAG=0;  LEFT_TURN_FLAG=1;  START_FLG=1;   OBJECT_IR_FLAG=0;
  }else if(RX_DATA=='d'){
        FARWARD_FLAG=0;   BACKWARD_FLG=0;  RIGHT_TURN_FLAG=1;  LEFT_TURN_FLAG=0;  START_FLG=1;   OBJECT_IR_FLAG=0;
  }
  
}else{
  
}
    
}





}

  
