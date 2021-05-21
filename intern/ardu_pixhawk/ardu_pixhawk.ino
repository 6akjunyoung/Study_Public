//  1. pixhawk에서 출력되는 rpm 입력받기
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
//  timer0 : measure duty of Pixhawk Pwm-signal  
//  timer1 : measure BLDC-motor Speed
//  timer2 : output for Step-motor
//  ----------------------------------------------

#include <time.h>
#define P10 0X04                    // 10번핀
const byte interruptPin = 2;        // 인터럽트핀
volatile int cnt = 0;               // 타이머0 카운트
volatile unsigned int ic = 0;       // ICR 저장
unsigned unsigned int past= 0;      // 이전 ic값
volatile int flag = 0;              // input_rpm이 0인지 확인하기 위함
volatile int want = 0;              // input_rpm - 원하는최종rpm/2
volatile unsigned int step_rpm = 0; // step-motor 로 구동해야하는 rpm
volatile unsigned int rpm = 100;    // (원하는 최종 rpm)/2
volatile int t_pix = 18;            // 픽스호크 duty비 계산위한 변수

void setup() {
  cli();  //stop interrupts

//--------------------------------------------------- 타이머 set -------------------------------------------------
// set timer0 
// 4us 의 timer, 64us마다 interrupt service routine
  TCCR0A = 0;
  TCCR0B = 0;
  TCCR0A |= (1 << WGM01);               // turn on CTC mode
  TCCR0B |= (1 << CS01) | (1 << CS00);  // 64 prescaler
  TCNT0  = 0;                           // initialize counter value to 0
  OCR0A = 15;                           // (64us/4us - 1) // 0부터 시작하므로 -1
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
  

 // set timer2  // step motor에 pulse 출력
 // 64us 의 timer, OCR 값은 중간에 계속 바꾸어줌
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;                           //initialize counter value to 0
  TCCR2A |= (1 << WGM21);               // turn on CTC mode
  TCCR2B |= (1 << CS22) | (1 << CS21);  // 256 prescaler
  OCR2A = 255;                          // initialize (스텝모터 주기이므로 초기값 최대한 천천히)  // maximum값
  TIMSK2 |= (1 << OCIE2A);              // enable timer compare interrupt
//----------------------------------------------------------------------------------------------------------------
  

//-----------------------------------------------------INTERRUPT----------------------------------------------------------------------------
// 레지스터로 동작하지 않아서 함수 사용
  attachInterrupt(digitalPinToInterrupt(interruptPin), check, CHANGE);  // INT0 (2번핀)에 logic이 rising or falling edge일 때 check 함수 실행
  //EICRA |= (1 << ISC00);  // interrupt at logic level change
  //EIMSK |= (1 << INT0);   // enable
//------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------ICR----------------------------------------
  TCCR1B|= (1 << ICNC1) | (1<<ICES1); // noise cancel, input capture at rising edge
  TIMSK1|= (1 << ICIE1);              // enable
//-----------------------------------------------------------------------------------

  
//----------------------------------- portb 입출력 mode -------------------------------------
  DDRB = 0x06;  // 0b00001100 // PB3,2 (10,9번핀) 출력모드  // PB0 (8번핀)는 입력으로 사용 예정
  PORTB = 0x00; // initialize
//------------------------------------------------------------------------------------------
  
  sei();  //enable interrupts
}




//--------------------------------------------- Interrupt Service Routine ---------------------------------------------------------
ISR(TIMER0_COMPA_vect){                     // timer1 CTC interrupt
  
  cnt++;        // cnt 1당 64us 의 time을 갖고있음
}


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



void check(){                               // INT0 (2번핀)에 interrupt
  if( digitalRead(interruptPin) == HIGH ) // rising 이라면
    cnt = 0;                              // reset cnt 
  else              // falling 이라면
    t_pix = cnt;        // cnt값 사용
}
//----------------------------------------------------------------------------------------------------------------------------------



