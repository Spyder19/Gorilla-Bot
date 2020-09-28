//Libraries needed
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <VarSpeedServo.h>
#include <QMC5883L.h>
#include <Wire.h>
#include <NewPing.h>


//Magnometer(digital compass)
QMC5883L compass;


//Servos needed
VarSpeedServo myservoFRONTFRONTLEFT;
VarSpeedServo myservoFRONTBACKLEFT;
VarSpeedServo myservoFRONTFRONTRIGHT;
VarSpeedServo myservoFRONTBACKRIGHT;
VarSpeedServo myservoBACKFRONTLEFT;
VarSpeedServo myservoBACKBACKLEFT;
VarSpeedServo myservoBACKFRONTRIGHT;
VarSpeedServo myservoBACKBACKRIGHT;


// Radio protocole needed:
RF24 radio(9, 10); // Create a Radio
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe
int message[9];  // 9 different messages to receive

// name of messages received
int  xAxis;
int  yAxis;
int buttonUp;
int buttonRight;
int buttonDown;
int buttonLeft;
int buttonE;
int buttonF;
int buttonJoystick;


//Joystick button counter settings (to tell us if autonomous mode is activated/deactivated)
int buttonJoystickPushCounter = 0;


// Step counter for autonomous mode
int StepCounter = 0;

// Left Right conters for autonomous mode

int LeftCounter = 0;
int RightCounter = 0;

// Orientation values to calculate

int Orientation = 0;
int OrientationError = 0;
int OrientationErrorR = 0;
int OrientationErrorL = 0;

//Array for storing orientation 
byte dataArrayF[1]; 
byte dataArrayB[1]; 
byte dataArrayL[1]; 
byte dataArrayR[1];          
int arrayIndexF = 0;
int arrayIndexB = 0;
int arrayIndexL = 0;
int arrayIndexR = 0;

void setup() 
{
  
  // Attach servos to pins  
  myservoFRONTFRONTLEFT.attach(4);     //3
  myservoFRONTBACKLEFT.attach(5);      //4
  myservoFRONTFRONTRIGHT.attach(17);   //10
  myservoFRONTBACKRIGHT.attach(16);    //9
  myservoBACKFRONTLEFT.attach(6);      //5
  myservoBACKBACKLEFT.attach(7);       //6
  myservoBACKFRONTRIGHT.attach(15);    //8 
  myservoBACKBACKRIGHT.attach(14);     //7
               
  myservoFRONTFRONTLEFT.write(180);
  myservoFRONTBACKLEFT.write(0);
  myservoFRONTFRONTRIGHT.write(0);
  myservoFRONTBACKRIGHT.write(180);
  myservoBACKFRONTLEFT.write(180);
  myservoBACKBACKLEFT.write(0);
  myservoBACKFRONTRIGHT.write(0);
  myservoBACKBACKRIGHT.write(180);

  delay(1000); 
     

  // detach servos to avoid jitter on power up
  myservoFRONTFRONTLEFT.detach();     
  myservoFRONTBACKLEFT.detach();
  myservoFRONTFRONTRIGHT.detach();
  myservoFRONTBACKRIGHT.detach();
  myservoBACKFRONTLEFT.detach();
  myservoBACKBACKLEFT.detach();
  myservoBACKFRONTRIGHT.detach();
  myservoBACKBACKRIGHT.detach();
    
      
  // Serial begin for debugging in serial monitor 
  Serial.begin(9600);

  // Magnometer (digital compass) settings
  Wire.begin();
  compass.init();
  compass.setSamplingRate(50);
  
  // Radio settings
  radio.begin();  
  radio.setChannel(101);  
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate( RF24_250KBPS ) ; 
  radio.openReadingPipe(1,pipe);


}

