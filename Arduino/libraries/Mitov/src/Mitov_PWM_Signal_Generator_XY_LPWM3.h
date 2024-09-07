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
		typename T_ClockInputPin_o_IsConnected,
		typename T_DutyCycleOutputPin_0,
		typename T_DutyCycleOutputPin_1,
		typename T_DutyCycleOutputPin_2,
		typename T_Enabled,
		typename T_FClocked,
		typename T_FDecimals,
		typename T_FDutyCycleIndex,
//		typename T_FHasEqual,
		typename T_FState,
		typename T_FValue,
		typename T_FWaitForInfomation,
		typename T_FrequencyOutputPin,
		typename T_TimeoutOutputPin
	> class TArduinoPWMSignalGenerator_XY_LPWM3_Read :
		public T_ClockInputPin_o_IsConnected,
		public T_DutyCycleOutputPin_0,
		public T_DutyCycleOutputPin_1,
		public T_DutyCycleOutputPin_2,
		public T_Enabled,
		public T_FClocked,
		public T_FDecimals,
		public T_FDutyCycleIndex,
//		public T_FHasEqual,
		public T_FState,
		public T_FValue,
		public T_FWaitForInfomation,
		public T_FrequencyOutputPin,
		public T_TimeoutOutputPin
	{
	public:
		_V_PIN_( DutyCycleOutputPin_0 )
		_V_PIN_( DutyCycleOutputPin_1 )
		_V_PIN_( DutyCycleOutputPin_2 )
		_V_PIN_( FrequencyOutputPin )
		_V_PIN_( TimeoutOutputPin )

	public:
		_V_PROP_( Enabled )

    private:
		_V_PROP_( ClockInputPin_o_IsConnected )

	private:
		_V_PROP_( FClocked )
		_V_PROP_( FDecimals )
		_V_PROP_( FDutyCycleIndex )
//		_V_PROP_( FHasEqual )
		_V_PROP_( FState )
		_V_PROP_( FValue )
		_V_PROP_( FWaitForInfomation )

	private:
		enum 
		{
			sNone,
			sInvalid,
			sFrequency,
			sSearchDutyCycle,
			sSearchDutyCycle_Index,
			sDutyCycle
		};

	private:
		void SendRead()
		{
			C_OWNER.FIndex() = 0;
			FState() = sNone;
//			FHasEqual() = false;
			FWaitForInfomation() = true;
			C_OWNER.GetSerial().GetStream().print( "read" );
			C_OWNER.FLastTime = millis();
//            C_OWNER.ClearSent();
		}

	public:
		inline void ProcessChar( char AChar )
		{
			switch( FState() )
			{
				case sNone :
					if( C_OWNER.FIndex() == 0 )
					{
						if( AChar == 'F' )
						{
							FState() = sFrequency;
							C_OWNER.FIndex() = 0;
							FValue() = 0;
							FDecimals() = 0;
							break;
						}

						if( AChar <= ' ' )
							break;

					}

					FState() = sInvalid;
					break;

				case sInvalid :
					if( AChar == 10 || AChar == 13 )
					{
						FValue() = 0;
						FDecimals() = 0;
                        C_OWNER.FIndex() = 0;
						FState() = sNone;
//						FHasEqual() = false;
					}

					break;

				case sFrequency :
					if( C_OWNER.FIndex() == 3 )
					{
						if( AChar == ',' )
							break;

						if( AChar == 'D' )
						{
							float AValue = FValue();
							switch( FDecimals() )
							{
								case 1: AValue *= 10; break;
								case 2: AValue *= 100; break;
								case 3: AValue *= 1000; break;
							}

							T_FrequencyOutputPin::SetPinValue( AValue );

							FState() = sSearchDutyCycle_Index;
							C_OWNER.FIndex() = 0;
							FValue() = 0;
							FDecimals() = 0;
							break;
						}

						FState() = sInvalid;
						break;
					}

					if( AChar >= '0' && AChar <= '9' )
					{
						FValue() = FValue() * 10 + ( AChar - '0' );
						C_OWNER.FIndex() = C_OWNER.FIndex() + 1;
                        break;
					}

					if( AChar == '.' )
					{
						if( C_OWNER.FIndex() == 1 )
						{
							if( FDecimals() == 0 )
							{
								FDecimals() = 1;
								break;
							}
						}

						else if( C_OWNER.FIndex() == 2 )
						{
							if( FDecimals() < 2 )
							{
								FDecimals() = FDecimals() + 2;
								break;
							}
						}
					}

					FState() = sInvalid;
					break;

				case sSearchDutyCycle :
					if( AChar == 10 || AChar == 13 )
					{
						FState() = sNone;
						C_OWNER.ClearSent();
						FWaitForInfomation() = false;
						C_OWNER.FLastTime = millis();
						break;
					}

					if( AChar == 'D' )
						FState() = sSearchDutyCycle_Index;

					else if( AChar != ',' )
						FState() = sInvalid;

					break;

				case sSearchDutyCycle_Index :
					if( AChar >= '1' && AChar <= '3' )
					{
						FDutyCycleIndex() = AChar - '1';
						break;
					}

					if( AChar == ':' )
					{
						FState() = sDutyCycle;
						break;
					}

					FState() = sInvalid;
					break;

				case sDutyCycle :
					if( C_OWNER.FIndex() == 3 )
					{
						if( AChar == ',' )
						{
							float AValue = float( FValue() ) / 100.0;
							switch( FDutyCycleIndex() )
							{
								case 0:	T_DutyCycleOutputPin_0::SetPinValue( AValue ); break;
								case 1:	T_DutyCycleOutputPin_1::SetPinValue( AValue ); break;
								case 2:	T_DutyCycleOutputPin_2::SetPinValue( AValue ); break;
							}

							FDutyCycleIndex() = FDutyCycleIndex() + 1;
							FState() = sSearchDutyCycle;
							C_OWNER.FIndex() = 0;
                            FValue() = 0;
//							FHasEqual() = false;
							break;
						}

						FState() = sInvalid;
					}

					if( AChar >= '0' && AChar <= '9' )
					{
						FValue() = FValue() * 10 + ( AChar - '0' );
						C_OWNER.FIndex() = C_OWNER.FIndex() + 1;
						break;
					}

					break;
			}
    
/*
			if( AChar == '\0' )
			{
				C_OWNER.FIndex() = 0;
				FState() = sNone;
				return;
			}
*/
/*
			if (AChar == 13 || AChar == 10 || FIndex > 31)
			{
				FBuffer[FIndex] = '\0';
				FIndex = 0;
				Mitov::String tmp = Mitov::String(FBuffer);

				if( tmp[0] == 'F' )
				{
					int AStrIndex = tmp.indexOf('D');
					Mitov::String freq = tmp.substring(0, AStrIndex);
					Mitov::String duty = tmp.substring(AStrIndex);
					FrequencyOutputPin().SetPinValue(freq.toFloat());
					DutyCycleOutputPin().SetPinValue(duty.toFloat());

					C_OWNER.ClearSent();
				}

				return;
			}

			FBuffer[ FIndex ++ ] = AChar;
*/
		}

		inline void NeedsRequest( bool &AValue )
		{
			AValue = ( ! ClockInputPin_o_IsConnected() ) || FClocked(); // || ( millis() - C_OWNER.FLastTime > C_OWNER.Delay() );
		}

		inline void IsBusy( bool &AValue )
		{
            AValue = FWaitForInfomation();
        }

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
				FClocked() = true;

		}

	public:
		inline void SystemLoopEnd()
		{
			if( ! Enabled().GetValue() )
                return;

			if( C_OWNER.GetIsBusy() )
				return;

			if( FClocked() )
			{
				FClocked() = false;
				SendRead();
				return;
			}

			unsigned int ACurrentMillis = millis();
			if( FWaitForInfomation() )
			{
				if( ACurrentMillis - C_OWNER.FLastTime > C_OWNER.Timeout())
				{
					TimeoutOutputPin().ClockPin();
					FWaitForInfomation() = false;
					C_OWNER.ClearSent();
				}
			}

			else if( ! ClockInputPin_o_IsConnected() )
			{
				if( ACurrentMillis - C_OWNER.FLastTime > C_OWNER.Delay() )
					SendRead();
			}

		}

	public:
		inline TArduinoPWMSignalGenerator_XY_LPWM3_Read()
		{
			FClocked() = false;
			FWaitForInfomation() = false;
			FState() = sNone;
//			FHasEqual() = false;
			FDecimals() = 0;
			FValue() = 0;
			FDutyCycleIndex() = 0;
        }

    };