void loop() {
  
  delay(1000);
  int past_step = 0;

  while(1){
    // ----- pixhawk에서 나오는 pwm -----
    //   period  |  duty   | throttle  
    //   2.5ms   |  1.1ms  |     0%
    //   2.5ms   |  1.5ms  |    50%
    //   2.5ms   |  1.9ms  |   100%
    // ---------------------------------

 

    //--------------------------------------- pixhawk scaling (다시해야함. 임의의 범위로 맞춤) ------------------------------------------------------
    // rpm : (원하는 최종 rpm) /2
    if ( t_pix <= 18) rpm = 2;      // 18*64= 1.1ms => 0 rpm // 0으로 하면 input rpm 0으로 들어오는 경우 나누기 연산할 때 오류 발생할 수 있으니 2로 설정
    
    else if (past_step*2 > t_pix){  // 이전 값과 2배 이상 차이나면 pass
      rpm = (t_pix-18)*30;          // t_pix == 18 일때 0% 이기 때문에 차이만큼에 *30을 해주었음.  
                                    // 임의의 scaling이어서 재조정 필요
    }
    //--------------------------------------------------------------------------------------------------------------------------------------------


    
    //-------------------------------------------- step motor에 인가할 pulse 주기 계산 ---------------------------------------------------
    if (past <(2*ic) && past >(ic/2)){    //이전값과 값이 두배이상 차이나면 pass // 값이 중간에 한번씩 튀는 현상 때문에 
      want = 1250000/ic-rpm;      // 12의 분해능을 가진 pulse, 4us 주기로 뛰는 타이머 값 ic, 원하는 rpm => 스텝모터에서 조절해야할 값 want
      if (want > 0) {             // 감속
        PORTB |= 0x02;            // 9번핀 1 출력 (counter clockwise)
        step_rpm = want*2/3;      // scaling
      }

      else {                      // 가속
        PORTB &= 0xFD;            // 9번핀 0 출력 (clockwise)
        step_rpm = (-want)*2/3;   // scaling
      }
      
      if (flag)  step_rpm = rpm*2/3;  // 값이 들어오지 않아 flag==1이 된다면 input rpm은 돌지 않고 있으므로 step motor가 rpm 만큼 회전하도록 함
      
      OCR2A = (unsigned int)((double)4687.5/step_rpm - 1);   // prescaler 256
    }
    //----------------------------------------------------------------------------------------------------------------------------------
    


    past = ic;          // ICR의 이전값을 정하기 위함
    past_step = t_pix;  // pixhawk pwm duty의 이전 값을 저장하기 위함
  }
} 




//  |---------------------------------------------------------------------------------------------------------------------------------------------------------------------|
//  |---------------------------------------------------------------------------- 추가 설명 -------------------------------------------------------------------------------|
//  |---------------------------------------------------------------------------------------------------------------------------------------------------------------------|




//  ----------------------------------------- struct -----------------------------------------------
//        input   : 1rpm (clockwise)  -->  output : 2rpm (counter-clockwise)
//     step motor : 1rpm (clockwise)  -->  output : 3rpm (counter-clockwise)
//
//      |-----------|    |----------------|     |----------|
//      |   input   |----|  main gearbox  |---> |  output  |
//      |-----------|    |----------------|     |----------|
//                                |
//      |------------|            |
//      | step motor |------------|
//      |------------|
//  ------------------------------------------------------------------------------------------------

//----- pixhawk에서 나오는 pwm ------
//   period  |  duty   | throttle  
//   2.5ms   |  1.1ms  |     0%
//   2.5ms   |  1.5ms  |    50%
//   2.5ms   |  1.9ms  |   100%
// ---------------------------------

//  ------------------------------------ timer at CTC mode -----------------------------------------
//  prescaler 64 일 경우 : 1/16Mhz*64 = 4us  (64개의 클럭 당 1개의 클럭으로 사용하겠다!)
//                        TCNT는 0부터 4us마다 1씩 증가 
//  OCRnA = 24 일 경우   : TCNTn값이 24일 때 TIMERn_COMPA_vect 인터럽트 발생하며 TCNT는 0부터 다시 시작
//                        0부터 cycle이 반복되므로  24+1 = 25
//                        25*4us = 100us 마다 타이머 인터럽트가 발생하게 된다
//  ------------------------------------------------------------------------------------------------

