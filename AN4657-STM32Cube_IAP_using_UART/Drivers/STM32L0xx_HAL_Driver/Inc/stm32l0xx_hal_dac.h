/**
  ******************************************************************************
  * @file    stm32l0xx_hal_dac.h
  * @author  MCD Application Team
  * @version $VERSION$
  * @date    $DATE$
  * @brief   Header file of DAC HAL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L0xx_HAL_DAC_H
#define __STM32L0xx_HAL_DAC_H

#ifdef __cplusplus
 extern "C" {
#endif
   
#if !defined (STM32L031xx) && !defined (STM32L041xx) && !defined (STM32L051xx) && !defined (STM32L061xx) && !defined (STM32L071xx) && !defined (STM32L081xx)
   
/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal_def.h"

/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

/** @defgroup DAC DAC
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup DAC_Exported_Types DAC Exported Types
  * @{
  */

/** 
  * @brief  HAL State structures definition  
  */ 
typedef enum
{
  HAL_DAC_STATE_RESET             = 0x00,  /*!< DAC not yet initialized or disabled  */
  HAL_DAC_STATE_READY             = 0x01,  /*!< DAC initialized and ready for use    */
  HAL_DAC_STATE_BUSY              = 0x02,  /*!< DAC internal processing is ongoing   */
  HAL_DAC_STATE_TIMEOUT           = 0x03,  /*!< DAC timeout state                    */
  HAL_DAC_STATE_ERROR             = 0x04   /*!< DAC error state                      */
 
}HAL_DAC_StateTypeDef;
 
/** 
  * @brief  DAC handle Structure definition  
  */ 
typedef struct
{
  DAC_TypeDef                 *Instance;     /*!< Register base address             */

  __IO HAL_DAC_StateTypeDef   State;         /*!< DAC communication state           */

  HAL_LockTypeDef             Lock;          /*!< DAC locking object                */

  DMA_HandleTypeDef           *DMA_Handle1;  /*!< Pointer DMA handler for channel 1 */

#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
  DMA_HandleTypeDef           *DMA_Handle2;  /*!< Pointer DMA handler for channel 2 */ 
#endif

  __IO uint32_t               ErrorCode;     /*!< DAC Error code                   */

}DAC_HandleTypeDef;

/** 
  * @brief   DAC Configuration regular Channel structure definition
  */
typedef struct
{
  uint32_t DAC_Trigger;       /*!< Specifies the external trigger for the selected DAC channel.
                                   This parameter can be a value of @ref DAC_trigger_selection */

  uint32_t DAC_OutputBuffer;  /*!< Specifies whether the DAC channel output buffer is enabled or disabled.
                                   This parameter can be a value of @ref DAC_output_buffer */

}DAC_ChannelConfTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup DAC_Exported_Constants DAC Exported Constants
  * @{
  */  

/** @defgroup DAC_Error_Code DAC Error Code
  * @{
  */
#define  HAL_DAC_ERROR_NONE              0x00    /*!< No error                          */
#define  HAL_DAC_ERROR_DMAUNDERRUNCH1    0x01    /*!< DAC channel1 DAM underrun error   */
#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
#define  HAL_DAC_ERROR_DMAUNDERRUNCH2    0x02    /*!< DAC channel2 DAM underrun error   */
#endif
#define  HAL_DAC_ERROR_DMA               0x04    /*!< DMA error                         */
/**
  * @}
  */

/** @defgroup DAC_trigger_selection DAC trigger selection
  * @{
  */
#define DAC_TRIGGER_NONE      ((uint32_t)0x00000000) /*!< Conversion is automatic once the DAC1_DHRxxxx register has been loaded, and not by external trigger */
#define DAC_TRIGGER_T6_TRGO   ((uint32_t)                                                    DAC_CR_TEN1)  /*!< TIM6 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T21_TRGO  ((uint32_t)(                 DAC_CR_TSEL1_1 | DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM21 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T2_TRGO   ((uint32_t)(DAC_CR_TSEL1_2 |                                   DAC_CR_TEN1)) /*!< TIM2 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_EXT_IT9   ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TSEL1_1 |                  DAC_CR_TEN1)) /*!< EXTI Line9 event selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_SOFTWARE  ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TSEL1_1 | DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< Conversion started by software trigger for DAC channel */

#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
#define DAC_TRIGGER_T3_TRGO   ((uint32_t)(                                  DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM3  TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T7_TRGO   ((uint32_t)(DAC_CR_TSEL1_2 |                  DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM7  TRGO selected as external conversion trigger for DAC channel */
#endif

#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
#define IS_DAC_TRIGGER(TRIGGER) (((TRIGGER) == DAC_TRIGGER_NONE)     || \
                                 ((TRIGGER) == DAC_TRIGGER_T6_TRGO)  || \
                                 ((TRIGGER) == DAC_TRIGGER_T3_TRGO)  || \
                                 ((TRIGGER) == DAC_TRIGGER_T7_TRGO)  || \
                                 ((TRIGGER) == DAC_TRIGGER_T21_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T2_TRGO)  || \
                                 ((TRIGGER) == DAC_TRIGGER_EXT_IT9)  || \
                                 ((TRIGGER) == DAC_TRIGGER_SOFTWARE))