//---------------------------------------------------------------------------
    template <
        typename T_SERIAL, T_SERIAL &C_SERIAL,
        typename T_ErrorOutputPin,
		typename T_FDutyCycle_0,
		typename T_FDutyCycle_1,
		typename T_FDutyCycle_2,
		typename T_FDutyCycle_Requested_0,
		typename T_FDutyCycle_Requested_1,
		typename T_FDutyCycle_Requested_2,
//		typename T_FDutyCycle_Sent_0,
//		typename T_FDutyCycle_Sent_1,
//		typename T_FDutyCycle_Sent_2,
		typename T_FFrequency,
		typename T_FFrequency_Requested,
		typename T_FInDelay,
//		typename T_FFrequency_Sent,
		typename T_FInDown,
//		typename T_FInFall,
		typename T_FIndex,
		typename T_FSent_Index,
		typename T_FWaitingDutyCycle_Response_0,
		typename T_FWaitingDutyCycle_Response_1,
		typename T_FWaitingDutyCycle_Response_2,
		typename T_FWaitingFrequency_Response,
		typename T_Read_IsBusy,
		typename T_Read_NeedsRequest,
		typename T_Read_ProcessChar,
		typename T_RefreshRate,
		typename T_Timeout
	> class TArduinoPWMSignalGenerator_XY_LPWM3 :
        public T_ErrorOutputPin,
		public T_FDutyCycle_0,
		public T_FDutyCycle_1,
		public T_FDutyCycle_2,
		public T_FDutyCycle_Requested_0,
		public T_FDutyCycle_Requested_1,
		public T_FDutyCycle_Requested_2,
