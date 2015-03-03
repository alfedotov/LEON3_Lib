#ifndef __leon_ssp_h
#define __leon_ssp_h


/*------------- Synchronous Serial Communication (SSP) -----------------------*/
typedef struct
    {
    volatile UINT32 CAP;          /* 0x00 Capability register                 */
    volatile UINT32 reserved[7];  /* 0x04-0x1C                                */
    volatile UINT32 MODE;         /* 0x20 Mode register                       */
    volatile UINT32 EVENT;        /* 0x24 Event register                      */
    volatile UINT32 MASK;         /* 0x28 Mask register                       */
    volatile UINT32 CMD;          /* 0x2C Command register                    */
    volatile UINT32 TX;           /* 0x30 Transmit register                   */
    volatile UINT32 RX;           /* 0x34 Receive register                    */
    volatile UINT32 SLAVESEL;     /* 0x38 Slave Select register (optional)    */
    volatile UINT32 AUTOSLAVESEL; /* 0x3C Automatic slave select register*    */
    volatile UINT32 AMCONFIG;     /* 0x40 AM Configuration register**         */
    volatile UINT32 AMPERIOD;     /* 0x44 AM Period register**                */
    volatile UINT32 reserved2[2]; /* 0x48-0x4C Reserved                       */
    volatile UINT32 AMMASK[4];    /* 0x50-0x5C AM Mask register(s)***         */
    volatile UINT32 gap[104];     /* gap from 0x5C to 0x200                   */
    volatile UINT32 AMTX[128];    /* 0x200-0x3FC AM Transmit register(s)****  */
    volatile UINT32 AMRX[128];    /* 0x400-0x5FC AM Receive register(s)****   */
    } LEON_SSP_TypeDef;

/* NOTE: 
* Only available if ASEL (bit 17) in the SPI controller Capability register is set.
** Only available if AMODE (bit 18) in the SPI controller Capability register is set.
*** Only available if AMODE (bit 18) in the SPI controller Capability register is set. 
Number of implemented registers depend on FDEPTH (bits 15:8) in the SPI controller 
Capability register in the following way: Number of registers = (FDEPTH-1)/32 + 1.
**** Only available if AMODE (bit 18) in the SPI controller Capability register is set. 
Number of implemented registers equals FDEPTH (bits 15:8) in the SPI controller 
Capability register.
*/


/*********************************************************************//**
 * Macro defines for Capability register
 **********************************************************************/
/* Slave Select register size (SSSZ) - If the core has been configured with slave select signals this field
contains the number of available signals. This field is only valid is the SSEN bit (bit 16) is ‘1 */
#define SSP_CAP_SSSZ_MASK    (0xFF)
#define SSP_CAP_SSSZ(n)      ((UINT32)((n&SSP_CAP_SSSZ_MASK)<<24))

/* Maximum word Length (MAXWLEN) - The maximum word length supported by the core:
0b0000 - 4-16, and 32-bit word length
0b0011-0b1111 - Word length is MAXWLEN+1, allows words of length 4-16 bits.
The core must not be configured to use a word length greater than what is defined by this register.*/
#define SSP_CAP_MAXWLEN_MASK (0xF)
#define SSP_CAP_MAXWLEN(n)   ((UINT32)((n&SSP_CAP_MAXWLEN_MASK)<<20))

/* Three-wire mode Enable (TWEN) - If this bit is ‘1’ the core supports three-wire mode. */
#define SSP_CAP_TWEN         ((UINT32)(1<<19))

/* Auto mode (AMODE) - If this bit is ‘1’ the core supports Automated transfers.*/
#define SSP_CAP_AMODE        ((UINT32)(1<<18))

/* Automatic slave select available (ASELA) - If this bit is set, the core has support for setting slave
select signals automatically. */
#define SPI_CAP_ASELA        ((UINT32)(1<<17))

/* Slave Select Enable (SSEN) - If the core has a slave select register, and corresponding slave select
lines, the value of this field is one. Otherwise the value of this field is zero.*/
#define SSP_CAP_SSEN         ((UINT32)(1<<16))

/* FIFO depth (FDEPTH) - This field contains the depth of the core’s internal FIFOs. The number of
words the core can store in each queue is FDEPTH+1, since the transmit and receive registers can
contain one word each. */
#define SSP_CAP_FDEPTH_MASK  (0xFF)
#define SSP_CAP_FDEPTH(n)    ((UINT32)((n&SSP_CAP_FDEPTH_MASK)<<8))

