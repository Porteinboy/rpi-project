#ifndef __GPIO_ADDRESS__
#define __GPIO_ADDRESS__

#define GPIO_BASE 0xFE200000	// 0x7e200000

#define GPFSEL0 0x00
#define GPFSEL1 0x04
#define GPFSEL2 0x08
#define GPFSEL3 0x0c
#define GPFSEL4 0x10
#define GPFSEL5 0x14
#define GPSET0 0x1c
#define GPSET1 0x20
#define GPCLR0 0x28
#define GPCLR1 0x2c
#define GPLEV0 0x34
#define GPLEV1 0x38
#define GPEDS0 0x40
#define GPEDS1 0x44
#define GPREN0 0x4c
#define GPREN1 0x50
#define GPFEN0 0x58
#define GPFEN1 0x5C
#define GPHEN0 0x64
#define GPHEN1 0x68
#define GPLEN0 0x70
#define GPLEN1 0x74
#define GPAREN0 0x7c
#define GPAREN1 0x80
#define GPAFEN0 0x88
#define GPAFEN1 0x8c
#define GPIO_PUP_PDN_CNTRL_REG0 0xe4
#define GPIO_PUP_PDN_CNTRL_REG1 0xe8
#define GPIO_PUP_PDN_CNTRL_REG2 0xec
#define GPIO_PUP_PDN_CNTRL_REG3 0xf0

#define UARTS_NUM 2	// Note: In this example, we just use UART2 and UART3

/* Define the UART register addresses */
#define UART0_BASE 0xfe201000	// 0x7e201000
#define UART2_BASE 0xfe201400	// 0x7e201400
#define UART3_BASE 0xfe201600	// 0x7e201600
#define UART4_BASE 0xfe201800	// 0x7e201800
#define UART5_BASE 0xfe201a00	// 0x7e201a00

#define UART_REGLEN	0x200

/* Define the UART registers offset */
#define DR 0x00		// Data register
#define RSRECR 0x04	// Receive status register/error clear register
#define FR 0x18		// Flag register
#define ILPR 0x20	// Not in use
#define IBRD 0x24	// Integer Baud rate divisor
#define FBRD 0x28	// Fraction Baud rate divisor
#define LCRH 0x2c	// Line Control register
#define CR 0x30		// Control register
#define	IFLS 0x34	// Interrupt FIFO Level Select Register
#define	IMSC 0x38	// Interrupt Mask Set Clear Register
#define RIS 0x3c	// Raw Interrupt Status Register
#define MIS 0x40	// Masked Interrupt Status Register
#define ICR 0x44	// Interrupt Clear Register
#define DMACR 0x48	// DMA Control Register
#define ITCR 0x80	// Test Control Register
#define ITIP 0x84	// Integration test input Register
#define ITOP 0x88	// Integration test output Register
#define TDR 0x8c	// Test Date Register

/* Define the bitmask of DR (Data) Register */
#define DR_OE	0x00000800	// Overrun error
#define DR_BE	0x00000400	// Break error
#define DR_PE	0x00000200	// Parity error
#define DR_FE	0x00000100	// Framing error
#define DR_DATA	0x000000FF	// Data (RW)

/* Define the bitmask of RSRECR (Receive status register/error clear) Register */
#define RSRECR_OE 0x00000008	// Overrun error
#define RSRECR_BE 0x00000004	// Break error
#define RSRECR_PE 0x00000002	// Parity error
#define RSRECR_FE 0x00000001	// Framing error

/* Define the bitmask of UART_FR (Flag) Register */
#define UART_FR_RI	0x00000100	// Don't care
#define UART_FR_TXFE	0x00000080	// Transmit FIFO empty
#define UART_FR_RXFF	0x00000040	// Receive FIFO full
#define UART_FR_TXFF	0x00000020	// Transmit FIFO full
#define UART_FR_RXFE	0x00000010	// Receive FIFO empty
#define UART_FR_BUSY	0x00000008	// UART busy
#define UART_FR_DCD	0x00000004	// Unsupport, don't care
#define UART_FR_DSR	0x00000002	// Unsupport, don't care
#define UART_FR_CTS	0x00000001	// Clear to send

/* Define the bitmask of UART_LCRH Register */
#define LCRH_SPS	0x00000080	// Stick parity select
#define LCRH_WLEN_8BITS	0x00000060	// Word length - 8 bits, bit 5, 6: b11
#define LCRH_WLEN_7BITS	0x00000040	// Word length - 7 bits, bit 5, 6: b10
#define LCRH_WLEN_6BITS	0x00000020	// Word length - 6 bits, bit 5, 6: b01
					// Word length - 5 bits, bit 5, 6: b00
#define LCRH_EN_FIFO	0x00000010	// Enable fifo
#define LCRH_STP2	0x00000008	// Two stop bits
#define LCRH_EPS	0x00000004	// Partiy select(enabled parity): 0 - odd parity, 1 - even parity
#define LCRH_EN_PARITY	0x00000002	// Partiy enable(enabled parity): 0 - disable parity, 1 - enable parity
#define LCRH_BREAK	0x00000001	// Send break: set 1 to low level continually output on TxD after transmission completing.

