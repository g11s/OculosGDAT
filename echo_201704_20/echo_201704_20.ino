#include <Narcoleptic.h>
#include <avr/power.h>

#define echoPin 11
#define motorpino 10
#define botao 9
#define vibraCall(a)((300-a))/2

void setup() {
  power_spi_disable();  
  power_twi_disable();
  power_timer2_disable(); 
  byte distancia, distanciaAnterior;
  unsigned long tempo;
  bool desligado = true;
  DDRB = B00010100;
  pinMode(botao,INPUT_PULLUP);
  //Serial.begin (9600);

  while(true){
    if( (PINB & (1 << PINB1)) ){
      if(desligado){
        analogWrite(motorpino, 150);
        delay(500);
        analogWrite(motorpino, 0);
        delay(500);
        desligado = false;
      }
      distancia = ultrassonic();
      
      if( !(distanciaAnterior*1.1 >= distancia && distanciaAnterior*0.9 <= distancia) ){
          if(200 > distancia > 0){
             //Serial.print("A distancia em CM: ");
             //Serial.println(distancia);
             //delay(500);
             //Serial.println(vibraCall(distancia));
             analogWrite(motorpino, vibraCall(distancia));
             distanciaAnterior = distancia;
             tempo = millis();
          }else{
             analogWrite(motorpino, 0);
          }
      }else{
          Serial.println(millis() - tempo);
          if( millis() - tempo > 3000 ){
            Narcoleptic.delay(500);
          }else if( millis() - tempo > 2000 ){
            analogWrite(motorpino, 0);
          }
      }
    }else{
      desligado = true;
      Narcoleptic.delay(1000);
    }
  }
}

byte ultrassonic(){
  PORTB |= (1 << PORTB4); //seta o pino 12 com pulso alto "HIGH" ou ligado ou ainda 1
  delayMicroseconds(10);  //delay de 10 microssegundos
  PORTB &= ~(1 << PORTB4); //seta o pino 12 com pulso baixo novamente
  
  //pulseIn lê o tempo entre a chamada e o pino entrar em high
  return pulseIn(echoPin, HIGH)/58.2;
}
