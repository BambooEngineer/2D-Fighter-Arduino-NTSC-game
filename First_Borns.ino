#include <TVout.h>
#include <stdlib.h> 
#include <avr/pgmspace.h> 
#include <fontALL.h> 
#include "bitmap_funcs.h" 
#include "bitmaps.h" 

#define Left2 2
#define Right2 3
#define Jump2 4
#define Slash2 12

#define Left 5
#define Right 6
#define Jump 10
#define Slash 13

TVout TV;
unsigned long CLR=0; // Screen refresh Speed
int newGame = 0;
unsigned long deb=0; // button debounce
unsigned long deb1=0; // button debounce
unsigned long deb2=0; // button debounce
unsigned long deb3=0; 

unsigned long SpriteT=0; // Animation speed
int spriteX = 0; // Animation incrementor

int shorthop=0;
unsigned long SH=0;

int x= 10;
int y = 80;
int slashtime = 0;
int gravity=0; 
int AirTime;
int jheight;
int platform;
bool jumping;
bool slash = false;
bool facingR=false;
bool facingL=false;
////////////////////////////
unsigned long Deb=0; // button debounce
unsigned long Deb1=0; // button debounce
unsigned long Deb2=0; // button debounce
unsigned long Deb3=0; 

unsigned long SSpriteT=0; // Animation speed
int sspriteX = 0; // Animation incrementor

int sshorthop=0;
unsigned long SSH=0;

int xx= 100;
int yy = 80;
int ggravity=0; 
int AAirTime;
int jjheight;
int pplatform;
bool jjumping;
bool sslash = false;
bool ffacingR=false;
bool ffacingL=false;

bool alive1 = true; 
bool alive2 = true;


void Attack(bool *slash){ 
  unsigned long slashtime = millis();
  if(*slash){ 
    if(slashtime - SpriteT >= 10){ 
          SpriteT = slashtime;
          if(spriteX != 33 && spriteX != 25){
            ++spriteX;  
            if( hitbox(facingR, x, y, xx, yy) ) alive2 = false; // player 2 dead
          }
          if(spriteX == 25 || spriteX == 33){   
              if(digitalRead(Slash)) *slash = false; 
           } 
      }
  }   
}

void Attack2(bool *slash){ 
  unsigned long slashtime = millis();
  if(*slash){ 
    if(slashtime - SSpriteT >= 10){ 
          SSpriteT = slashtime;
          if(sspriteX != 33 && sspriteX != 25){
            ++sspriteX;     
            if( hitbox(ffacingR, xx, yy, x, y) ) alive1 = false; // player 1 dead 
          }
          if(sspriteX == 25 || sspriteX == 33){  
              if(digitalRead(Slash2)) *slash = false; 
           } 
      }
  }   
}


bool hitbox(bool facing, int x1, int y1, int x2, int y2){ // checks for collision 
  if(facing){ // facing right
    int swordedge = x1+10;
    int person = x2+8;
    if((swordedge <= person && swordedge >= x2)&& y1 == y2){ // if the person is infront of the sword && the sword hits them
      return true;
    }
    else{
      return false;
    }
  }
  else{ // facing left
    int swordedge = x1-10;
    int person = x2 + 8;
    if((swordedge <= person && swordedge >= x2)&& y1 == y2){
      return true;
    }
    else{
      return false;
    }
  }
  
}

void setup() {
  pinMode(Left, INPUT);
  pinMode(Right, INPUT);
  pinMode(Jump, INPUT);
  pinMode(Slash, INPUT);

  pinMode(Left2, INPUT);
  pinMode(Right2, INPUT);
  pinMode(Jump2, INPUT);
  pinMode(Slash2, INPUT);
  
  TV.begin(NTSC,120,96);
  TV.select_font(font6x8);
  
  randomSeed(analogRead(0));
}


// extra = platforms and new moves 

