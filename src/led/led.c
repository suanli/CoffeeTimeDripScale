#include "esp_common.h"
#include "led.h"
#include "gpio.h"


#define LED_NORMAL_SLOW_INTERVAL (1000 / portTICK_RATE_MS)
#define LED_NORMAL_FAST_INTERVAL (500 / portTICK_RATE_MS)
#define LED_IDLE_LOOP_INTERVAL (1000 / portTICK_RATE_MS)
#define LED_IDLE_LOOP_SLICE_INTERVAL (100 / portTICK_RATE_MS)
#define LED_NOTIFY_STOP_AT_BEGINNING_INTERVAL (200 / portTICK_RATE_MS)
#define LED_NOTIFY_WAIT_AT_FINISHING_INTERVAL (500 / portTICK_RATE_MS)
#define LED_NOTIFY_FLASH_INTERVAL (50 / portTICK_RATE_MS)
#define LED_NOTIFY_LONG_INTERVAL (1500 / portTICK_RATE_MS)


static LED_NORMAL_TYPE g_led_normal_state = LED_NORMAL_OFF;
static LED_NOTIFY_TYPE g_led_notify_state = LED_NOTIFY_OFF;
static bool g_quick_exit_delay_flag = false;
static bool g_led_on = false;
static xTaskHandle g_notify_task = NULL;


static void gpio_init()
{
    g_led_on = false;
	gpio16_output_conf();
}
static void led_on() 
{
    if(g_led_on)
        return;
    g_led_on = true;
	gpio16_output_set(1);
}

static void led_off()
{
    if(!g_led_on)
        return;
    g_led_on = false;
	gpio16_output_set(0);
}

#define DELAY_WITH_QUICK_EXIT(x) \
    if(led_delay(x) != true) \
        break;

static bool led_delay(int time) 
{
    while(time != 0)
    {
        if(g_quick_exit_delay_flag == true)
        {
            g_quick_exit_delay_flag = false;
            return false;
        }

        if(time > LED_IDLE_LOOP_SLICE_INTERVAL)
        {
            vTaskDelay(LED_IDLE_LOOP_SLICE_INTERVAL);
            time -= LED_IDLE_LOOP_SLICE_INTERVAL;
        } else 
        {
            vTaskDelay(time);
            return true;
        }
    }
    return true;
}

void led_set(LED_NORMAL_TYPE type) 
{
    g_led_normal_state = type;
    g_quick_exit_delay_flag = true;
}

static void led_notify_task(void *pvParameters)
{
    printf("LED Task Start!\r\n");
    g_quick_exit_delay_flag = true;
    while (g_led_notify_state != LED_NOTIFY_OFF) {
        switch(g_led_notify_state)
        {
            case LED_NOTIFY_FLASH_2:
            {
                led_off();
                DELAY_WITH_QUICK_EXIT(LED_NOTIFY_STOP_AT_BEGINNING_INTERVAL);
                led_on();
                DELAY_WITH_QUICK_EXIT(LED_NOTIFY_FLASH_INTERVAL);
                led_off();
                DELAY_WITH_QUICK_EXIT(LED_NOTIFY_FLASH_INTERVAL);
                led_on();
                DELAY_WITH_QUICK_EXIT(LED_NOTIFY_FLASH_INTERVAL);
                led_off();
                DELAY_WITH_QUICK_EXIT(LED_NOTIFY_WAIT_AT_FINISHING_INTERVAL);
            }
            break;
            case LED_NOTIFY_FLASH_3:
            {
                led_off();
                DELAY_WITH_QUICK_EXIT(LED_NOTIFY_STOP_AT_BEGINNING_INTERVAL);
                led_on();
                DELAY_WITH_QUICK_EXIT(LED_NOTIFY_FLASH_INTERVAL);
                led_off();
                DELAY_WITH_QUICK_EXIT(LED_NOTIFY_FLASH_INTERVAL);
                led_on();
                DELAY_WITH_QUICK_EXIT(LED_NOTIFY_FLASH_INTERVAL);
                led_off();
                DELAY_WITH_QUICK_EXIT(LED_NOTIFY_FLASH_INTERVAL);
                led_on();
                DELAY_WITH_QUICK_EXIT(LED_NOTIFY_FLASH_INTERVAL);
                led_off();
                DELAY_WITH_QUICK_EXIT(LED_NOTIFY_WAIT_AT_FINISHING_INTERVAL);
            }
            break;
            case LED_NOTIFY_FLASH_LONG:
            {
                led_off();
                DELAY_WITH_QUICK_EXIT(LED_NOTIFY_STOP_AT_BEGINNING_INTERVAL);
                led_on();
                DELAY_WITH_QUICK_EXIT(LED_NOTIFY_LONG_INTERVAL);
                led_off();
                DELAY_WITH_QUICK_EXIT(LED_NOTIFY_WAIT_AT_FINISHING_INTERVAL);
            }
            break;
            default:
            break;
        }
    }
    printf("LED Task Stop!\r\n");
    vTaskDelete(g_notify_task);
    g_notify_task = NULL;
}


void led_notify(LED_NOTIFY_TYPE type)
{
    g_led_notify_state = type;
    if(g_notify_task != NULL)
        xTaskCreate(led_notify_task, "led_notify", 256, NULL, 2, &g_notify_task);
}

static void led_task(void *pvParameters)
{
    printf("LED Task Start!\r\n");
    while (1) {
        if(g_notify_task != NULL) 
            vTaskDelay(LED_IDLE_LOOP_SLICE_INTERVAL);
        switch(g_led_normal_state)
        {
            case LED_NORMAL_ON:
            {
                led_on();
                DELAY_WITH_QUICK_EXIT(LED_IDLE_LOOP_SLICE_INTERVAL);
            }
            break;
            case LED_NORMAL_FLASH_SLOW:
            {
                led_on();
                DELAY_WITH_QUICK_EXIT(LED_NORMAL_SLOW_INTERVAL);
                led_off();
                DELAY_WITH_QUICK_EXIT(LED_NORMAL_SLOW_INTERVAL);
            }
            break;
            case LED_NORMAL_FLASH_FAST:
            {
                led_on();
                DELAY_WITH_QUICK_EXIT(LED_NORMAL_FAST_INTERVAL);
                led_off();
                DELAY_WITH_QUICK_EXIT(LED_NORMAL_FAST_INTERVAL);
            }
            break;
            case LED_NORMAL_OFF:
                led_off();
            default:
            {
                // TODO: change to wait event
                DELAY_WITH_QUICK_EXIT(LED_IDLE_LOOP_SLICE_INTERVAL);
            }
            break;
        }
    }
    printf("LED Task Stop!\r\n");
    vTaskDelete(NULL);
}


void led_init() 
{
    gpio_init();
    xTaskCreate(led_task, "led", 256, NULL, 2, NULL);
}


