#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"

#define PWMPIN 15

void setServo(int angle);

bool timer_interrupt_function(__unused struct repeating_timer *t) {
    uint16_t result1 = adc_read();
    printf("%f\r\n", (float)result1 / 4095 * 3.3);
    return true;
}

int main()
{
    stdio_init_all();

    // timer interrupt
    struct repeating_timer timer;
    add_repeating_timer_ms(-100, timer_interrupt_function, NULL, &timer);

    // turn on pwm
    gpio_set_function(PWMPIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PWMPIN);
    float div = 50; 
    pwm_set_clkdiv(slice_num, div);
    uint16_t wrap = 60000;

    // set the PWM frequency and resolution
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true);
    pwm_set_gpio_level(PWMPIN, 0);

    adc_init();
    adc_gpio_init(26);
    adc_select_input(0);

    while (true) {
        int i = 0;
        for(i = 10; i < 170; i++){
            setServo(i);
            sleep_ms(10);
        }
        for(i = 170; i > 10; i--){
            setServo(i);
            sleep_ms(10);
        }
    }
}

void setServo(int angle){
    pwm_set_gpio_level(PWMPIN, (int)((0.015 + (angle / 180.0) * 0.105) * 60000)); // 1.5% + (angle/180)*10.5
}