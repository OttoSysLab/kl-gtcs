int cnt = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(13,OUTPUT);   //
}

void loop() {
  // put your main code here, to run repeatedly:
  // 讀取輸入狀態
  char revChar = Serial1.read();

  // 判斷狀態
  if ( revChar=='c'){
       Serial.println("收到資料");   // 執行動作
       digitalWrite(13,HIGH);       // 執行動作
      delay(5000);
  }
  else{           
      digitalWrite(13,LOW);       // 執行動作
      delay(1000);    
  }             
}
