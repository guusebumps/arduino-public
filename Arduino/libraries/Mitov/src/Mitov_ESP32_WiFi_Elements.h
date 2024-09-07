////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2024 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef VISUINO_MAKO
#ifdef VISUINO_ANY_ESP32
  #include <esp_eap_client.h>
#endif
#endif

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

//  #define __ESP32_WIFI__DEBUG__

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Channel,
		typename T_Config,
		typename T_Enabled,
		typename T_MacAddress,
		typename T_Password,
		typename T_SSID
	> class ESP32ModuleRemoteAccessPoint :
		public T_Channel,
		public T_Config,
		public T_Enabled,
		public T_MacAddress,
		public T_Password,
		public T_SSID
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( SSID )
	    _V_PROP_( Password )
		_V_PROP_( Channel )
		_V_PROP_( MacAddress )
		_V_PROP_( Config )

	public:
        inline void UpdateEnabled()
        {
            C_OWNER.WiFi_Disconnect_Update();
        }

	public:
		void Connect()
		{
			if( ! Enabled() )
				return;

			if( SSID().GetValue() == "" )
				return;

			if( Config().Enabled() )
				C_OWNER.WiFi_config( GetIPAddressValue( Config().IP() ), GetIPAddressValue( Config().Gateway() ), GetIPAddressValue( Config().Subnet() ), GetIPAddressValue( Config().DNS1() ), GetIPAddressValue( Config().DNS2() ) );

//			Serial.println( SSID );
//			Serial.println( APassword );

			Begin();

//			if( C_OWNER.HostName != "" )
//				WiFi.setHostname( (char *)C_OWNER.HostName.c_str() );

		}

		void Begin()
		{
//		    Serial.println( "TEST1" );
			const char *APassword;
			if( Password().GetValue() == "" )
				APassword = nullptr;

			else
				APassword = Password().c_str();

			int32_t channel;

			if( Channel().Enabled() )
				channel = Channel().Channel();

			else
				channel = 0;

			uint8_t AMacAddress[ 6 ];
			const uint8_t* bssid;

			if( MacAddress().Enabled() )
			{
				MacAddress().MacAddress().GetMacAddress( AMacAddress );
				bssid = AMacAddress;
			}

			else
				bssid = nullptr;

#ifdef __ESP32_WIFI__DEBUG__
			Serial.print( "WiFi.begin " );
			Serial.print( SSID().c_str() );
			Serial.print( " : " );
			Serial.println( APassword );

#endif // __ESP32_WIFI__DEBUG__

			C_OWNER.WiFi_disconnect();
			C_OWNER.WiFi_begin( (char *)SSID().c_str(), APassword, channel, bssid, false );

			if( C_OWNER.HostName().GetValue() != "" )
				C_OWNER.WiFi_setHostname( (char *)C_OWNER.HostName().c_str() );

			C_OWNER.WiFi_reconnect();
		}

	public:
		inline void IsEnabled( bool &AIsEnabled )
		{
			if( ( ! Enabled() ) || ( SSID().GetValue() == "" ))
				return;

//			Serial.println( "IsEnabled" );

			AIsEnabled = true;
		}

		inline void TryConnect( bool &AIsEnabled )
		{
			if( ( ! Enabled() ) || ( SSID().GetValue() == "" ))
				return;

//			Serial.println( "TryConnect" );
			AIsEnabled = true;
			Connect();
		}

		inline void AssignDNS()
		{
//				Serial.println( Config.DNS.IP.toString() );
			if( Config().DNS1().GetUInt32Value() == 0 )
			{
				IPAddress AIP = C_OWNER.WiFi_gatewayIP();
				C_OWNER.WiFi_disconnect();

//					delay( 10000 );

//					Serial.println( AIP.toString() );
				if( Config().Enabled() )
					C_OWNER.WiFi_config( GetIPAddressValue( Config().IP() ), GetIPAddressValue( Config().Gateway() ), GetIPAddressValue( Config().Subnet() ), AIP, GetIPAddressValue( Config().DNS2() ) );

				else
					C_OWNER.WiFi_config( ::IPAddress( 0, 0, 0, 0 ), ::IPAddress( 0, 0, 0, 0 ), ::IPAddress( 255, 255, 255, 0 ), AIP, ::IPAddress( 0, 0, 0, 0 ) );

/*
				ip_addr_t d;
				d.type = IPADDR_TYPE_V4;

				// Set DNS1-Server
				d.u_addr.ip4.addr = static_cast<uint32_t>( AIP );

				dns_setserver( 0, &d );
*/
				Begin();
//					WiFi.reconnect();
//					if(esp_wifi_disconnect() == ESP_OK)
//						esp_wifi_connect();
			}
		}

	};
