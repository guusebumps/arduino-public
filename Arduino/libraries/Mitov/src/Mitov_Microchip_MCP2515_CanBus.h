////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2024 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SPI.h>
#include <Mitov.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Extended,
		typename T_FBufferIndex,
		typename T_FailedOutputPin,
		typename T_ID,
        typename T_InitialValue,
		typename T_Priority,
		typename T_Request,
		typename T_SendTimeout,
		typename T_SentOutputPin
	> class Microchip_MCP2515_CanBus_SendMessage :
		public T_ClockInputPin_o_IsConnected,
		public T_Extended,
		public T_FBufferIndex,
		public T_FailedOutputPin,
		public T_ID,
        public T_InitialValue,
		public T_Priority,
		public T_Request,
		public T_SendTimeout,
		public T_SentOutputPin
	{
	public:
		_V_PIN_( FailedOutputPin )
		_V_PIN_( SentOutputPin )

	public:
		_V_PROP_( Extended )
		_V_PROP_( ID )
		_V_PROP_( InitialValue )
		_V_PROP_( Priority )
		_V_PROP_( Request )
		_V_PROP_( SendTimeout )

    public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		_V_PROP_( FBufferIndex )

		unsigned long	FSendTime;

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			uint8_t ABufferIndex = FBufferIndex();
            const uint8_t *ABuffer = T_InitialValue::Allocate();
   			C_OWNER.SendMessage( ID().GetValue(), Extended().GetValue(), Request().GetValue(), Priority().GetValue(), T_InitialValue::GetCount(), ABuffer, ABufferIndex );
            T_InitialValue::Release( ABuffer );

			FBufferIndex() = ABufferIndex;
//			Serial.println( ABufferIndex );
			FSendTime = millis();
		}

		inline void MessageInputPin_o_Receive( void *_Data )
		{
            InitialValue() = *(Mitov::TDataBlock*)_Data;
//			Mitov::TDataBlock *AData = (Mitov::TDataBlock*)_Data;
            
//            InitialValue().SetValue( AData->Read(), AData->GetSize() );
			if( ! ClockInputPin_o_IsConnected().GetValue() )
                ClockInputPin_o_Receive( nullptr );
		}

	public:
		inline void GetMessageTimeout( uint8_t ABufferIndex, bool & AResult )
		{
			if( AResult )
				return;

			if( ABufferIndex != FBufferIndex().GetValue() )
				return;

//			Serial.print( "." );
			if( millis() - FSendTime > SendTimeout().GetValue() )
			{
//				Serial.print( "Timeout" );
				AResult = true;
				FBufferIndex() = 3;
				T_FailedOutputPin::ClockPin();
			}

		}

		inline void MessageSendCompeled( uint8_t ABufferIndex )
		{
			if( ABufferIndex == FBufferIndex().GetValue() )
			{
				T_SentOutputPin::ClockPin();
				FBufferIndex() = 3;
			}

		}

	public:
		inline Microchip_MCP2515_CanBus_SendMessage()
		{
			FBufferIndex() = 3; // Invalid buffer (2 max)
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Extended,
		typename T_ExtendedOutputPin,
		typename T_FBufferIndex,
		typename T_FailedInputPin,
		typename T_ID,
        typename T_InitialValue,
		typename T_MessageOutputPin,
		typename T_Priority,
		typename T_RequestOutputPin,
		typename T_SendTimeout,
		typename T_SentInputPin
	> class Microchip_MCP2515_CanBus_RequestMessage : 
		public T_ClockInputPin_o_IsConnected,
		public T_Extended,
		public T_ExtendedOutputPin,
		public T_FBufferIndex,
		public T_FailedInputPin,
		public T_ID,
        public T_InitialValue,
		public T_MessageOutputPin,
		public T_Priority,
		public T_RequestOutputPin,
		public T_SendTimeout,
		public T_SentInputPin
	{
	public:
		_V_PIN_( ExtendedOutputPin )
		_V_PIN_( FailedInputPin )
		_V_PIN_( MessageOutputPin )
		_V_PIN_( RequestOutputPin )
		_V_PIN_( SentInputPin )

	public:
		_V_PROP_( Extended )
		_V_PROP_( ID )
		_V_PROP_( InitialValue )
		_V_PROP_( Priority )
		_V_PROP_( SendTimeout )

    public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		_V_PROP_( FBufferIndex )

		unsigned long	FSendTime;

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			uint8_t ABufferIndex = FBufferIndex();
            const uint8_t *ABuffer = T_InitialValue::Allocate();
   			C_OWNER.SendMessage( ID().GetValue(), Extended().GetValue(), true, Priority().GetValue(), T_InitialValue::GetCount(), ABuffer, ABufferIndex );
            T_InitialValue::Release( ABuffer );

			FBufferIndex() = ABufferIndex;
//			Serial.println( ABufferIndex );
			FSendTime = millis();

//			C_OWNER.SendMessage( ID().GetValue(), Extended().GetValue(), true, InitialValue.GetSize(), InitialValue.Read() );
		}

		inline void MessageInputPin_o_Receive( void *_Data )
		{
			InitialValue = *(Mitov::TDataBlock*)_Data;
			if( ! ClockInputPin_o_IsConnected().GetValue() )
                ClockInputPin_o_Receive( nullptr );

		}

	public:
		inline void GetMessageTimeout( uint8_t ABufferIndex, bool & AResult )
		{
			if( AResult )
				return;

			if( ABufferIndex != FBufferIndex().GetValue() )
				return;

//			Serial.print( "." );
			if( millis() - FSendTime > SendTimeout().GetValue() )
			{
//				Serial.print( "Timeout" );
				AResult = true;
				FBufferIndex() = 3;
				T_FailedInputPin::ClockPin();
			}

		}

		inline void MessageSendCompeled( uint8_t ABufferIndex )
		{
			if( ABufferIndex == FBufferIndex().GetValue() )
			{
				T_SentInputPin::ClockPin();
				FBufferIndex() = 3;
			}

		}

		inline void ProcessMessage( uint32_t AID, bool AExtended, bool ARequest, uint8_t  ALength, const uint8_t * ABuffer )
		{
			if( AID != ID().GetValue() )
				return;

//			T_IDOutputPin::SetPinValue( *(uint32_t *)ABuffer );

			T_ExtendedOutputPin::SetPinValue( AExtended );
			T_RequestOutputPin::SetPinValue( ARequest );

			if( T_MessageOutputPin::GetPinIsConnected() )
			{
				TDataBlock AData( ALength, ABuffer );
				T_MessageOutputPin::SetPinValue( AData );
			}

		}

	public:
		inline Microchip_MCP2515_CanBus_RequestMessage()
		{
			FBufferIndex() = 3; // Invalid buffer (2 max)
		}

	};
