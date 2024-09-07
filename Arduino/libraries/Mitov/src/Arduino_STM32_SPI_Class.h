////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2024 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov_Arduino_SPI.h>

//---------------------------------------------------------------------------
namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		SPIClass &C_SPI,
		typename T_MISO,
		typename T_MOSI,
		typename T_SCK
	> class Arduino_STM32_SPI :
		public ArduinoSPI<C_SPI>,
		public T_MISO,
		public T_MOSI,
		public T_SCK
	{
	public:
		_V_PROP_( MISO )
		_V_PROP_( MOSI )
		_V_PROP_( SCK )

	public:
		inline void SystemInit()
		{
//			delay( 1000 );
//			Serial.print( "SCK: " );
//			Serial.println( SCK().GetValue() );

			C_SPI.setMISO( MISO().GetValue() );
			C_SPI.setMOSI( MOSI().GetValue() );
			C_SPI.setSCLK( SCK().GetValue() );
//			C_SPI.setMOSI( MISO().GetValue() );
//			C_SPI.setMISO( MOSI().GetValue() );
//			C_SPI.setSCLK( SCK().GetValue() );
//  SPI.setMISO( PB4 );
//  SPI.setMOSI( PB5 );
//  SPI.setSCLK( PB3 );
//  SPI.begin();
			C_SPI.begin();
		}

/*
	public:
		Arduino_STM32_SPI()
		{
			C_SPI.setMISO( MISO().GetValue() );
			C_SPI.setMOSI( MOSI().GetValue() );
			C_SPI.setSCLK( SCK().GetValue() );
		}
*/
	};
//---------------------------------------------------------------------------
} // Mitov