void loop()
{  
  radio.startListening();
  
    while( radio.available() )
    {
      
      
      radio.read( message, sizeof(message) );
     

      // Define messages received while radio available
      xAxis           = message[0];
      yAxis           = message[1];
      buttonUp        = message[2];
      buttonRight     = message[3];
      buttonDown      = message[4];
      buttonLeft      = message[5];
      buttonE         = message[6];
      buttonF         = message[7];       
      buttonJoystick  = message[8];

      radio.stopListening();
      
      // Write what messages are received in serial monitor for debugging
      Serial.print( " X: ");
      Serial.print(xAxis);
      Serial.print( " Y: ");
      Serial.print(yAxis);
      Serial.print( " UP: ");
      Serial.print(buttonUp);
      Serial.print( " RIGHT: ");
      Serial.print(buttonRight);
      Serial.print( " DOWN: ");
      Serial.print(buttonDown);
      Serial.print( " LEFT: ");
      Serial.print(buttonLeft);
      Serial.print( " JOYSTICK: ");
      Serial.print(buttonJoystick);
      Serial.print( " JPC: ");
      Serial.print(buttonJoystickPushCounter);
      
      Orientation = compass.readHeading();
      Orientation = map(Orientation, 0, 360, 0, 255); 
      OrientationError = dataArrayF[arrayIndexF] - Orientation;
      if  (OrientationError < -127.5)
      {      
          OrientationError += 255;
      }
      
      if (OrientationError > 127.5)
      {
          OrientationError -= 255;
      }   

      if (OrientationError < 0)
      {
         OrientationErrorR = OrientationError * -1;
      }

      if (OrientationError >= 0)
      {
         OrientationErrorL = OrientationError;
      }

          
      
      
   
      

      

      ////////////////Manual Commands////////////






      // BUTTON UP
      if(buttonUp == 0)
      {
         dataArrayF[arrayIndexF] = Orientation;
      }

      // BUTTON DOWN
      if (buttonDown == 0) 
      {   
          RobotSitBack();
          delay(2000);
          RobotSitForward();
          delay(500);
          RobotSitBack();
          delay(500);
          RobotSitForward();
          delay(500);
          RobotSitBack();
          delay(500);
          RobotSitForward();
          delay(500);
          RobotSitBack();
          delay(500);
          RobotStop();
          RobotStrut();
          RobotStrut();
          RobotStop();
          RobotTilt4Corners();
          RobotTilt4Corners();
          RobotStop();
          Robotlie();
      }
        
      //BUTTON RIGHT
      if(buttonRight == 0)
      {
        dataArrayR[arrayIndexR] = OrientationErrorR;      
      }      

      //BUTTON LEFT
      if(buttonLeft == 0)
      {        
        dataArrayL[arrayIndexL]  = OrientationErrorL;                                
      }  
      
      // JOYSTICK CENTER
      if (xAxis > 490 && xAxis < 530 && yAxis > 490 && yAxis < 530 && (buttonJoystickPushCounter == 0 || buttonJoystickPushCounter >= 2))
      {
          RobotStop();
      }

      //JOYSTICK FORWARD
      if (xAxis > 300 && xAxis < 700 && yAxis > 1018 && yAxis <= 1023 && (buttonJoystickPushCounter == 0 || buttonJoystickPushCounter >= 2))
      {
          RobotForward();                   
      }

      //JOYSTICK BACKWARD
      if (xAxis > 300 && xAxis < 700 && yAxis >= 0 && yAxis < 5 && (buttonJoystickPushCounter == 0 || buttonJoystickPushCounter >= 2))
      {
          RobotBackward();              
      }

      //JOYSTICK LEFT
      if (xAxis >= 0 && xAxis < 5 && yAxis > 300 && yAxis < 700 && (buttonJoystickPushCounter == 0 || buttonJoystickPushCounter >= 2))
      {
          RobotLeft();
      }

      //JOYSTICK RIGHT
      if (xAxis > 1017 && xAxis <= 1023 && yAxis > 300 && yAxis < 700 && (buttonJoystickPushCounter == 0 || buttonJoystickPushCounter >= 2))
      {
          RobotRight();        
      }

     

     ////////////////Autonomous Commands//////////// 


     

      // JOYSTICK BUTTON AUTONOMOUS START
      if ((buttonJoystick == 0)&&(buttonJoystickPushCounter == 0))
      {  
          buttonJoystickPushCounter++;
          LeftCounter = 0;
          RightCounter = 0;
          StepCounter = 0; 
          RobotForward();
          RobotForward(); 
          StepCounter++;
          StepCounter++;  
          buttonJoystick = 1;        
      }

      // JOYSTICK BUTTON AUTONOMOUS STOP
      if ((buttonJoystick == 0)&&(buttonJoystickPushCounter == 1))
      {  
          RobotStop();
          buttonJoystickPushCounter = 0;
          delay(3000);     
      }

      if(buttonJoystickPushCounter == 1)
      {

          if(StepCounter <= 16)
          { 
  

                  
                     
                  if ((OrientationError <= dataArrayL[arrayIndexL]) && (OrientationError >= (dataArrayR[arrayIndexR]*-1)))   
                  {
                      RobotForward(); 
                      StepCounter++;
                  }

                  if (OrientationError > dataArrayL[arrayIndexL])  
                  {
                      RobotRight(); 
                  }

                  if (OrientationError < (dataArrayR[arrayIndexR]*-1))   
                  {
                      RobotLeft();
                  }
              }       
          
          
          
          

          else
          {
             RobotStop();
             RobotSitBack();
             delay(2000);
             RobotSitForward();
             delay(500);
             RobotSitBack();
             delay(500);
             RobotSitForward();
             delay(500);
             RobotSitBack();
             delay(500);
             RobotSitForward();
             delay(500);
             RobotSitBack();
             delay(500);
             RobotStop();
             RobotStrut();
             RobotStrut();
             RobotStop();
             RobotTilt4Corners();
             RobotTilt4Corners();
             RobotStop();
             Robotlie();
             buttonJoystickPushCounter = 0;
          }

      }
      Serial.print(" Orientation: ");
      Serial.print(Orientation);
      Serial.print(" OrientationError: ");
      Serial.print(OrientationError); 
      Serial.print(" F: ");
      Serial.print(dataArrayF[arrayIndexF]); 
      Serial.print(" L: ");
      Serial.print(dataArrayL[arrayIndexL]); 
      Serial.print(" R: ");
      Serial.print(dataArrayR[arrayIndexR]); 
      Serial.println(" ");                  
   } 
}

   

