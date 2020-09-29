#define RpwmKanan 5
#define LpwmKanan 6
#define RpwmKiri 9
#define LpwmKiri 10

#include <PPMReader.h>

int interruptPin = 2;
int channelAmount = 6;
int channel_1 = 1;
int channel_2 = 2;
int channel_3 = 3;
int channel_4 = 4;
int channel_5 = 5;
int channel_6 = 6;
//int min1 = 9; //aktifkan motor1 port 1
//int min2 = 10; //aktifkan motor1 port 2
//int men1 = 11; //port 3 enable  motor
//int men2 = 8; //port 4 enable motor
//int pwminput = 13; //pwm input

int pulseKiri;
int pulseKanan;
int Saklar;
unsigned long val2;
unsigned long val4;
unsigned long val5;
unsigned long val6;
const int deadzone = 20;
PPMReader ppm(interruptPin, channelAmount);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //  pinMode(min1, OUTPUT);
  //  pinMode(min2, OUTPUT);
  //  pinMode(men1, OUTPUT);
  //  pinMode(men2, OUTPUT);
  //  pinMode(pwminput, OUTPUT);
  pinMode(RpwmKanan, OUTPUT);
  pinMode(LpwmKanan, OUTPUT);
  pinMode(RpwmKiri, OUTPUT);
  pinMode(LpwmKiri, OUTPUT);
}

void loop() {
  // inisialisasi variable channel
  val2 = ppm.latestValidChannelValue(channel_2, 0);
  val4 = ppm.latestValidChannelValue(channel_4, 0);
  val5 = ppm.latestValidChannelValue(channel_5, 0);
  val6 = ppm.latestValidChannelValue(channel_6, 0);

  // mode cepat tidak dapat dikendalikan
  if (val6 > 1600) {
    pulseKanan = map(val2, 1000, 2000, -255, 255);
    pulseKiri = map(val4, 1000, 2000, -255, 255);
    if (abs(pulseKanan) <= deadzone) {
      pulseKanan = 0;
      stopMotor();
    }
    if (abs(pulseKiri) <= deadzone) {
      pulseKiri = 0;
      stopMotor();

    }
  }

  // mode cepat bisa dikendalikan
  else if (val6 < 1400) {
    pulseKanan = map(val2, 1000, 2000, -255, 255);
    pulseKiri = map(val4, 1000, 2000, -255, 255);
    if (abs(pulseKanan) <= deadzone) {
      pulseKanan = 0;
      stopMotor();
    }
    if (abs(pulseKiri) <= deadzone) {
      pulseKiri = 0;
      stopMotor();
    }  
  }
  
  // mode lambat bisa dikendalikan
  else if (val6 > 1400 && val6 < 1600){
    pulseKanan = map(val2, 1000, 2000, -120, 120);
    pulseKiri = map(val4, 1000, 2000, -120, 120);
    if (abs(pulseKanan) <= deadzone) {
      pulseKanan = 0;
      stopMotor();
    }
    if (abs(pulseKiri) <= deadzone) {
      pulseKiri = 0;
      stopMotor();
    }  
  }
  if (val5 < 1500) {
    pulseKanan = 0;
    pulseKiri = 0;
    stopMotor();
  }

  if (pulseKanan > 0) {
    maju();
  }
  if (pulseKanan < 0) {
    mundur();
  }
  if (pulseKiri > 0) {
    if(val6 < 1400) {
      pulseKiri = 30;
    }
    kanan();
  }
  if (pulseKiri < 0) {
    if(val6 < 1400) {
      pulseKiri = -30;
    }
    kiri();
  }
  Serial.print(val2); Serial.print(" ");
  Serial.print(val4); Serial.print(" ");
  Serial.print(val5); Serial.print(" ");
  Serial.print(val6); Serial.print(" ");
  Serial.println();
  //  Serial.print(pulseKanan);
  //  Serial.print(" ");
  //  Serial.print(pulseKiri);
  //  Serial.println();
  //  Serial.print(val6);
  //    Serial.println();
}

void maju() {
  analogWrite(RpwmKanan, abs(pulseKanan));
  analogWrite(LpwmKiri, abs(pulseKanan));
}

void mundur() {
  analogWrite(LpwmKanan, abs(pulseKanan));
  analogWrite(RpwmKiri, abs(pulseKanan));
}

void kiri() {
  analogWrite(RpwmKanan, abs(pulseKiri));
  analogWrite(RpwmKiri, abs(pulseKiri));
}
void kanan() {
  analogWrite(LpwmKanan, abs(pulseKiri));
  analogWrite(LpwmKiri, abs(pulseKiri));
}

void stopMotor() {
  analogWrite(RpwmKanan, 0);
  analogWrite(LpwmKanan, 0);
  analogWrite(RpwmKiri, 0);
  analogWrite(LpwmKiri, 0);
}
