#include "stm32f4xx.h"

#include "STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_tim.h"
#include "STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_rcc.h"

#include "FreeRTOS/include/FreeRTOS.h"
#include "FreeRTOS/include/task.h"
#include "FreeRTOS/include/semphr.h"

#include "Keys.h"

uint64_t u64IdleTicksCnt=0; // Counts when the OS has no task to execute.
uint64_t tickTime=0;        // Counts OS ticks (default = 1000Hz).

/*
 * When FreeRTOS crashes, you often end up in a hard fault.
 */
void HardFault_Handler (void)
{

}

#define GPIO_PORT_Pin_0 GPIOD

#define GPIO_PIN_Pin_LED_0 GPIO_Pin_0
#define GPIO_PIN_Pin_LED_1 GPIO_Pin_1
#define GPIO_PIN_Pin_LED_2 GPIO_Pin_2
#define GPIO_PIN_Pin_LED_3 GPIO_Pin_3
#define GPIO_PIN_Pin_LED_4 GPIO_Pin_6
#define GPIO_PIN_Pin_LED_5 GPIO_Pin_7
#define GPIO_PIN_Pin_LED_6 GPIO_Pin_8

#define NUMBER_OF_LEDS 7
#define NUMBER_OF_SEQUENCE_STEPS 12
const long SEQUENCE_DELAY = 150;
int sequence_step_number = 0;

int sequence[NUMBER_OF_SEQUENCE_STEPS][NUMBER_OF_LEDS] = {
  {1,0,0,0,0,0,0},
  {0,1,0,0,0,0,0},
  {0,0,1,0,0,0,0},
  {0,0,0,1,0,0,0},
  {0,0,0,0,1,0,0},
  {0,0,0,0,0,1,0},
  {0,0,0,0,0,0,1},
  {0,0,0,0,0,1,0},
  {0,0,0,0,1,0,0},
  {0,0,0,1,0,0,0},
  {0,0,1,0,0,0,0},
  {0,1,0,0,0,0,0}
};

void setSequenceStep(void) {
  for(int led_number=0 ; led_number < NUMBER_OF_LEDS ; ++led_number){
        int diod_state = sequence[sequence_step_number][led_number];

        switch(led_number)
        {
            case 0:
                (diod_state == 0) ? GPIO_ResetBits(GPIO_PORT_Pin_0, GPIO_PIN_Pin_LED_0) : GPIO_SetBits(GPIO_PORT_Pin_0, GPIO_PIN_Pin_LED_0);
            break;

            case 1:
                (diod_state == 0) ? GPIO_ResetBits(GPIO_PORT_Pin_0, GPIO_PIN_Pin_LED_1) : GPIO_SetBits(GPIO_PORT_Pin_0, GPIO_PIN_Pin_LED_1);
            break;

            case 2:
                (diod_state == 0) ? GPIO_ResetBits(GPIO_PORT_Pin_0, GPIO_PIN_Pin_LED_2) : GPIO_SetBits(GPIO_PORT_Pin_0, GPIO_PIN_Pin_LED_2);
            break;

            case 3:
                (diod_state == 0) ? GPIO_ResetBits(GPIO_PORT_Pin_0, GPIO_PIN_Pin_LED_3) : GPIO_SetBits(GPIO_PORT_Pin_0, GPIO_PIN_Pin_LED_3);
            break;

            case 4:
                (diod_state == 0) ? GPIO_ResetBits(GPIO_PORT_Pin_0, GPIO_PIN_Pin_LED_4) : GPIO_SetBits(GPIO_PORT_Pin_0, GPIO_PIN_Pin_LED_4);
            break;

            case 5:
                (diod_state == 0) ? GPIO_ResetBits(GPIO_PORT_Pin_0, GPIO_PIN_Pin_LED_5) : GPIO_SetBits(GPIO_PORT_Pin_0, GPIO_PIN_Pin_LED_5);
            break;

            case 6:
                (diod_state == 0) ? GPIO_ResetBits(GPIO_PORT_Pin_0, GPIO_PIN_Pin_LED_6) : GPIO_SetBits(GPIO_PORT_Pin_0, GPIO_PIN_Pin_LED_6);
            break;
        }
	}

  (sequence_step_number >= NUMBER_OF_SEQUENCE_STEPS - 1) ? sequence_step_number = 0 : ++sequence_step_number;
}

/***************************************************************
* Timer 2 interrupt function
***************************************************************/
void TIM2_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {

	  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        setSequenceStep();
  }
}

int main(void)
{
    SystemInit();

    GPIO_Keys_Conf();

    FM_TIM2_TIMER_Pre(10000,540);

    FM_TIM2_NVIC_Pre();

    while(1);

    return 1;
}

// This FreeRTOS callback function gets called once per tick (default = 1000Hz).
// ----------------------------------------------------------------------------
void vApplicationTickHook( void )
{
    ++tickTime;
}

// This FreeRTOS call-back function gets when no other task is ready to execute.
// On a completely unloaded system this is getting called at over 2.5MHz!
// ----------------------------------------------------------------------------
void vApplicationIdleHook( void )
{
    ++u64IdleTicksCnt;
}

// A required FreeRTOS function.
// ----------------------------------------------------------------------------
void vApplicationMallocFailedHook( void )
{
    configASSERT( 0 );  // Latch on any failure / error.
}

/*
 * Required callbacks for audio playback
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void)
{
    return 1;
}

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size)
{
}

void EVAL_AUDIO_HalfTransfer_CallBack(uint32_t pBuffer, uint32_t Size)
{
}

void EVAL_AUDIO_Error_CallBack(void* pData)
{
    while(1);
}

uint32_t Codec_TIMEOUT_UserCallback(void)
{
    while(1);
    return 1;
}
