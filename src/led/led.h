#ifndef __LED_H__
#define __LED_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
        LED_NORMAL_OFF,
        LED_NORMAL_ON,
        LED_NORMAL_FLASH_SLOW,
        LED_NORMAL_FLASH_FAST
}LED_NORMAL_TYPE;
typedef enum {
        LED_NOTIFY_OFF,
        LED_NOTIFY_FLASH_2,
        LED_NOTIFY_FLASH_3,
        LED_NOTIFY_FLASH_LONG,
}LED_NOTIFY_TYPE;

void led_init();
void led_set(LED_NORMAL_TYPE type);
void led_notify(LED_NOTIFY_TYPE type);

#ifdef __cplusplus
}
#endif
#endif


