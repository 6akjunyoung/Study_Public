
#define P10 0X04

void setup() {
  
  cli();//stop interrupts


// turn on CTC mode
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2A |= (1 << WGM21);
  
// prescaler
  TCCR2B = 0;// initialize
  TCCR2B |= (1 << CS21);   // 8 prescaler
  //TCCR2B |= (1 << CS21) | (1 << CS20);   // 32 prescaler
  //TCCR2B |= (1 << CS22);   // 64 prescaler
  //TCCR2B |= (1 << CS22) | (1 << CS20);   // 128 prescaler
  //TCCR2B |= (1 << CS22) | (1 << CS21);   // 256 prescaler
  //TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);   // 1024 prescaler

// initialize, maximum
  TCNT2  = 0;  // initialize
  OCR2A = 199;  // max
    
// enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);

      
//portb 
  DDRB = 0x04;
  PORTB = 0x00;

  
  sei();  // enable interrupt
}

ISR(TIMER2_COMPA_vect){ // timer2의 tcnt가 ocr2a과 일치하면 발생하는 interrupt service routine
  PORTB ^= P10;
}

void loop() {
}
