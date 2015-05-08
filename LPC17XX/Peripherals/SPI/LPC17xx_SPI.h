#ifndef _LPC17XX_SPI_H_
#define _LPC17XX_SPI_H_

//#define SPI_BIT_BASH_ENABLE

//====================================================================================
// Port definitions
enum
{
	SPI_PORT_0		= 0,
	SPI_PORT_1		= 1,
	SPI_BIT_BASH	= 2
};

//====================================================================================
// Configuration definitions to be used in the SPI_CFG_Type passed to SPI_Init

// Phase configuration, when is data samples 
enum
{
	SPI_CFG_CPHA_FIRST			= 0,	// First clock edge
	SPI_CFG_CPHA_SECOND			= 1		// Second clock edge
};

// Clock polarity configuration
enum
{
	SPI_CFG_CPOL_HIGH			= 0,	// Clcok is high when idle
	SPI_CFG_CPOL_LOW			= 1		// CLock is low when idle
};

// SPI mode configuration 
enum
{
	SPI_CFG_MODE_MASTER			= 0,	// We are the master
	SPI_CFG_MODE_SLAVE			= 1		// We are a slave
};

// Frame format configuration
enum
{
	SPI_CFG_FORAMT_SPI			= 0,	// Motorola SPI frame foramt
	SPI_CFG_FORMAT_TI			= 1,	// TI frame foramt
	SPI_CFG_FORMAT_MICROWIRE	= 2		// National microwaire frame wire
};

// SPI configuration structure
// This structure is used by the SPI_Init sturcture 
typedef struct
{
	uint32_t CPHA;			// Clock phase, should be:
							//	- SPI_CFG_CPHA_FIRST: first clock edge
							//	- SPI_CFG_CPHA_SECOND: second clock edge
	uint32_t CPOL;			// Clock polarity, should be:
							//	- SPI_CFG_CPOL_HIGH: high level
							//	- SPI_CFG_CPOL_LOW: low level
	uint32_t Mode;			// SSP mode, should be:
							// 	- SPI_CFG_MODE_MASTER: Master mode
							// 	- SPI_CFG_MODE_SLAVE: Slave mode
	uint32_t DataBits; 		// Databit number, should be SSP_DATABIT_x, where x is in range from 4 - 16
	uint32_t FrameFormat;	// Frame Format:
							//	- SPI_CFG_FORAMT_SPI: Motorola SPI frame format
							//	- SPI_CFG_FORMAT_TI: TI frame format
							//	- SPI_CFG_FORMAT_MICROWIRE: National Microwire frame format
	uint32_t Clock;			// Clock speed, in Hz
} SPI_CFG_Type;


//====================================================================================
// Function declerations
void SPI_SetConfigDefault(SPI_CFG_Type *config);
void SPI_Init(uint8_t port, SPI_CFG_Type *config);
void SPI_Deinit(uint8_t port);
void SPI_SendByte(uint8_t port, uint8_t data);
void SPI_WaitWhileBusy(uint8_t port);

# endif // _LPC17XX_SPI_H_