/* SYNCRAM (SR) - If this field is ‘1’ the core has buffers implemented with SYNCRAM components. */
#define SSP_CAP_SR           ((UINT32)(1<<7))

/* Fault-tolerance (FT) - This field signals if the core has any fault-tolerant capabilities. “00” - No
fault-tolerance. “01” - Parity DMR, “10” - TMR. */
#define SSP_CAP_FT_MASK      (0x3)
#define SSP_CAP_FT(n)        ((UINT32)((n&SSP_CAP_FT_MASK)<<5))

/* Core revision (REV) - This manual applies to core revision 5. */
#define SSP_CAP_REV_MASK     (0x1F)
#define SSP_CAP_REV          ((UINT32)(SSP_CAP_REV_MASK<<0)



/*********************************************************************//**
 * Macro defines for Mode register
 **********************************************************************/
/* Auto mode enable (AMEN) - When this bit is set to ‘1’ the core will be able to perform automated
periodic transfers. See the AM registers below. The core supports this mode if the AMODE field in
the capability register is set to ‘1’. Otherwise writes to this field has no effect. When this bit is set to
‘1’ the core can only perform automated transfers. Software is allowed to initialize the transmit
queue and to read out the receive queue but no transfers except the automated periodic transfers may
be performed. The core must be configured to act as a master (MS field set to ‘1’) when performing
automated transfers. */
#define SSP_MODE_AMEN        ((UINT32)(1<<31))

/* Loop mode (LOOP) - When this bit is set, and the core is enabled, the core’s transmitter and receiver
are interconnected and the core will operate in loopback mode. The core will still detect, and will be
disabled, on Multiple-master errors. */
#define SSP_MODE_LOOP        ((UINT32)(1<<30))

/* Clock polarity (CPOL) - Determines the polarity (idle state) of the SCK clock. */
#define SSP_MODE_CPOL        ((UINT32)(1<<29))

/* Clock phase (CPHA) - When CPHA is ‘0’ data will be read on the first transition of SCK. When
CPHA is ‘1’ data will be read on the second transition of SCK.*/
#define SSP_MODE_CPHA        ((UINT32)(1<<28))

/* Divide by 16 (DIV16) - Divide system clock by 16, see description of PM field below and see section
96.2.4 on clock generation. This bit has no significance in slave mode. */
#define SSP_MODE_DIV16       ((UINT32)(1<<27))

/* Reverse data (REV) - When this bit is ‘0’ data is transmitted LSB first, when this bit is ‘1’ data is
transmitted MSB first. This bit affects the layout of the transmit and receive registers.*/
#define SSP_MODE_REV         ((UINT32)(1<<26))

/* Master/Slave (MS) - When this bit is set to ‘1’ the core will act as a master, when this bit is set to ‘0’
the core will operate in slave mode. */
#define SSP_MODE_MS          ((UINT32)(1<<25))

/* Enable core (EN) - When this bit is set to ‘1’ the core is enabled. No fields in the mode register
should be changed while the core is enabled. This can bit can be set to ‘0’ by software, or by the core
if a multiple-master error occurs. */
#define SSP_MODE_EN          ((UINT32)(1<<24))

/* Word length (LEN) - The value of this field determines the length in bits of a transfer on the SPI bus.
Values are interpreted as:
0b0000 - 32-bit word length
0b0001-0b0010 - Illegal values
0b0011-0b1111 - Word length is LEN+1, allows words of length 4-16 bits.
The value of this field must never specify a word length that is greater than the maximum allowed
word length specified by the MAXWLEN field in the Capability register. */
#define SSP_MODE_LEN_MASK    (0xF)
#define SSP_MODE_LEN(n)      ((UINT32)((n-1)&SSP_MODE_LEN_MASK) << 20)

