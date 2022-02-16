/*
  WiFiClientSecure.h - Base class that provides Client SSL to ESP32
  Copyright (c) 2011 Adrian McEwen.  All right reserved.
  Additions Copyright (C) 2017 Evandro Luis Copercini.
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef WiFiClientSecure_h
#define WiFiClientSecure_h
#include "Arduino.h"
#include "IPAddress.h"
#include "Stream.h"
#include "Print.h"
//#include <WiFi.h>
//#include "ssl_client.h"

#include "JsonMockContainer.h"

class WiFiClientSecure //: public Print//Stream//: public WiFiClient
{
protected:
    //sslclient_context *sslclient;

    int _lastError = 0;
    int _peek = -1;
    int _timeout = 0;
    bool _use_insecure;
    bool _connected = false;

public:
    WiFiClientSecure();
    WiFiClientSecure(int socket);
    ~WiFiClientSecure();
    int connect(IPAddress ip, uint16_t port);
    int connect(IPAddress ip, uint16_t port, int32_t timeout);
    int connect(const char *host, uint16_t port);
    int connect(const char *host, uint16_t port, int32_t timeout);
    int connect(IPAddress ip, uint16_t port, const char *rootCABuff, const char *cli_cert, const char *cli_key);
    int connect(const char *host, uint16_t port, const char *rootCABuff, const char *cli_cert, const char *cli_key);
    int connect(IPAddress ip, uint16_t port, const char *pskIdent, const char *psKey);
    int connect(const char *host, uint16_t port, const char *pskIdent, const char *psKey);
    int peek();
    size_t write(uint8_t data);
    size_t write(const uint8_t *buf, size_t size);
    size_t print(const char* data);
    int available();
    int read();
    int read(uint8_t *buf, size_t size);
    void flush() {}
    void stop();
    uint8_t connected();
    int lastError(char *buf, const size_t size);
    void setInsecure(); // Don't validate the chain, just accept whatever is given.  VERY INSECURE!
    void setPreSharedKey(const char *pskIdent, const char *psKey); // psKey in Hex
    void setCACert(const char *rootCA);
    void setCertificate(const char *client_ca);
    void setPrivateKey (const char *private_key);
    bool loadCACert(Stream& stream, size_t size);
    void setCACertBundle(const uint8_t * bundle);
    bool loadCertificate(Stream& stream, size_t size);
    bool loadPrivateKey(Stream& stream, size_t size);
    bool verify(const char* fingerprint, const char* domain_name);
    void setHandshakeTimeout(unsigned long handshake_timeout);
    void setAlpnProtocols(const char **alpn_protos);
    //const mbedtls_x509_crt* getPeerCertificate() { return mbedtls_ssl_get_peer_cert(&sslclient->ssl_ctx); };
    //bool getFingerprintSHA256(uint8_t sha256_result[32]) { return get_peer_fingerprint(sslclient, sha256_result); };
    int setTimeout(uint32_t seconds);

    operator bool()
    {
        return connected();
    }
    WiFiClientSecure &operator=(const WiFiClientSecure &other);
    bool operator==(const bool value)
    {
        return bool() == value;
    }
    bool operator!=(const bool value)
    {
        return bool() != value;
    }
    bool operator==(const WiFiClientSecure &);
    bool operator!=(const WiFiClientSecure &rhs)
    {
        return !this->operator==(rhs);
    };

    int socket()
    {
        return -1;//return sslclient->socket = -1;
    }

    void addDeviceUuid(const char* uuid);
    void addValueUuid(const char* uuid);
    void addReportUuid(const char* uuid, const char* data);
    void addControlUuid(const char* uuid, const char* data);
    void testRefresh(const char* uuid, const char* url);
    void testControl(const char* uuid, const char* url, const char* data);


private:
    char *_streamLoad(Stream& stream, size_t size);
    JsonMockContainer *mockContainer;
    char _nextReply[2500];

};

#endif /* _WIFICLIENT_H_ */
