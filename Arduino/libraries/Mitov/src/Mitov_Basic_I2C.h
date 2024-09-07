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
#include <Wire.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

#ifndef BUFFER_LENGTH
	#define BUFFER_LENGTH 256
#endif

#if defined( VISUINO_STMDUINO_MAPLE_OLD )
	typedef HardWire TwoWire;
#endif



namespace Mitov
{
	namespace I2C
	{
		const uint16_t	I2C_DEFAULT_READ_TIMEOUT	= 1000;
//	public:
		template <typename T> bool ReadBytes( T &AWire, uint8_t devAddr, uint8_t regAddr, uint8_t length, void *data, uint16_t timeout = I2C_DEFAULT_READ_TIMEOUT )
		{
			int8_t count = 0;
			uint32_t t1 = millis();

            // Arduino v1.0.1+, Wire library
            // Adds official support for repeated start condition, yay!

            // I2C/TWI subsystem uses internal buffer that breaks with large data requests
            // so if user requests more than BUFFER_LENGTH bytes, we have to do it in
            // smaller chunks instead of all at once
			uint8_t *AByteData = (uint8_t *)data;
            for (uint16_t k = 0; k < length; k += MitovMin<uint16_t>(length, BUFFER_LENGTH ))
			{
                AWire.beginTransmission(devAddr);
                AWire.write(regAddr);
                AWire.endTransmission();

                AWire.requestFrom( uint8_t( devAddr ), _VISUINO_I2C_SIZE_( MitovMin<int16_t>(length - k, int( BUFFER_LENGTH ))));

				regAddr += BUFFER_LENGTH;

                for (; AWire.available() && (timeout == 0 || millis() - t1 < timeout) && ( count < length ); count++)
                    *AByteData ++ = AWire.read();

            }

			return ( count == length );
		}

		template <typename T> bool ReadBytes_16bitAddress( T &AWire, uint8_t devAddr, bool ABigIndianAddr, uint16_t regAddr, uint8_t length, void *data, uint16_t timeout = I2C_DEFAULT_READ_TIMEOUT )
		{
			int8_t count = 0;
			uint32_t t1 = millis();

            // Arduino v1.0.1+, Wire library
            // Adds official support for repeated start condition, yay!

            // I2C/TWI subsystem uses internal buffer that breaks with large data requests
            // so if user requests more than BUFFER_LENGTH bytes, we have to do it in
            // smaller chunks instead of all at once
			uint8_t *AByteData = (uint8_t *)data;
            for (uint16_t k = 0; k < length; k += MitovMin<uint16_t>(length, BUFFER_LENGTH ))
			{
                AWire.beginTransmission(devAddr);
				if( ABigIndianAddr )
				{
					AWire.write(regAddr >> 8);
					AWire.write(regAddr & 0xFF);
				}
				else
				{
					AWire.write(regAddr & 0xFF);
					AWire.write(regAddr >> 8);
				}

                AWire.endTransmission();

                AWire.requestFrom( uint8_t( devAddr ), _VISUINO_I2C_SIZE_( MitovMin<int16_t>(length - k, int( BUFFER_LENGTH ))));

				regAddr += BUFFER_LENGTH;

                for (; AWire.available() && (timeout == 0 || millis() - t1 < timeout) && ( count < length ); count++)
                    *AByteData++ = AWire.read();

            }

			return ( count == length );
		}

		template <typename T> void WriteByte( T &AWire, uint8_t devAddr, uint8_t regAddr, uint8_t AValue )
		{
//			Serial.print( "Address: " ); Serial.print( devAddr, HEX ); Serial.print( " Reg: " ); Serial.print( regAddr, HEX );  Serial.print( " = " ); Serial.println( AValue, BIN );
			AWire.beginTransmission( devAddr );
			AWire.write( regAddr );
			AWire.write( AValue );
			AWire.endTransmission();
		}

		template <typename T> uint8_t ReadByte( T AWire, uint8_t devAddr, uint8_t regAddr )
		{
			AWire.beginTransmission( devAddr );
			AWire.write( regAddr );
			AWire.endTransmission();
			AWire.requestFrom( uint8_t( devAddr ), _VISUINO_I2C_SIZE_( 1 ));

			return AWire.read();

/*
			AWire.beginTransmission( devAddr );
			AWire.write( regAddr );
			AWire.endTransmission();
			AWire.requestFrom( devAddr, _VISUINO_I2C_SIZE_( 1 ));

			return AWire.read();
*/
		}

		template <typename T> void WriteByte_16bitAddress( T &AWire, uint8_t devAddr, bool ABigIndianAddr, uint16_t regAddr, uint8_t AValue )
		{
//			Serial.print( "Address: " ); Serial.print( devAddr, HEX ); Serial.print( " Reg: " ); Serial.print( regAddr, HEX );  Serial.print( " = " ); Serial.println( AValue, BIN );
			AWire.beginTransmission( devAddr );
			if( ABigIndianAddr )
			{
				AWire.write(regAddr >> 8);
				AWire.write(regAddr & 0xFF);
			}

			else
			{
				AWire.write(regAddr & 0xFF);
				AWire.write(regAddr >> 8);
			}

			AWire.write( AValue );
			AWire.endTransmission();
		}