void RobotStop()
{           
  myservoFRONTFRONTLEFT.write(170);
  myservoFRONTBACKLEFT.write(10);
  myservoFRONTFRONTRIGHT.write(10);
  myservoFRONTBACKRIGHT.write(170);
  myservoBACKFRONTLEFT.write(170);
  myservoBACKBACKLEFT.write(10);
  myservoBACKFRONTRIGHT.write(10);
  myservoBACKBACKRIGHT.write(170);

  delay(400);

  // detach servos
  myservoFRONTFRONTLEFT.detach();     
  myservoFRONTBACKLEFT.detach();
  myservoFRONTFRONTRIGHT.detach();
  myservoFRONTBACKRIGHT.detach();
  myservoBACKFRONTLEFT.detach();
  myservoBACKBACKLEFT.detach();
  myservoBACKFRONTRIGHT.detach();
  myservoBACKBACKRIGHT.detach();  
}


void RobotForward()
{
  myservoFRONTFRONTLEFT.attach(4);     //3
  myservoFRONTBACKLEFT.attach(5);      //4
  myservoFRONTFRONTRIGHT.attach(17);   //10
  myservoFRONTBACKRIGHT.attach(16);    //9
  myservoBACKFRONTLEFT.attach(6);      //5
  myservoBACKBACKLEFT.attach(7);       //6
  myservoBACKFRONTRIGHT.attach(15);    //8 
  myservoBACKBACKRIGHT.attach(14);     //7

  myservoFRONTFRONTLEFT.write(90);
  myservoFRONTBACKLEFT.write(135);
  myservoFRONTFRONTRIGHT.write(0);
  myservoFRONTBACKRIGHT.write(135);
  myservoBACKFRONTLEFT.write(180);
  myservoBACKBACKLEFT.write(45);
  myservoBACKFRONTRIGHT.write(90);
  myservoBACKBACKRIGHT.write(45);
  
  delay(75); 

  myservoFRONTFRONTLEFT.write(45);
  myservoFRONTBACKLEFT.write(0);
  myservoFRONTFRONTRIGHT.write(0);
  myservoFRONTBACKRIGHT.write(135);
  myservoBACKFRONTLEFT.write(180);
  myservoBACKBACKLEFT.write(45);
  myservoBACKFRONTRIGHT.write(135);
  myservoBACKBACKRIGHT.write(180);

  delay(75);
               
  myservoFRONTFRONTLEFT.write(120);
  myservoFRONTBACKLEFT.write(0);
  myservoFRONTFRONTRIGHT.write(0);
  myservoFRONTBACKRIGHT.write(135);
  myservoBACKFRONTLEFT.write(180);
  myservoBACKBACKLEFT.write(45);
  myservoBACKFRONTRIGHT.write(60);
  myservoBACKBACKRIGHT.write(180);
  
  delay(75); 

  myservoFRONTFRONTLEFT.write(180);//
  myservoFRONTBACKLEFT.write(45);
  myservoFRONTFRONTRIGHT.write(90);
  myservoFRONTBACKRIGHT.write(45);
  myservoBACKFRONTLEFT.write(90);
  myservoBACKBACKLEFT.write(135);
  myservoBACKFRONTRIGHT.write(0);
  myservoBACKBACKRIGHT.write(135);
  
  delay(75); 

  myservoFRONTFRONTLEFT.write(180);//
  myservoFRONTBACKLEFT.write(45);
  myservoFRONTFRONTRIGHT.write(135);
  myservoFRONTBACKRIGHT.write(180);
  myservoBACKFRONTLEFT.write(45);
  myservoBACKBACKLEFT.write(0);
  myservoBACKFRONTRIGHT.write(0);
  myservoBACKBACKRIGHT.write(135);
  

  delay(75); 

  myservoFRONTFRONTLEFT.write(180);//
  myservoFRONTBACKLEFT.write(45);
  myservoFRONTFRONTRIGHT.write(60);
  myservoFRONTBACKRIGHT.write(180);
  myservoBACKFRONTLEFT.write(120);
  myservoBACKBACKLEFT.write(0);
  myservoBACKFRONTRIGHT.write(0);
  myservoBACKBACKRIGHT.write(135);
  

  delay(75);
  
}