/* Prescale modulus (PM) - This value is used in master mode to divide the system clock and generate
the SPI SCK clock. The value in this field depends on the value of the FACT bit.
If bit 13 (FACT) is ‘0’:The system clock is divided by 4*(PM+1) if the DIV16 field is ‘0’ and
16*4*(PM+1) if the DIV16 field is set to ‘1’. The highest SCK frequency is attained when PM is set
to 0b0000 and DIV16 to ‘0’, this configuration will give a SCK frequency that is (system clock)/4.
With this setting the core is compatible with the SPI register interface found in MPC83xx SoCs.
If bit 13 (FACT) is ‘1’: The system clock is divided by 2*(PM+1) if the DIV16 field is ‘0’ and
16*2*(PM+1) if the DIV16 field is set to ‘1’. The highest SCK frequency is attained when PM is set
to 0b0000 and DIV16 to ‘0’, this configuration will give a SCK frequency that is (system clock)/2.
In slave mode the value of this field defines the number of system clock cycles that the SCK input
must be stable for the core to accept the state of the signal. See section 96.2.5. */
#define SSP_MODE_PM_MASK     (0xF)
#define SSP_MODE_PM(n)       ((UINT32)((n&SSP_MODE_PM_MASK)<<16))

/* Three-wire mode (TW) - If this bit is set to ‘1’ the core will operate in 3-wire mode. This bit can
only be set if the TWEN field of the Capability register is set to ‘1’.*/
#define SSP_MODE_TWEN        ((UINT32)(1<<15))

/* Automatic slave select (ASEL) - If this bit is set to ‘1’ the core will swap the contents in the Slave
select register with the contents of the Automatic slave select register when a transfer is started and
the core is in master mode. When the transmit queue is empty, the slave select register will be
swapped back. Note that if the core is disabled (by writing to the core enable bit or due to a multiplemaster-
error (MME)) when a transfer is in progress, the registers may still be swapped when the core
goes idle. This bit can only be set if the ASELA field of the Capability register is set to ‘1’. Also see
the ASELDEL field which can be set to insert a delay between the slave select register swap and the
start of a transfer. */
#define SSP_MODE_ASEL        ((UINT32)(1<<14))

/* PM factor (FACT) - If this bit is 1 the core’s register interface is no longer compatible with the
MPC83xx register interface. The value of this bit affects how the PM field is utilized to scale the SPI
clock. See the description of the PM field. */
#define SSP_MODE_FACT        ((UINT32)(1<<13))

/* Open drain mode (OD) - If this bit is set to ‘0’, all pins are configured for operation in normal mode.
If this bit is set to ‘1’ all pins are set to open drain mode. The implementation of the core may or may
not support open drain mode. If this bit can be set to ‘1’ by writing to this location, the core supports
open drain mode. The pins driven from the slave select register are not affected by the value of this
bit. */
#define SSP_MODE_OD          ((UINT32)(1<<12))

/* Clock gap (CG) - The value of this field is only significant in master mode. The core will insert CG
SCK clock cycles between each consecutive word. This only applies when the transmit queue is kept
non-empty. After the last word of the transmit queue has been sent the core will go into an idle state
and will continue to transmit data as soon as a new word is written to the transmit register, regardless
of the value in CG. A value of 0b00000 in this field enables back-to-back transfers. */
#define SSP_MODE_CG_MASK     (0x1F)
#define SSP_MODE_CG(n)       ((UINT32)((n&SSP_MODE_CG_MASK)<<7))

/* Automatic Slave Select Delay (ASELDEL) - If the core is configured to use automatic slave select
(ASEL field set to ‘1’) the core will insert a delay corresponding to ASELDEL*(SPI SCK cycle
time)/2 between the swap of the slave select registers and the first toggle of the SCK clock. As an
example, if this field is set to “10” the core will insert a delay corresponding to one SCK cycle
between assigning the Automatic slave select register to the Slave select register and toggling SCK
for the first time in the transfer. This field can only be set if the ASELA field of the Capability register
is set to ‘1’. */
#define SSP_MODE_ASELDEL_MASK (0x3)
#define SSP_MODE_ASELDEL(n)  ((UINT32)((n&SSP_MODE_ASELDEL_MASK)<<5))

/* Toggle Automatic slave select during Clock Gap (TAC) - If this bit is set, and the ASEL field is set,
the core will perform the swap of the slave select registers at the start and end of each clock gap. The
clock gap is defined by the CG field and must be set to a value >= 2 if this field is set. This field can
only be set if the ASELA field of the Capability register is set to ‘1’. */
#define SSP_MODE_TAC         ((UINT32)(1<<4))

