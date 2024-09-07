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
#include <JPEGENC.h>
#include <Mitov_ImageBuffer.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
    namespace TArduinoImageToJpegSubSample
    {
        enum TArduinoImageToJpegSubSample 
        {  
            S444,
            S420
        };
    }
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
        typename T_OutputPin,
        typename T_Quality,
        typename T_SubSample
    > class TArduinoImageToJpeg :
		public T_Enabled,
        public T_OutputPin,
        public T_Quality,
        public T_SubSample
    {
	public:
        _V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Quality )
		_V_PROP_( SubSample )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled().GetValue() )
            {
                T_OutputPin::PinNotify( _Data );
                return;
            }

			TImageBuffer &ABuffer = *(TImageBuffer*)_Data;

            TStoredImageBufferHelper ATmpBuffer( ABuffer, TImageFormat::RGB24 );
			uint32_t AStride = ATmpBuffer.GetStride();
			uint8_t *APtrBase = ATmpBuffer.Modify();

            for( int y = 0; y < ATmpBuffer.GetHeight(); ++ y )
            {
  			    uint8_t *APtr = APtrBase;
                for( int x = 0; x < ATmpBuffer.GetWidth(); ++ x )
                {
                    uint8_t ATmp = APtr[ 0 ];
                    APtr[ 0 ] = APtr[ 2 ];
                    APtr[ 2 ] = ATmp;
                    APtr += 3;
                }

                APtrBase += AStride;
            }

            JPEGENC jpg;

            TStoredImageBuffer AOutBuffer( TImageFormat::JPEG, ATmpBuffer.GetWidth(), ATmpBuffer.GetHeight(), TImageFormat::GetSize( TImageFormat::RGB24, ATmpBuffer.GetWidth(), ATmpBuffer.GetHeight() ) );

            jpg.open( AOutBuffer.Write(), AOutBuffer.GetSize() );

            JPEGENCODE jpe;

//            int rc = jpg.encodeBegin( &jpe, ATmpBuffer.Width, ATmpBuffer.Height, JPEGE_PIXEL_RGB888, JPEGE_SUBSAMPLE_444, JPEGE_Q_HIGH );
            uint8_t AQuality;
            if( Quality() < 0.33333333 )
                AQuality = JPEGE_Q_LOW;

            else if( Quality() < 0.66666667 )
                AQuality = JPEGE_Q_MED;

            else if( Quality() < 1.0 )
                AQuality = JPEGE_Q_HIGH;

            else
                AQuality = JPEGE_Q_BEST;

            int rc = jpg.encodeBegin( &jpe, ATmpBuffer.GetWidth(), ATmpBuffer.GetHeight(), JPEGE_PIXEL_RGB888, SubSample(), AQuality );
//            int iBytePP = 2; // Temporary for RGB565!!!
            int iBytePP = 3; // Temporary for RGB24!!!
//            int32_t AStride = ATmpBuffer.GetStride();

            uint32_t iMCUCount = (( ATmpBuffer.GetWidth() + jpe.cx-1 ) / jpe.cx ) * (( ATmpBuffer.GetHeight() + jpe.cy-1 ) / jpe.cy );
            const uint8_t *AInData = ATmpBuffer.Read();
            for ( uint32_t i = 0; i < iMCUCount && rc == JPEGE_SUCCESS; ++ i )
        		rc = jpg.addMCU( &jpe, (uint8_t *)AInData + jpe.x * iBytePP + jpe.y * AStride, AStride );

/*
            for ( uint32_t i = 0; i < iMCUCount && rc == JPEGE_SUCCESS; i ++ ) 
                rc = jpg.addMCU( &jpe, & AInData[ (jpe.x * iBytePP) + (jpe.y * iPitch) ], iPitch );

            uint32_t AStride = ATmpBuffer.GetStride();
            for( int y = 0; y < ATmpBuffer.Height; ++ y )
                jpg.addMCU( &jpe, AInData + y * AStride, AStride );
*/
            AOutBuffer.SetSize( jpg.close(), true );

            T_OutputPin::SendPinValueRef( AOutBuffer );
        }

    };
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

