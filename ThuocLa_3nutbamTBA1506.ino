/*
   Relay Đẩy vào: Chân 3
   Step: PUL+ DIR+ ENA+ : 5V
         PUL-: Chân 5, DIR-: Chân 6
         GND: GND
         ENA-: Chân 10
         Nếu cảm thấy quay bị ngược chiều thì đổi lại cặp A sang B, B sang A
   Nút ấn: Chân 4 . Làm nút ấn ra cái board test lần trước anh làm đấy nhé.
   Cảm biến Tiệm Cận: Chân 7
   Relay: Chân tín hiệu: 8
          Dùng chân COM và chân NO.
   Cảm biến Laser: Chân tín hiệu: 9
*/
#define runEvery1(t) for (static long _lasttime;(uint32_t)((uint32_t)micros() - _lasttime) >= (t);_lasttime += (t))
#define runEvery(t) for (static long _lasttime;(uint16_t)((uint16_t)millis() - _lasttime) >= (t);_lasttime += (t))
#include <U8g2lib.h>

#include <Wire.h>
#include <math.h>
#include <stdio.h>

U8G2_ST7920_128X64_F_SW_SPI lcd(U8G2_R0, 23, 17, 16, U8X8_PIN_NONE);


#define DIR_PIN 2
#define STEP_PIN 5
#define Ena 10

// Nut bam tuy chon
#define RESET_PIN 13
#define CHAMTHUOC_PIN 11
#define HUT1_PIN 12

// Nut an
const int button = 14; // 
int dem = 0;
int hientai = 0;
int truoc = 0;

// Cam bien tiệm cận
const int tiemcan = 7;
int gt;

// Công tắc ngắt thuốc
const int laser = 9;
int gt_ls;

// Relay
const int Relay1 = 8; // Đốt
const int Relay2 = 3; // Cuộn hút
double x;
double t; // Thoi gian hoi hut hien tai 
int ngat = 0;
void setup() {
  Wire.begin();

  pinMode(STEP_PIN, OUTPUT); // Step
  pinMode(DIR_PIN, OUTPUT); // Dir
  pinMode(Ena, OUTPUT);
  digitalWrite(Ena, LOW);
  pinMode(button, INPUT);
  pinMode(7, INPUT);

  pinMode(3, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, INPUT);

  pinMode(RESET_PIN, INPUT);
  pinMode(CHAMTHUOC_PIN, INPUT);
  pinMode(HUT1_PIN, INPUT);

  Serial.begin(9600);
  lcd.begin();
  lcd.setCursor(12, 10);
  lcd.setFont(u8g2_font_helvB08_tf);
  lcd.print("SMOKING MACHINE");
  lcd.setFont(u8g2_font_profont12_tf);
  lcd.setCursor(45, 25);
  lcd.print("ADC Club");
  lcd.sendBuffer();
}

double puff = 0;
double puff2;
int het;

int nutReset()
{   
    int now = 0;
    now = digitalRead(RESET_PIN);
    if(now == HIGH)
    {
      return 1;
    }
    else
    {
      return 0;
    }
}
int nutChamthuoc()
{
    int now = 0;
    now = digitalRead(CHAMTHUOC_PIN);
    if(now == HIGH)
    {
      return 1;
    }
    else
    {
      return 0;
    }
}
int nutHut1()
{   
    int now = 0;
    now = digitalRead(HUT1_PIN);
    if(now == HIGH)
    {
      return 1;
    }
    else
    {
      return 0;
    }
}
void rotate_hut(int steps) { // Hut 1100 steps

  int dir = (steps > 0) ? HIGH : LOW;
  steps = abs(steps);
  digitalWrite(Ena, HIGH);
  digitalWrite(DIR_PIN, dir);

  for (int i = 1; i <= steps; i++) {
    digitalWrite(STEP_PIN, HIGH);
    x = 0.0095 * i * i - 10.22 * i + 3000;
    int y = (int)(x);
    delayMicroseconds(y);

    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(600);
    t = t + (y + 605) * 0.000001;
    if (i % 110 == 0 && i <= steps)
    {
      gt_ls = digitalRead(laser);
      if (gt_ls == 1)
      {
        het = i;
        i = steps;
      }
    puff = puff + 0.1;
    if(i == steps)
      {
        lcd.clear();
        lcd.setFont(u8g2_font_profont12_tf);
        lcd.setCursor(0, 10);
        lcd.print("Status: ");
        lcd.setCursor(0, 22);
        lcd.print("Puff Num:");
        lcd.setCursor(0, 34);
        lcd.print("V_flow: "); lcd.print("00 "); lcd.print("ml/s");
        lcd.setCursor(0, 46);
        lcd.print("Air flow: "); lcd.print("00 "); lcd.print("ml/s");
        lcd.setCursor(0, 58);
        lcd.print("Pressure drop: "); lcd.print("00 ") ; lcd.print("Pa");
      lcd.setCursor(65, 22);
      lcd.print(puff,1);
      lcd.setCursor(90, 22);
      lcd.print("/");
      lcd.setCursor(102, 22);
      lcd.print(t,2);
      lcd.sendBuffer();
      t=0;
      }
    }

  }
}
void rotate_rs(int steps) {

  int dir = (steps > 0) ? HIGH : LOW;
  steps = abs(steps);
  digitalWrite(Ena, HIGH);
  digitalWrite(DIR_PIN, dir);

  for (int i = 0; i <= steps; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(800);

    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(500);
  }
}
void rotate_day(int steps) {

  int dir = (steps > 0) ? HIGH : LOW;
  steps = abs(steps);
  digitalWrite(Ena, HIGH);
  digitalWrite(DIR_PIN, dir);

  for (int i = 0; i <= steps; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(800);

    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(500);
  }
  digitalWrite(Ena, LOW);
}


