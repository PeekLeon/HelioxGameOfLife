#include <FastLED.h>

/*** Lampe Init ***/
#define LED_PIN     6
#define POT_COLOR  A0
#define POT_LIFE   A1
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define BRIGHTNESS 128


int potLifeSelect = 18;
int potColorSelect = 18;
bool initialised = false;

const uint8_t matrixWith = 16;  // X
const uint8_t matrixHeight = 8; // Y

#define matrixSize (matrixWith * matrixHeight)
#define matrixSizeOut ((matrixWith + 1) * (matrixHeight + 1))
CRGB leds[matrixSize];

/*** Game of Life Init ***/
uint8_t matrixOfLife[matrixSize] = {0};
uint8_t matrixOfLifeNumberNeighbor[matrixSize] = {0};

long colorLife = 0xFF0000;
long colorDead = 0x0000FF;

void setup() {
  Serial.begin(9600); 
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, matrixSize).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
}


uint8_t XY( uint8_t x, uint8_t y)
{
  if( x & 0x01) {
    y = (matrixHeight - 1) - y;
  }
    
  return (x * matrixHeight) + y;
}

uint8_t numberNeighbor(uint8_t x, uint8_t y){
  uint8_t nb = 0;

  for(int i=x-1; i<=x+1;i++){
     for(int j=y-1;j<=y+1;j++){
      if(i >= 0 && j >= 0 && i < matrixWith && j < matrixHeight ){
         nb = nb + matrixOfLife[XY(i,j)];
      }
     }
  }

  return nb - matrixOfLife[XY(x,y)];
}

void setMatrix(){
  for (uint8_t x = 0; x < matrixWith; x++){
    for (uint8_t y = 0; y < matrixHeight; y++){
      matrixOfLifeNumberNeighbor[XY(x, y)] = numberNeighbor(x, y);
    }
  }
  
  for (uint8_t i = 0; i < matrixSize; i++) {

    if(matrixOfLife[i] == 1){
      // SURVIE ?
      if (matrixOfLifeNumberNeighbor[i] == 2 || matrixOfLifeNumberNeighbor[i] == 3){
        matrixOfLife[i] = 1;
      }else{
        matrixOfLife[i] = 0;
      }
    }else{
      // NAISSANCE ?
      if (matrixOfLifeNumberNeighbor[i] == 3){
        matrixOfLife[i] = 1;
      }else{
        matrixOfLife[i] = 0;
      }
    }
  }

}

void displayMatrix(){
  for (uint8_t i = 0; i < matrixSize; i++) {
      if(matrixOfLife[i] == 1){
        leds[i] = colorLife;
      }else{
        leds[i] = colorDead;
      }
    }
}

