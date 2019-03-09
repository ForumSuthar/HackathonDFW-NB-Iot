#include <board.h>
#include <BreakoutSDK.h>
// Install https://github.com/Seeed-Studio/Grove_Ultrasonic_Ranger
#include <Ultrasonic.h>

/** Change this to your device purpose */
static const char *device_purpose = "Dev-Kit";
/** Change this to your key for the SIM card inserted in this device
    You can find your PSK under the Breakout SDK tab of your Narrowband SIM detail at
    https://www.twilio.com/console/wireless/sims
*/
static const char *psk_key = "fd004093c57f8225102c556e2ca7fd2c";

/** This is the Breakout SDK top API */
Breakout *breakout = &Breakout::getInstance();

#define ULTRASONIC_PIN  (38)
#define INTERVAL        (1000)

Ultrasonic UltrasonicRanger(ULTRASONIC_PIN);

bool timeflag;
unsigned long timestart;

/**
   Setting up the Arduino platform. This is executed once, at reset.
*/
void setup() {

  pinMode(38, INPUT);

  // Feel free to change the log verbosity. E.g. from most critical to most verbose:
  //   - errors: L_ALERT, L_CRIT, L_ERR, L_ISSUE
  //   - warnings: L_WARN, L_NOTICE
  //   - information & debug: L_INFO, L_DB, L_DBG, L_MEM
  // When logging, the additional L_CLI level ensure that the output will always be visible, no matter the set level.
  owl_log_set_level(L_INFO);
  LOG(L_WARN, "Arduino setup() starting up\r\n");

  // Set the Breakout SDK parameters
  breakout->setPurpose(device_purpose);
  breakout->setPSKKey(psk_key);
  breakout->setPollingInterval(1 * 1);  // Optional, by default set to 10 minutes

  // Powering the modem and starting up the SDK
  LOG(L_WARN, "Powering on module and registering...");
  breakout->powerModuleOn();

  LOG(L_WARN, "... done powering on and registering.\r\n");
  LOG(L_WARN, "Arduino loop() starting up\r\n");
}


/**
   This is just a simple example to send a command and write out the status to the console.
*/

void sendCommand(const char * command) {
  if (breakout->sendTextCommand(command) == COMMAND_STATUS_OK) {
    LOG(L_INFO, "Tx-Command [%s]\r\n", command);
  } else {
    LOG(L_INFO, "Tx-Command ERROR\r\n");
  }
}

void loop()
{
  long distance;
  distance = UltrasonicRanger.MeasureInCentimeters();
  while (distance < 10) {

    if (timeflag == false)
    {
      timestart = millis();
      timeflag = true;
    }
    distance = UltrasonicRanger.MeasureInCentimeters();

    if ( (millis() - timestart) > 15000) {
      break;
    }
  }
  if (timeflag == true) {
    if ( (millis() - timestart ) > 10000) {
      char message[] ="{\"id\":2,\"severity\":\"High\"}";
      sendCommand(message);
      LOG(L_INFO, "2:High" );
    } else if ((millis() - timestart ) > 5000) {
      char message[] = "{\"id\":2,\"severity\":\"Medium\"}";
      sendCommand(message);
      LOG(L_INFO, "2:Medium" );
    }
    timeflag = false;
  }
  breakout->spin();

  //delay(INTERVAL);
}
