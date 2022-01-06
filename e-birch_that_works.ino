//modem pins
#define SIM800L_RX     27
#define SIM800L_TX     26
#define SIM800L_PWRKEY 4
#define SIM800L_RST    5
#define SIM800L_POWER  23

//sensors

#define FLOOD_PIN 12

//GPRS values
String apn = "iot.1nce.net";
String commands[] = {"AT", "ATE0", "AT+CGATT=1", "AT+CSTT", "AT+CIICR", "AT+CIFSR", "AT+CIPSTART=\"UDP\",", "AT+CIPSEND=" , "", "AT+CIPSHUT" };
#define endpoint "udp.connectivity-suite.cloud"
#define port 4445


byte len = 0;

//sleep stuff
#define uS_TO_S_FACTOR 1000000ULL   /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  600       /* Time ESP32 will go to sleep (in seconds) */

void ShowSerialData()
{
  while (Serial2.available() != 0)
    Serial.write(Serial2.read());
}

void setup()
{
  pinMode(SIM800L_POWER, OUTPUT);
  digitalWrite(SIM800L_POWER, HIGH);

  Serial.begin(115200);

  Serial2.begin(115200, SERIAL_8N1, SIM800L_TX, SIM800L_RX);
  Serial.println("ESP32+SIM800L AT");
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  commands[3] = commands[3] + "=\"" + apn + "\""; //define apn into commands variable
  commands[6] = commands[6] + "\"" + endpoint + "\",\"" + port + "\""; //define endpoint and port
  
}


void loop() {
  
  Serial.print("AT > ");
  for (int i = 0; i < 10; i++) //do NOT ask questions about this
    { Serial2.print("AT\r\n");
    delay(1000);
    }
  for (int i = 0; i < 10; i++)
    {Serial2.print(String(commands[i]+"\r\n"));
    Serial.print(String(commands[i]+"\r\n"));
    ShowSerialData();
    delay(3000);
    }
  Serial.println("done");
  digitalWrite(SIM800L_POWER, LOW); //turn off modem
  esp_deep_sleep_start(); //good night
}