//---------------------------------------------------------------------------
	namespace MCP2515Const
	{
		const uint8_t CAN_MAX_CHAR_IN_MESSAGE = (8);

		const uint8_t MCP_SIDH        = 0;
		const uint8_t MCP_SIDL        = 1;
		const uint8_t MCP_EID8        = 2;
		const uint8_t MCP_EID0        = 3;

// Define SPI Instruction Set

		const uint8_t MCP_WRITE       = 0x02;
		const uint8_t MCP_READ        = 0x03;
		const uint8_t MCP_BITMOD      = 0x05;
		const uint8_t MCP_LOAD_TX0    = 0x40;
		const uint8_t MCP_LOAD_TX1    = 0x42;
		const uint8_t MCP_LOAD_TX2    = 0x44;

		const uint8_t MCP_RTS_TX_BASE = 0x80;

		const uint8_t MCP_RTS_TX0     = 0x81;
		const uint8_t MCP_RTS_TX1     = 0x82;
		const uint8_t MCP_RTS_TX2     = 0x84;
		const uint8_t MCP_RTS_ALL     = 0x87;
		const uint8_t MCP_READ_RX0    = 0x90;
		const uint8_t MCP_READ_RX1    = 0x94;
		const uint8_t MCP_READ_STATUS = 0xA0;
		const uint8_t MCP_RX_STATUS   = 0xB0;
		const uint8_t MCP_RESET       = 0xC0;

		const uint8_t MCP_RXF0SIDH    = 0x00;
		const uint8_t MCP_RXF0SIDL    = 0x01;
		const uint8_t MCP_RXF0EID8    = 0x02;
		const uint8_t MCP_RXF0EID0    = 0x03;
		const uint8_t MCP_RXF1SIDH    = 0x04;
		const uint8_t MCP_RXF1SIDL    = 0x05;
		const uint8_t MCP_RXF1EID8    = 0x06;
		const uint8_t MCP_RXF1EID0    = 0x07;
		const uint8_t MCP_RXF2SIDH    = 0x08;
		const uint8_t MCP_RXF2SIDL    = 0x09;
		const uint8_t MCP_RXF2EID8    = 0x0A;
		const uint8_t MCP_RXF2EID0    = 0x0B;
		const uint8_t MCP_BFPCTRL     = 0x0C;
		const uint8_t MCP_TXRTSCTRL   = 0x0D;
		const uint8_t MCP_CANSTAT     = 0x0E;
		const uint8_t MCP_CANCTRL     = 0x0F;
		const uint8_t MCP_RXF3SIDH    = 0x10;
		const uint8_t MCP_RXF3SIDL    = 0x11;
		const uint8_t MCP_RXF3EID8    = 0x12;
		const uint8_t MCP_RXF3EID0    = 0x13;
		const uint8_t MCP_RXF4SIDH    = 0x14;
		const uint8_t MCP_RXF4SIDL    = 0x15;
		const uint8_t MCP_RXF4EID8    = 0x16;
		const uint8_t MCP_RXF4EID0    = 0x17;
		const uint8_t MCP_RXF5SIDH    = 0x18;
		const uint8_t MCP_RXF5SIDL    = 0x19;
		const uint8_t MCP_RXF5EID8    = 0x1A;
		const uint8_t MCP_RXF5EID0    = 0x1B;
		const uint8_t MCP_TEC         = 0x1C;
		const uint8_t MCP_REC         = 0x1D;
		const uint8_t MCP_RXM0SIDH    = 0x20;
		const uint8_t MCP_RXM0SIDL    = 0x21;
		const uint8_t MCP_RXM0EID8    = 0x22;
		const uint8_t MCP_RXM0EID0    = 0x23;
		const uint8_t MCP_RXM1SIDH    = 0x24;
		const uint8_t MCP_RXM1SIDL    = 0x25;
		const uint8_t MCP_RXM1EID8    = 0x26;
		const uint8_t MCP_RXM1EID0    = 0x27;
		const uint8_t MCP_CNF3        = 0x28;
		const uint8_t MCP_CNF2        = 0x29;
		const uint8_t MCP_CNF1        = 0x2A;
		const uint8_t MCP_CANINTE     = 0x2B;
		const uint8_t MCP_CANINTF     = 0x2C;
		const uint8_t MCP_EFLG        = 0x2D;
		const uint8_t MCP_TXB0CTRL    = 0x30;
		const uint8_t MCP_TXB0SIDH    = 0x31;
		const uint8_t MCP_TXB1CTRL    = 0x40;
		const uint8_t MCP_TXB1SIDH    = 0x41;
		const uint8_t MCP_TXB2CTRL    = 0x50;
		const uint8_t MCP_TXB2SIDH    = 0x51;
		const uint8_t MCP_RXB0CTRL    = 0x60;
		const uint8_t MCP_RXB0SIDH    = 0x61;
		const uint8_t MCP_RXB1CTRL    = 0x70;
		const uint8_t MCP_RXB1SIDH    = 0x71;

		// CANCTRL Register Values

		const uint8_t MODE_NORMAL     = 0x00;
		const uint8_t MODE_SLEEP      = 0x20;
		const uint8_t MODE_LOOPBACK   = 0x40;
		const uint8_t MODE_LISTENONLY = 0x60;
		const uint8_t MODE_CONFIG     = 0x80;
		const uint8_t MODE_POWERUP    = 0xE0;
		const uint8_t MODE_MASK       = 0xE0;
		const uint8_t ABORT_TX        = 0x10;
		const uint8_t MODE_ONESHOT    = 0x08;
		const uint8_t CLKOUT_ENABLE   = 0x04;
		const uint8_t CLKOUT_DISABLE  = 0x00;
		const uint8_t CLKOUT_PS1      = 0x00;
		const uint8_t CLKOUT_PS2      = 0x01;
		const uint8_t CLKOUT_PS4      = 0x02;
		const uint8_t CLKOUT_PS8      = 0x03;

		const uint8_t MCP2515_OK         = 0;
		const uint8_t MCP2515_FAIL       = 1;
//		const uint8_t MCP_ALLTXBUSY      = 2;

		const uint8_t MCP_N_TXBUFFERS	= 3;

		// CANINTF Register Bits

		const uint8_t MCP_RX0IF       = 0x01;
		const uint8_t MCP_RX1IF       = 0x02;
		const uint8_t MCP_TX0IF       = 0x04;
		const uint8_t MCP_TX1IF       = 0x08;
		const uint8_t MCP_TX2IF       = 0x10;
		const uint8_t MCP_ERRIF       = 0x20;
		const uint8_t MCP_WAKIF       = 0x40;
		const uint8_t MCP_MERRF       = 0x80;

		// speed 16M

		const uint8_t MCP_16MHz_1000kBPS_CFG1 = (0x00);
		const uint8_t MCP_16MHz_1000kBPS_CFG2 = (0xD0);
		const uint8_t MCP_16MHz_1000kBPS_CFG3 = (0x82);

		const uint8_t MCP_16MHz_500kBPS_CFG1 = (0x00);
		const uint8_t MCP_16MHz_500kBPS_CFG2 = (0xF0);
		const uint8_t MCP_16MHz_500kBPS_CFG3 = (0x86);

		const uint8_t MCP_16MHz_250kBPS_CFG1 = (0x41);
		const uint8_t MCP_16MHz_250kBPS_CFG2 = (0xF1);
		const uint8_t MCP_16MHz_250kBPS_CFG3 = (0x85);

		const uint8_t MCP_16MHz_200kBPS_CFG1 = (0x01);
		const uint8_t MCP_16MHz_200kBPS_CFG2 = (0xFA);
		const uint8_t MCP_16MHz_200kBPS_CFG3 = (0x87);

		const uint8_t MCP_16MHz_125kBPS_CFG1 = (0x03);
		const uint8_t MCP_16MHz_125kBPS_CFG2 = (0xF0);
		const uint8_t MCP_16MHz_125kBPS_CFG3 = (0x86);

		const uint8_t MCP_16MHz_100kBPS_CFG1 = (0x03);
		const uint8_t MCP_16MHz_100kBPS_CFG2 = (0xFA);
		const uint8_t MCP_16MHz_100kBPS_CFG3 = (0x87);

		const uint8_t MCP_16MHz_95kBPS_CFG1 = (0x03);
		const uint8_t MCP_16MHz_95kBPS_CFG2 = (0xAD);
		const uint8_t MCP_16MHz_95kBPS_CFG3 = (0x07);

		const uint8_t MCP_16MHz_83kBPS_CFG1 = (0x03);
		const uint8_t MCP_16MHz_83kBPS_CFG2 = (0xBE);
		const uint8_t MCP_16MHz_83kBPS_CFG3 = (0x07);

		const uint8_t MCP_16MHz_80kBPS_CFG1 = (0x03);
		const uint8_t MCP_16MHz_80kBPS_CFG2 = (0xFF);
		const uint8_t MCP_16MHz_80kBPS_CFG3 = (0x87);

		const uint8_t MCP_16MHz_50kBPS_CFG1 = (0x07);
		const uint8_t MCP_16MHz_50kBPS_CFG2 = (0xFA);
		const uint8_t MCP_16MHz_50kBPS_CFG3 = (0x87);

		const uint8_t MCP_16MHz_40kBPS_CFG1 = (0x07);
		const uint8_t MCP_16MHz_40kBPS_CFG2 = (0xFF);
		const uint8_t MCP_16MHz_40kBPS_CFG3 = (0x87);

		const uint8_t MCP_16MHz_33kBPS_CFG1 = (0x09);
		const uint8_t MCP_16MHz_33kBPS_CFG2 = (0xBE);
		const uint8_t MCP_16MHz_33kBPS_CFG3 = (0x07);

		const uint8_t MCP_16MHz_31kBPS_CFG1 = (0x0F);
		const uint8_t MCP_16MHz_31kBPS_CFG2 = (0xF1);
		const uint8_t MCP_16MHz_31kBPS_CFG3 = (0x85);

		const uint8_t MCP_16MHz_25kBPS_CFG1 = (0X0F);
		const uint8_t MCP_16MHz_25kBPS_CFG2 = (0XBA);
		const uint8_t MCP_16MHz_25kBPS_CFG3 = (0X07);

		const uint8_t MCP_16MHz_20kBPS_CFG1 = (0x0F);
		const uint8_t MCP_16MHz_20kBPS_CFG2 = (0xFF);
		const uint8_t MCP_16MHz_20kBPS_CFG3 = (0x87);

		const uint8_t MCP_16MHz_10kBPS_CFG1 = (0x1F);
		const uint8_t MCP_16MHz_10kBPS_CFG2 = (0xFF);
		const uint8_t MCP_16MHz_10kBPS_CFG3 = (0x87);

		const uint8_t MCP_16MHz_5kBPS_CFG1 = (0x3F);
		const uint8_t MCP_16MHz_5kBPS_CFG2 = (0xFF);
		const uint8_t MCP_16MHz_5kBPS_CFG3 = (0x87);

		const uint8_t MCP_16MHz_666kBPS_CFG1 = (0x00);
		const uint8_t MCP_16MHz_666kBPS_CFG2 = (0xA0);
		const uint8_t MCP_16MHz_666kBPS_CFG3 = (0x04);


		// speed 8M

		const uint8_t MCP_8MHz_1000kBPS_CFG1 = (0x00);
		const uint8_t MCP_8MHz_1000kBPS_CFG2 = (0x80);
		const uint8_t MCP_8MHz_1000kBPS_CFG3 = (0x00);

		const uint8_t MCP_8MHz_500kBPS_CFG1 = (0x00);
		const uint8_t MCP_8MHz_500kBPS_CFG2 = (0x90);
		const uint8_t MCP_8MHz_500kBPS_CFG3 = (0x02);

		const uint8_t MCP_8MHz_250kBPS_CFG1 = (0x00);
		const uint8_t MCP_8MHz_250kBPS_CFG2 = (0xb1);
		const uint8_t MCP_8MHz_250kBPS_CFG3 = (0x05);

		const uint8_t MCP_8MHz_200kBPS_CFG1 = (0x00);
		const uint8_t MCP_8MHz_200kBPS_CFG2 = (0xb4);
		const uint8_t MCP_8MHz_200kBPS_CFG3 = (0x06);

		const uint8_t MCP_8MHz_125kBPS_CFG1 = (0x01);
		const uint8_t MCP_8MHz_125kBPS_CFG2 = (0xb1);
		const uint8_t MCP_8MHz_125kBPS_CFG3 = (0x05);

		const uint8_t MCP_8MHz_100kBPS_CFG1 = (0x01);
		const uint8_t MCP_8MHz_100kBPS_CFG2 = (0xb4);
		const uint8_t MCP_8MHz_100kBPS_CFG3 = (0x06);

		const uint8_t MCP_8MHz_80kBPS_CFG1 = (0x01);
		const uint8_t MCP_8MHz_80kBPS_CFG2 = (0xbf);
		const uint8_t MCP_8MHz_80kBPS_CFG3 = (0x07);

		const uint8_t MCP_8MHz_50kBPS_CFG1 = (0x03);
		const uint8_t MCP_8MHz_50kBPS_CFG2 = (0xb4);
		const uint8_t MCP_8MHz_50kBPS_CFG3 = (0x06);

		const uint8_t MCP_8MHz_40kBPS_CFG1 = (0x03);
		const uint8_t MCP_8MHz_40kBPS_CFG2 = (0xbf);
		const uint8_t MCP_8MHz_40kBPS_CFG3 = (0x07);

		const uint8_t MCP_8MHz_31k25BPS_CFG1 = (0x07);
		const uint8_t MCP_8MHz_31k25BPS_CFG2 = (0xa4);
		const uint8_t MCP_8MHz_31k25BPS_CFG3 = (0x04);

		const uint8_t MCP_8MHz_20kBPS_CFG1 = (0x07);
		const uint8_t MCP_8MHz_20kBPS_CFG2 = (0xbf);
		const uint8_t MCP_8MHz_20kBPS_CFG3 = (0x07);

		const uint8_t MCP_8MHz_10kBPS_CFG1 = (0x0f);
		const uint8_t MCP_8MHz_10kBPS_CFG2 = (0xbf);
		const uint8_t MCP_8MHz_10kBPS_CFG3 = (0x07);

		const uint8_t MCP_8MHz_5kBPS_CFG1 = (0x1f);
		const uint8_t MCP_8MHz_5kBPS_CFG2 = (0xbf);
		const uint8_t MCP_8MHz_5kBPS_CFG3 = (0x07);

		const uint8_t MCP_RXB_RX_ANY      = 0x60;
		const uint8_t MCP_RXB_RX_EXT      = 0x40;
		const uint8_t MCP_RXB_RX_STD      = 0x20;
		const uint8_t MCP_RXB_RX_STDEXT   = 0x00;
		const uint8_t MCP_RXB_RX_MASK     = 0x60;
		const uint8_t MCP_RXB_BUKT_MASK   = (1<<2);


		const uint8_t MCP_STAT_TX_PENDING_MASK	= (0x54);
		const uint8_t MCP_STAT_TX0_PENDING		= (0x04);
		const uint8_t MCP_STAT_TX1_PENDING		= (0x10);
		const uint8_t MCP_STAT_TX2_PENDING		= (0x40);
		const uint8_t MCP_STAT_TXIF_MASK		= (0xA8);
		const uint8_t MCP_STAT_TX0IF			= (0x08);
		const uint8_t MCP_STAT_TX1IF			= (0x20);
		const uint8_t MCP_STAT_TX2IF			= (0x80);
		const uint8_t MCP_STAT_RXIF_MASK		= (0x03);
		const uint8_t MCP_STAT_RX0IF			= (1<<0);
		const uint8_t MCP_STAT_RX1IF			= (1<<1);


		const uint8_t MCP_TXB_EXIDE_M     = 0x08;                                        // In TXBnSIDL
		const uint8_t MCP_DLC_MASK        = 0x0F;                                        // 4 LSBits
		const uint8_t MCP_RTR_MASK        = 0x40;                                        // (1<<6) Bit 6
	}
