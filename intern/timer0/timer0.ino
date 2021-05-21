#define P10 0X04
#define P0 0X00

void setup() {

  cli();  // clear interrupt // 전체 인터럽트 enable bit 를 0 으로 만듬
  
// turn on CTC mode
  TCCR0A = 0;// initialize
  TCCR0A |= (1 << WGM01); // CTC mode

// 분주율
// 16MHz 보드이므로 0.0625us 의 주기, 하지만 64분주율 하였으므로 0.0625*64 = 4us 주기의 클럭을 가진 타이머 생성
  TCCR0B = 0;// initialize
  TCCR0B |= (1 << CS01) | (1 << CS00);   // 64 prescaler 
  //  TCCR0B |= (1 << CS00);   // No prescaler
  //  TCCR0B |= (1 << CS01);   // 8 prescaler
  //  TCCR0B |= (1 << CS02);   // 256 prescaler
  //  TCCR0B |= (1 << CS02) | (1 << CS00);   // 1024 prescaler
   
// 초기값과,Maximum값  => 인터럽트 주기 설정
  TCNT0  = 0;//initialize counter value to 0  // 초기값부터 증가하여 OCR0A만큼 도달하면 0으로 clear
  OCR0A = 124;  // max 값 설정
    
// enable timer compare interrupt 
  TIMSK0 |= (1 << OCIE0A);  

// 타이머와 관계 X
  DDRB = P10; // 0x04= 0b00004000 // PB3(10번핀) 출력모드
  PORTB = 0X00; // initialize
  
  sei();  // set interrupt // 전체 인터럽트 enable bit 를 1 으로 만듬

}

ISR(TIMER0_COMPA_vect){ // timer0의 tcnt가 ocr0a과 일치하면 발생하는 interrupt service routine
  PORTB ^= P10; // 10번핀 출력값 toggle
}

void loop() {
  
}
