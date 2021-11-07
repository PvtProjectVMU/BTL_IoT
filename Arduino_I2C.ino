#include <Wire.h>
#include <AccelStepper.h>

#define sensor 10
#define led 8
int temp_manual = 0;
int temp = 0;

//Định nghĩa chân động cơ:
// IN1 trên trình điều khiển ULN2003
#define motorPin1  4  

// IN2 trên trình điều khiển ULN2003    
#define motorPin2  5

// IN3 trên trình điều khiển ULN2003  
#define motorPin3  6   

// IN4 trên trình điều khiển ULN2003
#define motorPin4  7

// Xác định kiểu giao diện AccelStepper; Động cơ 4 dây ở chế độ nửa bước:
#define MotorInterfaceType 8

// Khởi tạo với trình tự chân IN1-IN3-IN2-IN4 để sử dụng thư viện AccelStepper với động cơ bước 28BYJ-48:
AccelStepper stepper = AccelStepper(MotorInterfaceType, motorPin1, motorPin3, motorPin2, motorPin4);


void setup() {
  // Cài đầu vào
  pinMode(sensor,INPUT);
  
  // Đặt số bước tối đa mỗi giây:
  stepper.setMaxSpeed(1000);
  
  // Cài đầu ra
  pinMode(led,OUTPUT);
  pinMode (motorPin1, OUTPUT);  
  pinMode (motorPin2, OUTPUT);  
  pinMode (motorPin3, OUTPUT);  
  pinMode (motorPin4, OUTPUT);  
  
  Wire.begin(8);                /* join i2c bus with address 8 */
  Wire.onReceive(receiveEvent); /* register receive event */
  Wire.onRequest(requestEvent); /* register request event */
  Serial.begin(9600);           /* start serial for debug */
}

void loop() {
  delay(100);
  if(Wire.available()) {
    int c = Wire.read();      /* receive byte as a character */
    if(c==1){
     if(temp_manual==0){
      //motor
      // Đặt lại vị trí thành 0:
      stepper.setCurrentPosition(0);
      // Chạy ngược động cơ với tốc độ 1000 bước / giây cho đến khi động cơ đạt -1024 bước (1/4 vòng quay):
      while (stepper.currentPosition() != 2048) {
        stepper.setSpeed(1000);
        stepper.runSpeed();
      }         
     temp_manual=1;
     }
    }else if(c==0){
      if(temp_manual==1){
        //motor
     // Đặt lại vị trí thành 0:
      stepper.setCurrentPosition(0);
      // Chạy ngược động cơ với tốc độ 1000 bước / giây cho đến khi động cơ đạt -1024 bước (1/4 vòng quay):
      while (stepper.currentPosition() != -2048) {
        stepper.setSpeed(-1000);
        stepper.runSpeed();
      }         
     temp_manual=0;
      }
    }else if(c==2){
      autoMode();
    }
  }
}

// function that executes whenever data is received from master
void receiveEvent(int howMany){}

// function that executes whenever data is requested from master
void requestEvent() {
  if(digitalRead(sensor)==0){
    Wire.write(0);
    digitalWrite(led, LOW);
  }else if(digitalRead(sensor)==1){
    Wire.write(1);
    digitalWrite(led, HIGH);
  }
}
void autoMode(){
  // So sánh giá trị cảm biến nếu là HIGH thì motor chạy chiều dương     
  if(digitalRead(sensor) == 1){                                         
    if(temp == 0){ 
      // Đặt vị trí hiện tại thành 0:
      stepper.setCurrentPosition(0);
    
      // Chạy động cơ về phía trước với tốc độ 1000 bước / giây cho đến khi động cơ đạt 1024 bước (1/4 vòng quay):
      while (stepper.currentPosition() != 2048) {
        stepper.setSpeed(1000);
        stepper.runSpeed();
        }  
        temp = 1;      
     }
  }         
  // So sánh giá trị cảm biến nếu là LOW thì motor chạy chiều âm                
  if(digitalRead(sensor) == 0){ 
    delay(3000);                                       
    if(temp == 1){
      // Đặt lại vị trí thành 0:
      stepper.setCurrentPosition(0);
    
      // Chạy ngược động cơ với tốc độ 1000 bước / giây cho đến khi động cơ đạt -1024 bước (1/4 vòng quay):
      while (stepper.currentPosition() != -2048) {
        stepper.setSpeed(-1000);
        stepper.runSpeed();
      }     
      temp = 0;               
    }
  }
}