void loop() { // 120 by 96 

  unsigned long pause = millis();
  
  if(pause - CLR >= 5){ // Screen Refresh rate 
    CLR = pause;
    TV.delay_frame(1);
    TV.clear_screen();
    if((!alive1 || !alive2)) newGame = newGame + 5;
  }

  
  if((!alive1 || !alive2) && newGame >= 1500){ // New match 
    newGame = 0;
    x = 10;
    xx= 100;
    alive1 = true;
    alive2 = true;
  }

  if(alive1) overlaybitmap( x, y, spritesP1[spriteX] ); // bitmap functions taken from poofy adventure who took it from parachute
  if(alive2) overlaybitmap( xx, yy, spritesP2[sspriteX] );
  
  jump(pause, jheight, platform);
  Attack(&slash);
  jump2(pause, jjheight, pplatform);
  Attack2(&sslash);

  /////////////////////////////////////PLAYER 1 ACTIONS
  if(!digitalRead(Slash) && !slash && !digitalRead(Left)){ 
           slash = true;
           spriteX = 26;
  }
  if(!digitalRead(Slash) && !slash && !digitalRead(Right)){ 
           slash = true;
           spriteX = 18;
  }

  if(!digitalRead(Jump) && gravity==0){ // if not jumping
    jumping = true;
    jheight = y-30;
    platform = y;
    gravity = 2; 
  } 

////////////////////////////////////////   PLAYER 2 ACTIONS
  if(!digitalRead(Slash2) && !sslash && !digitalRead(Left2)){ 
           sslash = true;
           sspriteX = 26;
  }
  if(!digitalRead(Slash2) && !sslash && !digitalRead(Right2)){ 
           sslash = true;
           sspriteX = 18;
  }

  if(!digitalRead(Jump2) && ggravity==0){ // if not jumping
    jjumping = true;
    jjheight = yy-30;
    pplatform = yy;
    ggravity = 2; 
  } 
  /////////////////////////////////////////////// PLAYER 1 MOVEMENT 
  if(!digitalRead(Left)){
      if(!facingL && !slash) spriteX = 10;
      facingL = true;
      if(pause - deb >= 5){  
       deb = pause;
       if(x >= 10) x--;
    }
    if(pause - SpriteT >= 50 && !slash){ 
        SpriteT = pause;
        if(!slash) ++spriteX;
          if(spriteX >= 18){
            if(!slash) spriteX = 10;
          }
        }
  }
  if(facingL && digitalRead(Left)){ 
    facingL = false;
    if(!slash) spriteX = 9; 
  }
  ///////////////////////////////////////////////////
  if(!digitalRead(Right)){
        facingR = true;
        if(pause - deb1 >= 5){ 
           deb1 = pause;
           if(x <= 103) x++;
        }
        if(pause - SpriteT >= 50 && !slash){ // animation speed variable 
        SpriteT = pause;
        if(!slash) ++spriteX;
          if(spriteX >= 8){
            if(!slash) spriteX = 1;
          }
        }
  }
  if(facingR && digitalRead(Right)){ 
    facingR = false;
    if(!slash) spriteX = 0; // FaceRight direction
  }
/////////////////////////////////////////////// PLAYER 2 MOVEMENT 
  if(!digitalRead(Left2)){
      if(!ffacingL && !sslash) sspriteX = 10;
      ffacingL = true;
      if(pause - Deb >= 5){  
       Deb = pause;
       if(xx >= 10) xx--;
    }
    if(pause - SSpriteT >= 50 && !sslash){ 
        SSpriteT = pause;
        if(!sslash) ++sspriteX;
          if(sspriteX >= 18){
            if(!sslash) sspriteX = 10;
          }
        }
  }
  if(ffacingL && digitalRead(Left2)){ 
    ffacingL = false;
    if(!sslash) sspriteX = 9; 
  }
  ///////////////////////////////////////////////////
  if(!digitalRead(Right2)){
        ffacingR = true;
        if(pause - Deb1 >= 5){ 
           Deb1 = pause;
           if(xx <= 103) xx++;
        }
        if(pause - SSpriteT >= 50 && !sslash){  
        SSpriteT = pause;
        if(!sslash) ++sspriteX;
          if(sspriteX >= 8){
            if(!sslash) sspriteX = 1;
          }
        }
  }
  if(ffacingR && digitalRead(Right2)){ 
    ffacingR = false;
    if(!sslash) sspriteX = 0; 
  }
  
}





void jump(unsigned long mills, int jheight, int platform){ // ground is y = 90, platform variable will turn into an array when we introduce platforms
  
  if(mills - deb2 >= AirTime){ // millis timer for velocity in air while jumping  
    deb2 = mills; 

    if(digitalRead(Jump)) jheight = jheight + 10; // if down being pressed jheight gets reduced(shorthopping)
       
    if(y <= jheight && jumping){ 
     
      jumping = false;
      AirTime = 50; 
      jh(); // helper to get out of this conditional
      
    }
    else{
      AirTime = 4;
      if(jumping){
        y = y - gravity;
      }
      if(!jumping){
        y = y + gravity;
        if(y >= platform) gravity = 0; // where to stop jumping
      }
    }
    //////////////////
  
  }
}

void jh(){
      y=y-1; 
      y=y-1;
      y=y-1;
      y=y-1;
      y=y-1;
      y=y+1;
      y=y+1;
      y=y+1;
      y=y+1;
      y=y+1;
}

void jump2(unsigned long mills, int jheight, int platform){ // ground is y = 90, platform variable will turn into an array when we introduce platforms
  
  if(mills - Deb2 >= AAirTime){ // millis timer for velocity in air while jumping  
    Deb2 = mills; 

    if(digitalRead(Jump2)) jjheight = jjheight + 10; // if down being pressed jheight gets reduced(shorthopping)
       
    if(yy <= jjheight && jjumping){ 
     
      jjumping = false;
      AAirTime = 50; 
      jh2(); // helper to get out of this conditional
      
    }
    else{
      AAirTime = 4;
      if(jjumping){
        yy = yy - ggravity;
      }
      if(!jjumping){
        yy = yy + ggravity;
        if(yy >= pplatform) ggravity = 0; // where to stop jumping
      }
    }
    //////////////////
  
  }
}

void jh2(){
      yy=yy-1; 
      yy=yy-1;
      yy=yy-1;
      yy=yy-1;
      yy=yy-1;
      yy=yy+1;
      yy=yy+1;
      yy=yy+1;
      yy=yy+1;
      yy=yy+1;
}

//void drawParatrooper( byte ndx, boolean draw )
//{
//  if ( draw )
 //   overlaybitmap( paratrooperPositions[ ndx ].x, paratrooperPositions[ ndx ].y, paratrooper_bitmaps[ ndx ] );
  //else
  //  erasebitmap( paratrooperPositions[ ndx ].x, paratrooperPositions[ ndx ].y, paratrooper_bitmaps[ ndx ] );
 //}
