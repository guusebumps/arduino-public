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
        typename T_RightChannel,
		typename T_SampleRate
    > class TArduinoPDMMicrophoneSPM1423HM4H :
        public T_RightChannel,
		public T_SampleRate
    {
    public:
        _V_PROP_( RightChannel )
        _V_PROP_( SampleRate )

    public:
		inline void ControlInputPin_o_Receive( void *_Data )
		{
//            Serial.println( "TEST_7" );

		    TI2SInfo &AInfo = *(TI2SInfo *)_Data;

		    AInfo.Input.Used = true;

		    for( int i = 0; i < AInfo.Input.Modes.size(); ++ i )
		    {
                if( AInfo.Input.Modes[ i ].Mode != TI2SMode::PDM )
                    AInfo.Input.Modes[ i ].Enabled = false;

                else
                {
//                    AInfo.Input.Modes[ i ].SetSampleRate( 1000000, 3250000 );
                    AInfo.Input.Modes[ i ].SetSampleRate( 8000, SampleRate().GetValue() ); 
//                    AInfo.Input.Modes[ i ].SetBitsPerSample( 16, 16 );
                    if( RightChannel() )
                        AInfo.Input.Modes[ i ].ChannelsMap |= 0b10;

                    else
                        AInfo.Input.Modes[ i ].ChannelsMap |= 0b01;

                }
            }
		}

    };
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif