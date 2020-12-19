#include "E32_PC1.h"

//#include <Arduino.h>
//#include "driver/pcnt.h"

#define PCNT_TEST_UNIT PCNT_UNIT_1
#define PCNT_H_LIM_VAL 30000
#define PCNT_L_LIM_VAL 0
#define PCNT_THRESH1_VAL 30000
#define PCNT_THRESH0_VAL 0
//#define PCNT_INPUT_SIG_IO 26  // Pulse Input GPIO
#define PCNT_INPUT_CTRL_IO -1 // Control GPIO HIGH=count up, LOW=count down

byte PCNT_INPUT_SIG_IO_v1 = 27;

// SOURCE: https://github.com/espressif/esp-idf/blob/master/examples/peripherals/pcnt/main/pcnt_example_main.c

xQueueHandle pcnt_evt_queue1; /*A queue to handle pulse counter event*/

typedef struct
{
  int unit;        /*pulse counter unit*/
  uint32_t status; /*pulse counter internal status*/
} pcnt_evt_t;

void IRAM_ATTR pcnt_intr_handler1(void *arg)
{
  uint32_t intr_status = PCNT.int_st.val;
  int i;
  pcnt_evt_t evt;
  portBASE_TYPE HPTaskAwoken = pdFALSE;

  for (i = 0; i < PCNT_UNIT_MAX; i++)
  {
    if (intr_status & (BIT(i)))
    {
      evt.unit = i;
      evt.status = PCNT.status_unit[i].val;
      PCNT.int_clr.val = BIT(i);
      /*H LIM EVT*/
      if (PCNT.status_unit[i].h_lim_lat)
      {
        //do something
      }
      /*L LIM EVT*/
      if (PCNT.status_unit[i].l_lim_lat)
      {
        //do something
      }
      /*THRES0 EVT*/
      if (PCNT.status_unit[i].thres0_lat)
      {
        //do something
      }
      /*THRES1 EVT*/
      if (PCNT.status_unit[i].thres1_lat)
      {
        //do something
      }
      /*ZERO EVT*/
      if (PCNT.status_unit[i].zero_lat)
      {
        //do something
      }
      xQueueSendFromISR(pcnt_evt_queue1, &evt, &HPTaskAwoken);
      if (HPTaskAwoken == pdTRUE)
      {
        portYIELD_FROM_ISR();
      }
    }
  }
}

static void pcnt_init(void)
{
  pcnt_config_t pcnt_config = {
      /*Set PCNT_INPUT_SIG_IO as pulse input gpio */
      .pulse_gpio_num = PCNT_INPUT_SIG_IO_v1,
      /*set PCNT_INPUT_CTRL_IO as control gpio */
      .ctrl_gpio_num = PCNT_INPUT_CTRL_IO,
      /*Counter mode reverse when control input is low level*/
      .lctrl_mode = PCNT_MODE_KEEP,
      /*Counter mode does not change when control input is high level*/
      .hctrl_mode = PCNT_MODE_KEEP, //when control signal is high,keep the primary counter mode
      /*Set counter and control mode*/
      /*Counter increase for positive edge on pulse input GPIO*/
      .pos_mode = PCNT_COUNT_INC,
      /*Counter decrease for negative edge on pulse input GPIO*/
      .neg_mode = PCNT_COUNT_INC, //keep the counter value
      /*Set maximum value for increasing counter*/
      .counter_h_lim = PCNT_H_LIM_VAL,
      /*Set minimum value for decreasing counter*/
      .counter_l_lim = PCNT_L_LIM_VAL,
      /*Choose unit 0 */
      .unit = PCNT_TEST_UNIT,
      /*Choose channel 0 */
      .channel = PCNT_CHANNEL_0,
  };
  /*Initialize PCNT unit */
  pcnt_unit_config(&pcnt_config);

  /*Configure input filter value*/
  pcnt_set_filter_value(PCNT_TEST_UNIT, 100);
  /*Enable input filter*/
  pcnt_filter_enable(PCNT_TEST_UNIT);

  /*Set value for watch point thresh1*/
  pcnt_set_event_value(PCNT_TEST_UNIT, PCNT_EVT_THRES_1, PCNT_THRESH1_VAL);
  /*Enable watch point event of thresh1*/
  pcnt_event_enable(PCNT_TEST_UNIT, PCNT_EVT_THRES_1);
  /*Set value for watch point thresh0*/
  pcnt_set_event_value(PCNT_TEST_UNIT, PCNT_EVT_THRES_0, PCNT_THRESH0_VAL);
  /*Enable watch point event of thresh0*/
  pcnt_event_enable(PCNT_TEST_UNIT, PCNT_EVT_THRES_0);
  /*Enable watch point event of h_lim*/
  pcnt_event_enable(PCNT_TEST_UNIT, PCNT_EVT_H_LIM);
  /*Enable watch point event of l_lim*/
  pcnt_event_enable(PCNT_TEST_UNIT, PCNT_EVT_L_LIM);
  /*Enable watch point event of zero*/
  pcnt_event_enable(PCNT_TEST_UNIT, PCNT_EVT_ZERO);

  /*Pause counter*/
  pcnt_counter_pause(PCNT_TEST_UNIT);
  /*Reset counter value*/
  pcnt_counter_clear(PCNT_TEST_UNIT);
  /*Register ISR handler*/
  pcnt_isr_register(pcnt_intr_handler1, NULL, 0, NULL);
  /*Enable interrupt for PCNT unit*/
  pcnt_intr_enable(PCNT_TEST_UNIT);
  /*Resume counting*/
  pcnt_counter_resume(PCNT_TEST_UNIT);
}

void E32_PC1::begin(byte pin)
{
  PCNT_INPUT_SIG_IO_v1=pin;
  pinMode(PCNT_INPUT_SIG_IO_v1, INPUT_PULLUP);
  /*Init PCNT event queue */
  pcnt_evt_queue1 = xQueueCreate(10, sizeof(pcnt_evt_t));
  /*Init PCNT functions*/
  pcnt_init();
}

int16_t E32_PC1::getCount()
{
  int16_t count = 0;
  pcnt_get_counter_value(PCNT_TEST_UNIT, &count);
  return count;
}

void E32_PC1::clearCount()
{
  /*Pause counter*/
  pcnt_counter_pause(PCNT_TEST_UNIT);
  /*Reset counter value*/
  pcnt_counter_clear(PCNT_TEST_UNIT);
  /*Resume counting*/
  pcnt_counter_resume(PCNT_TEST_UNIT);
}