#else /* STM32L072xx || STM32L073xx || STM32L082xx || STM32L083xx */
#define IS_DAC_TRIGGER(TRIGGER) (((TRIGGER) == DAC_TRIGGER_NONE)     || \
                                 ((TRIGGER) == DAC_TRIGGER_T6_TRGO)  || \
                                 ((TRIGGER) == DAC_TRIGGER_T21_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T2_TRGO)  || \
                                 ((TRIGGER) == DAC_TRIGGER_EXT_IT9)  || \
                                 ((TRIGGER) == DAC_TRIGGER_SOFTWARE))
#endif /* STM32L072xx || STM32L073xx || STM32L082xx || STM32L083xx */
/**
  * @}
  */

/** @defgroup DAC_output_buffer DAC output buffer
  * @{
  */
#define DAC_OUTPUTBUFFER_ENABLE            ((uint32_t)0x00000000)
#define DAC_OUTPUTBUFFER_DISABLE           ((uint32_t)DAC_CR_BOFF1)

#define IS_DAC_OUTPUT_BUFFER_STATE(STATE) (((STATE) == DAC_OUTPUTBUFFER_ENABLE) || \
                                           ((STATE) == DAC_OUTPUTBUFFER_DISABLE))
/**
  * @}
  */

/** @defgroup DAC_Channel_selection DAC Channel selection
  * @{
  */
#define DAC_CHANNEL_1                      ((uint32_t)0x00000000)
#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
#define DAC_CHANNEL_2                      ((uint32_t)0x00000010)
#endif

#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
#define IS_DAC_CHANNEL(CHANNEL) (((CHANNEL) == DAC_CHANNEL_1) || \
                                 ((CHANNEL) == DAC_CHANNEL_2))
#else
#define IS_DAC_CHANNEL(CHANNEL) ((CHANNEL) == DAC_CHANNEL_1)
#endif
/**
  * @}
  */

/** @defgroup DAC_data_alignement DAC data alignement
  * @{
  */
#define DAC_ALIGN_12B_R                    ((uint32_t)0x00000000)
#define DAC_ALIGN_12B_L                    ((uint32_t)0x00000004)
#define DAC_ALIGN_8B_R                     ((uint32_t)0x00000008)

#define IS_DAC_ALIGN(ALIGN) (((ALIGN) == DAC_ALIGN_12B_R) || \
                             ((ALIGN) == DAC_ALIGN_12B_L) || \
                             ((ALIGN) == DAC_ALIGN_8B_R))
/**
  * @}
  */

/** @defgroup DAC_data DAC data
  * @{
  */
#define IS_DAC_DATA(DATA) ((DATA) <= 0xFFF0)
/**
  * @}
  */

