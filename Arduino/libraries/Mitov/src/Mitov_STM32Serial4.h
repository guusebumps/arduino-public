////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2024 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#if ! defined(HAVE_HWSERIAL4)
  #if defined(USART4)
    HardwareSerial Serial4(USART4);
  #else
    HardwareSerial Serial4(UART4);
  #endif
//    void serialEvent4() __attribute__((weak));
#endif
