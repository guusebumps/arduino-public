////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2024 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <WiFi.h>
#include <Mitov.h>
#include <Mitov_BasicEthernet.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Channel,
//		typename T_Config,
		typename T_Enabled,
		typename T_Password,
		typename T_SSID
	> class TArduino_UNO_R4_WiFi_AccessPoint :
		public T_Channel,
//		public T_Config,
		public T_Enabled,
		public T_Password,
		public T_SSID
	{
	public:
		_V_PROP_( Channel )
//		_V_PROP_( Config )
		_V_PROP_( Enabled )
		_V_PROP_( Password )
		_V_PROP_( SSID )

	public:
		inline void TryStart()
		{
//			if( AOwner->Enabled )
				if( Enabled() )
					if( SSID().GetValue() != "" )
					{
//	Serial.println( "TEST1" );
	//Serial.println( SSID );
	//Serial.println( IsHidden );
//						if( Config().Enabled() )
//							WiFi.softAPConfig( GetIPAddressValue( Config().IP() ), GetIPAddressValue( Config().Gateway() ), GetIPAddressValue( Config().Subnet() ) );

#ifdef VISUINO_ARDUINO_PORTENTA
						if( Password().GetValue().length() >= 8 )
						{
							char *ASSID = new char[ SSID().GetValue().length() + 1 ];
							memcpy( ASSID, SSID().c_str(), SSID().GetValue().length() + 1 );
	//						WiFi.beginAP( ASSID, APassword );

//							if( Password().GetValue() != "" )
							{
								char *APassword = new char[ Password().GetValue().length() + 1 ];
								memcpy( APassword, Password().c_str(), Password().GetValue().length() + 1 );
								WiFi.beginAP( ASSID, APassword, Channel() );
								delete [] APassword;
							}

//							else
//								WiFi.beginAP( ASSID, nullptr, Channel() );

							delete [] ASSID;
						}

#else
						if( Password().GetValue() != "" )
							WiFi.beginAP( SSID().c_str(), Password().c_str(), Channel() );

						else
							WiFi.beginAP( SSID().c_str(), nullptr, Channel() );
#endif


//	Serial.println( "TEST2" );
//	delay( 1000 );
//	Serial.println( "TEST3" );
//						if( AOwner->HostName != "" )
//							WiFi.setHostname( (char *)AOwner->HostName.c_str() );

//						WiFi.mode( WIFI_AP );
//						WiFi.softAPIP();
					}

//	Serial.println( "TEST4" );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_AccessPoint,
		typename T_AccessPoints_AssignDNS,
		typename T_AccessPoints_IsEnabled,
		typename T_AccessPoints_TryConnect,
		typename T_AddressOutputPin,
		typename T_BSSIDOutputPin,
		uint8_t C_COUNT_AccessPoints,
		uint8_t COUNT_Sockets,
//		typename T_Config,
//		typename T_DNSServerOutputPin,
		typename T_Enabled,
		typename T_FAccessPointIndex,
		typename T_FDNSUpdated,
		typename T_FIsStarted,
		typename T_FRemoteStarted,
		typename T_FStatusChanged,
		typename T_FirmwareVersionOutputPin,
		typename T_GatewayIPOutputPin,
		typename T_HostName,
		typename T_MACOutputPin,
		typename T_RemoteConnectedOutputPin,
		typename T_Sockets_StopSocket,
		typename T_SubnetMaskIPOutputPin
	> class TArduino_UNO_R4_WiFi :
		public T_AccessPoint,
		public T_AddressOutputPin,
		public T_BSSIDOutputPin,
//		public T_Config,
//		public T_DNSServerOutputPin,
		public T_Enabled,
		public T_FAccessPointIndex,
		public T_FDNSUpdated,
		public T_FIsStarted,
		public T_FRemoteStarted,
		public T_FStatusChanged,
		public T_FirmwareVersionOutputPin,
		public T_GatewayIPOutputPin,
		public T_HostName,
		public T_MACOutputPin,
		public T_RemoteConnectedOutputPin,
		public T_SubnetMaskIPOutputPin
	{
	public:
		_V_PIN_( AddressOutputPin )
		_V_PIN_( BSSIDOutputPin )
//		_V_PIN_( DNSServerOutputPin )
		_V_PIN_( FirmwareVersionOutputPin )
		_V_PIN_( GatewayIPOutputPin )
		_V_PIN_( MACOutputPin )
		_V_PIN_( RemoteConnectedOutputPin )
		_V_PIN_( SubnetMaskIPOutputPin )

	public:
		_V_PROP_( AccessPoint )
//		_V_PROP_( Config )
		_V_PROP_( Enabled )
		_V_PROP_( HostName )

	protected:
		_V_PROP_( FAccessPointIndex )
		_V_PROP_( FDNSUpdated )
		_V_PROP_( FIsStarted )
		_V_PROP_( FRemoteStarted )
		_V_PROP_( FStatusChanged )

	protected:
		unsigned long	FLastReconnectTime;

	public:
		inline bool GetIsStarted() { return FIsStarted(); }

        void WiFi_Disconnect_Update()
        {
            StopEthernet();
        }

		bool GetIPFromHostName( Mitov::String AHostName, ::IPAddress &AAdress )
		{
//			Serial.println( "GetIPFromHostName" );
//			Serial.println( AHostName );
			bool AResult = ( WiFi.hostByName( AHostName.c_str(), AAdress ) == 1 );
			if( ! AResult )
				AAdress = INADDR_NONE;

//			Serial.println( AAdress.toString());
			return AResult;
		}

	public:
		inline void UpdateEnabled()
		{
			if( Enabled() )
				StartEthernet();

			else
				StopEthernet();

		}

	protected:
		void UpdateAccessPoint()
		{
			if( AccessPoint().Enabled() )
			{
				if( Enabled() )
					AccessPoint().TryStart();
			}

			else
				WiFi.disconnect();
//				WiFi.softAPdisconnect();

		}

        void RestartEthernet()
		{
			if( ! Enabled() )
				return;

//			C_SERIAL.end();
//			Ethernet.end();
			StartEthernet();
		}

		void StartEthernet()
		{
//WiFi.beginAP( "Test1211", "try1" );
//return;

			if( HostName().GetValue() != "" )
				WiFi.setHostname( (char *)HostName().c_str() );

//			WiFi.begin();

			WiFi.disconnect();

			if( Enabled() )
				AccessPoint().TryStart();

			if( T_MACOutputPin::GetPinIsConnected() )
			{
				uint8_t mac[6];
				WiFi.macAddress(mac);
				T_MACOutputPin::SetPinValue( MACAdressToString( mac ) );
			}

			if( T_BSSIDOutputPin::GetPinIsConnected() )
			{
				uint8_t mac[6];
				WiFi.BSSID( mac );
				T_BSSIDOutputPin::SetPinValue( MACAdressToString( mac ) );
			}

			if( T_GatewayIPOutputPin::GetPinIsConnected() )
				T_GatewayIPOutputPin::SetPinValue( IPAdressToString( WiFi.gatewayIP() ).c_str() );

			if( T_SubnetMaskIPOutputPin::GetPinIsConnected() )
				T_SubnetMaskIPOutputPin::SetPinValue( IPAdressToString( WiFi.subnetMask() ).c_str() );

		}

		void StopEthernet()
		{
//			T_Sockets_StopSocket::Call();

//			for( int i = 0; i < C_SOCKETS_COUNT; ++i )
//				Sockets[ i ]->StopSocket();

#ifdef __MURATA_1DX_WIFI__DEBUG__
			Serial.println( "StopEthernet" );
#endif
			WiFi.disconnect();
			WiFi.end();
			FIsStarted() = false;
			FRemoteStarted() = false;
//			FDNSUpdated() = false;
		}

	public:
		inline void SystemStart()
		{
			FLastReconnectTime = millis() - 30000;

			if( T_FirmwareVersionOutputPin::GetPinIsConnected() )
				T_FirmwareVersionOutputPin::SetPinValue( WiFi.firmwareVersion() );

			if( Enabled() )
				StartEthernet();

//			Serial.print( "Test11" );
		}

		inline void SystemLoopBegin()
		{
//			Serial.print( "Test12" );

//return;

			int ARes = WiFi.status();

//			Serial.print( "STATUS: " );
//			Serial.println( ARes );

			bool AIsEnabled = false; //( C_COUNT_AccessPoints == 0 );

//			if( ! WiFi.isConnected() )
            if( ARes != WL_CONNECTED )
			{
				if( FRemoteStarted() )
				{
					FLastReconnectTime = millis();
					FRemoteStarted() = false;
					FStatusChanged() = true;
				}

#ifdef __MURATA_1DX_WIFI__DEBUG__
//				Serial.println( "! WiFi.isConnected()" );
//				delay( 100 );
#endif
//				Serial.println( "! WiFi.isConnected()" );
//				FIsStarted() = false;
//				FDNSUpdated() = false;
			}

			if( C_COUNT_AccessPoints > 0 )
				if( ! FRemoteStarted() )
				{
//					Serial.println( "TRY CONNECT" );

					if( ( ARes != WL_CONNECTED ) ) //&& ( ARes != WL_DISCONNECTED ) )
					{
						unsigned long ATime = millis();
						if( ( ATime - FLastReconnectTime ) > 30000 )
						{
#ifdef __MURATA_1DX_WIFI__DEBUG__
							Serial.print( "STATUS: " );
							Serial.println( ARes );
							Serial.println( FAccessPointIndex().GetValue() );
#endif // __MURATA_1DX_WIFI__DEBUG__
							FAccessPointIndex() = FAccessPointIndex() + 1;
							if( FAccessPointIndex().GetValue() > C_COUNT_AccessPoints )
								FAccessPointIndex() = 1;

							T_AccessPoints_TryConnect::Call( FAccessPointIndex().GetValue() - 1, AIsEnabled );
							FLastReconnectTime = millis();
							FDNSUpdated() = false;
						}
					}

					else
						T_AccessPoints_IsEnabled::Call( FAccessPointIndex().GetValue() - 1, AIsEnabled );

				}

//			T_RemoteConnectedOutputPin::SetPinValue( WiFi.isConnected(), true );
            T_RemoteConnectedOutputPin::SetPinValue( ARes == WiFi.status(), true );

//			Serial.println( AIsEnabled );

			if( ! AIsEnabled )
				FIsStarted() = true;

//			else if( WiFi.isConnected() )
			else if( ARes == WiFi.status() )
				if( ! FRemoteStarted().GetValue() )
				{
					if( ! FDNSUpdated().GetValue() )
					{
#ifdef __MURATA_1DX_WIFI__DEBUG__
						Serial.println( "DNS Updated" );
#endif // __MURATA_1DX_WIFI__DEBUG__
						FDNSUpdated() = true;
//							bool AIsEnabled = false;
						T_AccessPoints_AssignDNS::Call( FAccessPointIndex().GetValue() - 1 );

//						Serial.println( "DNS Updated 1" );
						return;
					}

					else
					{
#ifdef __MURATA_1DX_WIFI__DEBUG__
						Serial.println( "FRemoteStarted() = true" );
#endif // __MURATA_1DX_WIFI__DEBUG__
						FIsStarted() = true;
						FRemoteStarted() = true;
						FStatusChanged() = true;
					}


					if( HostName() != "" )
					{
						WiFi.setHostname( (char *)HostName().c_str() );
//						WiFi.disconnect();
//						WiFi.reconnect();
//						ESP.restart();
//						Serial.println( HostName );
					}

					if( FStatusChanged() )
					{
						if( T_AddressOutputPin::GetPinIsConnected() )
							T_AddressOutputPin::SetPinValue( IPAdressToString( WiFi.localIP()).c_str() );

						if( T_MACOutputPin::GetPinIsConnected() )
						{
							uint8_t mac[6];
							WiFi.macAddress(mac);
							T_MACOutputPin::SetPinValue( MACAdressToString( mac ) );
						}

						if( T_BSSIDOutputPin::GetPinIsConnected() )
						{
							uint8_t mac[6];
							WiFi.BSSID( mac );
							T_BSSIDOutputPin::SetPinValue( MACAdressToString( mac ) );
						}

						if( T_GatewayIPOutputPin::GetPinIsConnected() )
							T_GatewayIPOutputPin::SetPinValue( IPAdressToString( WiFi.gatewayIP() ).c_str() );

						if( T_SubnetMaskIPOutputPin::GetPinIsConnected() )
							T_SubnetMaskIPOutputPin::SetPinValue( IPAdressToString( WiFi.subnetMask() ).c_str() );

					}
				}

		}

	public:
		inline TArduino_UNO_R4_WiFi()
		{
			FAccessPointIndex() = C_COUNT_AccessPoints;
//			FIsConnected() = false;
			FStatusChanged() = true;
			FIsStarted() = false;
			FRemoteStarted() = false;
			FDNSUpdated() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
        typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ChannelOutputPin,
		typename T_CountOutputPin,
//		typename T_FScanRequest,
		typename T_Enabled,
		typename T_EncryptionOutputPin,
		typename T_FoundNetworkOutputPin,
		typename T_MACAddressOutputPin,
		typename T_SSIDOutputPin,
		typename T_ScanningOutputPin,
		typename T_SignalStrengthOutputPin
	> class TArduino_UNO_R4_WiFiModuleScanNetworksOperation :
		public T_ChannelOutputPin,
		public T_CountOutputPin,
//		public T_FScanRequest,
		public T_Enabled,
		public T_EncryptionOutputPin,
		public T_FoundNetworkOutputPin,
		public T_MACAddressOutputPin,
		public T_SSIDOutputPin,
		public T_ScanningOutputPin,
		public T_SignalStrengthOutputPin
	{
	public:
		_V_PIN_( ChannelOutputPin )
		_V_PIN_( CountOutputPin )
		_V_PIN_( EncryptionOutputPin )
		_V_PIN_( FoundNetworkOutputPin )
		_V_PIN_( MACAddressOutputPin )
		_V_PIN_( ScanningOutputPin )
		_V_PIN_( SignalStrengthOutputPin )

	public:
		_V_PROP_( Enabled )

//	protected:
//		_V_PROP_( FScanRequest )

	public:
		void ScanInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled().GetValue() )
				return;

			if( ! C_OWNER.Enabled().GetValue() )
				return;

			T_ScanningOutputPin::SetPinValueHigh();

			int8_t ACount = WiFi.scanNetworks();
			for( int i = 0; i < ACount; i ++ )
			{
				T_SSIDOutputPin::SetPinValue( WiFi.SSID( i ));

				T_ChannelOutputPin::SetPinValue( WiFi.channel( i ));

				int32_t ASetrength = WiFi.RSSI( i );
				T_SignalStrengthOutputPin::SetPinValue( ASetrength );

				uint8_t bssid[ 6 ];

////				const uint8_t *bssid = WiFi.ap_list[ i ].get_bssid();

				WiFi.BSSID( i, bssid );

				T_MACAddressOutputPin::SetPinValue( MACAdressToString( bssid ) );

				T_EncryptionOutputPin::SetPinValue( WiFi.encryptionType( i ) );

				T_FoundNetworkOutputPin::ClockPin();
			}

			T_CountOutputPin::SetPinValue( ACount );

			T_ScanningOutputPin::SetPinValueLow();
		}

	public:
		inline void SystemStart()
		{
			ScanInputPin_o_Receive( nullptr );
		}

		inline void SystemLoopBegin()
		{
		}

	};
//---------------------------------------------------------------------------
} // Mitov

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
