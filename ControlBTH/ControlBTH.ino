//----------------------------------
//------------LIBRERIAS-------------ARDUINO NANO TUZOBOT SOCCER
//----------------------------------
#include <MD_MAX72xx.h>
#include <SPI.h>
//----------------------------------
//--------------DEFINE-------------
//----------------------------------
#define frente digitalWrite(D0, LOW);digitalWrite(D2, LOW);
#define atras digitalWrite(D0, HIGH);digitalWrite(D2, HIGH);
#define derecha digitalWrite(D0, LOW);digitalWrite(D2, HIGH);
#define izquierda digitalWrite(D0, HIGH);digitalWrite(D2, LOW);
#define DELAY_ANIMATION 50
//----------------------------------
//--------------PUERTOS-------------
//----------------------------------
const byte D0 = 2;
const byte D2 = 4;

const byte PWM1 = 3;
const byte PWM2 = 5;
//----------------------------------
//----------PANEL LED---------------
//----------------------------------
//GENERIC_HW  PAROLA_HW  DR0CR0RR1_HW
MD_MAX72XX mx = MD_MAX72XX(MD_MAX72XX::GENERIC_HW,11,13, 12, 1);
//----------------------------------
//----------Caracteres---------------
//----------------------------------
const byte smile1[] = {0x00,0x24,0x24,0x00,0x42,0x3C,0x00,0x00};
const byte sad1[] = {0x00,0x24,0x24,0x00,0x3C,0x42,0x00,0x00};

void drawRows( const byte fig[] ){
  for( int i = 0; i < 8; i++ ){
    mx.setRow(0, i, fig[i]);
  }
}

void smile(){
  mx.clear();
  drawRows(smile1);
}

void sad(){
  mx.clear();
  drawRows(sad1);
}

void animateChar(char letter){
 
  letter++;
  if( letter > 'z' ){
    letter = 'a';  
  }
  
  for( int i = 0; i < 12; i++ ){
    // limpiar la matriz
    mx.clear();
    // dibujar el caracter c empezando en la columna i
    mx.setChar( i, letter );
    // esperar un tiempo
    delay(DELAY_ANIMATION);
  }
}

void setup() {
  // put your setup code here, to run once:
  // open the serial port at 9600 bps:
  Serial.begin(9600);

  pinMode(D0, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  
  digitalWrite(D0, LOW); 
  digitalWrite(D2, LOW); 

  analogWrite(PWM1, 0); //PWM1 (0-255)
  analogWrite(PWM2, 0); //PWM2 (0-255)

  //PANEL LED
  // inicializar el objeto mx
  mx.begin();
  // poniendo la matriz en modo de prueba
  mx.control(MD_MAX72XX::TEST, true);
  delay(1000);
  // desactivando el modo de prueba
  mx.control(MD_MAX72XX::TEST, false);
  // Establecer intencidad a un valor de 5
  mx.control(MD_MAX72XX::INTENSITY, 5);
  mx.clear();
  smile();
}


      
void loop() {

  if(Serial.available()>0)
  {
    String value = Serial.readStringUntil('#');
    
    if(value.length()==7)
      {
        String angle = value.substring(0, 3);
        String strength = value.substring(3, 6);
        String button = value.substring(6, 8);
        int a = angle.toInt();
        int f = (int)(strength.toInt()*2.5);

        bool m1 = false;
        unsigned short v1 = 0;
        bool m2 = false;
        unsigned short v2 = 0;
        
        Serial.print("angle: ");Serial.print(a);Serial.print('\t');
        Serial.print("strength: ");Serial.print(f);Serial.print('\t');
        Serial.print("button: ");Serial.print(button);Serial.println("");
        
        Serial.flush();
        value="";

        v1 = f;
        v2 = f;
        
        if(button == "1")
          sad();
        if(button == "2")
          smile();
        if(button == "3"){
            animateChar('G'-1); //los caracteres esan avanzados 1+
            animateChar('O'-1);
            animateChar('O'-1);
            animateChar('L'-1);
            smile();
          }
        if(button == "4"){
            animateChar('M'-1); //los caracteres esan +1
            animateChar('E'-1);
            animateChar('X'-1);
            animateChar('I'-1);
            animateChar('C'-1);
            animateChar('O'-1);
            smile();
          }
          
          
        //MD
        if( (a >= 330) || ( a >= 0 && a < 120 ) )
          //digitalWrite(D2, LOW);
          m2 = false;
        else if ( a >= 120 && a < 210)
          //digitalWrite(D2, HIGH);
          m2 = true;
        else if ( a >= 210 && a < 240)
          //analogWrite(PWM2, 0); //PWM2 (0-255)
          v2=0;
        else if ( a >= 240 && a < 300)
          //digitalWrite(D2, HIGH);
          m2 = true;
        else if ( a >= 300 && a < 330)
          //analogWrite(PWM2, 0); //PWM2 (0-255)
          v2=0;

          
        //MI
        if( (a >= 330) || ( a >= 0 && a < 30 ) )
          //digitalWrite(D0, LOW);
          m1 = false;
        else if ( a >= 30 && a < 60)
          //analogWrite(PWM1, 0); //PWM2 (0-255)
          v1 = 0;
        else if ( a >= 60 && a < 120)
           //digitalWrite(D0, HIGH);
           m1 = true;
        else if ( a >= 120 && a < 150)
          //analogWrite(PWM1, 0); //PWM2 (0-255)
          v1 = 0;
        else if ( a >= 150 && a < 240)
          //digitalWrite(D0, HIGH);
          m1 = true;
        else if ( a >= 240 && a < 330)
          //digitalWrite(D0, LOW);
          m1 = false;

         
        digitalWrite(D0, m1);
        digitalWrite(D2, m2);
        analogWrite(PWM1, v1); //PWM2 (0-255)
        analogWrite(PWM2, v2); //PWM2 (0-255)

      }
  }
}
