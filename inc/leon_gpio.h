#ifndef __leon_gpio_h
#define __leon_gpio_h


/*------------- General Purpose Input/Output (GPIO) --------------------------*/
typedef struct
    {
    volatile UINT32 IO_DATA;    /* 0x00 I/O port data register                   */
    volatile UINT32 IO_OUTPUT;  /* 0x04 I/O port output register                 */
    volatile UINT32 IO_DIR;     /* 0x08 I/O port direction register              */
    volatile UINT32 INT_MASK;   /* 0x0C Interrupt mask register                  */
    volatile UINT32 INT_POL;    /* 0x10 Interrupt polarity register              */
    volatile UINT32 INT_EDGE;   /* 0x14 Interrupt edge register                  */
    volatile UINT32 BYPASS;     /* 0x18 Bypass register                          */
    volatile UINT32 CAP;        /* 0x1C Capability register                      */
    volatile UINT32 INT_MAP[8]; /* 0x20 - 0x3C Interrupt map register(s).
                                   Address 0x20 + 4*n contains interrupt map
                                   registers for IO[4*n : 3+4+n], if implemented */
    } LEON_GPIO_TypeDef;



/*********************************************************************//**
 * Macro defines for I/O port data register
 **********************************************************************/

#define GPIO_DIRECTION_INPUT        (0)
#define GPIO_DIRECTION_OUTPUT       (1)



/* GPIO Init/DeInit functions --------------------------------------------------*/
void GPIO_Init(void);
void GPIO_Deinit(void);
void GPIO_SetDir(LEON_GPIO_TypeDef *pGPIO, UINT32 bitValue, UINT8 dir);
void GPIO_SetValue(LEON_GPIO_TypeDef *pGPIO, UINT32 bitValue);
void GPIO_ClearValue(LEON_GPIO_TypeDef *pGPIO, UINT32 bitValue);
void GPIO_OutputValue(LEON_GPIO_TypeDef *pGPIO, UINT32 bitMask, UINT8 value);

UINT32 GPIO_ReadValue(LEON_GPIO_TypeDef *pGPIO);


#endif /* __leon_gpio_h */
