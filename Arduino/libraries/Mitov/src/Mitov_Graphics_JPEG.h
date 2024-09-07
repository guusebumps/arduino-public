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
#include <Mitov_ImageBuffer_JPEG.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_X,
		typename T_Y
	> class GraphicsElementImage_JPEG :
		public T_Enabled,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( X )
		_V_PROP_( Y )

	protected:
		TStoredImageBufferHelperJPEG FBuffer;

	public:
		void Render()
		{
			C_OWNER.drawBuffer( C_OWNER, X(), Y(), FBuffer.GetWidth(), FBuffer.GetHeight(), FBuffer, 0, 0 );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TImageBuffer &AValue = *(TImageBuffer*)_Data;

			if( AValue.GetPixelFormat() == TImageFormat::JPEG )
				FBuffer = TStoredImageBufferHelperJPEG( AValue, TImageFormat::RGB565 );

			else
				FBuffer = AValue;

//USBSerial.println( "TEST1" );
			Render();
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif