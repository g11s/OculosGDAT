#include <NewPing.h>
#include <avr/power.h>
#include <Narcoleptic.h>

#define trigPin 12
#define echoPin 11
#define motorpino 10
#define distanciaMax 200
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
  NewPing sensor(trigPin, echoPin, distanciaMax);

  while(true){
    if( (PINB & (1 << PINB1)) ){
      if(desligado){
        analogWrite(motorpino, 150);
        delay(500);
        analogWrite(motorpino, 0);
        delay(500);
        desligado = false;
      }
      delayMicroseconds(10);
      distancia = sensor.convert_cm(sensor.ping_median(3));

      //Serial.print("Distancia em CM: ");
      //Serial.println(distancia);
      //Serial.print("Distancia Anterior: ");
      //Serial.println(distanciaAnterior);
      
      if( !(distanciaAnterior*1.10 >= distancia && distanciaAnterior*0.9 <= distancia) ){
          if(distancia > 0){
             analogWrite(motorpino, vibraCall(distancia));
             distanciaAnterior = distancia;
             tempo = millis();
          }else{
             analogWrite(motorpino, 0);
          }
      }else{
          //Serial.print("Tempo: ");
          //Serial.println(millis() - tempo);
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
