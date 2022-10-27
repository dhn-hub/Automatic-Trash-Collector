#include <TimerOne.h>

int  msec_cnt;

const int GAR_IR2_PIN=2;
const int GAR_IR_PIN=3;

const int LEFT_IR_PIN=5;
const int RIGHT_IR_PIN=6;

const int RF_OUT_PIN=7;
const int BUZZ_PIN=8;

const int MOTOR1_PIN1=A4;      // the number of the LED pin
const int MOTOR1_PIN2=A5;      // the number of the LED pin

const int MOTOR2_PIN1=A0;      // the number of the LED pin
const int MOTOR2_PIN2=A1;      // the number of the LED pin


int LEFT_IR_PIN_STATE=0;
int RIGHT_IR_PIN_STATE=0;
unsigned char  PROCESS_STAGE=0,PSTATE_CNT=0;
int  RFT_CNT=0;

//boolean  GAS_flag=0,G_IR_flag=0;

boolean LEFT_IR_FLAG=0;
boolean RIGHT_IR_FLAG=0;
boolean START_FLG=0,STOP_FLAG=0;

boolean R_FLAG=0,L_FLAG=0;

//boolean GAS_DETECT_FLAG=0;
boolean  GARBAGE_IR_DETECT_FLG=0;

void READ_RIGHT_IR_SENSOR(void);
void READ_LEFT_IR_SENSOR(void);

void GAR_IR_SNS(void);
void GAR_GAS_SNS(void);
void setup(){
   
  pinMode(GAR_IR2_PIN,INPUT);
  pinMode(GAR_IR_PIN,INPUT);
    
  pinMode(LEFT_IR_PIN,INPUT);
  pinMode(RIGHT_IR_PIN,INPUT);
  
  pinMode(RF_OUT_PIN, OUTPUT);  
  pinMode(BUZZ_PIN, OUTPUT);
  
  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);
  
  pinMode(13, OUTPUT);    
  
  Timer1.initialize(1000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt(timerIsr); 
  Serial.begin(9600);
   PROCESS_STAGE=0;
}


void loop() {
                
}




