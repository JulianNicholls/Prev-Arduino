
#define BOTH_PINS_BITMASK         0x300000000    // 2^33 & 2^32 in hex
#define TOP_ONLY_BITMASK          0x200000000    // 2^33 in hex

#define BOTTOM_BIT                (1 << 14)
#define TOP_BIT                   (1 << 15)

#define uS_TO_S_FACTOR            1000000ULL     // Conversion factor for micro seconds to seconds */
#define DEFAULT_SLEEP_TIME        86400LL        // Default wakeup once a day

#define SLEEP_TIME_AFTER_TOP      10 * 60        // Initial top breach, 10 Minutes
#define SLEEP_TIME_AFTER_TOP2     5 * 60         // Second top breach, 5 Minutes
#define SLEEP_TIME_AFTER_BOTTOM   20 * 60        // Bottom breach, 20 Minutes

RTC_DATA_ATTR int bootData = 0;

// Print the reason why we have woken from sleep
void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT1: 
      Serial.println("Wakeup caused by EXT1 using RTC_CNTL"); 
      break;
      
    case ESP_SLEEP_WAKEUP_TIMER:
      Serial.println("Wakeup caused by Timer"); 
      break;

    case ESP_SLEEP_WAKEUP_EXT0:
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
    case ESP_SLEEP_WAKEUP_ULP:
      Serial.printf("Unexpected Wakeup: %d\n", wakeup_reason);
      break;
       
    default: 
      Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); 
      break;
  }
}

void setup() 
{
  // Set up and read the two inputs
  pinMode(32, INPUT);   // Bottom input
  pinMode(33, INPUT);   // Top input

  int bottom = digitalRead(32);
  int top    = digitalRead(33);
  
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1);
  delay(1000);          // Take some time to open up the Serial Monitor

  // Increment boot number and print it every reboot
  ++bootData;
  Serial.printf("\nBoot data: %04x\n\n", bootData);

  // Print the wakeup reason
  print_wakeup_reason();

  Serial.printf("Bottom: %02d, Top: %02d\n\n", digitalRead(32), digitalRead(33));

  long int      sleepTime = DEFAULT_SLEEP_TIME;
  const char*   signal;
  long long     bitMask = 0;    // Don't wake on inputs
  
  if (top) {                    // Breached  
    if (bootData & TOP_BIT) {   // Breached last time as well
      signal = "030100000004";  // Signal breached top, multiple
      Serial.println("TOP breached, MULTIPLE times");
      sleepTime = SLEEP_TIME_AFTER_TOP2;
      // bitMask = 0;   // Unnecessary
    }
    else {                      
      signal = "010100000002";    // Signal breached top, first time
      Serial.println("TOP breached, first time");
      sleepTime = SLEEP_TIME_AFTER_TOP;       
      // bitMask = 0;   // Unnecessary
    }
  }
  else if (bottom) {                  // Gutter is flowing
    signal    = "000100000001";       // Signal gutter is flowing
    Serial.println("BOTTOM breached");
    sleepTime = SLEEP_TIME_AFTER_BOTTOM;
    bitMask   = TOP_ONLY_BITMASK;     // Wake if top is breached
  }
  else {                              // Wakeup was Timer
    signal    = "000000000000";       // Signal all is well
    sleepTime = DEFAULT_SLEEP_TIME;   // and wait until tomorrow
    bitMask   = BOTH_PINS_BITMASK;    // Wake on either input
  }

  // Set status bits for next boot
  bootData = (bootData & 0x0fff) | (bottom << 14) | (top << 15);

  // Send SigFox message
  Serial.printf("Sending '%s'\nReply: ", signal);
  Serial1.printf("AT$SF=%s\r\n", signal);

  // Wait for reply...
  while (millis() < 11000) {
    if(Serial1.available()) {
      Serial.write(Serial1.read());
    }
  }
  
  // Set the wakeup for water inputs, if required
  if (bitMask) {
    esp_sleep_enable_ext1_wakeup(bitMask, ESP_EXT1_WAKEUP_ANY_HIGH);
  }
  
  // Set the timer as a wakeup.
  esp_sleep_enable_timer_wakeup(sleepTime * uS_TO_S_FACTOR);

  Serial.printf("Setting up sleep for %ld seconds.\nGoing to sleep now.\n", sleepTime);
  Serial.flush(); 

  esp_deep_sleep_start();
  
  Serial.println("This will never be printed");
}

void loop()
{
  // This is not going to be called
}