		template <typename T> void WriteBytes_16bitAddress( T &AWire, uint8_t devAddr, bool ABigIndianAddr, uint16_t regAddr, uint8_t length, void *data )
		{
//			Serial.print( "Address: " ); Serial.print( devAddr, HEX ); Serial.print( " Reg: " ); Serial.print( regAddr, HEX );  Serial.print( " = " ); Serial.println( AValue, BIN );
			AWire.beginTransmission( devAddr );
			if( ABigIndianAddr )
			{
				AWire.write(regAddr >> 8);
				AWire.write(regAddr & 0xFF);
			}

			else
			{
				AWire.write(regAddr & 0xFF);
				AWire.write(regAddr >> 8);
			}

			AWire.write( (uint8_t *)data, length );
			AWire.endTransmission();
		}

	};
//---------------------------------------------------------------------------
	template <
		TwoWire &C_I2C
	> class ArduinoBasicI2C
	{
	public:
        inline  TwoWire &I2C() { return C_I2C; }
        
	public:
		void Print( const Mitov::String AValue )
		{
#if ( defined( VISUINO_ANY_ESP32 ) || defined( VISUINO_CHIPKIT ) || defined( VISUINO_MAPLE_MINI ) || defined( VISUINO_NANO33BLE ) )
			C_I2C.write( (uint8_t *)AValue.c_str(), AValue.length());
#else
			C_I2C.write( AValue.c_str(), AValue.length());
#endif
			C_I2C.write( '\r' );
			C_I2C.write( '\n' );
		}

		void Print( float AValue )
		{
			char AText[ __VISUINO_FLOAT_TO_STR_LEN__ ];
			dtostrf( AValue,  1, 2, AText );
			Print( Mitov::String( AText ));
		}

		void Print( int32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			Print( Mitov::String( AText ));
		}

		void Print( uint32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			Print( Mitov::String( AText ));
		}

		template <typename T> void PrintChar( T AValue )
		{
			C_I2C.write( AValue );
		}

		void Write( uint8_t *AData, uint32_t ASize )
		{
#if ( defined( VISUINO_NANO33BLE ) )
			C_I2C.write( AData, int( ASize ));
#elif ( defined( VISUINO_RPI_PICO ) || defined( VISUINO_ARDUINO_PORTENTA ) )
			C_I2C.write( AData, size_t( ASize ));
#else
			C_I2C.write( AData, ASize );
#endif
		}

	public:
		inline void SystemInit()
		{
			C_I2C.begin();
		}

	};
//---------------------------------------------------------------------------
	template <
		TwoWire &C_I2C,
        typename T_INSTANCE_SendByte,
		typename T_OutputPin
	> class ArduinoI2CMaster :
		public ArduinoBasicI2C<C_I2C>,
		public T_OutputPin
	{
		typedef ArduinoBasicI2C<C_I2C> inherited;

	public:
		_V_PIN_( OutputPin )

	public:
#if defined( VISUINO_ARDUINO_101 ) || defined( VISUINO_BBC_MICRO_BIT ) || defined( VISUINO_ARDUINO_RASPBERRY_PI )
	protected:
		inline void SystemInitInternal() {}

	public:
		inline void SystemLoopBegin()
		{
			if( C_I2C.available() )
			{
				int AData = C_I2C.read();
				if( AData >= 0 )
				{
					uint8_t AByte = AData;
					T_OutputPin::SetPinValue( Mitov::TDataBlock( 1, &AByte ));
				}
			}
		}

#else // VISUINO_ARDUINO_101
	protected:
		inline void SystemInitInternal()
		{
			C_I2C.onReceive( receiveEvent );
		}

	public:
		inline void SystemLoopBegin() {}

	protected:
		static void receiveEvent(int howMany)
		{
//			Serial.println( "Test1" );
			while( 1 < C_I2C.available())
			{ // loop through all but the last
//  				Serial.println( "Test2" );
				uint8_t AByte = C_I2C.read();
                T_INSTANCE_SendByte::Call( AByte );
//				((ArduinoI2COutput *)FInstance )->OutputPin().SetPinValue( Mitov::TDataBlock( 1, &AByte ));
			}

			uint8_t AByte = C_I2C.read();
            T_INSTANCE_SendByte::Call( AByte );
//			((ArduinoI2COutput *)FInstance )->OutputPin().SetPinValue( Mitov::TDataBlock( 1, &AByte ));
		}

#endif // VISUINO_ARDUINO_101
    public:
        inline void SendByte( uint8_t AByte )
        {
            T_OutputPin::SetPinValue( Mitov::TDataBlock( 1, &AByte ));
        }

	public:
		inline void SystemInit()
		{
			C_I2C.begin();
            SystemInitInternal();
		}

	};
//---------------------------------------------------------------------------
/*
	template <
		TwoWire &C_I2C,
		typename T_INSTANCE_RequestData,
		typename T_INSTANCE_SendByte,
		typename T_OutputPin,
		typename T_RequestOutputPin
	> class ArduinoI2COutput :
		public T_OutputPin,
		public T_RequestOutputPin
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( RequestOutputPin )

//	protected:
//		static void *FInstance;

	public:
#if defined( VISUINO_ARDUINO_101 ) || defined( VISUINO_BBC_MICRO_BIT ) || defined( VISUINO_ARDUINO_RASPBERRY_PI )
		inline void SystemInit() {}

		inline void SystemLoopBegin()
		{
			if( C_I2C.available() )
			{
				int AData = C_I2C.read();
				if( AData >= 0 )
				{
					uint8_t AByte = AData;
					T_OutputPin::SetPinValue( Mitov::TDataBlock( 1, &AByte ));
				}
			}
		}

#else // VISUINO_ARDUINO_101
		inline void SystemInit()
		{
			C_I2C.onReceive( receiveEvent );
			C_I2C.onRequest( requestEvent );
		}

		inline void SystemLoopBegin() {}

	protected:
		static void receiveEvent(int howMany)
		{
//			Serial.println( "Test1" );
			while( 1 < C_I2C.available())
			{ // loop through all but the last
//  				Serial.println( "Test2" );
				uint8_t AByte = C_I2C.read();
                T_INSTANCE_SendByte::Call( AByte );
//				((ArduinoI2COutput *)FInstance )->OutputPin().SetPinValue( Mitov::TDataBlock( 1, &AByte ));
			}

			uint8_t AByte = C_I2C.read();
            T_INSTANCE_SendByte::Call( AByte );
//			((ArduinoI2COutput *)FInstance )->OutputPin().SetPinValue( Mitov::TDataBlock( 1, &AByte ));
		}

		static void requestEvent()
		{
		    T_INSTANCE_RequestData::Call();
		}

#endif // VISUINO_ARDUINO_101
    public:
        inline void SendByte( uint8_t AByte )
        {
            T_OutputPin::SetPinValue( Mitov::TDataBlock( 1, &AByte ));
        }

        inline void RequestData()
        {
            T_RequestOutputPin::ClockPin();
        }

	};
*/
//---------------------------------------------------------------------------
/*
template <
		TwoWire &C_I2C,
		typename T_OutputPin,
		typename T_RequestOutputPin
	> void *ArduinoI2COutput<C_I2C, T_OutputPin, T_RequestOutputPin>::FInstance;
*/
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address
	> class I2CDevice :
		public T_Address
	{
//	public:
//		const static bool Enabled = true;

	public:
		_V_PROP_( Address )

	public:
		inline T_I2C	&GetWire() { return C_I2C; }
		inline uint8_t	GetAddress() { return Address().GetValue(); }

		void Print( const Mitov::String AValue )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
#if ( defined( VISUINO_ANY_ESP32 ) || defined( VISUINO_CHIPKIT ) || defined( VISUINO_MAPLE_MINI) )
			C_I2C.write( (uint8_t *)AValue.c_str(), AValue.length());
#else
			C_I2C.write( AValue.c_str(), AValue.length());
#endif
			C_I2C.write( '\r' );
			C_I2C.write( '\n' );
			C_I2C.endTransmission();
		}