void RobotBackward()
{
  myservoFRONTFRONTLEFT.attach(4);     //3
  myservoFRONTBACKLEFT.attach(5);      //4
  myservoFRONTFRONTRIGHT.attach(17);   //10
  myservoFRONTBACKRIGHT.attach(16);    //9
  myservoBACKFRONTLEFT.attach(6);      //5
  myservoBACKBACKLEFT.attach(7);       //6
  myservoBACKFRONTRIGHT.attach(15);    //8 
  myservoBACKBACKRIGHT.attach(14);     //7
  
  myservoFRONTFRONTLEFT.write(45);
  myservoFRONTBACKLEFT.write(90);
  myservoFRONTFRONTRIGHT.write(45);
  myservoFRONTBACKRIGHT.write(180);
  myservoBACKFRONTLEFT.write(135);
  myservoBACKBACKLEFT.write(0);
  myservoBACKFRONTRIGHT.write(135);
  myservoBACKBACKRIGHT.write(90);
  
  delay(75); 

  myservoFRONTFRONTLEFT.write(180);
  myservoFRONTBACKLEFT.write(135);
  myservoFRONTFRONTRIGHT.write(45);
  myservoFRONTBACKRIGHT.write(180);
  myservoBACKFRONTLEFT.write(135);
  myservoBACKBACKLEFT.write(0);
  myservoBACKFRONTRIGHT.write(0);
  myservoBACKBACKRIGHT.write(45);

  delay(75);

  myservoFRONTFRONTLEFT.write(180);
  myservoFRONTBACKLEFT.write(60);
  myservoFRONTFRONTRIGHT.write(45);
  myservoFRONTBACKRIGHT.write(180);
  myservoBACKFRONTLEFT.write(135);
  myservoBACKBACKLEFT.write(0);
  myservoBACKFRONTRIGHT.write(0);
  myservoBACKBACKRIGHT.write(120);
  
  delay(75); 

  myservoFRONTFRONTLEFT.write(135);//
  myservoFRONTBACKLEFT.write(0);
  myservoFRONTFRONTRIGHT.write(135);
  myservoFRONTBACKRIGHT.write(90);
  myservoBACKFRONTLEFT.write(45);
  myservoBACKBACKLEFT.write(90);
  myservoBACKFRONTRIGHT.write(45);
  myservoBACKBACKRIGHT.write(180);
  
  delay(75); 

  myservoFRONTFRONTLEFT.write(135);//
  myservoFRONTBACKLEFT.write(0);
  myservoFRONTFRONTRIGHT.write(0);
  myservoFRONTBACKRIGHT.write(45);
  myservoBACKFRONTLEFT.write(180);
  myservoBACKBACKLEFT.write(135);
  myservoBACKFRONTRIGHT.write(45);
  myservoBACKBACKRIGHT.write(180);
  

  delay(75); 

  myservoFRONTFRONTLEFT.write(135);//
  myservoFRONTBACKLEFT.write(0);
  myservoFRONTFRONTRIGHT.write(0);
  myservoFRONTBACKRIGHT.write(120);
  myservoBACKFRONTLEFT.write(180);
  myservoBACKBACKLEFT.write(60);
  myservoBACKFRONTRIGHT.write(45);
  myservoBACKBACKRIGHT.write(180);
  

  delay(75);

  
}  