//  -------------------------------------- input capture -------------------------------------------
//  timer1만 사용 가능
//  TIMER1_CAPT_vect 인터럽트가 발생할 때 ICR1 <- TCNT1
//  rising or falling edge 에 interrupt를 발생시킬지 정할 수 있다.
//  아두이노 우노에는 1개의 ICP(input capture pin)만 존재
//  ------------------------------------------------------------------------------------------------

//  ------------------------------------- pulse 출력 방식 -------------------------------------------
//  step motor는 400pulse/rev 로 설정
//  ex. 60 rpm 으로 회전시키고 싶다면 1초에 1회전 이므로 1초에 400pulse 를 입력해야함.
//      => 1/400 = 2.5ms의 주기로 pulse 출력
//      => timer2를 이용하여 pulse를 출력시키는데 출력방식으로 인터럽트마다 출력 핀을 toggle 시키기 때문에
//         절반의 주기를 가진 타이머를 생성해야된다.
//      => 1.25ms 마다 인터럽트를 발생시키는 타이머 생성. 인터럽트마다 출력값 toggle 하여 pulse 출력
//  ------------------------------------------------------------------------------------------------

//  ---------------------------------------- pulse 측정 --------------------------------------------
//  1. 입력 rpm : [1250000/ic]
//      pulse 주기 측정 : input capture
//      TIMER1 : 64 prescaler => 4us clock  |  BLDC : 12pulse/rev   
//     1) ICP1에 인터럽트 들어올 때마다 ICR1 값을 ic 변수에 저장시키면서 TCNT1 reset
//     2) 1/(ic*4us) *60 = 1분에 들어오는 pulse의 수
//     3) input_rpm = 1/(ic*4us) *60 /12 = 1,250,000/ic
//
//  2. 원하는 최종 rpm : [rpm]
//      pulse duty 측정 : pixhawk_rpm => (원하는 최종출력 rpm/2) = 변수 rpm  
//      TIMER0 : 64 prescaler => 4us clock  |  OCR0A : 15 => 64us마다 interrupt  
//      인터럽트 : INT0 pin 에 들어오는 픽스호크 pwm의 rising or falling edge 에 check 함수 실행
//     1) 64us 마다 cnt++
//     2) pwm이 rising edge일 때 cnt=0 으로 reset
//              falling edge일 때 t_pix <- cnt
//     3) if(cnt<=18) =>  rpm=0 
//        else        =>  rpm = (t_pix-18)*30  (-> 이는 임의의 수치이므로 변경하여야한다.)
//        * 픽스호크의 스로틀이 0%일 때 1.1ms 이므로 1100/64us = 17~18 번의 cnt가 찍히게 된다.
//     => 높은 분주율에 의해 픽스호크 pwm을 정밀하게 체크하고있지 못한다는 한계
//     => 분주율이 더 낮으면 실행 오류, OCR 줄이는 방향으로 고려
//  ------------------------------------------------------------------------------------------------

//  ---------------------- OCR2A 설정 (step motor에 인가할 pulse의 period) ---------------------------
//    입력rpm = 1250000/ic  | 최종rpm/2 = rpm | 스텝모터rpm = step_rpm  | 스텝모터 : 400pulse/rev
//    timer2 : 256 prescaler => 16us
//    입력rpm을 최종rpm/2
//  1) want = (입력rpm - 최종rpm/2)
//  2) if(want > 0) // 감속 => 스텝모터rpm = want*2/3;    (counter clockwise) 
//     else         // 가속 => 스텝모터rpm = (-want)*2/3; (clockwise)         
//  3) OCR2A = {16MHz*(원하는 인터럽트의 주기)/prescaler} -1
//     원하는 인터럽트의 주기 = (스텝모터로 인가하여야 할 pulse 주기)/2
//                         = (1/400)/(스텝모터rpm)/2 *60
//     OCR2A = 16M *60/400/(스텝모터rpm)/2/256 - 1
//           = 4687.5/스텝모터rpm -1
//  
//  ------------------------------------------------------------------------------------------------
