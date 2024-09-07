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

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ExtendedOutputPin,
		typename T_ID,
		typename T_MessageOutputPin,
		typename T_RequestOutputPin
	> class CanBus_ReceiveMessage : 
		public T_ExtendedOutputPin,
		public T_ID,
		public T_MessageOutputPin,
		public T_RequestOutputPin
	{
	public:
		_V_PIN_( ExtendedOutputPin )
		_V_PIN_( MessageOutputPin )
		_V_PIN_( RequestOutputPin )

	public:
		_V_PROP_( ID )

	public:
		Mitov::TValueArray<uint8_t> InitialValue;

	public:
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

	};
//---------------------------------------------------------------------------
	template <
		typename T_FailedOutputPin,
		typename T_SendTimeout,
		typename T_SentOutputPin
	> class TArduinoBasicCanBus_Sending :
		public T_FailedOutputPin,
		public T_SendTimeout,
		public T_SentOutputPin
	{
	public:
		typedef T_FailedOutputPin T_T_FailedOutputPin;
		typedef T_SendTimeout T_T_SendTimeout;
		typedef T_SentOutputPin T_T_SentOutputPin;

	public:
		_V_PIN_( FailedOutputPin )
		_V_PIN_( SentOutputPin )

	public:
		_V_PROP_( SendTimeout )

	};
//---------------------------------------------------------------------------
	template <
		typename T_ExtractFromData,
		typename T_ID,
		typename T_IsExtended,
		typename T_IsRequest
	> class TArduinoBinaryToCanBusInfo :
		public T_ExtractFromData,
		public T_ID,
		public T_IsExtended,
		public T_IsRequest
	{
	public:
		_V_PROP_( ExtractFromData )
		_V_PROP_( ID )
		_V_PROP_( IsExtended )
		_V_PROP_( IsRequest )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Info,
		typename T_OutputPin
	> class BinaryToCanBus :
		public T_Enabled,
		public T_Info,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Info )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

			TDataBlock &ABuffer = *(TDataBlock*)_Data;
			const uint8_t *AData = ABuffer.Read();
			uint32_t ASize = ABuffer.GetSize();
			if( ASize < 2 )
				return;

			if( Info().ExtractFromData() )
			{
				TCanBusPacket APacket( AData[ 0 ], AData[ 1 ] & 1 != 0b10, AData[ 1 ] & 0b01 != 0, MitovMin<uint32_t>( 8, ASize - 2 ), AData + 2 );
				T_OutputPin::SetPinValue( APacket );
				return;
			}

			TCanBusPacket APacket( Info().ID(), Info().IsRequest(), Info().IsExtended(), MitovMin<uint32_t>( 8, ASize ), AData );
			T_OutputPin::SetPinValue( APacket );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_ExtendedOutputPin,
		typename T_IDOutputPin,
		typename T_IncludeInfo,
		typename T_OutputPin,
		typename T_RequestOutputPin
	> class CanBusToBinary :
		public T_Enabled,
		public T_ExtendedOutputPin,
		public T_IDOutputPin,
		public T_IncludeInfo,
		public T_OutputPin,
		public T_RequestOutputPin
	{
	public:
		_V_PIN_( ExtendedOutputPin )
		_V_PIN_( IDOutputPin )
		_V_PIN_( OutputPin )
		_V_PIN_( RequestOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( IncludeInfo )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

			Mitov::TCanBusPacket &ADataBlock = *(Mitov::TCanBusPacket*)_Data;

			T_IDOutputPin::SetPinValue( ADataBlock.GetID() );
			T_ExtendedOutputPin::SetPinValue( ADataBlock.IsExtended() );
			T_RequestOutputPin::SetPinValue( ADataBlock.IsRequest() );

			if( ! IncludeInfo().GetValue() )
			{
				TDataBlock ABuffer( ADataBlock.GetSize(), ADataBlock.Read() );
				T_OutputPin::SetPinValue( ABuffer );
				return;
			}

			uint8_t *AData = new uint8_t[ ADataBlock.GetSize() + 2 ];

			AData[ 0 ] = ADataBlock.GetID();

			AData[ 1 ] = ( ADataBlock.IsRequest() ) ? 0b10 : 0;
			if( ADataBlock.IsExtended() )
				AData[ 1 ] |= 0b01;

			memcpy( AData + 2, ADataBlock.Read(), ADataBlock.GetSize() );

			TDataBlock ABuffer( ADataBlock.GetSize() + 2, AData );
			T_OutputPin::SetPinValue( ABuffer );
			delete AData;
		}

	};	
