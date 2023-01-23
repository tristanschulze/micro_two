#include <Arduino.h>
  
// thanks for the help: https://registry.platformio.org/libraries/electro707/Simple%20LED%20Matrix
 // code doc is here: https://electro707.com/documentation/Libraries/simple_led_library/stable/classsimple_matrix.html
// fantastic standalone pixel array EDITOR > https://github.com/Electro707/LED_Matrix_Generator
 

uint8_t HAPPY1[] = { 
0b1111110,
0b11000011,
0b10000001,
0b11111111,
0b11011011,
0b11011011,
0b11111111,
0b1111110,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0
};



uint8_t HAPPY2[] = { 
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b0,
0b1111110,
0b11111111,
0b11011011,
0b11011011,
0b11111111,
0b10000001,
0b11000011,
0b1111110
};




#include "simple_matrix.h" //Import the library
//#include "spritesheets.h"
 //#include "leds.h"
 
  
 /*
// helper function to copy pixel arrays
void copy(uint8_t* src, uint8_t* dst, int len) {
     memcpy(dst, src,len);
}
*/
 
/*
If you notice that the display is upside-down per display, change 
simpleMatrix disp(4, false, NUMBER_OF_DISPLAYS); to simpleMatrix disp(4, true, NUMBER_OF_DISPLAYS);
 
DIN  D11 (Fixed)
CLK  D13 (Fixed)
CS  D4 (Adjustable by software)
 
*/
simpleMatrix dotmatrix(8, true, 4); // init the dotmatrix
   
int poti_one_pin = A0;
int poti_two_pin = A1;
  
int one_in_raw = 0;
int two_in_raw = 0;
float one_in = 0;
float two_in = 0;
 
float fposx = 0;
 
float p2_x = 4;
float p1_x = 4;
 
 
int p_bxi = 4;
int p_byi = 16;
float bx = 4;
float by = 16;
float bax = .3;
float bay = .8; 
 
float ball_speed= .4;
 
int game_state = 0;
int buzzer_pin = 3;
 
 
 bool game_running = false;
 
 
 
int p_bid = 0; // previous brush id - used to avoid overhead
int age = 0;
 
 
long reset_ts = -1;
 
/*
void update_brush(uint8_t* _IMG){
 
  copy(_IMG,BRUSH,8);
}
 
 */
  
 
int led1_pin = 5;
int led2_pin = 6;
 
int led1_age = 4;
int led2_age = 4;
 
void leds_update(){
 
    if( led1_age>0){led1_age--; analogWrite(led1_pin,44);}else{ digitalWrite(led1_pin,LOW);}
    if( led2_age>0){led2_age--;analogWrite(led2_pin,44);}else{ digitalWrite(led2_pin,LOW);}
  
}

 void readInputs(){
 
    one_in_raw = analogRead(poti_one_pin);
    two_in_raw = analogRead(poti_two_pin);
    one_in = one_in_raw*0.0009765625; // same as /1024 but faster :)
    two_in = two_in_raw*0.0009765625; //
}
 


void idleMode(){

    dotmatrix.setIntensity(1);

    int rx = random(8);
    int ry = random(32);
    
    dotmatrix.clearPixel(rx,ry);

    rx = random(8);
    ry = random(32);
       
    dotmatrix.setPixel(rx,ry);
       

       //tone(buzzer_pin,random(400)+400,20);

}

 
void resetBall(){
 
 dotmatrix.clearDisplay();
  game_state = 0 ;
  bx = 4;
  by = 16;
 
  bay = (random(4)+6)*.1;  
  bax = random(1)-.5; 
   
  if(random(10)>5){
    bay = -bay;
  }
  
  ball_speed= .5;
 
 
}
 
 
int p_one_score = 0;
int p_two_score = 0;