/** @defgroup DAC_flags_definition DAC flags definition
  * @{
  */ 
#define DAC_FLAG_DMAUDR1                   ((uint32_t)DAC_SR_DMAUDR1)
#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
#define DAC_FLAG_DMAUDR2                   ((uint32_t)DAC_SR_DMAUDR2)   
#endif

#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
#define IS_DAC_FLAG(FLAG) (((FLAG) == DAC_FLAG_DMAUDR1) || \
                           ((FLAG) == DAC_FLAG_DMAUDR2))  
#else
#define IS_DAC_FLAG(FLAG)  ((FLAG) == DAC_FLAG_DMAUDR1)
#endif
/**
  * @}
  */

/** @defgroup DAC_IT_definition DAC IT definition
  * @{
  */ 
#define DAC_IT_DMAUDR1                   ((uint32_t)DAC_SR_DMAUDR1)
#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
#define DAC_IT_DMAUDR2                   ((uint32_t)DAC_SR_DMAUDR2)   
#endif

#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
#define IS_DAC_IT(IT) (((IT) == DAC_IT_DMAUDR1) || \
                       ((IT) == DAC_IT_DMAUDR2))  
#else
#define IS_DAC_IT(IT) ((IT) == DAC_IT_DMAUDR1)  
#endif

/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/** @defgroup DAC_Exported_Macros DAC Exported Macros
  * @{
  */

/** @brief Reset DAC handle state
  * @param  __HANDLE__: specifies the DAC handle.
  * @retval None
  */
#define __HAL_DAC_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_DAC_STATE_RESET)

/** @brief Enable the DAC channel
  * @param  __HANDLE__: specifies the DAC handle.
  * @param  __DAC_CHANNEL__: specifies the DAC channel
  * @retval None
  */
#define __HAL_DAC_ENABLE(__HANDLE__, __DAC_CHANNEL__) \
SET_BIT((__HANDLE__)->Instance->CR, (DAC_CR_EN1 << (__DAC_CHANNEL__)))

/** @brief Disable the DAC channel
  * @param  __HANDLE__: specifies the DAC handle
  * @param  __DAC_CHANNEL__: specifies the DAC channel.
  * @retval None
  */
#define __HAL_DAC_DISABLE(__HANDLE__, __DAC_CHANNEL__) \
CLEAR_BIT((__HANDLE__)->Instance->CR, (DAC_CR_EN1 << (__DAC_CHANNEL__)))

/** @brief Set DHR12R1 alignment
  * @param  __ALIGNEMENT__: specifies the DAC alignement
  * @retval None
  */
#define __DAC_DHR12R1_ALIGNEMENT(__ALIGNEMENT__) (((uint32_t)0x00000008) + (__ALIGNEMENT__))

/** @brief  Set DHR12R2 alignment
  * @param  __ALIGNEMENT__: specifies the DAC alignement
  * @retval None
  */
#define __DAC_DHR12R2_ALIGNEMENT(__ALIGNEMENT__) (((uint32_t)0x00000014) + (__ALIGNEMENT__))

/** @brief  Set DHR12RD alignment
  * @param  __ALIGNEMENT__: specifies the DAC alignement
  * @retval None
  */
#define __DAC_DHR12RD_ALIGNEMENT(__ALIGNEMENT__) (((uint32_t)0x00000020) + (__ALIGNEMENT__))

/** @brief Enable the DAC interrupt
  * @param  __HANDLE__: specifies the DAC handle
  * @param  __INTERRUPT__: specifies the DAC interrupt.
  * @retval None
  */
#define __HAL_DAC_ENABLE_IT(__HANDLE__, __INTERRUPT__) \
SET_BIT((__HANDLE__)->Instance->CR, __INTERRUPT__)


/** @brief Disable the DAC interrupt
  * @param  __HANDLE__: specifies the DAC handle
  * @param  __INTERRUPT__: specifies the DAC interrupt.
  * @retval None
  */
