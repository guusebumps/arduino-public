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
//	class BasicMemoryElement;
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_Address,
		uint8_t  C_Mask,
		typename T_FModified,
		typename T_FRememberClocked,
		typename T_OutputPin,
		typename T_RecallInputPin_o_IsConnected,
		typename T_RecallOnRemember,
		typename T_RememberInputPin_o_IsConnected,
		typename TYPE_DATA,
		typename TYPE_PIN
	> class TypedMemoryElement :
		public T_FModified,
		public T_FRememberClocked,
		public T_OutputPin,
		public T_RecallInputPin_o_IsConnected,
		public T_RecallOnRemember,
		public T_RememberInputPin_o_IsConnected
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( RememberInputPin_o_IsConnected )
		_V_PROP_( RecallInputPin_o_IsConnected )

	public:
		_V_PROP_( RecallOnRemember )

	protected:
		_V_PROP_( FRememberClocked )
		_V_PROP_( FModified )

		TYPE_DATA	FValue;

	public:
		inline void ProcessWrite()
		{
			if( RememberInputPin_o_IsConnected() )
			{
				if( ! FRememberClocked() )
					return;

				FRememberClocked() = false;
			}

			else if( ! FModified() )
				return;

			FModified() = false;

//			Serial.println( "WRITE" );
//			Serial.println( InputPin.Value );
//			Serial.println( sizeof( T ) );

			C_OWNER.WriteData( C_Address, sizeof( TYPE_DATA ), (uint8_t *)&FValue );

			if( RecallOnRemember() || ( ! RecallInputPin_o_IsConnected() ))
				RecallInputPin_o_Receive( nullptr );
		}

	public:
		inline void RememberInputPin_o_Receive( void *_Data )
		{
			FRememberClocked() = true;
		}

		void RecallInputPin_o_Receive( void *_Data )
		{
			TYPE_DATA AValue = 0;
			if( C_OWNER.ReadData( C_Address, sizeof( TYPE_DATA ), (uint8_t *)&AValue ))
				T_OutputPin::SetPinValue( TYPE_PIN( AValue ));
		}

		inline void InputPin_o_Receive( void *_Data )
		{
			FValue = *(TYPE_PIN*)_Data;
			FModified() = true;
		}

	public:
		inline TypedMemoryElement()
		{
//			FRememberClocked() = false;
			FModified() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_Address,
		uint8_t  C_Mask,
		typename T_FRememberClocked,
		typename T_OutputPin,
		typename T_RecallInputPin_o_IsConnected,
		typename T_RecallOnRemember,
		typename T_RememberInputPin_o_IsConnected,
		typename TYPE_DATA,
		typename TYPE_PIN
	> class TypedMemoryElement_NonCached :
		public T_FRememberClocked,
		public T_OutputPin,
		public T_RecallInputPin_o_IsConnected,
		public T_RecallOnRemember,
		public T_RememberInputPin_o_IsConnected
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( RememberInputPin_o_IsConnected )
		_V_PROP_( RecallInputPin_o_IsConnected )

	public:
		_V_PROP_( RecallOnRemember )

	protected:
		_V_PROP_( FRememberClocked )

	public:
		inline void RememberInputPin_o_Receive( void *_Data )
		{
			FRememberClocked() = true;
		}

		void RecallInputPin_o_Receive( void *_Data )
		{
			TYPE_DATA AValue = 0;
			if( C_OWNER.ReadData( C_Address, sizeof( TYPE_DATA ), (uint8_t *)&AValue ))
				T_OutputPin::SetPinValue( TYPE_PIN( AValue ));
		}

		inline void InputPin_o_Receive( void *_Data )
		{
			if( RememberInputPin_o_IsConnected() )
			{
				if( ! FRememberClocked() )
					return;

				FRememberClocked() = false;
			}

			TYPE_PIN AValue = *(TYPE_PIN*)_Data;
			C_OWNER.WriteData( C_Address, sizeof( TYPE_DATA ), (uint8_t *)&AValue );

			if( RecallOnRemember() || ( ! RecallInputPin_o_IsConnected() ))
				RecallInputPin_o_Receive( nullptr );
		}

	public:
/*
		inline TypedMemoryElement_NonCached()
		{
			FRememberClocked() = false;
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_Address,
		uint8_t  C_Mask,
		typename T_FModified,
		typename T_FRememberClocked,
		typename T_FStarted,
		uint32_t C_MaxSize,
		typename T_OutputPin,
		typename T_RecallInputPin_o_IsConnected,
		typename T_RecallOnRemember,
		typename T_RememberInputPin_o_IsConnected
	> class TextMemoryElement :
		public T_FModified,
		public T_FRememberClocked,
		public T_FStarted,
		public T_OutputPin,
		public T_RecallInputPin_o_IsConnected,
		public T_RecallOnRemember,
		public T_RememberInputPin_o_IsConnected
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( RememberInputPin_o_IsConnected )
		_V_PROP_( RecallInputPin_o_IsConnected )

	public:
		_V_PROP_( RecallOnRemember )

	protected:
		_V_PROP_( FModified )
		_V_PROP_( FRememberClocked )
		_V_PROP_( FStarted )

	protected:
		::Mitov::String FValue;

	public:
		inline void ProcessWrite()
		{
			if( RememberInputPin_o_IsConnected() )
			{
				if( ! FRememberClocked() )
					return;

				FRememberClocked() = false;
			}

			else if( ! FModified() )
				return;

			FModified() = false;

//			Serial.println( "WRITE" );
//			Serial.println( InputPin.Value );
//			Serial.println( sizeof( T ) );
//			Mitov::String AText = FValue;
//			uint8_t ASize = MitovMin<unsigned int>( C_MaxSize, AText.length() );

//			Serial.println( ASize );
//			Serial.println( AText );

//			if( FValue.length() == 0 )
//				return;

			if( C_MaxSize <= 0xFF )
			{
				uint8_t ASize = FValue.length();
				C_OWNER.WriteData( C_Address, 1, &ASize );
				if( ASize > 0 )
					C_OWNER.WriteData( C_Address + 1, FValue.length(), (uint8_t *)FValue.c_str() );

			}

			else if( C_MaxSize <= 0xFFFF )
			{
				uint16_t ASize = FValue.length();
				uint8_t ASizeBuffer[ 2 ];
				ASizeBuffer[ 0 ] = ASize;
				ASizeBuffer[ 1 ] = ASize >> 8;
				C_OWNER.WriteData( C_Address, 2, ASizeBuffer );
				if( ASize > 0 )
					C_OWNER.WriteData( C_Address + 2, FValue.length(), (uint8_t *)FValue.c_str() );

			}

			else if( C_MaxSize <= 0xFFFFFF )
			{
				uint32_t ASize = FValue.length();
				uint8_t ASizeBuffer[ 3 ];
				ASizeBuffer[ 0 ] = ASize;
				ASizeBuffer[ 1 ] = ASize >> 8;
				ASizeBuffer[ 2 ] = ASize >> 16;
				C_OWNER.WriteData( C_Address, 3, ASizeBuffer );
				if( ASize > 0 )
					C_OWNER.WriteData( C_Address + 3, FValue.length(), (uint8_t *)FValue.c_str() );

			}

			else
			{
				uint32_t ASize = FValue.length();
				uint8_t ASizeBuffer[ 4 ];
				ASizeBuffer[ 0 ] = ASize;
				ASizeBuffer[ 1 ] = ASize >> 8;
				ASizeBuffer[ 2 ] = ASize >> 16;
				ASizeBuffer[ 3 ] = ASize >> 24;
				C_OWNER.WriteData( C_Address, 4, ASizeBuffer );
				if( ASize > 0 )
					C_OWNER.WriteData( C_Address + 4, FValue.length(), (uint8_t *)FValue.c_str() );
			}

//			C_OWNER.WriteData( C_Address, FValue.length(), (uint8_t *)FValue.c_str() );
//			C_OWNER.WriteData( C_Address + 1, ASize, (uint8_t *)AText.c_str() );

			if( RecallOnRemember() || ( ! RecallInputPin_o_IsConnected() ))
				RecallInputPin_o_Receive( nullptr );
		}

	public:
		inline void RememberInputPin_o_Receive( void *_Data )
		{
			FRememberClocked() = true;
		}

		void RecallInputPin_o_Receive( void *_Data )
		{
//			uint8_t *ABuffer = new uint8_t[ C_MaxSize ];
            if( FModified() )
            {
				T_OutputPin::SetPinValue( FValue );
                return;
            }

			if( C_MaxSize <= 0xFF )
			{
				uint8_t ASize;
				if( C_OWNER.ReadData( C_Address, 1, &ASize ))
				{
					FValue = "";
					::Mitov::Func::ExpandString( FValue, ASize );
					if( C_OWNER.ReadData( C_Address + 1, ASize, (uint8_t *)FValue.c_str() ))
						T_OutputPin::SetPinValue( FValue );

				}

				return;
			}

			if( C_MaxSize <= 0xFFFF )
			{
				uint8_t ASizeBuffer[ 2 ];
				if( C_OWNER.ReadData( C_Address, 2, ASizeBuffer ))
				{
					FValue = "";
					uint16_t ASize = ASizeBuffer[ 0 ] | ( uint16_t( ASizeBuffer[ 1 ] ) << 8 );
					::Mitov::Func::ExpandString( FValue, ASize );
					if( C_OWNER.ReadData( C_Address + 2, ASize, (uint8_t *)FValue.c_str() ))
						T_OutputPin::SetPinValue( FValue );

				}

				return;
			}

			if( C_MaxSize <= 0xFFFFFF )
			{
				uint8_t ASizeBuffer[ 3 ];
				if( C_OWNER.ReadData( C_Address, 3, ASizeBuffer ))
				{
					FValue = "";
					uint32_t ASize = ASizeBuffer[ 0 ] | ( uint32_t( ASizeBuffer[ 1 ] ) << 8 ) | ( uint32_t( ASizeBuffer[ 2 ] ) << 16 );
					::Mitov::Func::ExpandString( FValue, ASize );
					if( C_OWNER.ReadData( C_Address + 3, ASize, (uint8_t *)FValue.c_str() ))
						T_OutputPin::SetPinValue( FValue );

				}

				return;
			}

			uint8_t ASizeBuffer[ 4 ];
			if( C_OWNER.ReadData( C_Address, 4, ASizeBuffer ))
			{
				FValue = "";
				uint32_t ASize = ASizeBuffer[ 0 ] | ( uint32_t( ASizeBuffer[ 1 ] ) << 8 ) | ( uint32_t( ASizeBuffer[ 2 ] ) << 16 ) | ( uint32_t( ASizeBuffer[ 3 ] ) << 24 );
				::Mitov::Func::ExpandString( FValue, ASize );
				if( C_OWNER.ReadData( C_Address + 4, ASize, (uint8_t *)FValue.c_str() ))
					T_OutputPin::SetPinValue( FValue );

			}
		}

		inline void InputPin_o_Receive( void *_Data )
		{
			::Mitov::String AText = (char*)_Data;

			uint8_t ASize = MitovMin<unsigned int>( C_MaxSize, AText.length() );
			AText = AText.substring( 0, ASize );

			if( ! FStarted().GetValue() )
				if( FValue == AText )
					return;

			FValue = AText;

//			Serial.println( ASize );
//			Serial.println( AText );

			FModified() = true;
			FStarted() = true;
		}

	public:
		inline TextMemoryElement()
		{
			FModified() = false;
			FStarted() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_Address,
		uint8_t  C_Mask,
		uint8_t  C_SizeBytes,
		typename T_FModified,
		typename T_FRememberClocked,
		uint32_t C_MaxSize,
		typename T_OutputPin,
		typename T_RecallInputPin_o_IsConnected,
		typename T_RecallOnRemember,
		typename T_RememberInputPin_o_IsConnected
	> class BinaryMemoryElement :
		public T_FModified,
		public T_FRememberClocked,
		public T_OutputPin,
		public T_RecallInputPin_o_IsConnected,
		public T_RecallOnRemember,
		public T_RememberInputPin_o_IsConnected
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( RememberInputPin_o_IsConnected )
		_V_PROP_( RecallInputPin_o_IsConnected )

	public:
		_V_PROP_( RecallOnRemember )

	protected:
		_V_PROP_( FRememberClocked )
		_V_PROP_( FModified )

	protected:
		uint8_t	FValue[ C_MaxSize + C_SizeBytes ];

	public:
		inline void ProcessWrite()
		{
			if( RememberInputPin_o_IsConnected() )
			{
				if( ! FRememberClocked() )
					return;

				FRememberClocked() = false;
			}

			else if( ! FModified() )
				return;

			FModified() = false;

//			Serial.println( "WRITE" );
//			Serial.println( InputPin.Value );
//			Serial.println( sizeof( T ) );
//			Mitov::String AText = FValue;
//			uint8_t ASize = MitovMin<unsigned int>( C_MaxSize, AText.length() );

//			Serial.println( ASize );
//			Serial.println( AText );

            uint32_t ASize;
			if( C_SizeBytes < 2 )
				ASize = FValue[ 0 ];

			else if( C_SizeBytes < 3 )
				ASize = FValue[ 0 ] | ( uint32_t( FValue[ 1 ] ) << 8 );

			else if( C_SizeBytes < 4 )
				ASize = FValue[ 0 ] | ( uint32_t( FValue[ 1 ] ) << 8 ) | ( uint32_t( FValue[ 2 ] ) << 16 );

			else
				ASize = FValue[ 0 ] | ( uint32_t( FValue[ 1 ] ) << 8 ) | ( uint32_t( FValue[ 2 ] ) << 16 ) | ( uint32_t( FValue[ 3 ] ) << 24 );

//            Serial.println( "ProcessWrite" );
//            Serial.println( ASize );

//            C_OWNER.WriteData( C_Address, sizeof( FSize ), (uint8_t *)&FSize )
//            C_OWNER.WriteData( C_Address + sizeof( FSize ), FSize, (uint8_t *)FValue )

//			if( ASize == 0 )
//				return;

			C_OWNER.WriteData( C_Address, ASize + C_SizeBytes, (uint8_t *)FValue );
//			C_OWNER.WriteData( C_Address + 1, ASize, (uint8_t *)AText.c_str() );

			if( RecallOnRemember() || ( ! RecallInputPin_o_IsConnected() ))
				RecallInputPin_o_Receive( nullptr );
		}

	public:
		inline void RememberInputPin_o_Receive( void *_Data )
		{
			FRememberClocked() = true;
		}

		void RecallInputPin_o_Receive( void *_Data )
		{
            if( FModified() )
            {
                uint32_t ADataSize;
				if( C_SizeBytes < 2 )
					ADataSize = FValue[ 0 ];

				else if( C_SizeBytes < 3 )
					ADataSize = FValue[ 0 ] | ( uint32_t( FValue[ 1 ] ) << 8 );

				else if( C_SizeBytes < 4 )
					ADataSize = FValue[ 0 ] | ( uint32_t( FValue[ 1 ] ) << 8 ) | ( uint32_t( FValue[ 2 ] ) << 16 );

				else
					ADataSize = FValue[ 0 ] | ( uint32_t( FValue[ 1 ] ) << 8 ) | ( uint32_t( FValue[ 2 ] ) << 16 ) | ( uint32_t( FValue[ 3 ] ) << 24 );

				uint32_t ASize = MitovMin<uint32_t>( C_MaxSize, ADataSize );
//                Serial.println( ASize );
                TDataBlock AValue( ASize, FValue + C_SizeBytes );

				T_OutputPin::SetPinValue( AValue );
                return;
            }

//			uint8_t *ABuffer = new uint8_t[ C_MaxSize ];
			if( C_OWNER.ReadData( C_Address, C_MaxSize + C_SizeBytes, FValue ))
			{
//                Serial.println( "C_OWNER.ReadData" );
//				Mitov::String AValue;
                uint32_t ADataSize;
				if( C_SizeBytes < 2 )
					ADataSize = FValue[ 0 ];

				else if( C_SizeBytes < 3 )
					ADataSize = FValue[ 0 ] | ( uint32_t( FValue[ 1 ] ) << 8 );

				else if( C_SizeBytes < 4 )
					ADataSize = FValue[ 0 ] | ( uint32_t( FValue[ 1 ] ) << 8 ) | ( uint32_t( FValue[ 2 ] ) << 16 );

				else
					ADataSize = FValue[ 0 ] | ( uint32_t( FValue[ 1 ] ) << 8 ) | ( uint32_t( FValue[ 2 ] ) << 16 ) | ( uint32_t( FValue[ 3 ] ) << 24 );

				uint32_t ASize = MitovMin<uint32_t>( C_MaxSize, ADataSize );
//                Serial.println( ASize );
                TDataBlock AValue( ASize, FValue + C_SizeBytes );

				T_OutputPin::SetPinValue( AValue );
			}

//			delete [] ABuffer;
		}

		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::TDataBlock &AValue = *(TDataBlock*)_Data;

			uint32_t ASize = MitovMin<uint32_t>( C_MaxSize, AValue.GetSize() );
//            Serial.println( "InputPin_o_Receive" );
//            Serial.println( ASize );

			FValue[ 0 ] = ASize;
			FValue[ 1 ] = ASize >> 8;
			FValue[ 2 ] = ASize >> 16;
			FValue[ 3 ] = ASize >> 24;

			memcpy( FValue + C_SizeBytes, AValue.Read(), ASize );

//uint32_t ADataSize = uint32_t( FValue[ 0 ] ) | ( uint32_t( FValue[ 1 ] ) << 8 ) | ( uint32_t( FValue[ 2 ] ) << 16 ) | ( uint32_t( FValue[ 3 ] ) << 24 );
//Serial.println( "ADataSize" );
//Serial.println( ADataSize );

//			Serial.println( ASize );
//			Serial.println( AText );

//			C_OWNER.WriteData( C_Address, 1, (uint8_t *)&ASize );
//			C_OWNER.WriteData( C_Address + 1, ASize, (uint8_t *)AText.c_str() );
			FModified() = true;
		}

	public:
		inline BinaryMemoryElement()
		{
			FValue[ 0 ] = 0;
//			FRememberClocked() = false;
			FModified() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_Address,
		uint8_t  C_Mask,
		typename T_FRememberClocked,
		uint32_t C_MaxSize,
		typename T_OutputPin,
		typename T_RecallInputPin_o_IsConnected,
		typename T_RecallOnRemember,
		typename T_RememberInputPin_o_IsConnected
	> class TextMemoryElement_NonCached :
		public T_FRememberClocked,
		public T_OutputPin,
		public T_RecallInputPin_o_IsConnected,
		public T_RecallOnRemember,
		public T_RememberInputPin_o_IsConnected
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( RememberInputPin_o_IsConnected )
		_V_PROP_( RecallInputPin_o_IsConnected )

	public:
		_V_PROP_( RecallOnRemember )

	protected:
		_V_PROP_( FRememberClocked )

	public:
		inline void RememberInputPin_o_Receive( void *_Data )
		{
			FRememberClocked() = true;
		}

		void RecallInputPin_o_Receive( void *_Data )
		{
//			uint8_t *ABuffer = new uint8_t[ C_MaxSize ];
			uint8_t	AValue[ C_MaxSize + 1 ];

			if( C_OWNER.ReadData( C_Address, C_MaxSize, AValue ))
			{
				Mitov::String ATextValue;
				uint8_t ASize = MitovMin<unsigned int>( C_MaxSize, AValue[ 0 ] );
				ATextValue.reserve( ASize );

				for( int i = 0; i < ASize; ++ i )
					ATextValue += ((char *)AValue )[ i + 1 ];

				T_OutputPin::SetPinValue( ATextValue );
			}

//			delete [] ABuffer;
		}

		inline void InputPin_o_Receive( void *_Data )
		{
			if( RememberInputPin_o_IsConnected() )
			{
				if( ! FRememberClocked() )
					return;

				FRememberClocked() = false;
			}

			char *AText = (char*)_Data;

			uint8_t ASize = MitovMin<unsigned int>( C_MaxSize, strlen( AText ) + 1 );

			C_OWNER.WriteData( C_Address, ASize, (uint8_t *)AText );

			if( RecallOnRemember() || ( ! RecallInputPin_o_IsConnected() ))
				RecallInputPin_o_Receive( nullptr );
		}

	public:
/*
		inline TextMemoryElement_NonCached()
		{
			FRememberClocked() = false;
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_Address,
		uint8_t  C_Mask,
		typename T_FRememberClocked,
		uint32_t C_MaxSize,
		typename T_OutputPin,
		typename T_RecallInputPin_o_IsConnected,
		typename T_RecallOnRemember,
		typename T_RememberInputPin_o_IsConnected
	> class BinaryMemoryElement_NonCached :
		public T_FRememberClocked,
		public T_OutputPin,
		public T_RecallInputPin_o_IsConnected,
		public T_RecallOnRemember,
		public T_RememberInputPin_o_IsConnected
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( RememberInputPin_o_IsConnected )
		_V_PROP_( RecallInputPin_o_IsConnected )

	public:
		_V_PROP_( RecallOnRemember )

	protected:
		_V_PROP_( FRememberClocked )

	public:
		inline void RememberInputPin_o_Receive( void *_Data )
		{
			FRememberClocked() = true;
		}

		void RecallInputPin_o_Receive( void *_Data )
		{
//			uint8_t *ABuffer = new uint8_t[ C_MaxSize ];
			uint8_t	AValue[ C_MaxSize + 1 ];

			if( C_OWNER.ReadData( C_Address, C_MaxSize, AValue ))
			{
				uint8_t ASize = MitovMin<unsigned int>( C_MaxSize, AValue[ 0 ] );
                TDataBlock ABuffer( ASize, AValue + 1 );
				T_OutputPin::SetPinValue( ABuffer );
			}

//			delete [] ABuffer;
		}

		inline void InputPin_o_Receive( void *_Data )
		{
			if( RememberInputPin_o_IsConnected() )
			{
				if( ! FRememberClocked() )
					return;

				FRememberClocked() = false;
			}

			Mitov::TDataBlock &AValue = *(TDataBlock*)_Data;

			uint8_t ASize = MitovMin<unsigned int>( C_MaxSize, AValue.GetSize() + 1 );

			C_OWNER.WriteData( C_Address, ASize, AValue.Read() );

			if( RecallOnRemember() || ( ! RecallInputPin_o_IsConnected() ))
				RecallInputPin_o_Receive( nullptr );
		}

	public:
/*
		inline BinaryMemoryElement_NonCached()
		{
			FRememberClocked() = false;
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_Address,
		uint8_t  C_Mask,
		typename T_FModified,
		typename T_FRememberClocked,
		typename T_FValue,
		typename T_OutputPin,
		typename T_RecallInputPin_o_IsConnected,
		typename T_RecallOnRemember,
		typename T_RememberInputPin_o_IsConnected
	> class DigitalMemoryElement :
		public T_FModified,
		public T_FRememberClocked,
		public T_FValue,
		public T_OutputPin,
		public T_RecallInputPin_o_IsConnected,
		public T_RecallOnRemember,
		public T_RememberInputPin_o_IsConnected
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( RememberInputPin_o_IsConnected )
		_V_PROP_( RecallInputPin_o_IsConnected )

	public:
		_V_PROP_( RecallOnRemember )

	protected:
		_V_PROP_( FRememberClocked )
		_V_PROP_( FModified )
		_V_PROP_( FValue )
//	
	public:
		inline void ProcessWrite()
		{
			if( RememberInputPin_o_IsConnected() )
			{
				if( ! FRememberClocked() )
					return;

				FRememberClocked() = false;
			}

			else if( ! FModified() )
				return;

			FModified() = false;

//			Serial.println( "WRITE" );

			uint8_t AByteValue;
			if( C_OWNER.ReadData( C_Address, 1, &AByteValue ))
			{
				if( FValue() )
					AByteValue |= C_Mask;

				else
					AByteValue &= ~ C_Mask;

				C_OWNER.WriteData( C_Address, 1, &AByteValue );

				if( RecallOnRemember() || ( ! RecallInputPin_o_IsConnected() ))
					RecallInputPin_o_Receive( nullptr );
			}
		}

	public:
		inline void RememberInputPin_o_Receive( void *_Data )
		{
			FRememberClocked() = true;
		}

		void RecallInputPin_o_Receive( void *_Data )
		{
//			Serial.print( "READ: " ); Serial.println( FAddress, HEX );

			uint8_t AByteValue;
			if( C_OWNER.ReadData( C_Address, 1, &AByteValue ))
			{
				bool AValue = (( AByteValue & C_Mask ) != 0 );
				T_OutputPin::SetPinValue( AValue );
			}
		}

		inline void InputPin_o_Receive( void *_Data )
		{
			FValue() = *(bool*)_Data;
			FModified() = true;
		}

	public:
		inline DigitalMemoryElement()
		{
//			FRememberClocked() = false;
			FModified() = false;
			FValue() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_Address,
		uint8_t  C_Mask,
		typename T_FRememberClocked,
		typename T_OutputPin,
		typename T_RecallInputPin_o_IsConnected,
		typename T_RecallOnRemember,
		typename T_RememberInputPin_o_IsConnected
	> class DigitalMemoryElement_NonCached :
		public T_FRememberClocked,
		public T_OutputPin,
		public T_RecallInputPin_o_IsConnected,
		public T_RecallOnRemember,
		public T_RememberInputPin_o_IsConnected
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( RememberInputPin_o_IsConnected )
		_V_PROP_( RecallInputPin_o_IsConnected )

	public:
		_V_PROP_( RecallOnRemember )

	protected:
		_V_PROP_( FRememberClocked )
	
	public:
		inline void RememberInputPin_o_Receive( void *_Data )
		{
			FRememberClocked() = true;
		}

		void RecallInputPin_o_Receive( void *_Data )
		{
//			Serial.print( "READ: " ); Serial.println( FAddress, HEX );

			uint8_t AByteValue;
			if( C_OWNER.ReadData( C_Address, 1, &AByteValue ))
			{
				bool AValue = (( AByteValue & C_Mask ) != 0 );
				T_OutputPin::SetPinValue( AValue );
			}
		}

		inline void InputPin_o_Receive( void *_Data )
		{
			if( RememberInputPin_o_IsConnected() )
			{
				if( ! FRememberClocked() )
					return;

				FRememberClocked() = false;
			}

			bool AValue = *(bool*)_Data;

			uint8_t AByteValue;

			if( C_OWNER.ReadData( C_Address, 1, &AByteValue ))
			{
				if( AValue )
					AByteValue |= C_Mask;

				else
					AByteValue &= ~ C_Mask;

				C_OWNER.WriteData( C_Address, 1, &AByteValue );

				if( RecallOnRemember() || ( ! RecallInputPin_o_IsConnected() ))
					RecallInputPin_o_Receive( nullptr );
			}
		}

	public:
/*
		inline DigitalMemoryElement_NonCached()
		{
			FRememberClocked() = false;
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Position
	> class MemoryStorageElementReadSeek :
		public T_Position
	{
    public:
		_V_PROP_( Position )

    public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.SeekRead( Position().GetValue() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Position
	> class MemoryStorageElementWriteSeek :
		public T_Position
	{
    public:
		_V_PROP_( Position )

    public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.SeekWrite( Position().GetValue() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPin
	> class MemoryStorageElementReadTextLine :
		public T_OutputPin
	{
    public:
		_V_PIN_( OutputPin )

	protected:
		bool readStringLine( Mitov::String &AResult )
		{
//			AResult = "";
			int c = C_OWNER.Read();
//			Serial.println( c );
			if( c < 0 )
				return false;

//			Serial.println( "R1" );
			while ( ( c >= 0 ) && ( c != '\n' ) && ( c != '\r' ) )
			{
				AResult += char( c );
				c = C_OWNER.Read();
//				Serial.println( c );
			}

			if( c == '\n' )
			{
				if( C_OWNER.Peek() == '\r' )
					C_OWNER.Read();
			}

			else if( c == '\r' )
			{
				if( C_OWNER.Peek() == '\n' )
					C_OWNER.Read();
			}

/*
			for( int i = 0; ( c >= 0 ) && ( i < 2 ); ++i )
			{
				c = C_OWNER.Peek();
				Serial.println( c );
				if( ( c != '\n' ) && ( c != '\r' ))
					break;

				C_OWNER.Read();
			}
*/
/*
			while ( c >= 0 && ( c == '\n' || c == '\r' ))
				c = C_OWNER.FFile.read();
*/
			return true;
		}

	public:
		void Read()
		{
			if( ! C_OWNER.TryOpen() )
				return;

			Mitov::String ALine;			
			C_OWNER.BeginRead();
//			Serial.println( "READ1" );
			if( readStringLine( ALine ))
				T_OutputPin::SetPinValue( ALine );

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Read();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPin,
		typename T_TYPE
	> class MemoryStorageElementReadTyped :
		public T_OutputPin
	{
    public:
		_V_PIN_( OutputPin )

    public:
		void Read()
		{
			if( ! C_OWNER.TryOpen() )
				return;


			if( ! C_OWNER.DataAvailable() )
				return;

			C_OWNER.BeginRead();

			T_TYPE AData;
			size_t AReadSize = C_OWNER.ReadBytes( (uint8_t *)&AData, sizeof( AData ));
			if( AReadSize == sizeof( AData ) )
				T_OutputPin::SetPinValue( AData );

//			else
//				C_OWNER.ResetReadPosition();
		}

    public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Read();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPin,
		typename T_Size
	> class MemoryStorageElementReadBinary :
		public T_OutputPin,
		public T_Size
	{
    public:
		_V_PIN_( OutputPin )

    public:
		_V_PROP_( Size )

    public:
		void Read()
		{
			if( Size().GetValue() == 0 )
				return;

			if( ! C_OWNER.TryOpen() )
				return;

			if( ! C_OWNER.DataAvailable() )
				return;

			C_OWNER.BeginRead();

			TValueArray<uint8_t> ABuffer( Size().GetValue(), nullptr );

//			uint8_t *ABuffer = new uint8_t[ Size().GetValue() ];

			size_t AReadSize = C_OWNER.ReadBytes( ABuffer.Write(), Size().GetValue() );
			if( AReadSize == Size().GetValue() )
				T_OutputPin::SetPinValue( ABuffer );

//			delete [] ABuffer;
		}

    public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Read();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Count,
		typename T_Elements_Read,
		typename T_UntilEndOfFile
	> class MemoryStorageElementRepeatRead :
		public T_Count,
		public T_UntilEndOfFile
	{
	public:
		_V_PROP_( Count )
		_V_PROP_( UntilEndOfFile )

	public:
		inline bool TryOpen()
		{
			return C_OWNER.TryOpen();
		}

		inline void BeginRead()
		{
			C_OWNER.BeginRead();
		}

		inline int Read()
		{
			return C_OWNER.Read();
		}

		inline int Peek()
		{
			return C_OWNER.Peek();
		}

		inline size_t ReadBytes( uint8_t *AData, size_t ASize )
		{
			return C_OWNER.ReadBytes( AData, ASize );
		}

		inline bool DataAvailable()
		{
			return C_OWNER.DataAvailable();
		}

//		inline void ResetReadPosition()

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! C_OWNER.TryOpen() )
				return;

			if( UntilEndOfFile() )
			{
				while( C_OWNER.DataAvailable() )
					T_Elements_Read::Call();

			}

			else
			{
				for( int i = 0; i < Count().GetValue(); ++i )
					T_Elements_Read::Call();
			}
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T
	> class MemoryStorageElementWriteTyped
	{
    public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_OWNER.Write( _Data, sizeof( T ));
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER
	> class MemoryStorageElementWriteTextLine
	{
    public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_OWNER.println( (char *)_Data );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Size
	> class MemoryStorageElementWriteBinary :
		public T_Size
	{
    public:
		_V_PROP_( Size )

    public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::TDataBlock ADataBlock = *(Mitov::TDataBlock*)_Data;

			uint32_t AInSize = ADataBlock.GetSize();

			if( Size().GetValue() != 0 )
			{
				if( AInSize >= Size().GetValue() )
					C_OWNER.Write( (uint8_t *)ADataBlock.Read(), Size().GetValue() );

				else
				{
					C_OWNER.Write( (uint8_t *)ADataBlock.Read(), AInSize );
					static const uint8_t AZero = 0;
					for( int32_t i = AInSize; i < Size().GetValue(); ++i )
						C_OWNER.Write( (uint8_t *)&AZero, 1 );

				}
			}

			else
				C_OWNER.Write( (uint8_t *)ADataBlock.Read(), AInSize );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_Address,
		uint8_t  C_Mask,
		typename T_FReadPosition,
		typename T_FWritePosition,
		typename T_Loop,
		typename T_ReadPositionOutputPin,
		typename T_Size,
		typename T_WritePositionOutputPin
	> class MemoryStorageElement :
		public T_FReadPosition,
		public T_FWritePosition,
		public T_Loop,
		public T_ReadPositionOutputPin,
		public T_Size,
		public T_WritePositionOutputPin
	{
		typedef MemoryStorageElement T_SELF;

	public:
		_V_PIN_( ReadPositionOutputPin )
		_V_PIN_( WritePositionOutputPin )

	public:
		_V_PROP_( Loop )
		_V_PROP_( Size )

	protected:
		_V_PROP_( FReadPosition )
		_V_PROP_( FWritePosition )

	protected:
		void UpdateReadPosition( uint32_t AValue )
		{
			FReadPosition() = AValue;
			ReadPositionOutputPin().SetPinValue( AValue, false );
		}

		void UpdateWritePosition( uint32_t AValue )
		{
			FWritePosition() = AValue;
			WritePositionOutputPin().SetPinValue( AValue, false );
		}

	public:
		inline int Read()
		{
//			if( ! TryOpen() )
//				return -1;

//			Serial.println( "Read" );

			uint8_t AChar;
			if( ReadBytes( &AChar, 1 ) < 1 )
				return -1;

			return AChar;
		}

		inline int Peek()
		{
//			if( ! TryOpen() )
//				return -1;

//			Serial.println( "Peek" );

			uint8_t AChar;
			if( ! ReadBytesInternal( &AChar, 1, false ))
				return -1;

			return AChar;
		}

	public:
		template <typename T_CONFIG> inline static size_t write( T_SELF &AImplementation, T_CONFIG &AConfig, uint8_t AValue )
		{
//			Serial.println( "write" );
//			Serial.println( AValue );
			AImplementation.Write( &AValue, 1 );
			return 1;
		}

		template <typename T> void PrintChar( T AValue )
		{
			write( *this, *this, AValue );
		}

	public:
		template <typename T> void Print( T AValue )
		{
			println( AValue );
		}

	public:
		template <typename T_CONFIG, typename T> size_t print( T_CONFIG *AConfig, T AValue )
		{
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &T_SELF::template write<T_CONFIG> );
			return APrint.print( AValue );
		}

		template <typename T_CONFIG, typename T> size_t println( T_CONFIG *AConfig, T AValue )
		{
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &T_SELF::template write<T_CONFIG> );
			return APrint.println( AValue );
		}

		template <typename T> inline size_t print( T AValue )
		{
			return print<T_SELF, T>( this, AValue );
		}

		template <typename T> inline size_t println( T AValue )
		{
			return println<T_SELF, T>( this, AValue );
		}

	protected:
		inline size_t ReadBytesInternal( uint8_t *AData, size_t ASize, bool AUpdatePosition )
		{
			if( ( FReadPosition() + ASize ) > Size().GetValue() )
				if( ASize < Size().GetValue() )
					if( AUpdatePosition )
					{					
						if( Loop() )
							UpdateReadPosition( 0 );

					}

					else
						return 0;

			if( ( FReadPosition() + ASize ) <= Size().GetValue() )
				if( C_OWNER.ReadData( C_Address + FReadPosition().GetValue(), ASize, AData ))
				{
					if( AUpdatePosition )
						UpdateReadPosition( FReadPosition() + ASize );

					return ASize;
				}

			return 0;
		}

	public:
		inline void ProcessWrite()
		{
		}

//		inline void SetHasWrite() {} // Placeholder!
		inline void BeginRead() {} // Placeholder!
		inline bool TryOpen() { return true; }
		inline bool DataAvailable() { return true; }

		inline void SeekRead( uint32_t AValue )
		{
			FReadPosition() = AValue;
//			FReadMoved() = true;
		}

		inline void SeekWrite( uint32_t AValue )
		{
			FWritePosition() = AValue;
//			FWriteMoved() = true;
		}

		void Write( void *AData, size_t ASize )
		{
			if( ( FWritePosition() + ASize ) > Size().GetValue() )
				if( ASize < Size().GetValue() )
					if( Loop() )
						UpdateWritePosition( 0 );

			if( ( FWritePosition() + ASize ) <= Size().GetValue() )
			{
				C_OWNER.WriteData( C_Address + FWritePosition().GetValue(), ASize, (uint8_t *)AData );
				UpdateWritePosition( FWritePosition() + ASize );
			}
		}

		inline size_t ReadBytes( uint8_t *AData, size_t ASize )
		{
			return ReadBytesInternal( AData, ASize, true );
		}

	public:
		inline void SystemStart()
		{
			ReadPositionOutputPin().SetPinValue( FReadPosition().GetValue(), false );
			WritePositionOutputPin().SetPinValue( FWritePosition().GetValue(), false );
		}

	public:
		inline MemoryStorageElement()
		{
			FReadPosition() = 0;
			FWritePosition() = 0;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_FReadPosition,
		typename T_FWritePosition,
		typename T_Loop,
		typename T_ReadPositionOutputPin,
		uint32_t C_SIZE,
		typename T_WritePositionOutputPin
	> class MemoryStorage :
		public T_FReadPosition,
		public T_FWritePosition,
		public T_Loop,
		public T_ReadPositionOutputPin,
		public T_WritePositionOutputPin
	{
		typedef MemoryStorage T_SELF;

	protected:
		uint8_t	FBuffer[ C_SIZE ];

	public:
		_V_PIN_( ReadPositionOutputPin )
		_V_PIN_( WritePositionOutputPin )

	public:
		_V_PROP_( Loop )

	protected:
		_V_PROP_( FReadPosition )
		_V_PROP_( FWritePosition )

	protected:
		void UpdateReadPosition( uint32_t AValue )
		{
			FReadPosition() = AValue;
			ReadPositionOutputPin().SetPinValue( AValue, false );
		}

		void UpdateWritePosition( uint32_t AValue )
		{
			FWritePosition() = AValue;
			WritePositionOutputPin().SetPinValue( AValue, false );
		}

		inline size_t ReadBytesInternal( uint8_t *AData, size_t ASize, bool AUpdatePosition )
		{
			if( ( FReadPosition() + ASize ) > C_SIZE )
				if( ASize < C_SIZE )
					if( AUpdatePosition )
					{					
						if( Loop() )
							UpdateReadPosition( 0 );

					}

					else
						return 0;

			if( ( FReadPosition() + ASize ) <= C_SIZE )
//				if( C_OWNER.ReadData( C_Address + FReadPosition().GetValue(), ASize, AData ))
				{
					memcpy( AData, FBuffer + FReadPosition().GetValue(), ASize );
					if( AUpdatePosition )
						UpdateReadPosition( FReadPosition() + ASize );

					return ASize;
				}

			return 0;
		}


	public:
		inline int Read()
		{
//			if( ! TryOpen() )
//				return -1;

//			Serial.println( "Read" );

			uint8_t AChar;
			if( ReadBytes( &AChar, 1 ) < 1 )
				return -1;

			return AChar;
		}

		inline int Peek()
		{
//			if( ! TryOpen() )
//				return -1;

//			Serial.println( "Peek" );

			uint8_t AChar;
			if( ! ReadBytesInternal( &AChar, 1, false ))
				return -1;

			return AChar;
		}

		inline void ProcessWrite()
		{
		}

		inline void BeginRead() {} // Placeholder!
		inline bool TryOpen() { return true; }
		inline bool DataAvailable() { return true; }

		inline void SeekRead( uint32_t AValue )
		{
			FReadPosition() = AValue;
//			FReadMoved() = true;
		}

		inline void SeekWrite( uint32_t AValue )
		{
			FWritePosition() = AValue;
//			FWriteMoved() = true;
		}

		void Write( void *AData, size_t ASize )
		{
			if( ( FWritePosition() + ASize ) > C_SIZE )
				if( ASize < C_SIZE )
					if( Loop() )
						UpdateWritePosition( 0 );

			if( ( FWritePosition() + ASize ) <= C_SIZE )
			{
				memcpy( FBuffer + FWritePosition().GetValue(), AData, ASize );
//				C_OWNER.WriteData( C_Address + FWritePosition().GetValue(), ASize, (uint8_t *)AData );
				UpdateWritePosition( FWritePosition() + ASize );
			}
		}

		inline size_t ReadBytes( uint8_t *AData, size_t ASize )
		{
			return ReadBytesInternal( AData, ASize, true );
		}

	public:
		template <typename T_CONFIG> inline static size_t write( T_SELF &AImplementation, T_CONFIG &AConfig, uint8_t AValue )
		{
//			Serial.println( "write" );
//			Serial.println( AValue );
			AImplementation.Write( &AValue, 1 );
			return 1;
		}

		template <typename T> void PrintChar( T AValue )
		{
			write( *this, *this, AValue );
		}

	public:
		template <typename T> void Print( T AValue )
		{
			println( AValue );
		}

	public:
		template <typename T_CONFIG, typename T> size_t print( T_CONFIG *AConfig, T AValue )
		{
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &T_SELF::template write<T_CONFIG> );
			return APrint.print( AValue );
		}

		template <typename T_CONFIG, typename T> size_t println( T_CONFIG *AConfig, T AValue )
		{
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &T_SELF::template write<T_CONFIG> );
			return APrint.println( AValue );
		}

		template <typename T> inline size_t print( T AValue )
		{
			return print<T_SELF, T>( this, AValue );
		}

		template <typename T> inline size_t println( T AValue )
		{
			return println<T_SELF, T>( this, AValue );
		}

	public:
		inline void SystemStart()
		{
			ReadPositionOutputPin().SetPinValue( FReadPosition().GetValue(), false );
			WritePositionOutputPin().SetPinValue( FWritePosition().GetValue(), false );
		}

	public:
		inline MemoryStorage()
		{
			FReadPosition() = 0;
			FWritePosition() = 0;
		}

	};
//---------------------------------------------------------------------------
	struct TStorageStateAccess
	{
	public:
		enum TElement { eStartReadPosition, eReadPosition, eWritePosition, eSize, eRemainingReadSize };

	public:
		bool		Set;
		TElement	Element;
		uint32_t	Value;

	public:
		inline TStorageStateAccess( bool ASet, TElement AElement, uint32_t AValue ) :
			Set( ASet ),
			Element( AElement ),
			Value( AValue )
		{
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_Address,
		uint8_t  C_Mask,
		typename T_FModified
	> class StorageStateElementEEPROM :
		public T_FModified
	{
	protected:
		_V_PROP_( FModified )

	protected:
		uint32_t FValues[ 5 ];

	public:
		inline void ProcessWrite()
		{
			if( FModified().GetValue() == 0 )
				return;

//			Serial.println( "ProcessWrite" );
//			delay( 1000 );

			for( int i = 0; i < 5; ++ i )
				if( FModified().GetValue() & ( 1 << 1 ) )
					C_OWNER.WriteData( C_Address + i * sizeof( uint32_t ), sizeof( uint32_t ), (uint8_t *)( &( FValues[ i ] )));

			FModified() = 0;
//			C_OWNER.WriteData( C_Address, sizeof( TYPE_DATA ), (uint8_t *)&FValue );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TStorageStateAccess &AData = *(TStorageStateAccess *)_Data;
			if( AData.Set )
			{
//				Serial.println( "InputPin_o_Receive.SET" );
//				Serial.println( AData.Element );
//				delay( 1000 );
				FValues[ AData.Element ] = AData.Value;
				FModified() = FModified().GetValue() | ( 1 << AData.Element );
			}

			else
			{
//				Serial.println( "InputPin_o_Receive.GET" );
//				Serial.println( AData.Element );
//				delay( 1000 );
				C_OWNER.ReadData( C_Address + AData.Element * sizeof( uint32_t ), sizeof( uint32_t ), (uint8_t *)( &AData.Value ));
			}

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_Address,
		uint8_t  C_Mask
	> class StorageStateElement
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TStorageStateAccess &AData = *(TStorageStateAccess *)_Data;
			if( AData.Set )
				C_OWNER.WriteData( C_Address + AData.Element * sizeof( uint32_t ), sizeof( uint32_t ), (uint8_t *)( &AData.Value ));

			else
				C_OWNER.ReadData( C_Address + AData.Element * sizeof( uint32_t ), sizeof( uint32_t ), (uint8_t *)( &AData.Value ));

		}

	public:
		inline StorageStateElement()
		{
			uint32_t AValue = 0;
			C_OWNER.WriteData( C_Address + 0 * sizeof( uint32_t ), sizeof( uint32_t ), (uint8_t *)( &AValue ));
			C_OWNER.WriteData( C_Address + 1 * sizeof( uint32_t ), sizeof( uint32_t ), (uint8_t *)( &AValue ));
			C_OWNER.WriteData( C_Address + 2 * sizeof( uint32_t ), sizeof( uint32_t ), (uint8_t *)( &AValue ));
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif