#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "driver/i2s.h"

#define I2S_BCK 14
#define I2S_WS  27
#define I2S_SD  32
#define I2S_DIN 33

uint8_t targetAddress[] = {0xF0, 0x24, 0xF9, 0x46, 0x05, 0xC4};

struct Telemetry { float unghi; float fortaG; bool accident; } dateLocale, datePrimite;
Adafruit_MPU6050 mpu;
unsigned long lastSensor = 0;
float tiltOffset = 0;
bool sosActive = false;

void playAlarm() {
  int16_t buzzer[128];
  for(int i=0; i<20; i++) {
    for(int j=0; j<128; j++) buzzer[j] = (j % 12 < 6) ? 15000 : -15000;
    size_t w; i2s_write(I2S_NUM_0, buzzer, sizeof(buzzer), &w, portMAX_DELAY);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
  if (esp_now_init() != ESP_OK) return;

  esp_now_register_recv_cb([](const esp_now_recv_info *info, const uint8_t *data, int len) {
    if (len == sizeof(Telemetry)) {
      memcpy(&datePrimite, data, len);
      if(datePrimite.accident) sosActive = true;
    } else {
      size_t w; i2s_write(I2S_NUM_0, data, len, &w, 0);
    }
  });

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, targetAddress, 6);
  peerInfo.channel = 1;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);

  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .dma_buf_count = 8, .dma_buf_len = 64
  };

  i2s_pin_config_t pin_config = { .bck_io_num = 14, .ws_io_num = 27, .data_out_num = 33, .data_in_num = 32 };
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);

  Wire.begin(21, 22);
  if(mpu.begin()) {
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    sensors_event_t a, g, t; mpu.getEvent(&a, &g, &t);
    tiltOffset = atan2(a.acceleration.x, a.acceleration.y) * 180 / PI;
  }
}

void loop() {
  if(sosActive) { playAlarm(); sosActive = false; }
  int16_t samples[128];
  size_t bytes_read = 0;
  i2s_read(I2S_NUM_0, &samples, sizeof(samples), &bytes_read, 10);
  if (bytes_read > 0) esp_now_send(targetAddress, (uint8_t *)samples, bytes_read);

  if (millis() - lastSensor > 250) {
    sensors_event_t a, g, t; mpu.getEvent(&a, &g, &t);
    float gForce = sqrt(pow(a.acceleration.x, 2) + pow(a.acceleration.y, 2) + pow(a.acceleration.z, 2)) / 9.81;
    float angle = -(atan2(a.acceleration.x, a.acceleration.y) * 180 / PI - tiltOffset);
    dateLocale.unghi = angle; dateLocale.fortaG = gForce;
    dateLocale.accident = (gForce > 2.0);
    Serial.printf("LOCAL -> G: %.2f | Unghi: %.1f\n", gForce, angle);
    esp_now_send(targetAddress, (uint8_t *)&dateLocale, sizeof(dateLocale));
    lastSensor = millis();
  }
}