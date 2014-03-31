/*
  

 Author: Allen Downey 
 
 Based on http://arduino.cc/en/Tutorial/AnalogInput
 Created by David Cuartielles
 modified 30 Aug 2011
 By Tom Igoe

 License: Public Domain
 
 */
 
 
int ledPin = 5;       // select the pin for the LED
int buttonPin1 = 2;
int buttonPin2 = 3;

// counter specs
int low = 36;
int high = 255;
int stride = 5;
int samples = (high - low)/stride;
volatile int counter = low;

volatile boolean new_mode = true;
volatile int mode = 0;
const int freqs[] = { 523/2, 587/2, 659/2, 698/2, 783/2, 880/2, 987/2, 1046/2 };
const int num_modes = 8;

void change_mode() {
  new_mode = true;
  mode++;
  if (mode == num_modes) {
    mode = 0;
  }
}

float clock_speed = 16000000; // 10^6

long int get_ocr(long int Hz, long int prescaler, int bits) {
  long int cap = round(pow(2.0,float(bits)))-1;
  long int denom = (Hz*prescaler);
  float frac = clock_speed/denom;
  long int ocr = round(frac)-1;
  return max(min(cap,ocr),1);
}

void change_ocr(int timer, long int Hz) {
  cli();
  switch (timer) {
    case 0:
      OCR0A = get_ocr(Hz,64,8); //min(255, (16*10^6)/(Hz0*64)-1)
      break;
    case 1:
      OCR1A = get_ocr(Hz,1024,16); // min(65535, (16*10^6)/(Hz1*1024) - 1)
      break;
    case 2:
      OCR2A = get_ocr(Hz, 8, 8);// min(255, round((16.0*pow(10.0,6))/(Hz2*8.0)-1));// = (16*10^6) / (8000*8) - 1 (must be <256)
      break;
  }
  sei();
}

// Interrupt setup. Adapted from: http://www.instructables.com/id/Arduino-Timer-Interrupts/
void setup_interrupts(long int Hz0, long int Hz1, long int Hz2) {
  cli();//stop interrupts
  if (Hz0 > 0) {
    ////set timer0 interrupt at Hz0 Hz, with 64 prescaler
    TCCR0A = 0;// set entire TCCR0A register to 0
    TCCR0B = 0;// same for TCCR0B
    TCNT0  = 0;//initialize counter value to 0
    OCR0A = get_ocr(Hz0,64,8); //min(255, (16*10^6)/(Hz0*64)-1);// = (16*10^6) / (2000*64) - 1 (must be <256)
    // turn on CTC mode
    TCCR0A |= (1 << WGM01);
    // Set CS01 and CS00 bits for 64 prescaler
    TCCR0B |= (1 << CS01) | (1 << CS00);   
    // enable timer compare interrupt
    TIMSK0 |= (1 << OCIE0A); 
  }
  
  if (Hz1 > 0) {
    //set timer1 interrupt at Hz1 Hz, with 1024 prescaler
    TCCR1A = 0;// set entire TCCR1A register to 0
    TCCR1B = 0;// same for TCCR1B
    TCNT1  = 0;//initialize counter value to 0
    // set compare match register for 1hz increments
    OCR1A = get_ocr(Hz1,1024,16); // min(65535, (16*10^6)/(Hz1*1024) - 1);// = (16*10^6) / (1*1024) - 1 (must be <65536)
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS10 and CS12 bits for 1024 prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10);  
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
  }
  
  if (Hz2 > 0) {
    ////set timer2 interrupt at Hz2 Hz, with 8 prescaler
    TCCR2A = 0;// set entire TCCR2A register to 0
    TCCR2B = 0;// same for TCCR2B
    TCNT2  = 0;//initialize counter value to 0
    // set compare match register for 8khz increments
    OCR2A = get_ocr(Hz2, 8, 8);// min(255, round((16.0*pow(10.0,6))/(Hz2*8.0)-1));// = (16*10^6) / (8000*8) - 1 (must be <256)
    // turn on CTC mode
    TCCR2A |= (1 << WGM21);
    // Set CS21 bit for 8 prescaler
    TCCR2B |= (1 << CS21);   
    // enable timer compare interrupt
    TIMSK2 |= (1 << OCIE2A);
  }
  
  sei();//allow interrupts
}

void setup() {
//  Serial.begin(9600);
  
  pinMode(buttonPin1, INPUT_PULLUP);  
  pinMode(buttonPin2, INPUT_PULLUP);  
  attachInterrupt(1,change_mode,RISING);
  
  setup_interrupts(-1,1,200000);
//  Serial.println("Interrupts done.");

  pinMode(ledPin, OUTPUT);
  
  int i;
  for (i = 6; i < 14; i++) {
    pinMode(i, OUTPUT);
  } 
}

void writeByte(int x) {
    int pin;
    byte y = reverseByte(x);
    // write 6 MSBs of x to PORTB, pins 8-13
    PORTB = y >> 2;
    // write 2 LSBs of x to the MSB of PORTD, pins 7 and 6.
    // leave pins 0-5 alone.
    PORTD &= (B00111111 | (x << 6));
}

// Lazy byte reverse.
byte reverseByte(byte x) {
  // The arduino is too frustrating to do tricky bit twiddling
  byte result = 0;
  int i;
  for (i = 0; i<4; i++) {
    result |= (x & (1 << i)) << (7-2*i);
    result |= (x & (1 << 7-i)) >> (7-2*i);
  }
  return result;
}

// blink the led (at mode Hz)
boolean ledState = true;
ISR(TIMER1_COMPA_vect){
    ledState = !ledState;
    digitalWrite(ledPin,ledState);
}

// Increment the counter (at freq[mode] Hz)
ISR(TIMER2_COMPA_vect){ 
  counter += stride;
  if ((counter > high)){
    counter = low;
  }
}

void loop() {
  if (new_mode) {
    new_mode = false;
    change_ocr(1,mode);
    change_ocr(2,freqs[mode]*samples);
  }
  int button1 = digitalRead(buttonPin1);
  if (button1) return;
  
  writeByte(counter);
}