//---------------------------------------------------------------------------
	template <
		typename T_ChipSelectPin,
		typename T_SPISpeed
	> class TArduinoCanBusShield_Implementation :
		public T_ChipSelectPin,
		public T_SPISpeed
	{
	public:
		_V_PROP_( ChipSelectPin )
		_V_PROP_( SPISpeed )

	public:
		inline void Init()
		{
			Digital.Write( ChipSelectPin(), true );
		}

		inline void BeginTransaction() 
		{ 
			SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) ); 
			Digital.Write( ChipSelectPin(), false );
		}

		inline void EndTransaction() 
		{ 
			Digital.Write( ChipSelectPin(), true );
			SPI.endTransaction(); 
		}

		inline uint8_t Transfer( uint8_t AValue ) { return SPI.transfer( AValue ); }

	};
//---------------------------------------------------------------------------
	template <
		typename T_FailedOutputPin,
		typename T_Priority,
		typename T_SendTimeout,
		typename T_SentOutputPin
	> class TArduinoMicrochipCanBusMCP2515_Sending :
		public T_FailedOutputPin,
		public T_Priority,
		public T_SendTimeout,
		public T_SentOutputPin
	{
	public:
		_V_PIN_( FailedOutputPin )
		_V_PIN_( SentOutputPin )

	public:
		_V_PROP_( Priority )
		_V_PROP_( SendTimeout )

	};