//---------------------------------------------------------------------------
    namespace TArduinoESP32WiFiModuleRemoteEnterpriseWPA2AuthenticationMethod
    {
        enum TArduinoESP32WiFiModuleRemoteEnterpriseWPA2AuthenticationMethod
        {
            TLS = 0,
            PEAP = 1,
            TTLS = 2
        };
    };
//---------------------------------------------------------------------------
#ifdef VISUINO_ANY_ESP32
  #ifndef VISUINO_MAKO
	namespace TArduinoESP32WiFiModuleRemoteEnterpriseWPA2AuthenticationPhase2
	{
        enum TArduinoESP32WiFiModuleRemoteEnterpriseWPA2AuthenticationPhase2
        {
			None = -1,
			EAP = ESP_EAP_TTLS_PHASE2_EAP,				// EAP (Extensible Authentication Protocol)
			MSCHAPv2 = ESP_EAP_TTLS_PHASE2_MSCHAPV2,	// MS-CHAPv2 (Microsoft Challenge Handshake Authentication Protocol - Version 2)
			MSCHAP = ESP_EAP_TTLS_PHASE2_MSCHAP,		// MS-CHAP (Microsoft Challenge Handshake Authentication Protocol)
			PAP = ESP_EAP_TTLS_PHASE2_PAP,				// PAP (Password Authentication Protocol)
			CHAP = ESP_EAP_TTLS_PHASE2_CHAP				// CHAP (Challenge Handshake Authentication Protocol)
		};
	};
  #endif // VISUINO_MAKO
#endif // VISUINO_ANY_ESP32
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Authentication,
		typename T_CACert,
		typename T_Channel,
		typename T_ClientCert,
		typename T_ClientKey,
		typename T_Config,
		typename T_Enabled,
		typename T_Identity,
		typename T_MacAddress,
		typename T_Password,
#ifndef VISUINO_MAKO
		typename T_Phase2,
#endif
		typename T_SSID,
		typename T_UserName
	> class ESP32ModuleEnterpriseWPA2RemoteAccessPoint :
		public T_Authentication,
		public T_CACert,
		public T_Channel,
		public T_ClientCert,
		public T_ClientKey,
		public T_Config,
		public T_Enabled,
		public T_Identity,
		public T_MacAddress,
		public T_Password,
		public T_SSID,
#ifndef VISUINO_MAKO
		public T_Phase2,
