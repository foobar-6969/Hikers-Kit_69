#ifndef DHT_H
#define DHT_H

#define DHT_OK 0
#define DHT_TIMEOUT_ERR -1
#define DHT_CHECKSUM_ERR 1

int dht_humidity = 0;
int dht_temperature_int = 0;
float dht_temperature = 0.0;

int dht_read(int pin)
{
  uint8_t bytes[5];
  int bit_place = 7;
  int byte_no = 0;
  

  //Empty data buffer
  for(size_t i = 0;i<5;i++){
    bytes[i] = 0;
  }

  //Request sample
  pinMode(pin,OUTPUT);
  digitalWrite(pin,LOW);
  delay(18);
  digitalWrite(pin,HIGH);
  delayMicroseconds(40);
  pinMode(pin,INPUT);

  //Sensor ACK
  unsigned int loop_cnt = 10000;
  while(digitalRead(pin) == LOW){
    if(loop_cnt > 0){
      loop_cnt--;
    }else{
      return DHT_TIMEOUT_ERR;
    }
  }
  loop_cnt = 10000;
  while(digitalRead(pin) == HIGH){
    if(loop_cnt > 0){
      loop_cnt--;
    }else{
      return DHT_TIMEOUT_ERR;
    }
  }


  //Data Recv
  for(size_t i = 0;i<40;i++){
    loop_cnt = 10000;
    while(digitalRead(pin) == LOW){
      if(loop_cnt > 0){
        loop_cnt--;
      }else{
        return DHT_TIMEOUT_ERR;
      }
    }

    unsigned long t1 = micros();
    loop_cnt = 10000;
    while(digitalRead(pin) == HIGH){
      if(loop_cnt > 0){
        loop_cnt--;
      }else{
        return DHT_TIMEOUT_ERR;
      }
    }
    unsigned long time_diff  = micros() - t1;

    if(time_diff > 40){
      bytes[byte_no] |= (1<<bit_place);
    }
    if(bit_place == 0){
      bit_place = 7;
      byte_no++;
    }else{
      bit_place--;
    }
  }

  //Data Representation
  dht_humidity = bytes[0];
  dht_temperature_int = bytes[2];
  dht_temperature = dht_temperature_int + 0.1*bytes[3];
  uint8_t check_sum = bytes[0] + bytes[2] + bytes[1] + bytes[3];
  if(check_sum == bytes[4]){
    return DHT_OK;  
  }else{
    return DHT_CHECKSUM_ERR;
  }
}

#endif
