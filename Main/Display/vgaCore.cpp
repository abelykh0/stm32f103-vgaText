#include <string.h>

#include "startup.h"
#include "vgacore.h"
#include "timing.h"
#include "vgadraw.h"
#include "vgaText.h"
#include "font8x8.h"
#include "drawLookup.h"

namespace Vga
{
const VideoSettings* settings;
uint8_t* videoMemoryPixels;
}

static TIM_HandleTypeDef htim2;
static TIM_HandleTypeDef htim3;
static TIM_HandleTypeDef htim4;

#define REPEAT_LINES 2
static volatile int vline = 0; /* The current line being drawn */
static volatile int vflag = 0; /* When 1, can draw on the screen */
static volatile int vdraw = 0; /* Used to increment vline every REPEAT_LINES drawn lines */
static uint8_t *GPIO_ODR;

static uint16_t verticalPixelCount;

static void InitHSync(Vga::Timing::Polarity polarity, int wholeLine, int syncPulse, int startDraw);
static void InitVSync(Vga::Timing::Polarity polarity, int wholeFrame, int syncPulse, int startDraw);

#define __irq extern "C"

void Vga::InitVga(VideoSettings* videoSettings)
{
	settings = videoSettings;
	const Timing* timing = videoSettings->Timing;
	videoMemoryPixels = videoSettings->Pixels;

    GPIO_InitTypeDef gpioInit;

    // Set PA0..PA5 to OUTPUT with high speed
    __HAL_RCC_GPIOA_CLK_ENABLE();
    gpioInit.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
    gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
    gpioInit.Pull = GPIO_PULLUP;
    gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpioInit);

    // HSync on PB0 and VSync on PB6
    __HAL_RCC_GPIOB_CLK_ENABLE();
    gpioInit.Pin = GPIO_PIN_0 | GPIO_PIN_6;
    gpioInit.Mode = GPIO_MODE_AF_PP;
    gpioInit.Pull = GPIO_PULLUP;
    gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpioInit);

    GPIO_ODR = (uint8_t*)&GPIOA->ODR;

	double realPixelsPerPixel = timing->pixel_frequency_mhz / 12;
	uint16_t usedHorizontalPixels = HSIZE_CHARS * 8 * realPixelsPerPixel;
	if (usedHorizontalPixels > timing->horizPixels * realPixelsPerPixel)
	{
		usedHorizontalPixels = timing->horizPixels * realPixelsPerPixel;
	}
	uint16_t horizontalOffset = (timing->horizPixels - usedHorizontalPixels) / 2;

	uint16_t usedVerticalPixels = VSIZE_CHARS * 8 * REPEAT_LINES;
	if (usedVerticalPixels > timing->verticalPixels)
	{
		usedVerticalPixels = timing->verticalPixels;
	}
	verticalPixelCount = usedVerticalPixels / REPEAT_LINES;
	uint16_t verticalOffset = (timing->verticalPixels - usedVerticalPixels) / 2;

    double factor = HAL_RCC_GetHCLKFreq() / 1000000.0 / timing->pixel_frequency_mhz;
    int wholeLine = factor * timing->horizWholeLine;
    int syncPulse = factor * timing->horizSyncPulse;
    int startDraw = factor * timing->horizStartDraw - timing->horizLead;
    InitHSync(timing->horizSyncPolarity, wholeLine, syncPulse, startDraw + horizontalOffset);

    InitVSync(timing->verticalSyncPolarity,
    	timing->verticalWholeFrame,
        timing->verticalSyncPulse,
        timing->verticalStartLine + verticalOffset);

    vline = 0;
    vdraw = 0;
}

void Vga::ClearScreen()
{
    memset((void*)videoMemoryPixels, ' ', HSIZE_CHARS * VSIZE_CHARS);
}

//*****************************************************************************
//	This irq is generated slightly before TIM3_IRQHandler
//*****************************************************************************
__irq void TIM2_IRQHandler()
{
    if (__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_CC2) != RESET && __HAL_TIM_GET_IT_SOURCE(&htim2, TIM_FLAG_CC2) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&htim2, TIM_FLAG_CC2);

        if (vflag)
        {
            // Wait for interrupt
            __asm__ volatile("wfi \n\t" :::);
        }
    }
    else
    {
        HAL_TIM_IRQHandler(&htim2);
    }
}

//*****************************************************************************
//	This irq is generated at the end of the horizontal back porch.
//*****************************************************************************
__irq void TIM3_IRQHandler()
{
    if (__HAL_TIM_GET_FLAG(&htim3, TIM_FLAG_CC2) != RESET && __HAL_TIM_GET_IT_SOURCE(&htim3, TIM_FLAG_CC2) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&htim3, TIM_FLAG_CC2);

        if (vflag)
        {
            __disable_irq();

            vgaDraw((uint8_t*)Vga::font + (vline % 8),
            	&Vga::videoMemoryPixels[(vline / 8) * HSIZE_CHARS],
				(uint32_t*)drawLookup, GPIO_ODR);

            vdraw++;
            if (vdraw == 2)
            {
                vdraw = 0;
                vline++;
                if (vline == verticalPixelCount)
                {
                    vdraw = vline = vflag = 0;
                }
            }

            __enable_irq();
        }
    }
    else
    {
        HAL_TIM_IRQHandler(&htim3);
    }

    Vga::HBlankInterrupt();
}