#endif
		public T_UserName
	{
	public:
		_V_PROP_( Authentication )
		_V_PROP_( CACert )
		_V_PROP_( Channel )
		_V_PROP_( ClientCert )
		_V_PROP_( ClientKey )
#ifndef VISUINO_MAKO
		_V_PROP_( Phase2 )
#endif
		_V_PROP_( SSID )
	    _V_PROP_( Password )
		_V_PROP_( Enabled )
		_V_PROP_( Identity )
		_V_PROP_( MacAddress )
		_V_PROP_( Config )
		_V_PROP_( UserName )

	public:
        inline void UpdateEnabled()
        {
            C_OWNER.WiFi_Disconnect_Update();
        }

	protected:
		inline const char *GetStringPointer( const ::Mitov::String &AValue )
		{
			if( AValue == "" )
				return nullptr;

			return AValue.c_str();
		}

	public:
		void Connect()
		{
			if( ! Enabled() )
				return;

			if( SSID().GetValue() == "" )
				return;

			if( Identity().GetValue() == "" )
				return;

			if( Authentication().GetValue() == TArduinoESP32WiFiModuleRemoteEnterpriseWPA2AuthenticationMethod::PEAP || Authentication().GetValue() == TArduinoESP32WiFiModuleRemoteEnterpriseWPA2AuthenticationMethod::TTLS )
				if( UserName().GetValue() == "" )
					return;

			if( Config().Enabled() )
				C_OWNER.WiFi_config( GetIPAddressValue( Config().IP() ), GetIPAddressValue( Config().Gateway() ), GetIPAddressValue( Config().Subnet() ), GetIPAddressValue( Config().DNS1() ), GetIPAddressValue( Config().DNS2() ) );

//			Serial.println( SSID().GetValue() );
//			Serial.println( Password().GetValue() );

			Begin();

//			if( C_OWNER.HostName != "" )
//				WiFi.setHostname( (char *)C_OWNER.HostName.c_str() );

		}

		void Begin()
		{
//		    Serial.println( "TEST1" );
//			const char *APassword = GetStringPointer( Password() );
//			const char *AIdentity = GetStringPointer( Identity() );
//			const char *ACa_pem = GetStringPointer( CACert() );
//			const char *AClient_crt = GetStringPointer( ClientCert() );
//			const char *AClient_key = GetStringPointer( ClientKey() );
//			const char *AUserName = GetStringPointer( UserName() );

			int32_t channel;

			if( Channel().Enabled() )
				channel = Channel().Channel();

			else
				channel = 0;

			uint8_t AMacAddress[ 6 ];
			const uint8_t* bssid;

//		    Serial.println( "TEST2" );
			if( MacAddress().Enabled() )
			{
				MacAddress().MacAddress().GetMacAddress( AMacAddress );
				bssid = AMacAddress;
			}

			else
				bssid = nullptr;

#ifdef __ESP32_WIFI__DEBUG__
			Serial.print( "WiFi.begin " );
			Serial.print( SSID().c_str() );
			Serial.print( " : " );
			Serial.println( Password().c_str() );

#endif // __ESP32_WIFI__DEBUG__

			C_OWNER.WiFi_disconnect();
//			Serial.println( "TEST3_12" );
//		inline void WiFi_begin_WPA2(const char* ssid, ::Mitov::TArduinoESP32WiFiModuleRemoteEnterpriseWPA2AuthenticationMethod::TArduinoESP32WiFiModuleRemoteEnterpriseWPA2AuthenticationMethod AMethod,
			//const char* identity, const char* username, const char *password, const char* ca_pem, const char* client_crt, const char* client_key, int32_t channel, const uint8_t* bssid, bool connect )
			C_OWNER.WiFi_begin_WPA2( (char *)SSID().c_str(), Authentication(), GetStringPointer( Identity() ), GetStringPointer( UserName() ), GetStringPointer( Password() ), GetStringPointer( CACert() ), GetStringPointer( ClientCert() ), GetStringPointer( ClientKey() ), 
#if defined( VISUINO_MAKO ) || ( ! defined( VISUINO_ANY_ESP32 ))
            -1,
#else // VISUINO_MAKO
            int( Phase2() ),
#endif // VISUINO_MAKO
            channel, bssid, false );

//			Serial.println( "TEST3" );

			if( C_OWNER.HostName().GetValue() != "" )
				C_OWNER.WiFi_setHostname( (char *)C_OWNER.HostName().c_str() );

//			Serial.println( "TEST4" );

			C_OWNER.WiFi_reconnect();
//			Serial.println( "TEST5" );
		}

	public:
		inline void IsEnabled( bool &AIsEnabled )
		{
			if( ( ! Enabled() ) || ( SSID().GetValue() == "" ))
				return;

//			Serial.println( "IsEnabled" );

			AIsEnabled = true;
		}

		inline void TryConnect( bool &AIsEnabled )
		{
			if( ( ! Enabled() ) || ( SSID().GetValue() == "" ))
				return;

//			Serial.println( "TryConnect" );
			AIsEnabled = true;
			Connect();
		}

		inline void AssignDNS()
		{
//				Serial.println( Config.DNS.IP.toString() );
			if( Config().DNS1().GetUInt32Value() == 0 )
			{
				IPAddress AIP = C_OWNER.WiFi_gatewayIP();
				C_OWNER.WiFi_disconnect();

//					delay( 10000 );

//					Serial.println( AIP.toString() );
				if( Config().Enabled() )
					C_OWNER.WiFi_config( GetIPAddressValue( Config().IP() ), GetIPAddressValue( Config().Gateway() ), GetIPAddressValue( Config().Subnet() ), AIP, GetIPAddressValue( Config().DNS2() ) );

				else
					C_OWNER.WiFi_config( ::IPAddress( 0, 0, 0, 0 ), ::IPAddress( 0, 0, 0, 0 ), ::IPAddress( 255, 255, 255, 0 ), AIP, ::IPAddress( 0, 0, 0, 0 ) );

/*
				ip_addr_t d;
				d.type = IPADDR_TYPE_V4;

				// Set DNS1-Server
				d.u_addr.ip4.addr = static_cast<uint32_t>( AIP );

				dns_setserver( 0, &d );
*/
				Begin();
//					WiFi.reconnect();
//					if(esp_wifi_disconnect() == ESP_OK)
//						esp_wifi_connect();
			}
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

