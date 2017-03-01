/*
 * Copyright (c) 2014 Nuvoton Technology Corp.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Description:   NUC970 MAC driver header file
 */

#ifndef  _NUC970_ETH_
#define  _NUC970_ETH_

#ifdef CONFIG_NUC970_EMAC0
#define MAC_BASE 0xB0002000 
#else
#define MAC_BASE 0xB0003000 
#endif

/// EMC Control Registers
#define CAMCMR       (MAC_BASE + 0x00)   /* CAM Command Register */
#define CAMEN        (MAC_BASE + 0x04)   /* CAM Enable Register */
#define CAM0M        (MAC_BASE + 0x08)
#define CAM0L        (MAC_BASE + 0x0c)

#define TXDLSA       (MAC_BASE + 0x88)   /* Transmit Descriptor Link List Start Address Register */
#define RXDLSA       (MAC_BASE + 0x8C)   /* Receive Descriptor Link List Start Address Register */
#define MCMDR        (MAC_BASE + 0x90)   /* MAC Command Register */
#define MIID         (MAC_BASE + 0x94)   /* MII Management Data Register */
#define MIIDA        (MAC_BASE + 0x98)   /* MII Management Control and Address Register */
#define FFTCR        (MAC_BASE + 0x9C)   /* FIFO Threshold Control Register */
#define TSDR         (MAC_BASE + 0xa0)   /* Transmit Start Demand Register */
#define RSDR         (MAC_BASE + 0xa4)   /* Receive Start Demand Register */
#define DMARFC       (MAC_BASE + 0xa8)   /* Maximum Receive Frame Control Register */
#define MIEN         (MAC_BASE + 0xac)   /* MAC Interrupt Enable Register */
// EMC Status Registers
#define MISTA        (MAC_BASE + 0xb0)   /* MAC Interrupt Status Register */
#define MGSTA        (MAC_BASE + 0xb4)   /* MAC General Status Register */
#define MPCNT        (MAC_BASE + 0xb8)   /* Missed Packet Count Register */
#define MRPC         (MAC_BASE + 0xbc)   /* MAC Receive Pause Count Register */
#define MRPCC        (MAC_BASE + 0xc0)   /* MAC Receive Pause Current Count Register */
#define MREPC        (MAC_BASE + 0xc4)   /* MAC Remote Pause Count Register */
#define DMARFS       (MAC_BASE + 0xc8)   /* DMA Receive Frame Status Register */
#define CTXDSA       (MAC_BASE + 0xcc)   /* Current Transmit Descriptor Start Address Register */
#define CTXBSA       (MAC_BASE + 0xd0)   /* Current Transmit Buffer Start Address Register */
#define CRXDSA       (MAC_BASE + 0xd4)   /* Current Receive Descriptor Start Address Register */
#define CRXBSA       (MAC_BASE + 0xd8)   /* Current Receive Buffer Start Address Register */



// CAM Command Register(CAMCMR)
#define CAM_AUP  0x0001  // Accept Packets with Unicast Address
#define CAM_AMP  0x0002  // Accept Packets with Multicast Address
#define CAM_ABP  0x0004  // Accept Packets with Broadcast Address
#define CAM_CCAM 0x0008  // 0: Accept Packets CAM Recognizes and Reject Others
                         // 1: Reject Packets CAM Recognizes and Accept Others
#define CAM_ECMP 0x0010  // Enable CAM Compare


// MAC Command Register(MCMDR)
#define MCMDR_RXON    0x00000001  // Receive ON
#define MCMDR_ALP     0x00000002  // Accept Long Packet
#define MCMDR_ARP     0x00000004  // Accept Runt Packet
#define MCMDR_ACP     0x00000008  // Accept Control Packet
#define MCMDR_AEP     0x00000010  // Accept Error Packet
#define MCMDR_SPCRC   0x00000020  // Accept Strip CRC Value
#define MCMDR_TXON    0x00000100  // Transmit On
#define MCMDR_NDEF    0x00000200  // No defer
#define MCMDR_SDPZ    0x00010000  // Send Pause
#define MCMDR_EnSQE   0x00020000  // Enable SQE test
#define MCMDR_FDUP    0x00040000  // Full Duplex
#define MCMDR_EnMDC   0x00080000  // Enable MDC signal
#define MCMDR_OPMOD   0x00100000  // Operation Mode
#define MCMDR_LBK     0x00200000  // Loop Back
#define MCMDR_SWR     0x01000000  // Software Reset


// MAC MII Management Data Control and Address Register(MIIDA)
#define MDCCR    0x00a00000  // MDC clock rating
//#define PHYAD    0x00000000  // PHY Address
#define PHYWR    0x00010000  // Write Operation
#define PHYBUSY  0x00020000  // Busy Bit
#define PHYPreSP 0x00040000  // Preamble Suppress



#define MAX_FRAME_SIZE        1520 // Rx Frame Max Size = 1520

#define RX_DESCRIPTOR_NUM CONFIG_SYS_RX_ETH_BUFFER   // Max number of Rx Frame Descriptors
#define TX_DESCRIPTOR_NUM 4    // Max number of Tx Frame Descriptors


// RX Frame Descriptor's Owner bit
#define RXfOwnership_DMA 0x80000000  // 10 = DMA
#define RXfOwnership_CPU 0x3fffffff  // 00 = CPU

// TX Frame Descriptor's Owner bit
#define TXfOwnership_DMA 0x80000000  // 1 = DMA
#define TXfOwnership_CPU 0x7fffffff  // 0 = CPU


// Rx Frame Descriptor Status
#define RXFD_RXGD    0x00100000  // Receiving Good packet received



// Tx Frame Descriptor's Control bits
#define MACTxIntEn    0x04
#define CRCMode       0x02
#define NoCRCMode     0x00
#define PaddingMode   0x01
#define NoPaddingMode 0x00

// Tx Frame Descriptor Status
#define TXFD_TXINTR 0x0001  // Interrupt on Transmit
#define TXFD_DEF    0x0002  // Transmit deferred
#define TXFD_TXCP   0x0008  // Transmission Completion
#define TXFD_EXDEF  0x0010  // Exceed Deferral
#define TXFD_NCS    0x0020  // No Carrier Sense Error
#define TXFD_TXABT  0x0040  // Transmission Abort
#define TXFD_LC     0x0080  // Late Collision
#define TXFD_TXHA   0x0100  // Transmission halted
#define TXFD_PAU    0x0200  // Paused
#define TXFD_SQE    0x0400  // SQE error


// Tx/Rx buffer descriptor structure
struct eth_descriptor;
struct eth_descriptor
{
        unsigned int  status1;
        unsigned char *buf;
        unsigned int  status2;
        struct eth_descriptor *next;
};


#endif  /* _NUC970_ETH_ */