//*****************************************************************************
//	This irq is generated at the end of the vertical back porch.
//*****************************************************************************
__irq void TIM4_IRQHandler()
{
    if (__HAL_TIM_GET_FLAG(&htim4, TIM_FLAG_CC4) != RESET && __HAL_TIM_GET_IT_SOURCE(&htim4, TIM_FLAG_CC4) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&htim4, TIM_FLAG_CC4);

        vflag = 1;
        vline = 0;
    }
    else
    {
        HAL_TIM_IRQHandler(&htim4);
    }
}

static void InitVSync(
    Vga::Timing::Polarity polarity,
    int wholeFrame,
    int syncPulse,
    int startDraw)
{
    __HAL_RCC_TIM4_CLK_ENABLE();

    htim4.Instance = TIM4;
    htim4.Init.Prescaler = 0;
    htim4.Init.Period = wholeFrame - 1;
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&htim4);

    // Slave mode Gated, triggered by TIM3
    TIM_SlaveConfigTypeDef sSlaveConfig;
    sSlaveConfig.SlaveMode = TIM_SLAVEMODE_GATED;
    sSlaveConfig.InputTrigger = TIM_TS_ITR2;
    sSlaveConfig.TriggerPolarity = TIM_TRIGGERPOLARITY_NONINVERTED;
    sSlaveConfig.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;
    sSlaveConfig.TriggerFilter = 0;
    HAL_TIM_SlaveConfigSynchronization(&htim4, &sSlaveConfig);

    HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);

    TIM_OC_InitTypeDef sConfigOC;

    // VSync on pin PB6
    sConfigOC.OCMode = polarity == Vga::Timing::Polarity::negative ? TIM_OCMODE_PWM2 : TIM_OCMODE_PWM1;
    sConfigOC.Pulse = syncPulse;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1);
    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC1);
    TIM_CCxChannelCmd(htim4.Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);

    sConfigOC.OCMode = TIM_OCMODE_INACTIVE;
    sConfigOC.Pulse = startDraw;
    HAL_TIM_OC_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4);
    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC4);
    TIM_CCxChannelCmd(htim4.Instance, TIM_CHANNEL_4, TIM_CCx_ENABLE);

    if (IS_TIM_BREAK_INSTANCE(htim4.Instance) != RESET)
    {
        __HAL_TIM_MOE_ENABLE(&htim4);
    }

    __HAL_TIM_ENABLE(&htim4);
}

static void InitHSync(
    Vga::Timing::Polarity polarity,
    int wholeLine,
    int syncPulse,
    int startDraw)
{
    TIM_OC_InitTypeDef sConfigOC;
    TIM_MasterConfigTypeDef sMasterConfig;

    __HAL_RCC_TIM2_CLK_ENABLE();

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 0;
    htim2.Init.Period = wholeLine - 1;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&htim2);

    // Master mode
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_ENABLE;
    HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

    HAL_NVIC_SetPriority(TIM2_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

    sConfigOC.OCMode = TIM_OCMODE_INACTIVE;
    sConfigOC.Pulse = startDraw - 12 - 1;
    HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2);
    __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_CC2);
    TIM_CCxChannelCmd(htim2.Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);

    if (IS_TIM_BREAK_INSTANCE(htim2.Instance) != RESET)
    {
        __HAL_TIM_MOE_ENABLE(&htim2);
    }

    __HAL_RCC_TIM3_CLK_ENABLE();

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 0;
    htim3.Init.Period = wholeLine - 1;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&htim3);

    // Master mode
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

    // Slave mode Trigger, triggered by TIM2
    TIM_SlaveConfigTypeDef sSlaveConfig;
    sSlaveConfig.SlaveMode = TIM_SLAVEMODE_TRIGGER;
    sSlaveConfig.InputTrigger = TIM_TS_ITR1;
    sSlaveConfig.TriggerPolarity = TIM_TRIGGERPOLARITY_NONINVERTED;
    sSlaveConfig.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;
    sSlaveConfig.TriggerFilter = 0;
    HAL_TIM_SlaveConfigSynchronization(&htim3, &sSlaveConfig);

    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);

    // HSync on pin PB0
    sConfigOC.OCMode = polarity == Vga::Timing::Polarity::negative ? TIM_OCMODE_PWM2 : TIM_OCMODE_PWM1;
    sConfigOC.Pulse = syncPulse;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);
    __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC3);
    TIM_CCxChannelCmd(htim3.Instance, TIM_CHANNEL_3, TIM_CCx_ENABLE);

    sConfigOC.OCMode = TIM_OCMODE_INACTIVE;
    sConfigOC.Pulse = startDraw - 1;
    HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2);
    __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC2);
    TIM_CCxChannelCmd(htim3.Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);

    if (IS_TIM_BREAK_INSTANCE(htim3.Instance) != RESET)
    {
        __HAL_TIM_MOE_ENABLE(&htim3);
    }

    __HAL_TIM_SET_COUNTER(&htim2, 0);
    __HAL_TIM_SET_COUNTER(&htim3, 0);

    // Starts TIM3 as well
    __HAL_TIM_ENABLE(&htim2);
    //__HAL_TIM_ENABLE(&htim3);
}
