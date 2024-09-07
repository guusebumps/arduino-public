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

#if defined( VISUINO_ESP32 )
//SPIClass SPI1( HSPI );

#ifndef __BORLANDC__
SPIClass SPI1_Instance( HSPI ); // Workaround ESP32 bug!
#define SPI1 SPI1_Instance
#endif // __BORLANDC__

#endif

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