void RobotLeft()
{

  myservoFRONTFRONTLEFT.attach(4);     //3
  myservoFRONTBACKLEFT.attach(5);      //4
  myservoFRONTFRONTRIGHT.attach(17);   //10
  myservoFRONTBACKRIGHT.attach(16);    //9
  myservoBACKFRONTLEFT.attach(6);      //5
  myservoBACKBACKLEFT.attach(7);       //6
  myservoBACKFRONTRIGHT.attach(15);    //8 
  myservoBACKBACKRIGHT.attach(14);     //7
  
  myservoFRONTFRONTRIGHT.write(0);
  myservoFRONTBACKRIGHT.write(135);
  myservoBACKFRONTRIGHT.write(90);
  myservoBACKBACKRIGHT.write(45);
  myservoFRONTFRONTLEFT.write(45);
  myservoFRONTBACKLEFT.write(90);  
  myservoBACKFRONTLEFT.write(135);
  myservoBACKBACKLEFT.write(0);
  
  
  delay(75); 

  
  myservoFRONTFRONTRIGHT.write(0);
  myservoFRONTBACKRIGHT.write(135);
  myservoBACKFRONTRIGHT.write(135);
  myservoBACKBACKRIGHT.write(180);
  myservoFRONTFRONTLEFT.write(180);
  myservoFRONTBACKLEFT.write(135); 
  myservoBACKFRONTLEFT.write(135);
  myservoBACKBACKLEFT.write(0);
 

  delay(75);
               
  
  myservoFRONTFRONTRIGHT.write(0);
  myservoFRONTBACKRIGHT.write(135);  
  myservoBACKFRONTRIGHT.write(60);
  myservoBACKBACKRIGHT.write(180);
  myservoFRONTFRONTLEFT.write(180);
  myservoFRONTBACKLEFT.write(60);;
  myservoBACKFRONTLEFT.write(135);
  myservoBACKBACKLEFT.write(0);
  
  delay(75); 

  
  myservoFRONTFRONTRIGHT.write(90);
  myservoFRONTBACKRIGHT.write(45); 
  myservoBACKFRONTRIGHT.write(0);
  myservoBACKBACKRIGHT.write(135);
  myservoFRONTFRONTLEFT.write(135);//
  myservoFRONTBACKLEFT.write(0);
  myservoBACKFRONTLEFT.write(45);
  myservoBACKBACKLEFT.write(90);
  
  delay(75); 

  
  myservoFRONTFRONTRIGHT.write(135);
  myservoFRONTBACKRIGHT.write(180);
  myservoBACKFRONTRIGHT.write(0);
  myservoBACKBACKRIGHT.write(135);
  myservoFRONTFRONTLEFT.write(135);//
  myservoFRONTBACKLEFT.write(0);
  myservoBACKFRONTLEFT.write(180);
  myservoBACKBACKLEFT.write(135);

  

  delay(75); 

 
  myservoFRONTFRONTRIGHT.write(60);
  myservoFRONTBACKRIGHT.write(180); 
  myservoBACKFRONTRIGHT.write(0);
  myservoBACKBACKRIGHT.write(135);
  myservoFRONTFRONTLEFT.write(135);//
  myservoFRONTBACKLEFT.write(0);
  myservoBACKFRONTLEFT.write(180);
  myservoBACKBACKLEFT.write(60);

  delay(75);
}