/* 3-wire Transfer Order (TTO) - This bit controls if the master or slave transmits a word first in 3-wire
mode.If this bit is ‘0’, data is first transferred from the master to the slave. If this bit is ‘1’, data is
first transferred from the slave to the master. This bit can only be set if the TWEN field of the Capability
register is set to ‘1’. */
#define SSP_MODE_TTO         ((UINT32)(1<<3))

/* Ignore SPISEL input (IGSEL) - If this bit is set to ‘1’ then the core will ignore the value of the
SPISEL input. */
#define SSP_MODE_IGSEL       ((UINT32)(1<<2))

/* Require Clock Idle for Transfer End (CITE) - If this bit is ‘0’ the core will regard the transfer of a
word as completed when the last bit has been sampled. If this bit is set to ‘1’ the core will wait until
it has set the SCK clock to its idle level (see CI field) before regarding a transfer as completed. This
setting only affects the behavior of the TIP status bit, and automatic slave select toggling at the end
of a transfer, when the clock phase (CP field) is ‘0’. */
#define SSP_MODE_CITE        ((UINT32)(1<<1))

/* RESERVED (R) - Read as zero and should be written as zero to ensure forward compatibility. */
#define SSP_MODE_R           ((UINT32)(1<<0))



/*********************************************************************//**
 * Macro defines for Event register
 **********************************************************************/
/* Transfer in progress (TIP) - This bit is ‘1’ when the core has a transfer in progress. Writes have no
effect. This bit is set when the core starts a transfer and is reset to ‘0’ once the core considers the
transfer to be finished. Behavior affected by setting of CITE field in Mode register. */
#define SSP_EVENT_TIP        ((UINT32)(1<<31))

/* RESERVED (R) - Read as zero and should be written to zero to ensure forward compatibility. */
#define SSP_EVENT_R2_MASK    (0xFFFF)
#define SSP_EVENT_R2(n)      ((UINT32)((n&SSP_EVENT_R2_MASK)<<15))

/* Last character (LT) - This bit is set when a transfer completes if the transmit queue is empty and the
LST bit in the Command register has been written. This bit is cleared by writing ‘1’, writes of ‘0’
have no effect. */
#define SSP_EVENT_LT         ((UINT32)(1<<14))

/* RESERVED (R) - Read as zero and should be written to zero to ensure forward compatibility. */
#define SSP_EVENT_R1         ((UINT32)(1<<13))

/* Overrun (OV) - This bit gets set when the receive queue is full and the core receives new data. The
core continues communicating over the SPI bus but discards the new data. This bit is cleared by writing
‘1’, writes of ‘0’ have no effect. */
#define SSP_EVENT_OV         ((UINT32)(1<<12))

/* Underrun (UN) - This bit is only set when the core is operating in slave mode. The bit is set if the
core’s transmit queue is empty when a master initiates a transfer. When this happens the core will
respond with a word where all bits are set to ‘1’. This bit is cleared by writing ‘1’, writes of ‘0’ have
no effect. */
#define SSP_EVENT_UN         ((UINT32)(1<<11))

/* Multiple-master error (MME) - This bit is set when the core is operating in master mode and the
SPISEL input goes active. In addition to setting this bit the core will be disabled. This bit is cleared
by writing ‘1’, writes of ‘0’ have no effect. */
#define SSP_EVENT_MME        ((UINT32)(1<<10))

/* Not empty (NE) - This bit is set when the receive queue contains one or more elements. It is cleared
automatically by the core, writes have no effect. */
#define SSP_EVENT_NE         ((UINT32)(1<<9))

/* Not full (NF) - This bit is set when the transmit queue has room for one or more words. It is cleared
automatically by the core when the queue is full, writes have no effect. */
#define SSP_EVENT_NF         ((UINT32)(1<<8))

/* RESERVED (R) - Read as zero and should be written to zero to ensure forward compatibility. */
#define SSP_EVENT_R0_MASK    (0xFF)
#define SSP_EVENT_R0(n)      ((UINT32)((n&SSP_EVENT_R0_MASK)<<0))



/*********************************************************************//**
 * Macro defines for Mask register
 **********************************************************************/
 /* Transfer in progress enable (TIPE) - When this bit is set the core will generate an interrupt when the
TIP bit in the Event register transitions from ‘0’ to ‘1’. */
#define SSP_MASK_TIPE     ((UINT32)(1<<31))

