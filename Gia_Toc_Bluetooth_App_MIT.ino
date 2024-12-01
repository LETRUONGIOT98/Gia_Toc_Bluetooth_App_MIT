#include <Wire.h>
#include <MPU6050.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#include <SoftwareSerial.h> // Thêm thư viện SoftwareSerial

MPU6050 mpu;
SoftwareSerial BTSerial(10, 11); // RX, TX cho Bluetooth HC-05

// Chọn mức độ nhạy
#define ACCEL_SCALE 0 // 0: ±2g, 1: ±4g, 2: ±8g, 3: ±16g

void setup() {
  Serial.begin(9600); // Kết nối với Serial Monitor
  BTSerial.begin(9600); // Khởi động Bluetooth HC-05 với tốc độ 9600 bps
  Wire.begin();
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  // Khởi động MPU6050
  mpu.initialize();

  // Cấu hình độ nhạy cho gia tốc
  mpu.setFullScaleAccelRange(ACCEL_SCALE);
}

void loop() {
  // Đọc gia tốc
  int16_t ax, ay, az;
  
  mpu.getAcceleration(&ax, &ay, &az);

  // Chuyển đổi giá trị gia tốc từ raw sang g
  float accelX = ax / 16384.0; // Đối với ±2g
  float accelY = ay / 16384.0; // Đối với ±2g
  float accelZ = az / 16384.0; // Đối với ±2g

  // Điều chỉnh độ nhạy
  switch (ACCEL_SCALE) {
    case 1: // ±4g
      accelX = ax / 8192.0;
      accelY = ay / 8192.0;
      accelZ = az / 8192.0;
      break;
    case 2: // ±8g
      accelX = ax / 4096.0;
      accelY = ay / 4096.0;
      accelZ = az / 4096.0;
      break;
    case 3: // ±16g
      accelX = ax / 2048.0;
      accelY = ay / 2048.0;
      accelZ = az / 2048.0;
      break;
    default: // ±2g
      // Không cần thay đổi vì đã mặc định ở trên
      break;
  }

  // In ra giá trị gia tốc lên Serial Monitor
  /*Serial.print("Trục X ");
  Serial.print(accelX);
  Serial.print("g, Y: ");
  Serial.print(accelY);
  Serial.print("g, Z: ");
  Serial.print(accelZ);
  Serial.println("g");
*/
  // Tính gia tốc tổng hợp
  float totalAccel = sqrt(accelX * accelX + accelY * accelY + accelZ * accelZ);
  
  // Đưa ra thông báo theo mức độ
  String message;
  if (totalAccel > 16.0) {
    message = "   NGUY HIEM    ";
  } else if (totalAccel > 8.0) {
    message = "  TANG QUA CAO  ";
  } else if (totalAccel > 4.0) {
    message = "    TANG CAO    ";
  } else if (totalAccel > 2.0) {
    message = "    TANG VUA    ";
  } else if (totalAccel > 1.0) {
    message = "    TANG NHE    ";
  } else {
    message = "   BINH THUONG  ";
  }
  lcd.setCursor(0, 0);
  lcd.print("Accel: ");
  lcd.setCursor(9, 0);
  lcd.print(totalAccel);
  lcd.setCursor(0, 1);
  lcd.print(message);
  // In ra thông báo lên Serial Monitor
  
  
  String data = String(accelX,2) + " " + String(accelY,2) + " " + String(accelZ,2);
  
  // Gửi dữ liệu qua Bluetooth
  
//Serial.println(data); // In ra dữ liệu
int dataLength = data.length(); // Độ dài chuỗi tính bằng số ký tự

  // Gửi độ dài chuỗi và kích thước của biến dataLength
  BTSerial.print(dataLength + sizeof(dataLength)+1); // Gửi tổng độ dài chuỗi + kích thước của dataLength
  BTSerial.print(" "); // Gửi khoảng trắng để phân tách
  BTSerial.println(data); // Gửi chuỗi dữ liệu

  // In ra chuỗi và độ dài
  Serial.print("Chuỗi: ");
  Serial.println(data); // In ra chuỗi
  Serial.print("Độ dài chuỗi (byte): ");
  Serial.println(dataLength + sizeof(dataLength)); // In ra tổng độ dài

  delay(100); // Gửi dữ liệu mỗi giây
 
   
    // In ra chuỗi và độ dài của nó
    //Serial.print("Chuỗi: ");
    //Serial.println(data); // In ra chuỗi 
  /*
 BTSerial.write((byte *)&accelX, sizeof(float)); // Gửi accelX dưới dạng byte
BTSerial.write((byte *)&accelY, sizeof(float)); // Gửi accelY dưới dạng byte
BTSerial.write((byte *)&accelZ, sizeof(float)); // Gửi accelZ dưới dạng byte
*/

  /*BTSerial.print("Trục X ");
  BTSerial.print(accelX);
  BTSerial.print("g, Y: ");
  BTSerial.print(accelY);
  BTSerial.print("g, Z: ");
  BTSerial.print(accelZ);
  BTSerial.println("g");/
  // Gửi thông điệp qua Bluetooth*/ 
}