void RobotRight()
{
  myservoFRONTFRONTLEFT.attach(4);     //3
  myservoFRONTBACKLEFT.attach(5);      //4
  myservoFRONTFRONTRIGHT.attach(17);   //10
  myservoFRONTBACKRIGHT.attach(16);    //9
  myservoBACKFRONTLEFT.attach(6);      //5
  myservoBACKBACKLEFT.attach(7);       //6
  myservoBACKFRONTRIGHT.attach(15);    //8 
  myservoBACKBACKRIGHT.attach(14);     //7

  myservoFRONTFRONTLEFT.write(90);
  myservoFRONTBACKLEFT.write(135);
  myservoBACKFRONTLEFT.write(180);
  myservoBACKBACKLEFT.write(45);
  myservoFRONTFRONTRIGHT.write(45);
  myservoFRONTBACKRIGHT.write(180);
  myservoBACKFRONTRIGHT.write(135);
  myservoBACKBACKRIGHT.write(90);
  
  delay(75); 

  myservoFRONTFRONTLEFT.write(45);
  myservoFRONTBACKLEFT.write(0);
  myservoBACKFRONTLEFT.write(180);
  myservoBACKBACKLEFT.write(45);
  myservoFRONTFRONTRIGHT.write(45);
  myservoFRONTBACKRIGHT.write(180);
  myservoBACKFRONTRIGHT.write(0);
  myservoBACKBACKRIGHT.write(45);


  delay(75);
               
  myservoFRONTFRONTLEFT.write(120);
  myservoFRONTBACKLEFT.write(0);
  myservoBACKFRONTLEFT.write(180);
  myservoBACKBACKLEFT.write(45);
  myservoFRONTFRONTRIGHT.write(45);
  myservoFRONTBACKRIGHT.write(180);
  myservoBACKFRONTRIGHT.write(0);
  myservoBACKBACKRIGHT.write(120);
  
  delay(75); 

  myservoFRONTFRONTLEFT.write(180);//
  myservoFRONTBACKLEFT.write(45);
  myservoBACKFRONTLEFT.write(90);
  myservoBACKBACKLEFT.write(135);
  myservoFRONTFRONTRIGHT.write(135);
  myservoFRONTBACKRIGHT.write(90);
  myservoBACKFRONTRIGHT.write(45);
  myservoBACKBACKRIGHT.write(180);
  
  delay(75); 

  myservoFRONTFRONTLEFT.write(180);//
  myservoFRONTBACKLEFT.write(45);
  myservoBACKFRONTLEFT.write(45);
  myservoBACKBACKLEFT.write(0);
  myservoFRONTFRONTRIGHT.write(0);
  myservoFRONTBACKRIGHT.write(45);
  myservoBACKFRONTRIGHT.write(45);
  myservoBACKBACKRIGHT.write(180);
  

  delay(75); 

  myservoFRONTFRONTLEFT.write(180);//
  myservoFRONTBACKLEFT.write(45);
  myservoBACKFRONTLEFT.write(120);
  myservoBACKBACKLEFT.write(0);  
  myservoFRONTFRONTRIGHT.write(0);
  myservoFRONTBACKRIGHT.write(120);
  myservoBACKFRONTRIGHT.write(45);
  myservoBACKBACKRIGHT.write(180);

  delay(75);
}

void RobotStrut()
{
  // Attach servos to pins  
  myservoFRONTFRONTLEFT.attach(4);     //3
  myservoFRONTBACKLEFT.attach(5);      //4
  myservoFRONTFRONTRIGHT.attach(17);   //10
  myservoFRONTBACKRIGHT.attach(16);    //9
  myservoBACKFRONTLEFT.attach(6);      //5
  myservoBACKBACKLEFT.attach(7);       //6
  myservoBACKFRONTRIGHT.attach(15);    //8 
  myservoBACKBACKRIGHT.attach(14);     //7

  myservoFRONTFRONTLEFT.write(180); //INITIAL START
  myservoFRONTBACKLEFT.write(45);
  myservoFRONTFRONTRIGHT.write(0);
  myservoFRONTBACKRIGHT.write(135);
  myservoBACKFRONTLEFT.write(180);
  myservoBACKBACKLEFT.write(45);
  myservoBACKFRONTRIGHT.write(0);
  myservoBACKBACKRIGHT.write(135);
  
  delay(150);

  myservoFRONTFRONTLEFT.write(90); 
  myservoFRONTBACKLEFT.write(45);
  myservoFRONTFRONTRIGHT.write(0);
  myservoFRONTBACKRIGHT.write(135);
  myservoBACKFRONTLEFT.write(180);
  myservoBACKBACKLEFT.write(45);
  myservoBACKFRONTRIGHT.write(90);
  myservoBACKBACKRIGHT.write(135);
  
  delay(150);

  myservoFRONTFRONTLEFT.write(135); 
  myservoFRONTBACKLEFT.write(0);
  myservoFRONTFRONTRIGHT.write(0);
  myservoFRONTBACKRIGHT.write(135);
  myservoBACKFRONTLEFT.write(180);
  myservoBACKBACKLEFT.write(45);
  myservoBACKFRONTRIGHT.write(45);
  myservoBACKBACKRIGHT.write(180);
  
  delay(150);

  myservoFRONTFRONTLEFT.write(180); //INITIAL START
  myservoFRONTBACKLEFT.write(45);
  myservoFRONTFRONTRIGHT.write(0);
  myservoFRONTBACKRIGHT.write(135);
  myservoBACKFRONTLEFT.write(180);
  myservoBACKBACKLEFT.write(45);
  myservoBACKFRONTRIGHT.write(0);
  myservoBACKBACKRIGHT.write(135);

  delay(150);

  myservoFRONTFRONTLEFT.write(180); 
  myservoFRONTBACKLEFT.write(45);
  myservoFRONTFRONTRIGHT.write(90);
  myservoFRONTBACKRIGHT.write(135);
  myservoBACKFRONTLEFT.write(90);
  myservoBACKBACKLEFT.write(45);
  myservoBACKFRONTRIGHT.write(0);
  myservoBACKBACKRIGHT.write(135);

  delay(150);

  myservoFRONTFRONTLEFT.write(180); 
  myservoFRONTBACKLEFT.write(45);
  myservoFRONTFRONTRIGHT.write(45);
  myservoFRONTBACKRIGHT.write(180);
  myservoBACKFRONTLEFT.write(135);
  myservoBACKBACKLEFT.write(0);
  myservoBACKFRONTRIGHT.write(0);
  myservoBACKBACKRIGHT.write(135);

  delay(150);

  
}