/* RESERVED (R) - Read as zero and should be written to zero to ensure forward compatibility. */
#define SSP_MASK_R2_MASK  (0xFFFF)
#define SSP_MASK_R2(n)    ((UINT32)((n&SSP_MASK_R2_MASK)<<15))

/* Last character enable (LTE) - When this bit is set the core will generate an interrupt when the LT bit
in the Event register transitions from ‘0’ to ‘1’. */
#define SSP_MASK_LTE      ((UINT32)(1<<14))

/* RESERVED (R) - Read as zero and should be written to zero to ensure forward compatibility. */
#define SSP_MASK_R1       ((UINT32)(1<<13))

/* Overrun enable (OVE) - When this bit is set the core will generate an interrupt when the OV bit in
the Event register transitions from ‘0’ to ‘1’. */
#define SSP_MASK_OVE      ((UINT32)(1<<12))

/* Underrun enable (UNE) - When this bit is set the core will generate an interrupt when the UN bit in
the Event register transitions from ‘0’ to ‘1’. */
#define SSP_MASK_UNE      ((UINT32)(1<<11))

/* Multiple-master error enable (MMEE) - When this bit is set the core will generate an interrupt when
the MME bit in the Event register transitions from ‘0’ to ‘1’. */
#define SSP_MASK_MMEE     ((UINT32)(1<<10))

/* Not empty enable (NEE) - When this bit is set the core will generate an interrupt when the NE bit in
the Event register transitions from ‘0’ to ‘1’. */
#define SSP_MASK_NEE      ((UINT32)(1<<9))

/* Not full enable (NFE) - When this bit is set the core will generate an interrupt when the NF bit in the
Event register transitions from ‘0’ to ‘1’. */
#define SSP_MASK_NFE      ((UINT32)(1<<8))

/* RESERVED (R) - Read as zero and should be written to zero to ensure forward compatibility. */
#define SSP_MASK_R0_MASK  (0xFF)
#define SSP_MASK_R0(n)    ((UINT32)((n&SSP_MASK_R0_MASK)<<0))



/*********************************************************************//**
 * Macro defines for Commande register
 **********************************************************************/
/* RESERVED (R) - Read as zero and should be written to zero to ensure forward compatibility. */
#define SSP_CMD_R1_MASK   (0x1FF)
#define SSP_CMD_R1(n)     ((UINT32)((n&0xSSP_CMD_R1_MASK)<<23))

/* Last (LST) - After this bit has been written to ‘1’ the core will set the Event register bit LT when a
character has been transmitted and the transmit queue is empty. If the core is operating in 3-wire
mode the Event register bit is set when the whole transfer has completed. This bit is automatically
cleared when the Event register bit has been set and is always read as zero. */
#define SSP_CMD_LST       ((UINT32)(1<<22))

/* RESERVED (R) - Read as zero and should be written to zero to ensure forward compatibility. */
#define SSP_CMD_R0_MASK   (0x3FFFFF)
#define SSP_CMD_R0(n)     ((UINT32)((n&SSP_CMD_R0_MASK)<<0))



/*********************************************************************//**
 * Macro defines for Transmit register
 **********************************************************************/
 /** SSP data bit mask */
#define SSP_TX_BITMASK(n)   (n&0xFFFFFFFF)

 /*********************************************************************//**
 * Macro defines for Receive register
 **********************************************************************/
/** SSP data bit mask */
#define SSP_RX_BITMASK(n)   (n&0xFFFFFFFF)


 /*********************************************************************//**
 * Macro defines for Slave select register (optional)
 **********************************************************************/

 /*********************************************************************//**
 * Macro defines for Automatic slave select register
 **********************************************************************/

 /*********************************************************************//**
 * Macro defines for AM configuration register
 **********************************************************************/

 /*********************************************************************//**
 * Macro defines for AM period register
 **********************************************************************/

 /*********************************************************************//**
 * Macro defines for AM Mask register(s)
 **********************************************************************/

 /*********************************************************************//**
 * Macro defines for AM Transmit register(s)
 **********************************************************************/

 /*********************************************************************//**
 * Macro defines for AM Receive register(s)
 **********************************************************************/