#define __HAL_DAC_DISABLE_IT(__HANDLE__, __INTERRUPT__) \
CLEAR_BIT((__HANDLE__)->Instance->CR, __INTERRUPT__)


/** @brief  Get the selected DAC's flag status.
  * @param  __HANDLE__: specifies the DAC handle.
  * @param  __FLAG__: specifies the FLAG.
  * @retval None
  */
#define __HAL_DAC_GET_FLAG(__HANDLE__, __FLAG__) ((((__HANDLE__)->Instance->SR) & (__FLAG__)) == (__FLAG__))

/** @brief  Clear the DAC's flag.
  * @param  __HANDLE__: specifies the DAC handle.
  * @param  __FLAG__: specifies the FLAG.
  * @retval None
  */
#define __HAL_DAC_CLEAR_FLAG(__HANDLE__, __FLAG__) (((__HANDLE__)->Instance->SR) = (__FLAG__))

/**
  * @}
  */ 


/* Include DAC HAL Extension module */
#include "stm32l0xx_hal_dac_ex.h"

/* Exported functions --------------------------------------------------------*/

/** @defgroup DAC_Exported_Functions DAC Exported Functions
  * @{
  */

/** @defgroup DAC_Exported_Functions_Group1 Initialization and de-initialization functions
  * @{
  */ 
/* Initialization and de-initialization functions *****************************/
HAL_StatusTypeDef HAL_DAC_Init(DAC_HandleTypeDef* hdac);
HAL_StatusTypeDef HAL_DAC_DeInit(DAC_HandleTypeDef* hdac);
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac);
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac);

/**
  * @}
  */

/** @defgroup DAC_Exported_Functions_Group2 IO operation functions
  * @{
  */
/* IO operation functions *****************************************************/
HAL_StatusTypeDef HAL_DAC_Start(DAC_HandleTypeDef* hdac, uint32_t Channel);
HAL_StatusTypeDef HAL_DAC_Stop(DAC_HandleTypeDef* hdac, uint32_t Channel);
HAL_StatusTypeDef HAL_DAC_Start_DMA(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t* pData, uint32_t Length, uint32_t Alignment);
HAL_StatusTypeDef HAL_DAC_Stop_DMA(DAC_HandleTypeDef* hdac, uint32_t Channel);
HAL_StatusTypeDef HAL_DAC_SetValue(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t Alignment, uint32_t Data);
uint32_t HAL_DAC_GetValue(DAC_HandleTypeDef* hdac, uint32_t Channel);
void HAL_DAC_IRQHandler(DAC_HandleTypeDef* hdac);
void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac);
void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef* hdac);
void HAL_DAC_ErrorCallbackCh1(DAC_HandleTypeDef *hdac);
void HAL_DAC_DMAUnderrunCallbackCh1(DAC_HandleTypeDef *hdac);

/**
  * @}
  */

/** @defgroup DAC_Exported_Functions_Group3 Peripheral Control functions
  * @{
  */
/* Peripheral Control functions ***********************************************/
HAL_StatusTypeDef HAL_DAC_ConfigChannel(DAC_HandleTypeDef* hdac, DAC_ChannelConfTypeDef* sConfig, uint32_t Channel);

/**
  * @}
  */

/** @defgroup DAC_Exported_Functions_Group4 Peripheral State and Errors functions
  * @{
  */
/* Peripheral State functions ***************************************************/
HAL_DAC_StateTypeDef HAL_DAC_GetState(DAC_HandleTypeDef* hdac);
uint32_t HAL_DAC_GetError(DAC_HandleTypeDef *hdac);
__weak HAL_StatusTypeDef HAL_DAC_SetValue(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t Alignment, uint32_t Data);


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* STM32L031xx && STM32L041xx && STM32L061xx && STM32L071xx && STM32L081xx*/
  
#ifdef __cplusplus
}
#endif

#endif /*__STM32L0xx_HAL_DAC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