void RobotTilt4Corners()
{
  int SpeedTiltCorners = 25;

  
  // Attach servos to pins  
  myservoFRONTFRONTLEFT.attach(4);     //3
  myservoFRONTBACKLEFT.attach(5);      //4
  myservoFRONTFRONTRIGHT.attach(17);   //10
  myservoFRONTBACKRIGHT.attach(16);    //9
  myservoBACKFRONTLEFT.attach(6);      //5
  myservoBACKBACKLEFT.attach(7);       //6
  myservoBACKFRONTRIGHT.attach(15);    //8 
  myservoBACKBACKRIGHT.attach(14);     //7

  myservoFRONTFRONTLEFT.write(125,SpeedTiltCorners);
  myservoFRONTBACKLEFT.write(55,SpeedTiltCorners);
  myservoFRONTFRONTRIGHT.write(45,SpeedTiltCorners);
  myservoFRONTBACKRIGHT.write(135,SpeedTiltCorners);
  myservoBACKFRONTLEFT.write(135,SpeedTiltCorners);
  myservoBACKBACKLEFT.write(45,SpeedTiltCorners);
  myservoBACKFRONTRIGHT.write(0,SpeedTiltCorners);
  myservoBACKBACKRIGHT.write(180,SpeedTiltCorners);
  
  delay(500);


  myservoFRONTFRONTLEFT.write(135,SpeedTiltCorners);
  myservoFRONTBACKLEFT.write(45,SpeedTiltCorners);
  myservoFRONTFRONTRIGHT.write(55,SpeedTiltCorners);
  myservoFRONTBACKRIGHT.write(125,SpeedTiltCorners);
  myservoBACKFRONTLEFT.write(180,SpeedTiltCorners);
  myservoBACKBACKLEFT.write(0,SpeedTiltCorners);
  myservoBACKFRONTRIGHT.write(45,SpeedTiltCorners);
  myservoBACKBACKRIGHT.write(135,SpeedTiltCorners);
  
  delay(500);

  myservoFRONTFRONTLEFT.write(180,SpeedTiltCorners);
  myservoFRONTBACKLEFT.write(0,SpeedTiltCorners);
  myservoFRONTFRONTRIGHT.write(45,SpeedTiltCorners);
  myservoFRONTBACKRIGHT.write(135,SpeedTiltCorners);
  myservoBACKFRONTLEFT.write(135,SpeedTiltCorners);
  myservoBACKBACKLEFT.write(45,SpeedTiltCorners);
  myservoBACKFRONTRIGHT.write(55,SpeedTiltCorners);
  myservoBACKBACKRIGHT.write(125,SpeedTiltCorners);
  
  delay(500);

  myservoFRONTFRONTLEFT.write(135,SpeedTiltCorners);
  myservoFRONTBACKLEFT.write(45,SpeedTiltCorners);
  myservoFRONTFRONTRIGHT.write(0,SpeedTiltCorners);
  myservoFRONTBACKRIGHT.write(180,SpeedTiltCorners);
  myservoBACKFRONTLEFT.write(125,SpeedTiltCorners);
  myservoBACKBACKLEFT.write(55,SpeedTiltCorners);
  myservoBACKFRONTRIGHT.write(45,SpeedTiltCorners);
  myservoBACKBACKRIGHT.write(135,SpeedTiltCorners);
  
  delay(500);

}