//---------------------------------------------------------------------------
	template <
		typename T_IMPLEMENTATION,
		typename T_BusSpeed,
		typename T_ClockFrequency,
		typename T_Elements_GetMessageTimeout,
		typename T_Elements_MessageSendCompeled,
		typename T_Elements_ProcessMessage,
		typename T_Enabled,
		typename T_ExtendedOutputPin,
		typename T_FBufferIndex,
		typename T_FSendTime,
		typename T_IDOutputPin,
		typename T_ListenOnly,
		typename T_Loopback,
		typename T_MessageOutputPin,
		typename T_OneShot,
		typename T_OutputPin,
		typename T_RequestOutputPin,
		typename T_Sending,
		typename T_Sleep
	> class Microchip_MCP2515_CanBus :
		public T_IMPLEMENTATION,
		public T_BusSpeed,
		public T_ClockFrequency,
		public T_Elements_ProcessMessage,
		public T_Enabled,
		public T_ExtendedOutputPin,
		public T_FBufferIndex,
		public T_FSendTime,
		public T_IDOutputPin,
		public T_ListenOnly,
		public T_Loopback,
		public T_MessageOutputPin,
		public T_OneShot,
		public T_OutputPin,
		public T_RequestOutputPin,
		public T_Sending,
		public T_Sleep
	{
	public:
		_V_PIN_( ExtendedOutputPin )
		_V_PIN_( IDOutputPin )
		_V_PIN_( MessageOutputPin )
		_V_PIN_( RequestOutputPin )
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( BusSpeed )
		_V_PROP_( ClockFrequency )
		_V_PROP_( Enabled )
		_V_PROP_( ListenOnly )
		_V_PROP_( Loopback )
		_V_PROP_( OneShot )
		_V_PROP_( Sending )
		_V_PROP_( Sleep )

	protected:
//		uint8_t	FMode;

		_V_PROP_( FBufferIndex )
		_V_PROP_( FSendTime )

	protected:
		void StartWriteRegisters( const uint8_t AAddress )
		{
//			T_IMPLEMENTATION::ChipSelectLow();

			T_IMPLEMENTATION::BeginTransaction();

			T_IMPLEMENTATION::Transfer( MCP2515Const::MCP_WRITE );
			T_IMPLEMENTATION::Transfer( AAddress );
		}

		void EndTransaction()
		{
			T_IMPLEMENTATION::EndTransaction();
//			T_IMPLEMENTATION::ChipSelectHigh();
		}

		uint8_t ReadRegister8( const uint8_t AAddress )
		{
//			T_IMPLEMENTATION::ChipSelectLow();

			T_IMPLEMENTATION::BeginTransaction();

			T_IMPLEMENTATION::Transfer( MCP2515Const::MCP_READ );
			T_IMPLEMENTATION::Transfer( AAddress );

			uint8_t AResult = T_IMPLEMENTATION::Transfer( 0 );

//			T_IMPLEMENTATION::ChipSelectHigh();

			T_IMPLEMENTATION::EndTransaction();

			return AResult;
		}

		void WriteRegister8( const uint8_t AAddress, const uint8_t AValue )
		{
			T_IMPLEMENTATION::BeginTransaction();

//			T_IMPLEMENTATION::ChipSelectLow();

			T_IMPLEMENTATION::Transfer( MCP2515Const::MCP_WRITE );
			T_IMPLEMENTATION::Transfer( AAddress );
			T_IMPLEMENTATION::Transfer( AValue );

//			T_IMPLEMENTATION::ChipSelectHigh();

			T_IMPLEMENTATION::EndTransaction();
		}

		void ModifyRegister8( const uint8_t address, const uint8_t mask, const uint8_t data )
		{
			T_IMPLEMENTATION::BeginTransaction();

//			T_IMPLEMENTATION::ChipSelectLow();

			T_IMPLEMENTATION::Transfer( MCP2515Const::MCP_BITMOD );
			T_IMPLEMENTATION::Transfer( address );
			T_IMPLEMENTATION::Transfer( mask );
			T_IMPLEMENTATION::Transfer( data );

//			T_IMPLEMENTATION::ChipSelectHigh();

			T_IMPLEMENTATION::EndTransaction();
		}

		uint8_t ReadStatus(void)
		{
			T_IMPLEMENTATION::BeginTransaction();

			T_IMPLEMENTATION::Transfer( MCP2515Const::MCP_READ_STATUS );
			uint8_t AResult = T_IMPLEMENTATION::Transfer( 0 );

			T_IMPLEMENTATION::EndTransaction();

			return AResult;
		}

		void ZeroBuffer( const uint8_t AAddress )
		{
			StartWriteRegisters( AAddress );

			for( uint8_t i = 0; i < 14; ++i )
				T_IMPLEMENTATION::Transfer( 0 );

			EndTransaction();
		}

	protected:
		uint8_t GetMode()
		{
			return ReadRegister8( MCP2515Const::MCP_CANSTAT ) & MCP2515Const::MODE_MASK;
		}

		uint8_t SetMode( const uint8_t opMode )
		{
//			if( opMode != MCP2515Const::MODE_SLEEP ) // if going to sleep, the value stored in opMode is not changed so that we can return to it later
//				FMode = opMode;

			return SetCANCTRL_Mode( opMode );
		}

		uint8_t RequestNewMode( const uint8_t newmode )
		{
			unsigned long startTime = millis();

			// Spam new mode request and wait for the operation  to complete
			while(1)
			{
				// Request new mode
				// This is inside the loop as sometimes requesting the new mode once doesn't work (usually when attempting to sleep)
				ModifyRegister8( MCP2515Const::MCP_CANCTRL, MCP2515Const::MODE_MASK, newmode );

				uint8_t statReg = ReadRegister8( MCP2515Const::MCP_CANSTAT );
				if(( statReg & MCP2515Const::MODE_MASK ) == newmode ) // We're now in the new mode
					return MCP2515Const::MCP2515_OK;

				else if((millis() - startTime) > 200 ) // Wait no more than 200ms for the operation to complete
					return MCP2515Const::MCP2515_FAIL;
			}
		}

		uint8_t SetCANCTRL_Mode( const uint8_t newmode )
		{
			// If the chip is asleep and we want to change mode then a manual wake needs to be done
			// This is done by setting the wake up interrupt flag
			// This undocumented trick was found at https://github.com/mkleemann/can/blob/master/can_sleep_mcp2515.c
			if(( GetMode()) == MCP2515Const::MODE_SLEEP && newmode != MCP2515Const::MODE_SLEEP )
			{
				// Make sure wake interrupt is enabled
				uint8_t wakeIntEnabled = ( ReadRegister8( MCP2515Const::MCP_CANINTE ) & MCP2515Const::MCP_WAKIF );
				if( ! wakeIntEnabled )
					ModifyRegister8( MCP2515Const::MCP_CANINTE, MCP2515Const::MCP_WAKIF, MCP2515Const::MCP_WAKIF );

				// Set wake flag (this does the actual waking up)
				ModifyRegister8( MCP2515Const::MCP_CANINTF, MCP2515Const::MCP_WAKIF, MCP2515Const::MCP_WAKIF );

				// Wait for the chip to exit SLEEP and enter LISTENONLY mode.

				// If the chip is not connected to a CAN bus (or the bus has no other powered nodes) it will sometimes trigger the wake interrupt as soon
				// as it's put to sleep, but it will stay in SLEEP mode instead of automatically switching to LISTENONLY mode.
				// In this situation the mode needs to be manually set to LISTENONLY.

				if( RequestNewMode( MCP2515Const::MODE_LISTENONLY ) != MCP2515Const::MCP2515_OK )
					return MCP2515Const::MCP2515_FAIL;

				// Turn wake interrupt back off if it was originally off
				if( ! wakeIntEnabled )
					ModifyRegister8( MCP2515Const::MCP_CANINTE, MCP2515Const::MCP_WAKIF, 0 );
			}

			// Clear wake flag
			ModifyRegister8( MCP2515Const::MCP_CANINTF, MCP2515Const::MCP_WAKIF, 0 );

			return RequestNewMode(newmode);
		}

		void ReadCanMessage( const uint8_t buffer_load_addr, uint8_t &ALength, uint8_t *buf )
		{
			uint8_t tbufdata[4];

//			T_IMPLEMENTATION::ChipSelectLow();

			T_IMPLEMENTATION::BeginTransaction();

			T_IMPLEMENTATION::Transfer( buffer_load_addr );
			// mcp2515 has auto-increment of address-pointer
			for( uint8_t i = 0; i < 4; ++ i )
				tbufdata[ i ] = T_IMPLEMENTATION::Transfer( 0 );

			uint32_t &id = *(uint32_t *)buf;
			uint8_t &AFlags = *(uint8_t *)( buf + sizeof( uint32_t ) );

			buf += sizeof( uint32_t ) + 1;

			id = (tbufdata[ MCP2515Const::MCP_SIDH ] << 3) + (tbufdata[ MCP2515Const::MCP_SIDL ] >> 5);

			AFlags = 0;
			if ( (tbufdata[ MCP2515Const::MCP_SIDL ] & MCP2515Const::MCP_TXB_EXIDE_M ) ==  MCP2515Const::MCP_TXB_EXIDE_M )
			{
				// extended id
				id = (id << 2) + (tbufdata[ MCP2515Const::MCP_SIDL ] & 0x03);
				id = (id << 8) + tbufdata[ MCP2515Const::MCP_EID8 ];
				id = (id << 8) + tbufdata[ MCP2515Const::MCP_EID0 ];
				AFlags = 0b01; // Extended
			}

			uint8_t pMsgSize = T_IMPLEMENTATION::Transfer( 0 );
			ALength = pMsgSize & MCP2515Const::MCP_DLC_MASK;
			if( pMsgSize & MCP2515Const::MCP_RTR_MASK )
				AFlags |= 0b10; // Is Request
//				id |= ( 1 << 31 );

//			*rtrBit = (pMsgSize & MCP_RTR_MASK) ? ( 1 << 31 ) : 0;

			for( uint8_t i = 0; i < ALength && i < MCP2515Const::CAN_MAX_CHAR_IN_MESSAGE; ++ i )
				buf[i] = T_IMPLEMENTATION::Transfer( 0 );

			T_IMPLEMENTATION::EndTransaction();
//			T_IMPLEMENTATION::ChipSelectHigh();
		}

		void IdToBuf( const bool AExtended, const uint32_t id, uint8_t *tbufdata )
		{
			uint16_t ACanId = uint16_t( id & 0x0FFFF );

			if( AExtended )
			{
				tbufdata[ MCP2515Const::MCP_EID0 ] = uint8_t( ACanId & 0xFF );
				tbufdata[ MCP2515Const::MCP_EID8 ] = uint8_t( ACanId >> 8 );
				ACanId = uint16_t( id >> 16 );
				tbufdata[ MCP2515Const::MCP_SIDL ] = uint8_t( ACanId & 0x03 );
				tbufdata[ MCP2515Const::MCP_SIDL ] += uint8_t( ( ACanId & 0x1C ) << 3 );
				tbufdata[ MCP2515Const::MCP_SIDL ] |= MCP2515Const::MCP_TXB_EXIDE_M;
				tbufdata[ MCP2515Const::MCP_SIDH ] = uint8_t( ACanId >> 5 );
			}

			else
			{
				tbufdata[ MCP2515Const::MCP_SIDH ] = uint8_t( ACanId >> 3 );
				tbufdata[ MCP2515Const::MCP_SIDL ] = uint8_t( ( ACanId & 0x07 ) << 5 );
				tbufdata[ MCP2515Const::MCP_EID0 ] = 0;
				tbufdata[ MCP2515Const::MCP_EID8 ] = 0;
			}
		}

		void StartTransmit( const uint8_t ABufferIndex )
		{
//			T_IMPLEMENTATION::ChipSelectLow();

			T_IMPLEMENTATION::BeginTransaction();

			// MCP_RTS_TX0, MCP_RTS_TX1, MCP_RTS_TX2

			T_IMPLEMENTATION::Transfer( MCP2515Const::MCP_RTS_TX_BASE + ( 1 << ABufferIndex ));

			T_IMPLEMENTATION::EndTransaction();
//			T_IMPLEMENTATION::ChipSelectHigh();
		}

		void WriteCanMessage( const uint8_t ABufferIndex, uint32_t id, bool AExtended, bool ARequest, uint8_t APriority, uint8_t len, volatile const uint8_t *buf )
		{
			StartWriteRegisters( MCP2515Const::MCP_TXB0CTRL + ABufferIndex * 0x10 );
			T_IMPLEMENTATION::Transfer( APriority );
			T_IMPLEMENTATION::EndTransaction();

//			uint8_t load_addr=txSidhToTxLoad(buffer_sidh_addr);
			uint8_t load_addr = MCP2515Const::MCP_LOAD_TX0 + ABufferIndex * 2;

			uint8_t tbufdata[4];
			uint8_t dlc = len | ( ARequest ? MCP2515Const::MCP_RTR_MASK : 0 ) ;

			IdToBuf( AExtended, id, tbufdata );

			T_IMPLEMENTATION::BeginTransaction();

			T_IMPLEMENTATION::Transfer( load_addr );
			for( uint8_t i = 0; i < 4; ++i )
				T_IMPLEMENTATION::Transfer( tbufdata[ i ] );

			T_IMPLEMENTATION::Transfer( dlc );

			for( uint8_t i = 0; i < len && i < MCP2515Const::CAN_MAX_CHAR_IN_MESSAGE; ++i )
				T_IMPLEMENTATION::Transfer( buf[ i ] );

			T_IMPLEMENTATION::EndTransaction();

			StartTransmit( ABufferIndex );
		}

		void AbortBuffer( uint8_t AAddress )
		{
			StartWriteRegisters( AAddress );
			T_IMPLEMENTATION::Transfer( 0 );
			EndTransaction();
		}

		uint8_t GetNextFreeTXBuf( uint8_t &AOutBufferIndex )                 // get Next free txbuf
		{
			uint8_t status = ReadStatus() & MCP2515Const::MCP_STAT_TX_PENDING_MASK;

			AOutBufferIndex = 0x00;

			if ( status == MCP2515Const::MCP_STAT_TX_PENDING_MASK )
				return false; // All buffers are pending

/*
			if( AOutBufferIndex < 3 ) // Wait for specific buffer index to finish!
				if ( (status & ( MCP2515Const::MCP_STAT_TX0_PENDING << ( AOutBufferIndex * 2 ) ) ) != 0 ) 
				{
					return false;
				}
*/
			uint8_t AMinBuffer;
			uint8_t AMaxBuffer;

			if( AOutBufferIndex < 3 )
				AMinBuffer = AMaxBuffer = AOutBufferIndex;

			else
			{
				AMinBuffer = 0;
				AMaxBuffer = MCP2515Const::MCP_N_TXBUFFERS - 1;
			}

			// check all 3 TX-Buffers except reserved
//			for( uint8_t i = 0; i < MCP2515Const::MCP_N_TXBUFFERS - nReservedTx; i++)
			for( uint8_t i = AMinBuffer; i <= AMaxBuffer; ++ i )
			{
				if ( (status & ( MCP2515Const::MCP_STAT_TX0_PENDING << ( i * 2 ) ) ) == 0 )
				{
//					Serial.println( "COMPLETED" );

					ModifyRegister8( MCP2515Const::MCP_CANINTF, ( MCP2515Const::MCP_TX0IF << i ), 0 );
					if( FBufferIndex().GetValue() == i )
					{
						FBufferIndex() = 3;
						Sending().SentOutputPin().ClockPin();
					}

					T_Elements_MessageSendCompeled::Call( i );
					AOutBufferIndex = i;
					return true;
				}

				if( FBufferIndex().GetValue() == i )
				{
					if( millis() - FSendTime().GetValue() > Sending().SendTimeout().GetValue() )
					{
						Sending().FailedOutputPin().ClockPin();
						AOutBufferIndex = i;
						FBufferIndex() = 3;
						AbortBuffer( MCP2515Const::MCP_TXB0CTRL + i * 0x10 );
						return true;
					}
				}

				bool ATimeput = false;
				T_Elements_GetMessageTimeout::Call( i, ATimeput );

				if( ATimeput )
				{
					AOutBufferIndex = i;
					AbortBuffer( MCP2515Const::MCP_TXB0CTRL + i * 0x10 );
					return true;
				}
			}

			return false;
		}

		inline void TryGetNextFreeTXBuf( uint8_t &ABufferIndex )
		{
			while( ! GetNextFreeTXBuf( ABufferIndex ))
				;

		}

		void Reset()
		{
//			T_IMPLEMENTATION::ChipSelectLow();

			T_IMPLEMENTATION::BeginTransaction();

			T_IMPLEMENTATION::Transfer( MCP2515Const::MCP_RESET );

			T_IMPLEMENTATION::EndTransaction();

//			T_IMPLEMENTATION::ChipSelectHigh();

			delay( 10 );
		}

		void ConfigRate()
		{
			float ABusSpeed = BusSpeed().GetValue();
			if( ClockFrequency().GetValue() < 16 )
			{
				if( ABusSpeed < 10 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_5kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_5kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_5kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 20 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_10kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_10kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_10kBPS_CFG3 );
					return;
				}

/*
				if( ABusSpeed < 25 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_20kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_20kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_20kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 31 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_25kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_25kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_25kBPS_CFG3 );

					return;
				}

				if( ABusSpeed < 33 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_31kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_31kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_31kBPS_CFG3 );
					return;
				}
*/
				if( ABusSpeed < 40 )
				{
//					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_33kBPS_CFG1 );
//					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_33kBPS_CFG2 );
//					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_33kBPS_CFG3 );

					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_20kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_20kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_20kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 50 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_40kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_40kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_40kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 80 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_50kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_50kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_50kBPS_CFG3 );
					return;
				}

/*
				if( ABusSpeed < 83 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_80kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_80kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_80kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 95 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_83kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_83kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_83kBPS_CFG3 );
					return;
				}
*/
				if( ABusSpeed < 100 )
				{
//					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_95kBPS_CFG1 );
//					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_95kBPS_CFG2 );
//					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_95kBPS_CFG3 );

					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_80kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_80kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_80kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 125 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_100kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_100kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_100kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 200 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_125kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_125kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_125kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 250 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_200kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_200kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_200kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 500 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_250kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_250kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_250kBPS_CFG3 );
					return;
				}