void changeLife() {
  int potLifeValue = analogRead(POT_LIFE) / 100 % 100;
  if(potLifeSelect != potLifeValue){
    potLifeSelect = potLifeValue;
   memset(matrixOfLife, 0, sizeof(matrixOfLife));
   initialised = false;
    switch(potLifeValue) {
      case 0:
      /*
       ##########
      */
        matrixOfLife[XY(3,3)] = 1;
        matrixOfLife[XY(4,3)] = 1;
        matrixOfLife[XY(5,3)] = 1;
        matrixOfLife[XY(6,3)] = 1;
        matrixOfLife[XY(7,3)] = 1;
        matrixOfLife[XY(8,3)] = 1;
        matrixOfLife[XY(9,3)] = 1;
        matrixOfLife[XY(10,3)] = 1;
        matrixOfLife[XY(11,3)] = 1;
        matrixOfLife[XY(12,3)] = 1;
        break;
      case 1:
      /*
        ###
      */
        matrixOfLife[XY(7,3)] = 1;
        matrixOfLife[XY(8,3)] = 1;
        matrixOfLife[XY(9,3)] = 1;
        break;
      case 2:
      /*
         #
         #
        ###
      */
        matrixOfLife[XY(7,3)] = 1;
        matrixOfLife[XY(8,3)] = 1;
        matrixOfLife[XY(9,3)] = 1;
        matrixOfLife[XY(8,4)] = 1;
        matrixOfLife[XY(8,5)] = 1;
        break;
      case 3:
        /*
           ###
          ###
        */
        matrixOfLife[XY(7,5)] = 1;
        matrixOfLife[XY(7,4)] = 1;
        matrixOfLife[XY(8,5)] = 1;
        matrixOfLife[XY(8,4)] = 1;
        matrixOfLife[XY(6,4)] = 1;
        matrixOfLife[XY(9,5)] = 1;
        break;
      case 4:
        /*
        #####      #####
        */
        matrixOfLife[XY(0,3)] = 1;
        matrixOfLife[XY(1,3)] = 1;
        matrixOfLife[XY(2,3)] = 1;
        matrixOfLife[XY(3,3)] = 1;
        matrixOfLife[XY(4,3)] = 1;
        matrixOfLife[XY(8,3)] = 1;
        matrixOfLife[XY(9,3)] = 1;
        matrixOfLife[XY(10,3)] = 1;
        matrixOfLife[XY(11,3)] = 1;
        matrixOfLife[XY(12,3)] = 1;
        break;
      case 5:
        /*
         ###    ### 
        ###      ###


            ####
        */
        matrixOfLife[XY(2,4)] = 1;
        matrixOfLife[XY(3,5)] = 1;
        matrixOfLife[XY(3,4)] = 1;
        matrixOfLife[XY(4,5)] = 1;
        matrixOfLife[XY(4,4)] = 1;
        matrixOfLife[XY(5,5)] = 1;
        matrixOfLife[XY(6,0)] = 1;
        matrixOfLife[XY(7,0)] = 1;
        matrixOfLife[XY(8,0)] = 1;
        matrixOfLife[XY(9,0)] = 1;
        matrixOfLife[XY(10,5)] = 1;
        matrixOfLife[XY(11,5)] = 1;
        matrixOfLife[XY(11,4)] = 1;
        matrixOfLife[XY(12,5)] = 1;
        matrixOfLife[XY(12,4)] = 1;
        matrixOfLife[XY(13,4)] = 1;
        break;
      default:
        /*
         ###
        */
        matrixOfLife[XY(7,3)] = 1;
        matrixOfLife[XY(8,3)] = 1;
        matrixOfLife[XY(9,3)] = 1;
    }
  }
  
}


void changeColor() {
  int potColorValue = analogRead(POT_COLOR) / 100 % 100;
  if(potColorSelect != potColorValue){
    potColorSelect = potColorValue;
    switch(potColorValue) {
      case 0:
        colorLife = 0xFF0000;
        colorDead = 0x0000FF;
        break;
      case 1:
        colorLife = 0x00FF00;
        colorDead = 0x0000FF;
        break;
      case 2:
        colorLife = 0x0000FF;
        colorDead = 0xFF0000;
        break;
      case 3:
        colorLife = 0x0000FF;
        colorDead = 0x00FF00;
        break;
      case 4:
        colorLife = 0xFF00FF;
        colorDead = 0xFFFF00;
        break;
      case 5:
        colorLife = 0xF000FF;
        colorDead = 0x000000;
        break;
      case 6:
        colorLife = 0xFF0000;
        colorDead = 0x00FF00;
        break;
      case 7:
        colorLife = 0xFF0000;
        colorDead = 0x00FF00;
        break;
      case 8:
        colorLife = 0x00FF00;
        colorDead = 0xFF0000;
        break;
      case 9:
        colorLife = 0x000000;
        colorDead = 0xFFFFFF;
        break;
      default:
        colorLife = 0xFF00FF;
        colorDead = 0xFFFF00;
    }
  }
  
}

void loop() {
  changeLife();
  changeColor();
  if(initialised){
    setMatrix();
    displayMatrix();
    FastLED.show();
    delay(300); 
  }else{
    displayMatrix();
    FastLED.show();
    delay(500);
    initialised = true;
  } 
}