//---------------------------------------------------------------------------
	template <
		typename T_ID,
		typename T_IsExtended,
		typename T_IsRequest,
		typename T_Message,
        typename T_Message_ApplyValues,
        typename T_Message_GetValue
	> class CanBusValueSourcePersistent :
		public T_ID,
		public T_IsExtended,
		public T_IsRequest,
		public T_Message
	{
	public:
        _V_PROP_( ID )
        _V_PROP_( IsExtended )
        _V_PROP_( IsRequest )
        _V_PROP_( Message )

	public:
		inline void AssignToCanBusValue( TStoredCanBusPacket &APacket )
		{
			APacket.Assign( ID(), IsExtended(), IsRequest(), Message().GetArray(), Message().GetCount() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_Value
	> class ArduinoCanBusSetValueElement :
		public T_Enabled,
		public T_Value
	{
	public:
        _V_PROP_( Enabled )
        _V_PROP_( Value )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TStoredCanBusPacket APacket( Value().ID(), Value().IsRequest(), Value().IsExtended(), MitovMin<uint32_t>( 8, Value().Message().GetCount() ), Value().Message().GetArray() );
			C_OWNER.ExternalSetValue( APacket );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_TryActive,
		typename T_OutputPin,
		typename T_Value
	> class CanBusValueSource :
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

	public:
        _V_PROP_( Value )

    protected:
		void SetValue()
		{
			TCanBusPacket APacket( Value().ID(), Value().IsRequest(), Value().IsExtended(), MitovMin<uint32_t>( 8, Value().Message().GetCount() ), Value().Message().GetValue() );

			T_OutputPin::SetPinValue( APacket );
		}

	public:
		void ExternalSetValue( const TCanBusPacket &ABuffer )
		{
			T_OutputPin::SetPinValue( ABuffer );
		}

	public:
		inline void SystemStart()
		{
            SetValue();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            SetValue();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_TryActive,
		typename T_OutputPin,
        typename T_Value
//        typename T_Value_ApplyValues,
//        typename T_Value_GetValue
	> class CanBusValueSourceState :
		public T_OutputPin,
        public T_Value
	{
	public:
		_V_PIN_( OutputPin )

	public:
        _V_PROP_( Value )

	protected:
		TStoredCanBusPacket FCurrentValue;

	public:
		void ExternalSetValue( const Mitov::TCanBusPacket &AValue )
		{
			if( FCurrentValue == AValue )
				return;

			FCurrentValue = AValue;
			T_OutputPin::SetPinValue( FCurrentValue );
		}

		void ResetValue()
		{
			bool AResult = false;
			T_Elements_TryActive::Call( AResult );
			if( AResult )
				return;

            T_Value::AssignToCanBusValue( FCurrentValue );
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		inline void SystemStart()
		{
            T_Value::AssignToCanBusValue( FCurrentValue );
			if( T_Value::GetCount() )
    			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_FInputValue,
        typename T_Value
//        typename T_Value_ApplyValues,
//        typename T_Value_GetValue
	> class ArduinoCanBusStateSetValueElement :
		public T_Enabled,
		public T_FInputValue,
        public T_Value
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Value )

	protected:
		_V_PROP_( FInputValue )

	public:
		inline void TryActive( bool &AResult )
		{
			if( ! Enabled().GetValue() )
				return;

			if( AResult )
				return;

			if( FInputValue() )
			{
				TStoredCanBusPacket APacket( Value().ID(), Value().IsRequest(), Value().IsExtended(), MitovMin<uint32_t>( 8, Value().Message().GetCount() ), Value().Message().GetArray() ); 
                C_OWNER.ExternalSetValue( APacket );
				AResult = true;
			}
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled().GetValue() )
				return;

			if( FInputValue() == *(bool *)_Data )
				return;

			FInputValue() = *(bool *)_Data;
			C_OWNER.ResetValue();
		}

	public:
		ArduinoCanBusStateSetValueElement()
		{
			FInputValue() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_ID,
		typename T_IsExtended,
		typename T_IsRequest,
		typename T_Message,
        typename T_Message_ApplyValues,
        typename T_Message_GetValue
	> class TArduinoCanBusValueElement :
		public T_ID,
		public T_IsExtended,
		public T_IsRequest,
		public T_Message
	{
	public:
		_V_PROP_( ID )
		_V_PROP_( IsExtended )
		_V_PROP_( IsRequest )
		_V_PROP_( Message )

	public:
		inline void GetValue( Mitov::TStoredCanBusPacket & AValue )
		{
            uint8_t *ABuffer = Message().Allocate();
			AValue = Mitov::TStoredCanBusPacket( ID(), IsRequest(), IsExtended(), Message().GetCount(), ABuffer );
            Message().Release( ABuffer );
		}

	};
//---------------------------------------------------------------------------
	template <
        uint32_t C_COUNT_Elements,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Elements_GetValue,
		typename T_InitialIndex,
		typename T_OutputPin
	> class CanBusValueArray :
		public T_ClockInputPin_o_IsConnected,
		public T_InitialIndex,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
        _V_PROP_( InitialIndex )

	protected:
        _V_PROP_( ClockInputPin_o_IsConnected )

    protected:
        inline void SendOutput()
        {
            uint32_t AIndex = InitialIndex();
			Mitov::TStoredCanBusPacket AValue;
            T_Elements_GetValue::Call( AIndex, AValue );
			T_OutputPin::SetPinValue( AValue );
        }

	public:
		inline void SystemStart()
		{
			if( InitialIndex() > C_COUNT_Elements - 1 )
				InitialIndex() = C_COUNT_Elements - 1;

			UpdateArrayValue();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            SendOutput();
		}

		inline void IndexInputPin_o_Receive( void *_Data )
		{
			uint32_t AIndex = *(uint32_t *)_Data;
			if( AIndex > C_COUNT_Elements - 1 )
				AIndex = C_COUNT_Elements - 1;

			if( InitialIndex() == AIndex )
				return;

			InitialIndex() = AIndex;

			UpdateArrayValue();
		}

	public:
		void UpdateArrayValue()
		{
			if( ! ClockInputPin_o_IsConnected() )
                SendOutput();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