/* Define the bitmask of UART_CR (Control) Register */
#define CR_CTSEN	0x00008000	// CTS hardware flow control enable
#define CR_RTSEN	0x00004000	// RTS hardware flow control enable
#define CR_OUT2		0x00002000	// Unsupported, write zero, read as don't care
#define CR_OUT1		0x00001000	// Unsupported, write zero, read as don't care
#define CR_RTS		0x00000800	// Request to send
#define CR_DTR		0x00000400	// Unsupported, write zero, read as don't care
#define CR_RXE		0x00000200	// Receive enable.
#define CR_TXE		0x00000100	// Transmit enable.
#define CR_LBE		0x00000080	// Loopback enable.
#define CR_SIRLP	0x00000004	// Unsupported, write zero, read as don't care
#define CR_SIREN	0x00000002	// Unsupported, write zero, read as don't care
#define CR_UARTEN	0x00000001	// UART enable.

/* Define the bitmask of UART_IFLS (Interrupt FIFO Level Select) Register */
#define	IFLS_RXIFPSEL	0x00000E00	// Unsupported, write zero, read as don't care
#define	IFLS_TXIFPSEL	0x000001C0	// Unsupported, write zero, read as don't care
#define IFLS_RXIFLSEL	0x00000038	// Receive interrupt FIFO level select.
					// 0b000 = Receive FIFO become 1/8 full
					// 0b001 = Receive FIFO become 1/4 full
					// 0b010 = Receive FIFO become 1/2 full
					// 0b011 = Receive FIFO become 3/4 full
					// 0b100 = Receive FIFO become 7/8 full
#define IFLS_TXIFLSEL	0x00000007	// Transmit interrupt FIFO level select.
					// 0b000 = Transmit FIFO become 1/8 full
					// 0b001 = Transmit FIFO become 1/4 full
					// 0b010 = Transmit FIFO become 1/2 full
					// 0b011 = Transmit FIFO become 3/4 full
					// 0b100 = Transmit FIFO become 7/8 full

/* Define the bitmask of Interrupt Mask set/clear Register */
#define IMSC_OEIM	0x00000400	// Overrun error interrupt mask
#define IMSC_BEIM	0x00000200	// Break error interrupt mask
#define IMSC_PEIM	0x00000100	// Parity error interrupt mask
#define IMSC_FEIM	0x00000080	// Framing error interrupt mask
#define IMSC_REIM	0x00000040	// Receive timeout interrupt mask
#define IMSC_TXIM	0x00000020	// Transmit interrupt mask
#define IMSC_RXIM	0x00000010	// Receive interrupt mask
#define IMSC_CTS	0x00000002	// UARTCTS modem interrupt mask

/* Define the bitmask of RIS (Raw Interrupt Status) Register */
#define UARTOEINTR	0x00000400	// Overrun error interrupt status.
#define UARTBEINTR	0x00000200	// Break error interrupt status.
#define UARTPEINTR	0x00000100	// Parity error interrupt status.
#define UARTFEINTR	0x00000080	// Framing error interrupt status.
#define UARTRTINTR	0x00000040	// Receive timeout interrupt status.
#define UARTTXINTR	0x00000020	// Transmit interrupt status.
#define UARTCTSRMIS	0x00000002	// UART modem interrupt status.

/* Define the bitmask of MIS (Mask Interrupt Status) Register */
#define OEMIS		0x00000400	// Overrun error masked interrupt status
#define BEMIS		0x00000200	// Break error masked interrupt status
#define PEMIS		0x00000100	// Parity error masked interrupt status
#define FEMIS		0x00000080	// Framing error masked interrupt status
#define REMIS		0x00000040	// Receive timeout masked interrupt status
#define TXMIS		0x00000020	// Transmit masked interrupt status
#define RXMIS		0x00000010	// Receive masked interrupt status
#define CTSMMIS		0x00000002	// UARTCTS modem  masked interrupt status

/* Define the bitmask of ICR (Interrupt Clear Register) */
#define OEIC	0x00000400	// Overrun error interrupt clear.
#define BEIC	0x00000200	// Break error interrupt clear.
#define PEIC	0x00000100	// Parity error interrupt clear.
#define FEIC	0x00000080	// Framing error interrupt clear.
#define RTIC	0x00000040	// Receive timeout interrupt clear.
#define TXIC	0x00000020	// Transmit timeout interrupt clear.
#define RXIC	0x00000010	// Receive interrupt clear.
#define CTSMIC	0x00000002	// UART modem interrupt clear.

/* Define the bitmask of DMACR (DMA Control Register) */
#define DMAONERR	0x00000004	// DMA on error.
#define TXDMAE	0x00000002	// TX DMA enable.
#define RXDMAE	0x00000001	// RX DMA enable.

#endif // #define def __GPIO_ADDRESS__

