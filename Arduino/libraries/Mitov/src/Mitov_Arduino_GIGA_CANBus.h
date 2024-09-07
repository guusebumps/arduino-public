////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2024 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov.h>
#include <Mitov_CanBus.h>
#include <hal/can_api.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_FailedOutputPin,
		typename T_FSendTime,
		typename T_FSendingID,
		typename T_ID,
		typename T_SendTimeout,
		typename T_SentOutputPin
	> class TArduino_Giga_CanBusSendingImplementation :
		public T_FailedOutputPin,
		public T_FSendTime,
		public T_FSendingID,
		public T_ID,
		public T_SendTimeout,
		public T_SentOutputPin
	{
	public:
		_V_PIN_( FailedOutputPin )
		_V_PIN_( SentOutputPin )

	public:
		_V_PROP_( SendTimeout )
		_V_PROP_( ID )

	protected:
		_V_PROP_( FSendTime )
		_V_PROP_( FSendingID )

/*
	protected:
		volatile bool FSending = false; // Do not template to allow multithreading!
		volatile bool FSendFailed = false; // Do not template to allow multithreading!
		volatile bool FMessageSent = false; // Do not template to allow multithreading!
*/
	protected:
/*
		void ProcessStatus()
		{
			if( FMessageSent )
			{
//				Serial.println( "FSent" );
				T_SentOutputPin::ClockPin();
//				FSending = false;
//				FMessageSent = false;
			}

			if( FSendFailed )
			{
//				Serial.println( "FSendFailed" );
//				Serial.println( FSendFailed );
				T_FailedOutputPin::ClockPin();
//				FSending = false;
//				FSendFailed = false;
			}
		}
*/
		inline void WaitForSending()
		{
/*
			while( FSending )
			{
				delay( 1 );
				if( ! FMessageSent )
					if( millis() - FSendTime().GetValue() > SendTimeout().GetValue() )
					{
						T_FailedOutputPin::ClockPin();
						FSending = false;
						break;
					}

				ProcessStatus();
			}
*/
		}

	public:
		inline void SystemLoopBegin()
		{
/*
			if( ! FSending )
				return;

			ProcessStatus();
*/
		}

	public:
		inline void MessageSendCompeled( uint32_t AID, bool ASuccess )
		{
/*
			if( ! FSending )
				return;
*/
			if( T_FSendingID::GetIsConstant() )
			{
				if( AID != FSendingID().GetValue() )
					return;
			}

			else
			{
				if( AID != ID().GetValue() )
					return;
			}

			if( ASuccess )
			{
//				FMessageSent = true;
				T_SentOutputPin::ClockPin();
			}

			else
			{
//				FSendFailed = true;
				T_FailedOutputPin::ClockPin();
			}
		}

		inline void IsSending( uint32_t AID, bool &AResult )
		{
/*
			WaitForSending();
			if( ! FSending )
				return;
*/
			if( AResult )
				return;

			if( T_FSendingID::GetIsConstant() )
				AResult = ( AID == FSendingID().GetValue() );

			else
				AResult = ( AID == ID().GetValue() );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Extended,
        typename T_FSendTime,
		typename T_FSendingID,
		typename T_FailedOutputPin,
		typename T_ID,
//		uint8_t C_INDEX,
        typename T_InitialValue,
		typename T_Request,
		typename T_SendTimeout,
		typename T_SentOutputPin
	> class TArduinoCanBusSendMessage_Giga_Element :
		public TArduino_Giga_CanBusSendingImplementation <
				T_FailedOutputPin,
                T_FSendTime,
				T_FSendingID,
				T_ID,
				T_SendTimeout,
				T_SentOutputPin
			>,
		public T_ClockInputPin_o_IsConnected,
		public T_Extended,
//		public T_FMessageSent,
//		public T_FSendFailed,
//		public T_FSending,
//		public T_ID,
        public T_InitialValue,
		public T_Request
	{
        typedef TArduino_Giga_CanBusSendingImplementation <
				T_FailedOutputPin,
                T_FSendTime,
				T_FSendingID,
				T_ID,
				T_SendTimeout,
				T_SentOutputPin
			> inherited;
            
	public:
		_V_PROP_( Extended )
//		_V_PROP_( ID )
		_V_PROP_( InitialValue )
		_V_PROP_( Request )

    public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            inherited::WaitForSending();

            const uint8_t *ABuffer = T_InitialValue::Allocate();
			inherited::FSendingID() = inherited::ID().GetValue();
   			if( C_OWNER.SendMessage( inherited::ID().GetValue(), Extended().GetValue(), Request().GetValue(), T_InitialValue::GetCount(), ABuffer ))
			{
//				inherited::FSending = true;
//				inherited::FSendingID() = inherited::ID().GetValue();
//				inherited::FSendTime() = millis();
			}

            T_InitialValue::Release( ABuffer );
		}

		inline void MessageInputPin_o_Receive( void *_Data )
		{
            InitialValue() = *(Mitov::TDataBlock*)_Data;
			if( ! ClockInputPin_o_IsConnected().GetValue() )
                ClockInputPin_o_Receive( nullptr );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Extended,
		typename T_ExtendedOutputPin,
        typename T_FSendTime,
		typename T_FSendingID,
		typename T_FailedOutputPin,
		typename T_ID,
        typename T_InitialValue,
		typename T_MessageOutputPin,
		typename T_RequestOutputPin,
		typename T_SendTimeout,
		typename T_SentOutputPin
	> class Microchip_Giga_CanBus_RequestMessage : 
		public TArduino_Giga_CanBusSendingImplementation <
				T_FailedOutputPin,
                T_FSendTime,
				T_FSendingID,
				T_ID,
				T_SendTimeout,
				T_SentOutputPin
			>,
		public T_ClockInputPin_o_IsConnected,
		public T_Extended,
		public T_ExtendedOutputPin,
        public T_InitialValue,
		public T_MessageOutputPin,
		public T_RequestOutputPin
	{
        typedef TArduino_Giga_CanBusSendingImplementation <
				T_FailedOutputPin,
                T_FSendTime,
				T_FSendingID,
				T_ID,
				T_SendTimeout,
				T_SentOutputPin
			> inherited;
            
	public:
		_V_PIN_( ExtendedOutputPin )
		_V_PIN_( MessageOutputPin )
		_V_PIN_( RequestOutputPin )

	public:
		_V_PROP_( Extended )
		_V_PROP_( InitialValue )

    public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
            inherited::WaitForSending();

            const uint8_t *ABuffer = T_InitialValue::Allocate();
			inherited::FSendingID() = inherited::ID().GetValue();
   			if( C_OWNER.SendMessage( inherited::ID().GetValue(), Extended().GetValue(), true, T_InitialValue::GetCount(), ABuffer ))
			{
/*
				inherited::FSending = true;
				inherited::FSendingID() = inherited::ID().GetValue();
				inherited::FSendTime() = millis();
*/
			}

            T_InitialValue::Release( ABuffer );
		}

		inline void MessageInputPin_o_Receive( void *_Data )
		{
			InitialValue = *(Mitov::TDataBlock*)_Data;
			if( ! ClockInputPin_o_IsConnected().GetValue() )
                ClockInputPin_o_Receive( nullptr );

		}

	public:
		inline void ProcessMessage( uint32_t AID, bool AExtended, bool ARequest, uint8_t  ALength, const uint8_t * ABuffer )
		{
			if( AID != inherited::ID().GetValue() )
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

	};
//---------------------------------------------------------------------------
    namespace TArduino_Giga_CanBus_TestMode
    {
        enum TArduino_Giga_CanBus_TestMode
        {
			None = MODE_NORMAL,
			Listen = MODE_SILENT,
			ExternalLoopback = MODE_TEST_LOCAL,
			InternalLoopback = MODE_TEST_SILENT
        };
    }
//---------------------------------------------------------------------------
	template <
		typename T_BusSpeed,
		PinName C_PIN_RX,
		PinName C_PIN_TX,
		typename T_Elements_MessageSendCompeled,
		typename T_Elements_ProcessMessage,
		typename T_Enabled,
		typename T_ExtendedOutputPin,
		typename T_IDOutputPin,
		typename T_MessageOutputPin,
		typename T_OutputPin,
		typename T_RequestOutputPin,
		typename T_TestMode
	> class TArduino_GIGA_CANBus_Module :
		public T_BusSpeed,
		public T_Enabled,
		public T_ExtendedOutputPin,
		public T_IDOutputPin,
		public T_MessageOutputPin,
		public T_OutputPin,
		public T_RequestOutputPin,
		public T_TestMode
	{
	public:
		_V_PIN_( ExtendedOutputPin )
		_V_PIN_( IDOutputPin )
		_V_PIN_( MessageOutputPin )
		_V_PIN_( OutputPin )
		_V_PIN_( RequestOutputPin )

	public:
		_V_PROP_( BusSpeed )
		_V_PROP_( Enabled )
		_V_PROP_( TestMode )

	protected:
		can_t	FCanBus;

//		Thread *blinky_thread; // = new Thread(osPriorityNormal, BLINKY_THREAD_STACK, nullptr, "blinky_thread");

	public:
		inline void UpdateEnabled()
		{
			if( Enabled() )
				can_init_freq( &FCanBus, C_PIN_RX, C_PIN_TX, BusSpeed().GetValue() );

			else
				can_free( &FCanBus );

		}

		inline void UpdateTestMode()
		{
			can_mode( &FCanBus, CanMode( TestMode().GetValue() ));
		}

	public:
		bool SendMessage( uint32_t id, bool AExtended, bool ARequest, uint8_t len, const uint8_t *buf ) //, bool wait_sent )
		{
			if( ! Enabled().GetValue() )
				return false;

			CAN_Message AMessage;
			AMessage.id = id;
			AMessage.format = ( AExtended ) ? CANExtended : CANStandard;
			AMessage.type = ( ARequest ) ? CANRemote : CANData;
			AMessage.len = min( len, 8 );
			memcpy( AMessage.data, buf, AMessage.len );

//			Serial.println( "TRY" );
			bool AResult = can_write( &FCanBus, AMessage, 0 );
			T_Elements_MessageSendCompeled::Call( 0, // Temporary frame ID 
				AResult );

		}

	public:
		inline void SystemInit()
		{
			can_init_freq( &FCanBus, C_PIN_RX, C_PIN_TX, BusSpeed().GetValue() );
			UpdateTestMode();
		}

		inline void SystemLoopBegin()
		{
			if( ! Enabled().GetValue() )
				return;

			CAN_Message AMessage;

			if( can_read( &FCanBus, &AMessage, 0 ))
			{
				T_Elements_ProcessMessage::Call( AMessage.id, AMessage.format == CANExtended, AMessage.type == CANRemote, AMessage.len, AMessage.data );

//				Serial.println( "TRY" ); 
				T_IDOutputPin::SetPinValue( AMessage.id );

				T_ExtendedOutputPin::SetPinValue( AMessage.format == CANExtended );

				T_RequestOutputPin::SetPinValue( AMessage.type == CANRemote );

				if( T_MessageOutputPin::GetPinIsConnected() )
				{
					TDataBlock AData( AMessage.len, AMessage.data );
					T_MessageOutputPin::SetPinValue( AData );
				}

				if( T_OutputPin::GetPinIsConnected() )
				{
					Mitov::TCanBusPacket AData( AMessage.id, AMessage.format == CANExtended, AMessage.type == CANRemote, AMessage.len, AMessage.data );
					T_OutputPin::SetPinValue( AData );
				}
			}

		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif