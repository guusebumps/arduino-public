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
#include <Mitov_ImageBuffer.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
        typename T_Format,
        typename T_OutputPin,
        typename T_CONVERT
    > class TArduinoChangeImageFormat :
		public T_Enabled,
        public T_Format,
        public T_OutputPin
    {
	public:
        _V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
        _V_PROP_( Format )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled().GetValue() )
            {
                T_OutputPin::PinNotify( _Data );
                return;
            }

			TImageBuffer &ABuffer = *(TImageBuffer*)_Data;
			if( ABuffer.GetPixelFormat() == Format().GetValue() )
            {
                T_OutputPin::PinNotify( _Data );
                return;
            }

            T_CONVERT AOutBuffer( ABuffer, Format().GetValue() );
            T_OutputPin::SendPinValueRef( AOutBuffer );
//            T_OutputPin::PinNotify( & AOutBuffer );
        }

    };
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