		void Print( float AValue )
		{
			char AText[ __VISUINO_FLOAT_TO_STR_LEN__ ];
			dtostrf( AValue,  1, 2, AText );
			Print( Mitov::String( AText ));
		}

		void Print( int32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			Print( Mitov::String( AText ));
		}

		void Print( uint32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			Print( Mitov::String( AText ));
		}

		template <typename T> void PrintChar( T AValue )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( AValue );
			C_I2C.endTransmission();
		}

		void Write( uint8_t *AData, uint32_t ASize )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
#ifdef VISUINO_MBED_NANO
			C_I2C.write( AData, int( ASize ));
#else // VISUINO_MBED_NANO
			C_I2C.write( AData, ASize );
#endif // VISUINO_MBED_NANO
			C_I2C.endTransmission();
		}

//	protected:
//		virtual void DoReceive( void *_Data ) override
//		{
//			FWire.beginTransmission( Address );
//		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		typename T_OutputPin,
		typename T_Size
	> class I2CDeviceRequest :
		public T_OutputPin,
		public T_Size
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Size )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_INSTANCE.GetWire().requestFrom( uint8_t( C_INSTANCE.GetAddress() ), _VISUINO_I2C_SIZE_( Size() ));

			for( int i = 0; i < Size(); ++i )
				if( C_INSTANCE.GetWire().available() )
				{
					uint8_t AByte = C_INSTANCE.GetWire().read();
//					Serial.println( AByte );
					T_OutputPin::SetPinValue( Mitov::TDataBlock( 1, &AByte ));
				}

		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif


