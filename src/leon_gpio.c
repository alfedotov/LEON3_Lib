
/* Includes ------------------------------------------------------------------- */
#include <sys/types.h>
#include "common.h"
#include "leon_gpio.h"
#include "HAL.h"




/* GPIO ------------------------------------------------------------------------------ */

/*********************************************************************//**
 * @brief       Initialize the GPIO component by power on the clock
 *              supplied to it.
 * @return      None
 *
 * Note:
 **********************************************************************/
void GPIO_Init(void)
{
//CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCGPIO, ENABLE);

return;
}


/*********************************************************************//**
 * @brief       De-initialize the GPIO component by power off the clock
 *              supplied to it.
 * @return      None
 *
 * Note:
 **********************************************************************/
void GPIO_Deinit(void)
{
//CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCGPIO, DISABLE);

return;
}


/*********************************************************************//**
 * @brief       Set Direction for GPIO port.
 * @param[in]   portNum     Port Number value, should be in range from 0 to 4
 * @param[in]   bitValue    Value that contains all bits to set direction,
 *                          in range from 0 to 0xFFFFFFFF.
 *                          example: value 0x5 to set direction for bit 0 and bit 1.
 * @param[in]   dir         Direction value, should be:
 *                          - 0: Input.
 *                          - 1: Output.
 * @return      None
 *
 * Note: All remaining bits that are not activated in bitValue (value '0')
 * will not be effected by this function.
 **********************************************************************/
void GPIO_SetDir(LEON_GPIO_TypeDef *pGPIO, UINT32 bitValue, UINT8 dir)
{
if (pGPIO != NULL)
    {
    // Enable Output
    (dir)? (pGPIO->IO_DIR |= bitValue) : (pGPIO->IO_DIR &= ~bitValue);
    }
}


/*********************************************************************//**
 * @brief       Set Value for bits that have output direction on GPIO port.
 * @param[in]   portNum     Port number value, should be in range from 0 to 4
 * @param[in]   bitValue    Value that contains all bits on GPIO to set,
 *                          in range from 0 to 0xFFFFFFFF.
 *                          example: value 0x5 to set bit 0 and bit 1.
 * @return      None
 *
 * Note:
 * - For all bits that has been set as input direction, this function will
 * not effect.
 * - For all remaining bits that are not activated in bitValue (value '0')
 * will not be effected by this function.
 **********************************************************************/
void GPIO_SetValue(LEON_GPIO_TypeDef *pGPIO, UINT32 bitValue)
{
if (pGPIO != NULL)
    {
    pGPIO->IO_OUTPUT |= bitValue;
    }
}

/*********************************************************************//**
 * @brief       Clear Value for bits that have output direction on GPIO port.
 * @param[in]   portNum     Port number value, should be in range from 0 to 4
 * @param[in]   bitValue    Value that contains all bits on GPIO to clear,
 *                          in range from 0 to 0xFFFFFFFF.
 *                          example: value 0x5 to clear bit 0 and bit 1.
 * @return      None
 *
 * Note:
 * - For all bits that has been set as input direction, this function will
 * not effect.
 * - For all remaining bits that are not activated in bitValue (value '0')
 * will not be effected by this function.
 **********************************************************************/
void GPIO_ClearValue(LEON_GPIO_TypeDef *pGPIO, UINT32 bitValue)
{
if (pGPIO != NULL)
    {
    pGPIO->IO_OUTPUT &= ~bitValue;
    }
}

/*********************************************************************//**
 * @brief       Output to the GPIO pin an expected value
 * @param[in]   portNum     Port number value, should be in range from 0 to 4
 * @param[in]   bitValue    Value that contains all bits on GPIO to clear,
 *                          in range from 0 to 0xFFFFFFFF.
 *                          example: value 0x5 to clear bit 0 and bit 1.
 * @return      None
 *
 * Note:
 * - For all bits that has been set as input direction, this function will
 * not effect.
 * - For all remaining bits that are not activated in bitValue (value '0')
 * will not be effected by this function.
 **********************************************************************/
void GPIO_OutputValue(LEON_GPIO_TypeDef *pGPIO, UINT32 bitMask, UINT8 value)
{
(value == 0)? GPIO_ClearValue(pGPIO, bitMask) : GPIO_SetValue(pGPIO, bitMask);
}


/*********************************************************************//**
 * @brief       Read Current state on port pin that have input direction of GPIO
 * @param[in]   portNum     Port number to read value, in range from 0 to 4
 * @return      Current value of GPIO port.
 *
 * Note: Return value contain state of each port pin (bit) on that GPIO regardless
 * its direction is input or output.
 **********************************************************************/
UINT32 GPIO_ReadValue(LEON_GPIO_TypeDef *pGPIO)
{
if (pGPIO != NULL)
    {
    return pGPIO->IO_DATA;
    }

return (0);
}



