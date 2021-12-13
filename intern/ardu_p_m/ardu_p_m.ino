//  1. 시리얼 모니터에서 원하는 rpm 입력받기
//  2. input rpm 확인
//  3. 그 차이만큼 step motor로 구동하기
//
//  -------------------- Pin ---------------------
//  Pin 2  : Pixhawk Pwm-signal
//  Pin 8  : BLDC-motor Speed
//  Pin 9  : Step-motor DIR+
//  Pin 10 : Step-motor PUL+
//  ----------------------------------------------
//
//  ------------------- Timer --------------------
//  timer1 : measure BLDC-motor Speed
//  timer2 : output for Step-motor
//  ----------------------------------------------

#include <time.h>
#define P10 0X04

unsigned long past= 0;                // main문 에서 ic 이전 값 사용위해 저장
volatile unsigned long ic = 0;        // input capture ISR 때 ICR 값 읽어서 main 에서 사용
volatile int flag = 0;                // input rpm 들어오고 있는지 확인하기 위한 flag
volatile int want = 0;                // step motor 에 인가할 pulse의 주기 정하기 위해
volatile unsigned int step_rpm = 0;   // step motor 에 인가할 rpm
volatile unsigned int rpm = 100;      // (원하는 최종 rpm)/2


void setup() {
  cli();  //stop interrupts
  Serial.begin(9600); //set Serial baudrate

//------------------------------------------------ 타이머 set ---------------------------------------------
 // set timer1 // input의 rpm 측정
 // 4us 의 timer, 262.144ms마다 interrupt service routine
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;                           //initialize counter value to 0
  TCCR1B |= (1 << WGM12);               // turn on CTC mode
  TCCR1B |= (1 << CS11) | (1 << CS10);  // 64 prescaler
  OCR1A = 65535;                        // maximum 값
  TIMSK1 |= (1 << OCIE1A);              // enable timer compare interrupt
  

 // set timer2  // step motor에 pulse 출력
 // 64us 의 timer, OCR 값은 중간에 계속 바꾸어줌
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;                           //initialize counter value to 0
  TCCR2A |= (1 << WGM21);               // turn on CTC mode
  TCCR2B |= (1 << CS22) | (1 << CS21);  // 256 prescaler
  OCR2A = 255;                          // initialize (스텝모터 주기이므로 초기값 최대한 천천히)  // maximum값
  TIMSK2 |= (1 << OCIE2A);              // enable timer compare interrupt
//--------------------------------------------------------------------------------------------------------



//------------------------------------------ ICR ------------------------------------------------------
// input capture는 타이머1만 가능  // (ICP)8번핀에 interrupt 들어오면 증가하고 있던 TCNT1 -> ICR1 로 capture
  TCCR1B|= (1 << ICNC1) | (1<<ICES1); // noise cancel, input capture at rising edge
  TIMSK1|= (1 << ICIE1);  // enable interrupt
//-----------------------------------------------------------------------------------------------------



//------------------------------------ portb 입출력 mode -------------------------------------
  DDRB = 0x06;  // 0b00001100 // PB3,2 (10,9번핀) 출력모드  // PB0(8번핀)는 입력으로 사용 예정
  PORTB = 0x00; // initialize
//-------------------------------------------------------------------------------------------
  
  sei();  //enable interrupts
}





//-------------------------------------------- Interrupt Service Routine ----------------------------------------------
ISR(TIMER1_CAPT_vect){                      // ICP (8번핀) 에 인터럽트 들어올 때 // input capture interrupt
  
  ic = ICR1;    // ICR1값 사용
  TCNT1=0;      // TCNT1 retset
  flag=0;       // flag = 0 // input rpm 이 존재한다.
}



ISR(TIMER1_COMPA_vect){                     // timer1 CTC interrupt
  
  flag = 1;     // 262.144ms넘게 input rpm 이 없을 때
}



ISR(TIMER2_COMPA_vect){                     // timer2 CTC interrupt
  
  if(step_rpm >18 && step_rpm < 2000) // step_rpm>18 : OCR<255  // step_rpm<2000 : OCR>2  // OCR이 정상 작동 범위에 있을 때 
    PORTB ^= P10;                     // pulse 출력
}
//-----------------------------------------------------------------------------------------------------------------------






void loop() {
  
  delay(1000);
  int a;
  
  while(1){

    //------------------------- 원하는 최종 rpm의 절반 값 입력받기 ------------------------------
    // loop 문 한바퀴 돌면 풀려서 rpm을 0 으로 인식하는 현상으로 0일때 이전값 받아서 유지하도록 조치
    if(Serial.available()){
      rpm = Serial.parseInt();
      
      if (rpm != 0)
        a = rpm;
      else rpm = a;
        
      Serial.println(rpm);
    }
    //----------------------------------------------------------------------------------------




    //------------------------------------------------- step motor에 인가할 pulse 주기 계산 -------------------------------------------------------
    if (past <(2*ic) && past >(ic/2)){  //이전값과 값이 두배이상 차이나면 pass // 값이 중간에 한번씩 튀는 현상 때문에 
      want = 1250000/ic-rpm;    // 12의 분해능을 가진 pulse, 4us 주기로 뛰는 타이머 값 ic, 원하는 rpm => 스텝모터에서 조절해야할 값 want
      if (want > 0) {           // 감속
        PORTB |= 0x02;          // 9번핀 1 출력 (counter clockwise)
        step_rpm = want*2/3;    // scaling
      }

      else {                    // 가속
        PORTB &= 0xFD;          // 9번핀 0 출력 (clockwise)
        step_rpm = (-want)*2/3; // scaling
      }
      
      if (flag)  step_rpm = rpm*2/3;  // 값이 들어오지 않아 flag==1이 된다면 input rpm은 돌지 않고 있으므로 step motor가 원하는 출력만큼 회전하도록 함
      
      OCR2A = (unsigned int)((double)4600/step_rpm);   // timer2는 256분주율 이므로 그에 따라 계산 // 오차 줄이기 위해 숫자 변경
      //  OCR2A = (unsigned int)((double)4687.5/step_rpm);   // prescaler 256   // 이론적으로 맞는 수치

    }
    //-------------------------------------------------------------------------------------------------------------------------------------------



    past = ic; // ICR의 이전값을 정하기 위함
  }
} 
