//  bldc, encoder rpm 출력
//
//  -------------------- Pin ---------------------
//  Pin 2  : Encoder A
//  Pin 8  : BLDC-motor Speed
//  ----------------------------------------------


#include <time.h>

const byte interruptPin = 2;
volatile unsigned long cnt = 0;
volatile unsigned long ic = 0;
volatile unsigned int a=0;

void setup() {
  cli();    // stop interrupt
  
  Serial.begin(9600);                     // Serial baudrate 9600 
  pinMode(interruptPin, INPUT_PULLUP);    // INT0
  
//set timer0 
//64us 의 timer, 10ms마다 interrupt service routine
  TCCR0A = 0;
  TCCR0B = 0;
  TCCR0A |= (1 << WGM01);               // turn on CTC mode
  TCCR0B |= (1 << CS02) | (1 << CS00);  // 1024 prescaler
  TCNT0  = 0;                           // initialize counter value to 0
  OCR0A = 156;                          // 10ms
  TIMSK0 |= (1 << OCIE0A);              // enable timer compare interrupt
  

// set timer1 // input의 rpm 측정
// 4us 의 timer, 262.144ms마다 interrupt service routine
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;                           //initialize counter value to 0
  TCCR1B |= (1 << WGM12);               // turn on CTC mode
  TCCR1B |= (1 << CS11) | (1 << CS10);  // 64 prescaler
  OCR1A = 65535;                        // maximum 값
  TIMSK1 |= (1 << OCIE1A);              // enable timer compare interrupt
  

//INTERRUPT
  EICRA |= (1 << ISC01) | (1 << ISC00);   // interrupt at rising edge
  EIMSK |= (1 << INT0);                   // enable INT0

//ICR
  TCCR1B|= (1 << ICNC1) | (1<<ICES1);     // noise cancel, input capture at rising edge
  TIMSK1|= (1 << ICIE1);                  // enable
  
  sei();    // enable interrupt
}



ISR(TIMER0_COMPA_vect){ // 10ms마다 (TCNT0가 OCR0A에 도달할 때)
  a= cnt;               // cnt : pulse 개수
  cnt=0;                // clear
}

ISR(TIMER1_CAPT_vect){  // input capture pin interrupt
  ic = ICR1;            // ICR1 : pulse의 주기
  TCNT1=0;              // clear
}

ISR(INT0_vect){         // INT0 pin interrupt
  cnt++;                // cnt++
}

void loop() {
  //-------------------------------------------------------
  // encoder : a*10/3       bldc : 1250000/ic
  //
  // encoder 분해능 : 1800  |  a: 10ms 동안 들어온 pulse 수
  // a*100*60/1800 => encoder가 측정한 rpm
  //-------------------------------------------------------

  
  Serial.print("encoder : ");
  //Serial.print(a*10/3);   // 이론상
  Serial.print(a*19/6);     // bldc 측정과 비슷하게 맞추기 위해 scaling
  
  Serial.print("    bldc : ");
  Serial.println(1250000/ic);
}