/*
				if( ABusSpeed < 666 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_500kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_500kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_500kBPS_CFG3 );
					return;
				}
*/
				if( ABusSpeed < 1000 )
				{
//					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_666kBPS_CFG1 );
//					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_666kBPS_CFG2 );
//					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_666kBPS_CFG3 );

					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_500kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_500kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_500kBPS_CFG3 );
					return;
				}

				WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_8MHz_1000kBPS_CFG1 );
				WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_8MHz_1000kBPS_CFG2 );
				WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_8MHz_1000kBPS_CFG3 );
			}

			else
			{
				if( ABusSpeed < 10 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_16MHz_5kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_16MHz_5kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_16MHz_5kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 20 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_16MHz_10kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_16MHz_10kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_16MHz_10kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 25 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_16MHz_20kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_16MHz_20kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_16MHz_20kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 31 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_16MHz_25kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_16MHz_25kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_16MHz_25kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 33 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_16MHz_31kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_16MHz_31kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_16MHz_31kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 40 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_16MHz_33kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_16MHz_33kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_16MHz_33kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 50 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_16MHz_40kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_16MHz_40kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_16MHz_40kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 80 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_16MHz_50kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_16MHz_50kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_16MHz_50kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 83 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_16MHz_80kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_16MHz_80kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_16MHz_80kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 95 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_16MHz_83kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_16MHz_83kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_16MHz_83kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 100 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_16MHz_95kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_16MHz_95kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_16MHz_95kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 125 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_16MHz_100kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_16MHz_100kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_16MHz_100kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 200 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_16MHz_125kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_16MHz_125kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_16MHz_125kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 250 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_16MHz_200kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_16MHz_200kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_16MHz_200kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 500 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_16MHz_250kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_16MHz_250kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_16MHz_250kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 666 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_16MHz_500kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_16MHz_500kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_16MHz_500kBPS_CFG3 );
					return;
				}

				if( ABusSpeed < 1000 )
				{
					WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_16MHz_666kBPS_CFG1 );
					WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_16MHz_666kBPS_CFG2 );
					WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_16MHz_666kBPS_CFG3 );
					return;
				}

				WriteRegister8( MCP2515Const::MCP_CNF1, MCP2515Const::MCP_16MHz_1000kBPS_CFG1 );
				WriteRegister8( MCP2515Const::MCP_CNF2, MCP2515Const::MCP_16MHz_1000kBPS_CFG2 );
				WriteRegister8( MCP2515Const::MCP_CNF3, MCP2515Const::MCP_16MHz_1000kBPS_CFG3 );
			}
		}

		void InitCanBuffers()
		{
			ZeroBuffer( MCP2515Const::MCP_TXB0CTRL );
			ZeroBuffer( MCP2515Const::MCP_TXB1CTRL );
			ZeroBuffer( MCP2515Const::MCP_TXB2CTRL );

			WriteRegister8( MCP2515Const::MCP_RXB0CTRL, 0 );
			WriteRegister8( MCP2515Const::MCP_RXB1CTRL, 0 );
		}

	public:
		void UpdateMode()
		{
			if( Sleep() )
				SetMode( MCP2515Const::MODE_SLEEP );

			else if( Loopback() )
				SetMode( MCP2515Const::MODE_LOOPBACK );

			else if( ListenOnly() )
				SetMode( MCP2515Const::MODE_LISTENONLY );

			else
				SetMode( MCP2515Const::MODE_NORMAL );

			if( OneShot() )
				ModifyRegister8( MCP2515Const::MCP_CANCTRL, MCP2515Const::MODE_ONESHOT, MCP2515Const::MODE_ONESHOT );

			else
				ModifyRegister8( MCP2515Const::MCP_CANCTRL, MCP2515Const::MODE_ONESHOT, 0 );

			delay( 10 );
		}

	public:
		bool SendMessage( uint32_t id, bool AExtended, bool ARequest, uint8_t APriority, uint8_t len, const uint8_t *buf, uint8_t &ABufferIndex ) //, bool wait_sent )
		{
			if( ! Enabled().GetValue() )
				return true;

			if( id > 0b11111111111 )
				AExtended = true;

//			uint8_t ABufferIndex;
//			uint16_t uiTimeOut = 0;

//			can_id = id;
//			ext_flg = AExtended;
//			rtr=rtrBit;

			TryGetNextFreeTXBuf( ABufferIndex );

			WriteCanMessage( ABufferIndex, id, AExtended, ARequest, APriority, len, buf );

/*
			if( wait_sent )
			{
				uiTimeOut = 0;
				do
				{
					if( uiTimeOut > 0 )
						delayMicroseconds(10);

					uiTimeOut++;
					uint8_t res1 = mcp2515_readRegister( ABufferIndex );  // read send buff ctrl reg
					res1 = res1 & 0x08;
				}
				while( res1 && ( uiTimeOut < SendTimeout().GetValue() ) );

				if( uiTimeOut == SendTimeout().GetValue() )                                       // send msg timeout
					return false;

			}
*/
			return true;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::TCanBusPacket &ADataBlock = *(Mitov::TCanBusPacket*)_Data;

//			Serial.println( ADataBlock.GetID() );

			uint8_t ABufferIndex = FBufferIndex();
			SendMessage( ADataBlock.GetID(), ADataBlock.IsExtended(), ADataBlock.IsRequest(), Sending().Priority().GetValue(), ADataBlock.GetSize(), ADataBlock.Read(), ABufferIndex );

			FBufferIndex() = ABufferIndex;
			FSendTime() = millis();
//			SendMessage( )
		}

		inline void ResetInputPin_o_Receive( void *_Data )
		{
			Reset();
		}

	public:
		inline void SystemInit()
		{
			T_IMPLEMENTATION::Init();
		}

		inline void SystemStart()
		{
			Reset();

			SetCANCTRL_Mode( MCP2515Const::MODE_CONFIG );

			// set boadrate
//			mcp2515_configRate( canSpeed, clock );
			ConfigRate();

			delay( 10 );

			InitCanBuffers();

			// interrupt mode
			WriteRegister8( MCP2515Const::MCP_CANINTE, MCP2515Const::MCP_RX0IF | MCP2515Const::MCP_RX1IF );

			// TEMPORARY! Disable the filters
			ModifyRegister8( MCP2515Const::MCP_RXB0CTRL,
									MCP2515Const::MCP_RXB_RX_MASK | MCP2515Const::MCP_RXB_BUKT_MASK,
									MCP2515Const::MCP_RXB_RX_ANY | MCP2515Const::MCP_RXB_BUKT_MASK );

			ModifyRegister8( MCP2515Const::MCP_RXB1CTRL, MCP2515Const::MCP_RXB_RX_MASK,
									MCP2515Const::MCP_RXB_RX_ANY );

			UpdateMode();
		}

		inline void SystemLoopBegin()
		{
			if( ! Enabled().GetValue() )
				return;

			uint8_t AStatus = ReadStatus();
			if( AStatus & MCP2515Const::MCP_STAT_RXIF_MASK )
			{
				uint8_t ALength = 0;
				uint8_t ABuffer[ MCP2515Const::CAN_MAX_CHAR_IN_MESSAGE + sizeof( uint32_t ) + 1 ];
				ReadCanMessage( ( AStatus & MCP2515Const::MCP_RX0IF ) ? MCP2515Const::MCP_READ_RX0 : MCP2515Const::MCP_READ_RX1, ALength, ABuffer );

				T_Elements_ProcessMessage::Call( *(uint32_t *)ABuffer, ABuffer[ 4 ] & 0b01, ABuffer[ 4 ] & 0b10, ALength, ABuffer + 5 );

				T_IDOutputPin::SetPinValue( *(uint32_t *)ABuffer );

				T_ExtendedOutputPin::SetPinValue( ( ABuffer[ 4 ] & 0b01 ) != 0 );
				T_RequestOutputPin::SetPinValue( ( ABuffer[ 4 ] & 0b10 ) != 0 );

				if( T_MessageOutputPin::GetPinIsConnected() )
				{
					TDataBlock AData( ALength, ABuffer + 5 );
					T_MessageOutputPin::SetPinValue( AData );
				}

				if( T_OutputPin::GetPinIsConnected() )
				{
                    Mitov::TCanBusPacket AData( *(uint32_t *)ABuffer, ( ABuffer[ 4 ] & 0b10 ) != 0, ( ABuffer[ 4 ] & 0b01 ) != 0, ALength, ABuffer + 5 );
//					TDataBlock AData( ALength + sizeof( uint32_t ) + 1, ABuffer );
					T_OutputPin::SetPinValue( AData );
				}


/*
				if ( AStatus & MCP_RX0IF )                                        // Msg in Buffer 0
				{
					ReadCanMessage( MCP_READ_RX0, len, buf);
				}

				else // if ( AStatus & MCP_RX1IF )                                   // Msg in Buffer 1
				{
					ReadCanMessage( MCP_READ_RX1, len, buf);
				}
*/
			}

			uint8_t ATmpBufferIndex;
			GetNextFreeTXBuf( ATmpBufferIndex ); // Handles Success/Failure pins!
		}

	public:
		inline Microchip_MCP2515_CanBus()
		{
			FBufferIndex() = 3; // Invalid buffer (2 max)
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