void Robotlie()
{
  int Speedlie = 20;
  
  // Attach servos to pins  
  myservoFRONTFRONTLEFT.attach(4);     //3
  myservoFRONTBACKLEFT.attach(5);      //4
  myservoFRONTFRONTRIGHT.attach(17);   //10
  myservoFRONTBACKRIGHT.attach(16);    //9
  myservoBACKFRONTLEFT.attach(6);      //5
  myservoBACKBACKLEFT.attach(7);       //6
  myservoBACKFRONTRIGHT.attach(15);    //8 
  myservoBACKBACKRIGHT.attach(14);     //7
  
  myservoFRONTFRONTLEFT.write(90,Speedlie);
  myservoFRONTBACKLEFT.write(90,Speedlie);
  myservoFRONTFRONTRIGHT.write(90,Speedlie);
  myservoFRONTBACKRIGHT.write(90,Speedlie);
  myservoBACKFRONTLEFT.write(90,Speedlie);
  myservoBACKBACKLEFT.write(90,Speedlie);
  myservoBACKFRONTRIGHT.write(90,Speedlie);
  myservoBACKBACKRIGHT.write(90,Speedlie);

  delay(2500);     

  // detach servos
  myservoFRONTFRONTLEFT.detach();     
  myservoFRONTBACKLEFT.detach();
  myservoFRONTFRONTRIGHT.detach();
  myservoFRONTBACKRIGHT.detach();
  myservoBACKFRONTLEFT.detach();
  myservoBACKBACKLEFT.detach();
  myservoBACKFRONTRIGHT.detach();
  myservoBACKBACKRIGHT.detach();
}

void RobotSitBack()
{
  int SpeedSitBack = 150;
  
  // Attach servos to pins  
  myservoFRONTFRONTLEFT.attach(4);     //3
  myservoFRONTBACKLEFT.attach(5);      //4
  myservoFRONTFRONTRIGHT.attach(17);   //10
  myservoFRONTBACKRIGHT.attach(16);    //9
  myservoBACKFRONTLEFT.attach(6);      //5
  myservoBACKBACKLEFT.attach(7);       //6
  myservoBACKFRONTRIGHT.attach(15);    //8 
  myservoBACKBACKRIGHT.attach(14);     //7
  
  myservoFRONTFRONTLEFT.write(180,SpeedSitBack);
  myservoFRONTBACKLEFT.write(0,SpeedSitBack);
  myservoFRONTFRONTRIGHT.write(0,SpeedSitBack);
  myservoFRONTBACKRIGHT.write(180,SpeedSitBack);
  myservoBACKFRONTLEFT.write(45,SpeedSitBack);
  myservoBACKBACKLEFT.write(135,SpeedSitBack);
  myservoBACKFRONTRIGHT.write(135,SpeedSitBack);
  myservoBACKBACKRIGHT.write(45,SpeedSitBack); 
}   

void RobotSitForward()
{
  int SpeedSitForward = 150;
  
  // Attach servos to pins  
  myservoFRONTFRONTLEFT.attach(4);     //3
  myservoFRONTBACKLEFT.attach(5);      //4
  myservoFRONTFRONTRIGHT.attach(17);   //10
  myservoFRONTBACKRIGHT.attach(16);    //9
  myservoBACKFRONTLEFT.attach(6);      //5
  myservoBACKBACKLEFT.attach(7);       //6
  myservoBACKFRONTRIGHT.attach(15);    //8 
  myservoBACKBACKRIGHT.attach(14);     //7
  
  myservoFRONTFRONTLEFT.write(45,SpeedSitForward);
  myservoFRONTBACKLEFT.write(135,SpeedSitForward);
  myservoFRONTFRONTRIGHT.write(135,SpeedSitForward);
  myservoFRONTBACKRIGHT.write(45,SpeedSitForward);
  myservoBACKFRONTLEFT.write(180,SpeedSitForward);
  myservoBACKBACKLEFT.write(0,SpeedSitForward);
  myservoBACKFRONTRIGHT.write(0,SpeedSitForward);
  myservoBACKBACKRIGHT.write(180,SpeedSitForward);
}
