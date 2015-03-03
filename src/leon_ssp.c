
/* Includes ------------------------------------------------------------------- */
#include "common.h"
#include "leon_ssp.h"
#include "HAL.h"

static void setSSPclock(LEON_SSP_TypeDef *SSPx, UINT32 target_clock);



/*********************************************************************//**
 * @brief       Setup clock rate for SSP device
 * @param[in]   SSPx    selected SSP peripheral definition
 *
 * @param[in]   target_clock : clock of SSP (Hz)
 * @return      None
 ***********************************************************************/
static void setSSPclock (LEON_SSP_TypeDef *SSPx, UINT32 target_clock)
{
UINT32 ssp_clk = 0;
UINT32 reg = 0;
UINT8 pm    = 0;
UINT8 fact  = 1;
UINT8 div16 = 1;
BOOLEAN divider_found = FALSE;

while(!divider_found)
    {
    ssp_clk = CPU_CLOCK_HZ / div16 / (4-2*fact) / (pm + 1);
    
    if (ssp_clk > target_clock)
        {
        if (pm == SSP_MODE_PM_MASK && div16 == 16 && fact == 0)
            {
            /* No applicable dividers were found */
            break;
            }
        
        pm ++;
        
        if(pm == SSP_MODE_PM_MASK)
            {
            if (div16 == 1)
                {
                pm = 0;
                div16 = 16;
                }
            else if (fact == 1)
                {
                pm >>= 1;
                fact = 0;
                }
            }
        }
    else
        {
        divider_found = TRUE;
        }
    }

if (divider_found)
    {
    reg |= SSP_MODE_PM(pm);
    if (fact == 1)
        reg |= SSP_MODE_FACT;
    if (div16 == 16)
        reg |= SSP_MODE_DIV16;
    
    SSPx->MODE |= reg;
    }
}



/********************************************************************//**
* @brief        Initializes the SSP peripheral according to the specified
*               parameters in the SSP_ConfigStruct.
* @param[in]    SSPx selected SSP peripheral
* 
* @param[in]    SSP_ConfigStruct Pointer to a SSP_CFG_Type structure
*                    that contains the configuration information for the
*                    specified SSP peripheral.
* @return       None
*********************************************************************/
void SSP_Init(LEON_SSP_TypeDef *SSPx, SSP_CFG_Type *SSP_ConfigStruct)
{
UINT32 tmp = 0;

tmp |= SSP_MODE_REV; /* output MSB first */

tmp |= (SSP_ConfigStruct->CPHA | SSP_ConfigStruct->CPOL | SSP_ConfigStruct->Mode |
       SSP_ConfigStruct->Databit);

SSPx->MODE = tmp;

// Set clock rate for SSP peripheral
setSSPclock(SSPx, SSP_ConfigStruct->ClockRate);
}



/*****************************************************************************//**
* @brief        Fills each SSP_InitStruct member with its default value:
* 
* @param[in]    SSP_InitStruct Pointer to a SSP_CFG_Type structure
*                    which will be initialized.
* @return       None
*******************************************************************************/
void SSP_ConfigStructInit(SSP_CFG_Type *SSP_InitStruct)
{
SSP_InitStruct->CPHA = SSP_CPHA_FIRST;
SSP_InitStruct->CPOL = SSP_CPOL_HI;
SSP_InitStruct->ClockRate = 1000000;
SSP_InitStruct->Databit = SSP_DATABIT_8;
SSP_InitStruct->Mode = SSP_MASTER_MODE;

}


/*********************************************************************//**
 * @brief       Transmit a single data through SSP peripheral
 * @param[in]   SSPx    selected SSP peripheral
 *
 * @param[in]   Data    Data to transmit (must be 16 or 8-bit long,
 *                      this depend on SSP data bit number configured)
 * @return      none
 **********************************************************************/
void SSP_SendData(LEON_SSP_TypeDef* SSPx, UINT32 Data)
{
SSPx->TX = SSP_TX_BITMASK(Data);
}


/*********************************************************************//**
* @brief        Receive a single data from SSPx peripheral
* @param[in]    SSPx    selected SSP peripheral
*
* @return       Data received(32 - bit long)
***********************************************************************/
UINT32 SSP_ReceiveData(LEON_SSP_TypeDef* SSPx)
{
return ((UINT32)(SSP_RX_BITMASK(SSPx->RX)));
}


/*********************************************************************//**
 * @brief       Checks whether the specified SSP status flag is set or not
 * @param[in]   SSPx    selected SSP peripheral
 *
 * @param[in]   FlagType    Type of flag to check status, should be one
 *                          of following:
 *                          - SSP_STAT_TXFIFO_EMPTY: TX FIFO is empty
 *                          - SSP_STAT_TXFIFO_NOTFULL: TX FIFO is not full
 *                          - SSP_STAT_RXFIFO_NOTEMPTY: RX FIFO is not empty
 *                          - SSP_STAT_RXFIFO_FULL: RX FIFO is full
 *                          - SSP_STAT_BUSY: SSP peripheral is busy
 * @return      New State of specified SSP status flag
 **********************************************************************/
FlagStatus SSP_GetStatus(LEON_SSP_TypeDef* SSPx, UINT32 FlagType)
{
return ((SSPx->EVENT & FlagType) ? SET : RESET);
}


/*********************************************************************//**
 * @brief       Enable or disable SSP peripheral's operation
 * @param[in]   SSPx    selected SSP peripheral
 * 
 * @param[in]   NewState New State of SSPx peripheral's operation
 * @return      none
 **********************************************************************/
void SSP_Cmd(LEON_SSP_TypeDef* SSPx, FunctionalState NewState)
{
if (NewState == ENABLE)
    {
    SSPx->MODE |= SSP_MODE_EN;
    }
else
    {
    SSPx->MODE &= (~SSP_MODE_EN) & SSP_MODE_EN;
    }
}