void loop()
{
  runEvery(200)
  {
    if(dem==0 || dem%2==1)
  {
    hientai = digitalRead(button);// Kiem tra cong tac Start duoc bat hay ko  truoc = 0
    if (hientai != truoc)
    {
      if (hientai == HIGH) // Bam nut lan dau hien thi cac thong so tren man hinh
      {
        dem++;
        lcd.clear();
        lcd.setFont(u8g2_font_profont12_tf);
        lcd.setCursor(0, 10);
        lcd.print("Status: ");
        lcd.setCursor(0, 22);
        lcd.print("Puff num:");
        lcd.setCursor(0, 34);
        lcd.print("V_flow: "); lcd.print("00 "); lcd.print("ml/s");
        lcd.setCursor(0, 46);
        lcd.print("Air flow: "); lcd.print("00 "); lcd.print("ml/s");
        lcd.setCursor(0, 58);
        lcd.print("Pressure drop: "); lcd.print("00 ") ; lcd.print("Pa");
        lcd.sendBuffer();
       }
      else
      {
      }
      truoc = hientai; // Nut start dang nhan giu 
    }
  }
  }

    // Xét điểm bắt đầu khi ấn nút lần 1, thi se reset may
    if (dem == 1)
    {
      gt = digitalRead(tiemcan);
      if (gt == 1)
      {
        rotate_rs(200);
      }
      else if (gt == 0)
      {
        rotate_day(-1950);
        delay(500);
        dem += 1;
      }
    }

    if (dem >= 2 && dem % 2 == 0)
    {
      if(ngat == 0)
      {
        // Quay servo vào để đốt
        digitalWrite(Relay1, HIGH);
        delay(1500);
        digitalWrite(Relay2, HIGH);
        delay(1000);
        lcd.setCursor(50, 10);
        lcd.print(" ... ");
        lcd.sendBuffer();
        ngat = 1;
      }
      
      rotate_hut(1100);
      
      if (ngat == 1)
      {
        digitalWrite(Relay1, LOW);
        digitalWrite(Relay2, LOW);
        ngat = 2;
      }
      delay(2000);
      // Kiểm tra hút hết thuốc chưa.
      if (gt_ls == 1) // Cam bien Laser set len 1 
      {
        rotate_day(-(het));
        het = 0;
        dem += 1;
        puff = 0.0;
        ngat = 0;
        lcd.setCursor(50, 10);
        lcd.print(" Finish ");
        lcd.sendBuffer();
      }
      else
      {
        rotate_day(-1100);
        for(int tg=1; tg<=53; tg++)
        {
            gt_ls = digitalRead(laser);
            if (gt_ls == 1)
            {
              puff = 0.0;
              ngat = 0;
              lcd.setCursor(50, 10);
              lcd.print(" Finish ");
              lcd.sendBuffer();
              dem+=1;
              tg=53;
            }
            else {
              delay(1000);
            }
        }
        delay(1000);
      }
    }

    runEvery(200) // CHeck nut Reset va lam sach hoi hut
    {
      if(nutReset() == 1)    // Nút Reset
      {
        lcd.clear();
        lcd.setFont(u8g2_font_profont12_tf);
        lcd.setCursor(0, 10);
        lcd.print("Status: ");
        lcd.setCursor(0, 22);
        lcd.print("Puff num:");
        lcd.setCursor(0, 34);
        lcd.print("V_flow: "); lcd.print("00 "); lcd.print("ml/s");
        lcd.setCursor(0, 46);
        lcd.print("Air flow: "); lcd.print("00 "); lcd.print("ml/s");
        lcd.setCursor(0, 58);
        lcd.print("Pressure drop: "); lcd.print("00 ") ; lcd.print("Pa");
        lcd.sendBuffer();
        
        dem = 3;
        puff = 0;
        digitalWrite(Relay1, LOW);
        digitalWrite(Relay2, LOW);
        for(int Home=1; Home<20; Home++)// Kiem tra moi 200 xung mot lan de xem Xi lanh nhan cam bien hay chua
        {
          gt = digitalRead(tiemcan);
          if (gt == 1)
          {
            rotate_rs(200);
          }
          else if (gt == 0)// Cham cam bien can duoi thi day xi lanh len het muc 
          {
            rotate_day(-1950);
            Home = 20;
            delay(500);
          }
        }
     }

     if(nutHut1() == 1)     // Nút hút 1 hơi làm sạch xi lanh.
     {
        rotate_day(1900);
        delay(1000);
        rotate_day(-1900);
     }

  }
}
