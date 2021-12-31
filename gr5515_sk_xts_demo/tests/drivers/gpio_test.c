/*
 * Copyright (c) 2021 GOODIX.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "gpio_if.h"
#include "driver_test.h"

#define GPIO_LED1         4
#define GPIO_LED2         44
#define GPIO_BUTTON_UP    12
#define GPIO_BUTTON_DOWN  13
#define GPIO_BUTTON_LEFT  14
#define GPIO_BUTTON_RIGH  15

static void gpio_output_test(void)
{
    static int cnt = 0;

    LOG_I("set gpio %d output\r\n", GPIO_LED1);
    GpioSetDir(GPIO_LED1, GPIO_DIR_OUT);
    GpioSetDir(GPIO_LED2, GPIO_DIR_OUT);

    if (cnt++ % 2) {
        LOG_I("set led on\r\n");
        GpioWrite(GPIO_LED1, GPIO_VAL_LOW);
        GpioWrite(GPIO_LED2, GPIO_VAL_HIGH);
    } else {
        LOG_I("set led of\r\n");
        GpioWrite(GPIO_LED1, GPIO_VAL_HIGH);
        GpioWrite(GPIO_LED2, GPIO_VAL_LOW);
    }
}

static void gpio_input_test(void)
{
    int cnt = 0;
    uint16_t val = 1;

    GpioSetDir(GPIO_BUTTON_UP, GPIO_DIR_IN);
    LOG_I("please push up key: >>> ");
    while (cnt++ < 10) {
        GpioRead(GPIO_BUTTON_UP, &val);
        if (0 == val) {
            LOG_I("up key\r\n");
            break;
        }

        osDelay(100);
    }

    if (val) {
        LOG_I("none key\r\n");
    }
}

static int32_t gpio_irq_handler(uint16_t gpio, void *data)
{
    LOG_I("%s enter, gpio = %d, data = %d\r\n", __func__, gpio, *(int *)data);
    return 0;
}

static void gpio_irq_test(void)
{
    static int val = 1234;

    LOG_I("set key_left (gpio %d) input, @%p\r\n", GPIO_BUTTON_LEFT, gpio_irq_handler);
    GpioSetDir(GPIO_BUTTON_LEFT, GPIO_DIR_IN);
    GpioSetIrq(GPIO_BUTTON_LEFT, GPIO_IRQ_TRIGGER_RISING, (GpioIrqFunc)gpio_irq_handler, &val);
    GpioEnableIrq(GPIO_BUTTON_LEFT);
}

void gpio_test()
{
    gpio_output_test();
    gpio_input_test();
    gpio_irq_test();
}