void resetGame(){


   p_one_score = 0;
   p_two_score = 0;

}
 
  
void setup() {
   
  //Serial.begin(9600);
  dotmatrix.begin();
  //Set the LED's intensity. This value can be anywhere between 0 and 15.
  dotmatrix.setIntensity(8);
  dotmatrix.clearDisplay();
 
  resetBall();
 
  pinMode(led2_pin, OUTPUT);
  pinMode(led1_pin, OUTPUT);
   
}
  

    void showWinner(int wid){

        game_running = false;

        
       for(int i=0;i<9;i++){
        dotmatrix.clearDisplay();
         delay(100);
        
        if(wid==1){
           dotmatrix.sendMatrixBuffer(HAPPY2);
        }

         if(wid==2){
            dotmatrix.sendMatrixBuffer(HAPPY1);
        }
          tone(buzzer_pin,888+int(random(200)),20);
        delay(100);
        }

        //noTone();
         resetGame();
        //idleMode = true;
    }

  void showScore(){


    if(p_two_score>7){
        showWinner(2);
    }

     if(p_one_score>7){
        showWinner(1);
    }


dotmatrix.clearDisplay();
    //dotmatrix.print("1",1,)

    for(int p1=0;p1< p_two_score;p1++){
      for(int i=0;i<6;i++){

          dotmatrix.setPixel(i+1,1+p1*2);
      }
    }

     for(int p2=0;p2< p_one_score;p2++){
      for(int i=0;i<6;i++){

          dotmatrix.setPixel(i+1,31-p2*2);
      }
    }

    delay(1000);

  }
   
 
void loop() {
   

  if(digitalRead(2) && !game_running){

        dotmatrix.clearDisplay();
        game_running = true;
         tone(buzzer_pin,888+int(random(200)),20);

  }

  if(!game_running){
      idleMode();
      return;
  }


  if( game_state == 0){

        // repel X
        if(bx >= 7 || bx < 1){ bax *=-1;}
      
      
        if(int(by) == 2){ 
      
                float diff = abs(bx - float(p2_x));
                if(diff<1.7){
      
                    bay *=-1;
                    by++;
                    bax *= random(9,11)*.1;
                    ball_speed += .06;
                    tone(buzzer_pin,1449,18);
                    
                    led1_age = 4;
      
                }
        }
      
          if(int(by) <= 1){
                  
                  tone(buzzer_pin,1009,43);
                    p_one_score++;
                  game_state = 1;
                  reset_ts = millis()+360;
                  //by = 16;
                  showScore();
                  
              }
      
              // if player one collision
              if(int(by) == 30){
      
                float diff = abs(bx - float(p1_x));
      
                if(diff<1.7){
                    
                    by--;
                    bay *=-1; // repel!
                    bax *= random(9,11)*.1;
      
                    led2_age = 4;
      
                    ball_speed += .06;
                    tone(buzzer_pin,1999,18);
                }
              }
      
              if(int(by) >= 31){
                  
                  tone(buzzer_pin,1009,43);
                  
                  game_state = 1;
                  reset_ts = millis()+360;
                  p_two_score++;
                  showScore();
                  //by=16;
                  
              }
      
        bx += bax*ball_speed;
        by += bay*ball_speed;
      
        
      
        
        //p2_x = bx; // autopilot
        p2_x = int(8-two_in*8);
        p1_x = int(one_in*8);

  }
 
   
 
 // read analog inputs
 readInputs();
  
   if( game_state == 0){
 
    // GAMEPLAY!!! ------------
       // draw ball!
      dotmatrix.setPixel(bx,by);
 
      if(p_bxi != int(bx) || p_byi != int(by)){
        dotmatrix.clearPixel(p_bxi,p_byi);
      }
 
      // just clear pedal row
      for(int i=0;i<8;i++){
 
         if( abs(p2_x -i ) > 1.1){
            dotmatrix.clearPixel(i,1);
         }else{
            dotmatrix.setPixel(i,1);
         }
 
        if( abs(p1_x -i ) > 1.1){
          dotmatrix.clearPixel(i,30);
        }else{
          dotmatrix.setPixel(i,30);
        }
      }
       
 
   }
 
   
   if( game_state == 1){
    // pause mode if goal is made!
 
      

        if( reset_ts < millis() ){
 
            // reset the game!
            resetBall();
             
 
        } 

        return;
 
   }
 
    // save last postions for elements for cleanup drawing!
    p_bxi = int(bx);
    p_byi = int(by);
 
    leds_update();
 
    
    delay(1);
 
}