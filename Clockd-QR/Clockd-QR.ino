#include <WiFi.h>
#include "esp_camera.h"
#include "quirc.h"

#define CAMERA_MODEL_AI_THINKER

#include "camera_pins.h"

struct quirc* qr;

const char* ssid     = "BTHub6-SZ8H";
const char* password = "RwyNcn7qdmqi";

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_GRAYSCALE;
  //init with high specs to pre-allocate larger buffers
//  if (psramFound()) {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 10;
    config.fb_count = 1;
//  } else {
//    config.frame_size = FRAMESIZE_SVGA;
//    config.jpeg_quality = 12;
//    config.fb_count = 1;
//  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    //    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  //initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);//flip it back
    s->set_brightness(s, 1);//up the blightness just a bit
    s->set_saturation(s, -2);//lower the saturation
  }
  //drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);

  Serial.print("Connecting to WiFi ");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");

  // Initialise QR code library

  qr = quirc_new();

  if (!qr) {
    Serial.println("Could not allocate quirc struct");
    return;
  }
  else {
    Serial.println("Allocated quirc struct");
  }

  if (quirc_resize(qr, 320, 240) < 0) {
    Serial.println("Could not allocate video memory");
    return;
  }
  else {
    Serial.println("Allocated video memory");
  }
}

camera_fb_t * fb = NULL;
struct quirc_code code;
struct quirc_data data;

void loop() {
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
  }
  else {
    Serial.printf("%d bytes from camera\n", fb->len);

    unsigned char* image;
    int w, h;

    Serial.println("Sending to Quirc");
    image = quirc_begin(qr, &w, &h);
    memcpy(image, fb->buf, fb->len);
    
    Serial.println("Calling quirc_end()");
    quirc_end(qr);

    int num_codes = quirc_count(qr);
    Serial.printf("Quirc returned: %d codes\n", num_codes);

//    for(int i = 0; i < num_codes; ++i) {
//      quirc_decode_error_t err;
//  
//      quirc_extract(qr, i, &code);
//  
//      /* Decoding stage */
//      err = quirc_decode(&code, &data);
//      if (err)
//          Serial.printf("DECODE FAILED: %s\n", quirc_strerror(err));
//      else
//          Serial.printf("Data: %s\n", data.payload);
//    }
  }
  
  esp_camera_fb_return(fb);
}

void debug_println(char* s) {
  Serial.println(s);
}
