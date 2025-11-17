/*---------------------------------------------------------------------------
 * Copyright (c) 2024-2025 Arm Limited (or its affiliates).
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include "main.h"
#include "cmsis_os2.h"
#include "cmsis_vio.h"

/* Thread attributes for the app_main thread */
static const osThreadAttr_t thread_attr_main = { .name = "app_main" };

/* Thread attributes for the LED thread */
static const osThreadAttr_t thread_attr_LED = { .name = "LED" };

/* Thread ID for the LED thread */
static osThreadId_t tid_LED;

/*
  Thread that blinks LED.
*/
static __NO_RETURN void thread_LED (void *argument) {
  uint32_t active_flag = 0U;

  (void)argument;

  for (;;) {
      vioSetSignal(vioLED0, vioLEDon);          // Switch LED0 on
      osDelay(500U);                            // Delay 500 ms
      vioSetSignal(vioLED0, vioLEDoff);         // Switch LED0 off
      osDelay(500U);                            // Delay 500 ms
  }
}

/*
  Application main thread.
*/
__NO_RETURN void app_main_thread (void *argument) {
  (void)argument;

  printf("Blinky example\n");

  /* Create LED and Button threads */
  tid_LED = osThreadNew(thread_LED, NULL, &thread_attr_LED);

  for (;;) {
    /* Delay indefinitely */
    osDelay(osWaitForever);
  }
}

/*
  Application initialization.
*/
int app_main (void) {
  osKernelInitialize();                         // Initialize CMSIS-RTOS2
  osThreadNew(app_main_thread, NULL, &thread_attr_main);
  osKernelStart();                              // Start thread execution
  return 0;
}