/** @brief SSP configuration structure */
typedef struct {
    UINT32 Databit;        /** Databit number, should be SSP_DATABIT_x,
                           where x is in range from 4 - 32              */
    UINT32 CPHA;           /** Clock phase, should be:
                           - SSP_CPHA_FIRST: first clock edge
                           - SSP_CPHA_SECOND: second clock edge         */
    UINT32 CPOL;           /** Clock polarity, should be:
                           - SSP_CPOL_HI: high level
                           - SSP_CPOL_LO: low level                     */
    UINT32 Mode;           /** SSP mode, should be:
                           - SSP_MASTER_MODE: Master mode
                           - SSP_SLAVE_MODE: Slave mode                 */
    UINT32 FrameFormat;    /*                                           */
    UINT32 ClockRate;      /** Clock rate,in Hz                         */
} SSP_CFG_Type;


/** Clock phase control bit */
#define SSP_CPHA_FIRST         ((UINT32)(0))
#define SSP_CPHA_SECOND        SSP_MODE_CPHA

/** Clock polarity control bit */
/* There's no bug here!!!
 * - If bit[29] in MODE is 0: SSP controller maintains the bus clock low between frames.
 * That means the active clock is in HI state.
 * - If bit[29] in MODE is 1 (SSP_MODE_CPOL): SSP controller maintains the bus clock
 * high between frames. That means the active clock is in LO state.
 */
#define SSP_CPOL_HI             ((UINT32)(0))
#define SSP_CPOL_LO             SSP_MODE_CPOL


/** SSP master mode enable */
#define SSP_SLAVE_MODE          ((UINT32)(0))
#define SSP_MASTER_MODE         SSP_MODE_MS


/** SSP data bit number defines */
#define SSP_DATABIT_4           SSP_MODE_LEN(4)         /*!< Databit number = 4 */
#define SSP_DATABIT_5           SSP_MODE_LEN(5)         /*!< Databit number = 5 */
#define SSP_DATABIT_6           SSP_MODE_LEN(6)         /*!< Databit number = 6 */
#define SSP_DATABIT_7           SSP_MODE_LEN(7)         /*!< Databit number = 7 */
#define SSP_DATABIT_8           SSP_MODE_LEN(8)         /*!< Databit number = 8 */
#define SSP_DATABIT_9           SSP_MODE_LEN(9)         /*!< Databit number = 9 */
#define SSP_DATABIT_10          SSP_MODE_LEN(10)        /*!< Databit number = 10 */
#define SSP_DATABIT_11          SSP_MODE_LEN(11)        /*!< Databit number = 11 */
#define SSP_DATABIT_12          SSP_MODE_LEN(12)        /*!< Databit number = 12 */
#define SSP_DATABIT_13          SSP_MODE_LEN(13)        /*!< Databit number = 13 */
#define SSP_DATABIT_14          SSP_MODE_LEN(14)        /*!< Databit number = 14 */
#define SSP_DATABIT_15          SSP_MODE_LEN(15)        /*!< Databit number = 15 */
#define SSP_DATABIT_16          SSP_MODE_LEN(16)        /*!< Databit number = 16 */
#define SSP_DATABIT_32          SSP_MODE_LEN(0+1)       /*!< Databit number = 32 */


/** SSP status SSP Busy bit */
#define SSP_STAT_BUSY           SSP_EVENT_TIP


typedef enum { DISABLE = 0, ENABLE = !DISABLE } FunctionalState;
typedef enum { RESET = 0, SET = !RESET } FlagStatus, IntStatus, SetState;

/* SSP Init/DeInit functions --------------------------------------------------*/
void SSP_Init(LEON_SSP_TypeDef *SSPx, SSP_CFG_Type *SSP_ConfigStruct);

/* SSP configure functions ----------------------------------------------------*/
void SSP_ConfigStructInit(SSP_CFG_Type *SSP_InitStruct);


/* SSP enable/disable functions -----------------------------------------------*/
void SSP_Cmd(LEON_SSP_TypeDef* SSPx, FunctionalState NewState);

/* SSP get information functions ----------------------------------------------*/
FlagStatus SSP_GetStatus(LEON_SSP_TypeDef* SSPx, UINT32 FlagType);


/* SSP transfer data functions ------------------------------------------------*/
void SSP_SendData(LEON_SSP_TypeDef* SSPx, UINT32 Data);
UINT32 SSP_ReceiveData(LEON_SSP_TypeDef* SSPx);



#endif /* __leon_ssp_h */