void timerIsr()
{
    // Toggle LED
   msec_cnt++;if(msec_cnt>=500){
      if(START_FLG){
                digitalWrite(13, digitalRead( 13 ) ^ 1 ); 
      }else{ 
                digitalWrite(13,0); 
      }
               
    if((PROCESS_STAGE==2)||(PROCESS_STAGE==7)){
               digitalWrite(8, digitalRead( 8 ) ^ 1 ); 
    }else{
               digitalWrite(8,0); 
    }
     
    if(PROCESS_STAGE==1){
        digitalWrite(RF_OUT_PIN, HIGH); PSTATE_CNT++;if(PSTATE_CNT>=2){  digitalWrite(RF_OUT_PIN, LOW);  PROCESS_STAGE=2; PSTATE_CNT=0; }//vehicle start
   }else if(PROCESS_STAGE==3){
        digitalWrite(RF_OUT_PIN, HIGH);                                                                                 //vehicle stop
        PSTATE_CNT++;if(PSTATE_CNT>=2){   digitalWrite(RF_OUT_PIN, LOW); PROCESS_STAGE=4;  PSTATE_CNT=0;  }
   }else if(PROCESS_STAGE==4){
      digitalWrite(MOTOR1_PIN1, HIGH);     digitalWrite(MOTOR1_PIN2, LOW);                                                  //garbage motor ON
        PSTATE_CNT++;if(PSTATE_CNT>=4){    digitalWrite(MOTOR1_PIN1, LOW);   digitalWrite(MOTOR1_PIN2, LOW); PROCESS_STAGE=5;  PSTATE_CNT=0; } //garbage motor OFF
   } else if(PROCESS_STAGE==5){
      digitalWrite(MOTOR1_PIN1, LOW);   digitalWrite(MOTOR1_PIN2, LOW);    
     PSTATE_CNT++;if(PSTATE_CNT>=10){     PROCESS_STAGE=6;  PSTATE_CNT=0; }
   } else if(PROCESS_STAGE==6){
      digitalWrite(MOTOR1_PIN1, LOW);   digitalWrite(MOTOR1_PIN2, HIGH);    //garbage motor NORMAL POSIT & VEHICLE START
      RFT_CNT++;if( RFT_CNT>=2){   digitalWrite(RF_OUT_PIN, HIGH);    RFT_CNT=0;} 
     PSTATE_CNT++;if(PSTATE_CNT>=4){    digitalWrite(MOTOR1_PIN1, LOW);   digitalWrite(MOTOR1_PIN2, LOW);  digitalWrite(RF_OUT_PIN, LOW); PROCESS_STAGE=7;  PSTATE_CNT=0; }
    
   }else if(PROCESS_STAGE==8){
        digitalWrite(RF_OUT_PIN, HIGH);                                                                                 //vehicle stop
       PSTATE_CNT++;if(PSTATE_CNT>=2){   digitalWrite(RF_OUT_PIN, LOW); PROCESS_STAGE=9;  PSTATE_CNT=0;  }
   }else if(PROCESS_STAGE==9){
      digitalWrite(MOTOR2_PIN1, HIGH);  digitalWrite(MOTOR2_PIN2, LOW);                                                  //garbage motor ON
      PSTATE_CNT++;if(PSTATE_CNT>=5){    digitalWrite(MOTOR2_PIN1, LOW);   digitalWrite(MOTOR2_PIN2, LOW); PROCESS_STAGE=10;  PSTATE_CNT=0; } //garbage motor OFF
   }else if(PROCESS_STAGE==10){
      digitalWrite(MOTOR2_PIN1, LOW);  digitalWrite(MOTOR2_PIN2, LOW);                                                  //garbage motor ON
      PSTATE_CNT++;if(PSTATE_CNT>=10){   PROCESS_STAGE=11;  PSTATE_CNT=0; } //garbage motor OFF
   }else if(PROCESS_STAGE==11){
      digitalWrite(MOTOR2_PIN1, LOW);   digitalWrite(MOTOR2_PIN2, HIGH);   digitalWrite(RF_OUT_PIN, HIGH);   //garbage motor NORMAL POSIT & VEHICLE START
      PSTATE_CNT++;if(PSTATE_CNT>=5){    digitalWrite(MOTOR2_PIN1, LOW);   digitalWrite(MOTOR2_PIN2, LOW);  digitalWrite(RF_OUT_PIN, LOW); PROCESS_STAGE=0;  PSTATE_CNT=0; }
   }
       
       
        msec_cnt=0;
  }
    
   LEFT_IR_PIN_STATE=digitalRead(LEFT_IR_PIN);
   RIGHT_IR_PIN_STATE=digitalRead(RIGHT_IR_PIN);
   READ_RIGHT_IR_SENSOR();
   READ_LEFT_IR_SENSOR();
   
   GAR_IR_SNS();
   GAR_GAS_SNS();    

   if((GAS_DETECT_FLAG)||(GARBAGE_IR_DETECT_FLG)){
        PROCESS_STAGE=1;   GAS_DETECT_FLAG=0; GARBAGE_IR_DETECT_FLG=0;
   }

 
}


void READ_LEFT_IR_SENSOR(void){    
    if(LEFT_IR_PIN_STATE==HIGH)
    {    if(L_FLAG==0){   
              L_FLAG=1;  PROCESS_STAGE=3; 
         }            
    }else{  L_FLAG=0; }
           
 }





void READ_RIGHT_IR_SENSOR(void){
      
    if(RIGHT_IR_PIN_STATE==HIGH)
    {     
        if(R_FLAG==0){
               R_FLAG=1;   PROCESS_STAGE=8; 
         }
    }else{  
            R_FLAG=0; 
    }
    
 }




void GAR_IR_SNS(void){
           
    if(digitalRead(GAR_IR_PIN))
    {        
         if(G_IR_flag==0)
          { 
                GARBAGE_IR_DETECT_FLG=1;          
              G_IR_flag=1;
          }
    }else{ 
            G_IR_flag=0;
    }  
    
 }



void GAR_GAS_SNS(void){
           
    if(digitalRead(GAR_IR2_PIN))
    {        
         if(GAS_flag==0)
          { 
               if(PROCESS_STAGE==0){ GAS_DETECT_FLAG=1;   }     
               GAS_flag=1;
          }
    }else{ 
            GAS_flag=0;
    }  
    
 }




  

