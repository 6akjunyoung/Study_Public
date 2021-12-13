#define P10 0X04
#define P0 0X00

void setup() {
  
  cli();  //stop interrupts

  
  
// initialize
  TCCR1A = 0;// set entire TCCR1A register to 0
  
// CTC mode, prescaler
  TCCR1B = 0;// initialize
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS11) | (1 << CS10);  // 64 prescaler
  // TCCR1B |= (1 << CS12) | (1 << CS10);  // 1024 prescaler
  // TCCR1B |= (1 << CS11);  // 8 prescaler
  // TCCR1B |= (1 << CS10);  // No prescaler
  // TCCR1B |= (1 << CS12);  // 256 prescaler

//초기값과 Maximum값
  TCNT1  = 0;//initialize counter value to 0
  OCR1A = 199;  // max

// enable timer compare interrupt  
  TIMSK1 |= (1 << OCIE1A);  

// 타이머와 관계x
  DDRB = 0x04;
  PORTB = 0x00;


  
  sei();  // 인터럽트 허용
}

ISR(TIMER1_COMPA_vect){ // timer1의 tcnt가 ocr1a과 일치하면 발생하는 interrupt service routine
  PORTB ^= P10;
}

void loop() {
}