//		public T_FDutyCycle_Sent_0,
//		public T_FDutyCycle_Sent_1,
//		public T_FDutyCycle_Sent_2,
		public T_FFrequency,
		public T_FFrequency_Requested,
//		public T_FFrequency_Sent,
		public T_FInDelay,
		public T_FInDown,
//        public T_FInFall,
		public T_FIndex,
		public T_FSent_Index,
		public T_FWaitingDutyCycle_Response_0,
		public T_FWaitingDutyCycle_Response_1,
		public T_FWaitingDutyCycle_Response_2,
		public T_FWaitingFrequency_Response,
		public T_RefreshRate,
		public T_Timeout
	{
	public:
		_V_PIN_( ErrorOutputPin )

	public:
		_V_PROP_( RefreshRate )
		_V_PROP_( Timeout )

	public:
		_V_PROP_( FDutyCycle_0 )
		_V_PROP_( FDutyCycle_1 )
		_V_PROP_( FDutyCycle_2 )
//		_V_PROP_( FDutyCycle_Sent_0 )
//		_V_PROP_( FDutyCycle_Sent_1 )
//		_V_PROP_( FDutyCycle_Sent_2 )
		_V_PROP_( FDutyCycle_Requested_0 )
		_V_PROP_( FDutyCycle_Requested_1 )
		_V_PROP_( FDutyCycle_Requested_2 )
		_V_PROP_( FFrequency )
//		_V_PROP_( FFrequency_Sent )
		_V_PROP_( FFrequency_Requested )
		_V_PROP_( FInDown )
		_V_PROP_( FInDelay )
//		_V_PROP_( FInFall )
		_V_PROP_( FIndex )
		_V_PROP_( FSent_Index )
		_V_PROP_( FWaitingDutyCycle_Response_0 )
		_V_PROP_( FWaitingDutyCycle_Response_1 )
		_V_PROP_( FWaitingDutyCycle_Response_2 )
		_V_PROP_( FWaitingFrequency_Response )

    public:
        unsigned int FLastTime = 0;

	public:
		inline T_SERIAL &GetSerial()
		{
			return C_SERIAL;
		}

		inline bool GetIsBusy()
		{
			return ( FWaitingDutyCycle_Response_0() || FWaitingDutyCycle_Response_1() || FWaitingDutyCycle_Response_2() || FWaitingFrequency_Response() );
		}

		inline void ClearSent()
		{
            FSent_Index() = 0;
//			FDutyCycle_Sent_0() = false;
		}

		inline uint32_t Delay()
		{
			return ( 1000 / RefreshRate() ) + 0.5;
        }

	private:
/*
		bool IsRequested( uint8_t AIndex )
		{
			switch( AIndex )
			{
				case 0 :
				default :
					return FFrequency_Requested();

				case 1 :
					return FDutyCycle_Requested_0();

				case 2 :
					return FDutyCycle_Requested_1();

				case 3 :
					return FDutyCycle_Requested_2();

//				case 4 :

			}
		}
*/
		void ResetIndex()
		{
			FInDown() = false;
//			FInFall() = false;
			FIndex() = 0;
        }

		inline void ProcessChar( char AChar )
		{
            if( AChar == '\0' )
            {
                FIndex() = 0;
                return;
            }

			if( ! GetIsBusy() )
			{
				T_Read_ProcessChar::Call( AChar );
				return;
			}

			if( AChar == ' ' )
				if( FIndex() == 4 )
                    return;

			if( AChar == 13 || AChar == 10 )
			{
				if( FIndex() == 4 )
				{
					if( FInDown() )
						T_ErrorOutputPin::SetPinValue( false );

					else
					{
						T_ErrorOutputPin::SetPinValue( true );
//						if( FWaitingFrequencyResponse() )
//							FFrequencySent() = false;

					}

					if( FWaitingFrequency_Response() )
						FFrequency_Requested() = false;

					else if( FWaitingDutyCycle_Response_0() )
					{
						FDutyCycle_Requested_0() = false;
//						FFrequency_Sent() = false;
					}

					else if( FWaitingDutyCycle_Response_1() )
					{
						FDutyCycle_Requested_1() = false;
//						FFrequency_Sent() = false;
					}

					else if( FWaitingDutyCycle_Response_2() )
					{
						FDutyCycle_Requested_2() = false;
//						FFrequency_Sent() = false;
					}

					FWaitingFrequency_Response() = false;
					FWaitingDutyCycle_Response_0() = false;
					FWaitingDutyCycle_Response_1() = false;
					FWaitingDutyCycle_Response_2() = false;
                    FInDelay() = true;
					//

				}

				FInDown() = false;
				FIndex() = 0;
				FLastTime = millis();
				return;
			}

			if( FIndex() == 0 )
			{
				if( AChar == 'D' )
				{
					FInDown() = true;
					FIndex() = 1;
				}

				else if( AChar == 'F' )
				{
//					FInFall() = true;
					FIndex() = 1;
				}

				else
					ResetIndex();

				return;
			}

			static const char CMessages[ 2 ][ 3 ] = {
				{
					'O',
					'W',
					'N',
				},
				{
					'A',
					'I',
					'L',
				}
			};

			if( AChar == CMessages[ FInDown().GetValue() ? 0 : 1 ][ FIndex() - 1 ] )
			{
				FIndex() = FIndex() + 1;
				return;
			}

			else if( ( ! FInDown().GetValue() ) && ( FIndex().GetValue() == 2 ) && ( AChar == 'L' ) )
			{
				FIndex() = FIndex() + 1;
				return;
			}

            FIndex() = 0;
			FInDown() = false;
/*
			if (AChar == 13 || AChar == 10 || FIndex > 31)
			{
				FBuffer[FIndex] = '\0';
				FIndex = 0;
				Mitov::String tmp = Mitov::String(FBuffer);

				if (tmp.startsWith("DOWN"))
					T_ErrorOutputPin::SetPinValue(false);

				else if (tmp.startsWith("FALL"))
					T_ErrorOutputPin::SetPinValue(true);
*/
		}

    public:
		inline void _DirectPinReceive( void *_Data )
		{
			Mitov::TDataBlock ADataBlock = *(Mitov::TDataBlock *)_Data;

			uint32_t ASize = ADataBlock.GetSize();
			const uint8_t *AData = ADataBlock.Read();

			for( uint32_t i = 0; i < ASize; ++ i )
				ProcessChar( AData[ i ] );
		}

	public:
		inline void FrequencyInputPin_o_Receive( void *_Data )
		{
			float AValue = *(float *)_Data;
			if( AValue < 0 )
				FFrequency() = 0;

			else if( AValue > 150000 )
				FFrequency() = 150000;

			else
				FFrequency() = AValue + 0.5;

			FFrequency_Requested() = true;
		}

		inline void DutyCycleInputPins_o_Receive( uint8_t AIndex, void *_Data )
		{
			float AValue = *(float *)_Data;
			AValue = MitovConstrain<float>( AValue, 0.0, 1.0 ) * 100.0 + 0.5;

			switch( AIndex )
			{
				case 0 :
					FDutyCycle_0() = AValue;
					FDutyCycle_Requested_0() = true;
					break;

				case 1 :
					FDutyCycle_1() = AValue;
					FDutyCycle_Requested_1() = true;
					break;

				case 2 :
					FDutyCycle_2() = AValue;
					FDutyCycle_Requested_2() = true;
//					Serial.println( "TEST2" );
					break;

			}

		}

	public:
		inline void SystemLoopEnd()
		{
            bool AIsBusy = GetIsBusy();
			if( AIsBusy )
			{
				if( millis() - FLastTime <= Timeout())
					return;

				T_ErrorOutputPin::SetPinValue( true );
//                FSent_Index() = FSent_Index() - 1;
				if( FWaitingFrequency_Response() )
				{
//					FFrequency_Sent() = false;
					FWaitingFrequency_Response() = false;
				}

				if( FWaitingDutyCycle_Response_0() )
				{
//					FDutyCycle_Sent_0() = false;
					FWaitingDutyCycle_Response_0() = false;
				}

				if( FWaitingDutyCycle_Response_1() )
				{
//					FDutyCycle_Sent_0() = false;
					FWaitingDutyCycle_Response_1() = false;
				}

				if( FWaitingDutyCycle_Response_2() )
				{
//					FDutyCycle_Sent_0() = false;
					FWaitingDutyCycle_Response_2() = false;
				}
			}

			if( T_Read_IsBusy::Count() )
			{
				bool ARead_IsBusy = false;
				T_Read_IsBusy::Call( ARead_IsBusy );
				if( ARead_IsBusy )
					return;
			}

////			if( FDutyCycle_Sent_0() )
////				return;

//			if( ! AIsBusy )
//				if( millis() - FLastTime < Delay() )
//					return;

			if( FFrequency_Requested() || FDutyCycle_Requested_0() || FDutyCycle_Requested_1() || FDutyCycle_Requested_2() )
			{
				if( FInDelay() )
					if( millis() - FLastTime < Delay() )
						return;

				FInDelay() = false;
				uint8_t AIndex = FSent_Index();
				for( int i = 0; i < 4; ++ i )
				{
                    int ATest1 = AIndex;
					if( AIndex >= 4 )
					{
						if( T_Read_NeedsRequest::Count() )
						{
							bool ANeedsRefresh = false;
							T_Read_NeedsRequest::Call( ANeedsRefresh );
							if( ANeedsRefresh )
								return;

						}

						AIndex = 0;
					}

					switch( AIndex ++ )
					{
						case 0 :
						default :
							if( FFrequency_Requested() )
							{
								FInDown() = false;
								FIndex() = 0;
								FWaitingFrequency_Response() = true;
				//				FFrequency_Requested() = false;
								FSent_Index() = 1;
				//				FFrequency_Sent() = true;
								Mitov::String AFrequency = Mitov::String( FFrequency() );
								C_SERIAL.GetStream().write( 'F' );
								if( FFrequency() <= 999 )
								{
									for( int i = AFrequency.length(); i < 3; ++ i )
										C_SERIAL.GetStream().write( '0' );

									C_SERIAL.GetStream().print( AFrequency );
								}

								else
								{
									C_SERIAL.GetStream().write( AFrequency[ 0 ] );
									if( FFrequency() <= 9999 )
									{
										C_SERIAL.GetStream().write( '.' );
										C_SERIAL.GetStream().write( AFrequency[ 1 ] );
										C_SERIAL.GetStream().print( AFrequency[ 2 ] );
									}

									else if( FFrequency() <= 99999 )
									{
										C_SERIAL.GetStream().write( AFrequency[ 1 ] );
										C_SERIAL.GetStream().write( '.' );
										C_SERIAL.GetStream().print( AFrequency[ 2 ] );
									}

									else
									{
										C_SERIAL.GetStream().write( '.' );
										C_SERIAL.GetStream().write( AFrequency[ 1 ] );
										C_SERIAL.GetStream().write( '.' );
										C_SERIAL.GetStream().print( AFrequency[ 2 ] );
									}
								}

								FLastTime = millis();
								return;
							}

							break;

						case 1 :
							if( FDutyCycle_Requested_0())
							{
								FInDown() = false;
								FIndex() = 0;
								FWaitingDutyCycle_Response_0() = true;
								FSent_Index() = 2;
				//				FFrequencySent() = false;

				//				FDutyCycle_Requested() = false;
								Mitov::String ACycle = Mitov::String( FDutyCycle_0() );
								C_SERIAL.GetStream().print( "D1:" );
								for( int i = ACycle.length(); i < 3; ++ i )
									C_SERIAL.GetStream().write( '0' );

								C_SERIAL.GetStream().print( ACycle );

								FLastTime = millis();
								return;
							}

							break;

						case 2 :
							if( FDutyCycle_Requested_1())
							{
								FInDown() = false;
								FIndex() = 0;
								FWaitingDutyCycle_Response_1() = true;
								FSent_Index() = 3;
				//				FFrequencySent() = false;

				//				FDutyCycle_Requested() = false;
								Mitov::String ACycle = Mitov::String( FDutyCycle_1() );
								C_SERIAL.GetStream().print( "D2:" );
								for( int i = ACycle.length(); i < 3; ++ i )
									C_SERIAL.GetStream().write( '0' );

								C_SERIAL.GetStream().print( ACycle );

								FLastTime = millis();
								return;
							}

							break;

						case 3 :
							if( FDutyCycle_Requested_2())
							{
								FInDown() = false;
								FIndex() = 0;
								FWaitingDutyCycle_Response_2() = true;
								FSent_Index() = 4;
				//				FFrequencySent() = false;

				//				FDutyCycle_Requested() = false;
								Mitov::String ACycle = Mitov::String( FDutyCycle_2() );
								C_SERIAL.GetStream().print( "D3:" );
								for( int i = ACycle.length(); i < 3; ++ i )
									C_SERIAL.GetStream().write( '0' );

								C_SERIAL.GetStream().print( ACycle );

								FLastTime = millis();
								return;
                            }

							break;

		//				case 4 :

					}

				}

            }


/*
//			if( FFrequency_Requested() && ! ( ( FDutyCycle_Requested_0() || FDutyCycle_Requested_1() || FDutyCycle_Requested_2() ) && FFrequency_Sent() ) )
			{
				FInDown() = false;
				FIndex() = 0;
				FWaitingFrequency_Response() = true;
//				FFrequency_Requested() = false;
                FSent_Index() = 1;
//				FFrequency_Sent() = true;
				Mitov::String AFrequency = Mitov::String( FFrequency() );
				C_SERIAL.GetStream().write( 'F' );
				if( FFrequency() <= 999 )
				{
					for( int i = AFrequency.length(); i < 3; ++ i )
						C_SERIAL.GetStream().write( '0' );

					C_SERIAL.GetStream().println( AFrequency );
				}

				else
				{
					C_SERIAL.GetStream().write( AFrequency[ 0 ] );
					if( FFrequency() <= 9999 )
					{
						C_SERIAL.GetStream().write( '.' );
						C_SERIAL.GetStream().write( AFrequency[1] );
						C_SERIAL.GetStream().println( AFrequency[2] );
					}

					else if( FFrequency() <= 99999 )
					{
						C_SERIAL.GetStream().write( AFrequency[1] );
						C_SERIAL.GetStream().write( '.' );
						C_SERIAL.GetStream().println( AFrequency[2] );
					}

					else
					{
						C_SERIAL.GetStream().write( '.' );
						C_SERIAL.GetStream().write( AFrequency[1] );
						C_SERIAL.GetStream().write( '.' );
						C_SERIAL.GetStream().println( AFrequency[2] );
					}
				}

				FLastTime = millis();
			}

			else if( FDutyCycle_Requested_0() )
			{
				FInDown() = false;
				FIndex() = 0;
				FWaitingDutyCycle_Response_0() = true;
//				FFrequencySent() = false;
				if( T_Read_NeedsRequest::Count() )
				{
					bool ANeedsRefresh = false;
					T_Read_NeedsRequest::Call( ANeedsRefresh );
					if( ANeedsRefresh )
						FDutyCycle_Sent_0() = true;
				}

//				FDutyCycle_Requested() = false;
				Mitov::String ACycle = Mitov::String( FDutyCycle_0() );
				C_SERIAL.GetStream().print( "D1:" );
				for( int i = ACycle.length(); i < 3; ++ i )
					C_SERIAL.GetStream().write( '0' );

				C_SERIAL.GetStream().print( ACycle );

				FLastTime = millis();
			}

			else if( FDutyCycle_Requested_1() )
			{
				FInDown() = false;
				FIndex() = 0;
				FWaitingDutyCycle_Response_1() = true;
//				FFrequencySent() = false;
				if( T_Read_NeedsRequest::Count() )
				{
					bool ANeedsRefresh = false;
					T_Read_NeedsRequest::Call( ANeedsRefresh );
					if( ANeedsRefresh )
						FDutyCycle_Sent_1() = true;
				}

//				FDutyCycle_Requested() = false;
				Mitov::String ACycle = Mitov::String( FDutyCycle_1() );
				C_SERIAL.GetStream().print( "D2:" );
				for( int i = ACycle.length(); i < 3; ++ i )
					C_SERIAL.GetStream().write( '0' );

				C_SERIAL.GetStream().print( ACycle );

				FLastTime = millis();
			}

			else if( FDutyCycle_Requested_2() )
			{
				FInDown() = false;
				FIndex() = 0;
				FWaitingDutyCycle_Response_2() = true;
//				FFrequencySent() = false;
				if( T_Read_NeedsRequest::Count() )
				{
					bool ANeedsRefresh = false;
					T_Read_NeedsRequest::Call( ANeedsRefresh );
					if( ANeedsRefresh )
						FDutyCycle_Sent_2() = true;
				}

//				FDutyCycle_Requested() = false;
				Mitov::String ACycle = Mitov::String( FDutyCycle_2() );
				C_SERIAL.GetStream().print( "D3:" );
				for( int i = ACycle.length(); i < 3; ++ i )
					C_SERIAL.GetStream().write( '0' );

				C_SERIAL.GetStream().print( ACycle );

				FLastTime = millis();
			}
*/
		}

	public:
		inline TArduinoPWMSignalGenerator_XY_LPWM3()
		{
			FInDown() = false;
//			FInFall() = false;
			FWaitingDutyCycle_Response_0() = false;
			FWaitingDutyCycle_Response_1() = false;
			FWaitingDutyCycle_Response_2() = false;
			FWaitingFrequency_Response() = false;
			FIndex() = 0;
			FDutyCycle_Requested_0() = false;
			FDutyCycle_Requested_1() = false;
			FDutyCycle_Requested_2() = false;
			FFrequency_Requested() = false;
			FSent_Index() = 0;
		}

	};
//---------------------------------------------------------------------------
}


#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
