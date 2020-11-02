#include "hal.h"
#include "ch.h"
#include <stdio.h>
#include <chprintf.h>
#include <string.h>
#include <stdbool.h>

#include "winc1500/config/conf_winc.h"
#include "winc1500/driver/include/m2m_wifi.h"
#include "winc1500/driver/source/nmasic.h"
#include "winc1500/driver/include/m2m_wifi.h"
#include "winc1500/driver/source/m2m_hif.h"
#include "winc1500/bsp/include/nm_bsp.h"

#define MAIN_WLAN_SSID        "DIAS" /* < Destination SSID */
#define MAIN_WLAN_AUTH        M2M_WIFI_SEC_WPA_PSK /* < Security manner */
#define MAIN_WLAN_PSK         "diaspass" /* < Password for Destination SSID */

/* Serial configuration. */
static const SerialConfig myserialcfg = {
  115200,
  0,
  USART_CR2_STOP1_BITS,
  0
};

static uint8_t scan_request_index = 0;
static uint8_t num_founded_ap = 0;  /** Number of APs found. */

static void wifi_cb(uint8_t u8MsgType, void *pvMsg)
{
    switch (u8MsgType) {
    case M2M_WIFI_RESP_SCAN_DONE:
    {
        tstrM2mScanDone *pstrInfo = (tstrM2mScanDone *)pvMsg;
        scan_request_index = 0;
        if (pstrInfo->u8NumofCh >= 1) {
            m2m_wifi_req_scan_result(scan_request_index);
            scan_request_index++;
        } else {
            m2m_wifi_request_scan(M2M_WIFI_CH_ALL);
        }
        break;
    }

    case M2M_WIFI_RESP_SCAN_RESULT:
    {
        tstrM2mWifiscanResult *pstrScanResult = (tstrM2mWifiscanResult *)pvMsg;
        uint16_t demo_ssid_len;
        uint16_t scan_ssid_len = strlen((const char *)pstrScanResult->au8SSID);

        myPrintf("[%d] SSID:%s\r\n", scan_request_index, pstrScanResult->au8SSID);  /* display founded AP. */
        num_founded_ap = m2m_wifi_get_num_ap_found();
        if (scan_ssid_len) {
            /* check same SSID. */
            demo_ssid_len = strlen((const char *)MAIN_WLAN_SSID);
            if
            (
                (demo_ssid_len == scan_ssid_len) &&
                (!memcmp(pstrScanResult->au8SSID, (uint8_t *)MAIN_WLAN_SSID, demo_ssid_len))
            ) {
                /* A scan result matches an entry in the preferred AP List.
                 * Initiate a connection request.
                 */
                myPrintf("Found %s \r\n", MAIN_WLAN_SSID);
                m2m_wifi_connect((char *)MAIN_WLAN_SSID,
                        sizeof(MAIN_WLAN_SSID),
                        MAIN_WLAN_AUTH,
                        (void *)MAIN_WLAN_PSK,
                        M2M_WIFI_CH_ALL);
                break;
            }
        }

        if (scan_request_index < num_founded_ap) {
            m2m_wifi_req_scan_result(scan_request_index);
            scan_request_index++;
        } else {
            myPrintf("can not find AP %s\r\n", MAIN_WLAN_SSID);
            m2m_wifi_request_scan(M2M_WIFI_CH_ALL);
        }
        break;
    }

    case M2M_WIFI_RESP_CON_STATE_CHANGED:
    {
        tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged *)pvMsg;
        if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED) {
            m2m_wifi_request_dhcp_client();
        } else if (pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED) {
            myPrintf("Wi-Fi disconnected\r\n");
            m2m_wifi_request_scan(M2M_WIFI_CH_ALL); /* Request scan. */
        }
        break;
    }

    case M2M_WIFI_REQ_DHCP_CONF:
    {
        uint8_t *pu8IPAddress = (uint8_t *)pvMsg;
        myPrintf("Wi-Fi connected\r\n");
        myPrintf("Wi-Fi IP is %u.%u.%u.%u\r\n",
                pu8IPAddress[0], pu8IPAddress[1], pu8IPAddress[2], pu8IPAddress[3]);
        break;
    }

    default:
    {
        break;
    }
    }
}

static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {
  (void)arg;
  while (true) {
    palSetPad(GPIOA, 5);       /* Built-in Nucleo64 STM32F410RB.  */
    chThdSleepMilliseconds(250);
    palClearPad(GPIOA, 5);
    chThdSleepMilliseconds(250);
  }
}

int main(void) {
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  sdStart(&SD2, &myserialcfg);
  tstrWifiInitParam param;
  int8_t ret;
  myPrintf("Bismillah\n");
  myPrintf("initializing bsp_init\n");
  nm_bsp_init();
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  memset((uint8_t *)&param, 0, sizeof(tstrWifiInitParam));

  /* Initialize Wi-Fi driver with data and status callbacks. */
  param.pfAppWifiCb = wifi_cb;
  myPrintf("initializing wifi_init\n");
  ret = m2m_wifi_init(&param);

  if (M2M_SUCCESS != ret) {
      myPrintf("main: m2m_wifi_init call error!(%d)\r\n", ret);
      while (1) {
      }
  }
  myPrintf("initializing wifi_request_scan\n");
  m2m_wifi_request_scan(M2M_WIFI_CH_ALL);   /* Request scan. */

  while (1)
  {
      /* Handle pending events from network controller. */
      while (m2m_wifi_handle_events(NULL) != M2M_SUCCESS) {
      }
      chThdSleepMilliseconds(100);
  }
}
