#include "WiFiClientSecure.h"

#undef connect
#undef write
#undef read

#define REPLY_MAX_SIZE 2500

// EpoxyDuino/cores/epoxy/Print.cpp change #define PRINTF_BUFFER_SIZE 250 to higher number for full debug
#define WIFI_PRINTF(...) Serial.print("\e[1;35m[WIFI] "); Serial.printf(__VA_ARGS__); Serial.print("\e[1;37m");
//#define WIFI_PRINTF(...)

WiFiClientSecure::WiFiClientSecure()
{
    _connected = false;
    this->mockContainer = new JsonMockContainer();
    memset(this->_nextReply, 0x00, REPLY_MAX_SIZE);
}

void WiFiClientSecure::addDeviceUuid(const char* uuid)
{
    this->mockContainer->addDeviceUuid(uuid);
}

void WiFiClientSecure::addValueUuid(const char* uuid)
{
    this->mockContainer->addValueUuid(uuid);
}

void WiFiClientSecure::addReportUuid(const char* uuid, const char* data)
{
    this->mockContainer->addStateUuid(uuid, data, true);
}

void WiFiClientSecure::addControlUuid(const char* uuid, const char* data)
{
    this->mockContainer->addStateUuid(uuid, data, false);
}

void WiFiClientSecure::testRefresh(const char* uuid, const char* url)
{
    this->mockContainer->testRefresh(uuid, url, this->_nextReply);
}

void WiFiClientSecure::testControl(const char* uuid, const char* url, const char* data)
{
    this->mockContainer->testControl(uuid, url, data, this->_nextReply);
}

WiFiClientSecure::WiFiClientSecure(int sock)
{
    (void)sock;
    _connected = false;
}

WiFiClientSecure::~WiFiClientSecure()
{
    stop();
}

int WiFiClientSecure::connect(IPAddress ip, uint16_t port)
{
    (void)ip;
    (void)port;
    _connected = true;
    return 1;
}

int WiFiClientSecure::connect(IPAddress ip, uint16_t port, int32_t timeout){
    (void)ip;
    (void)port;
    (void)timeout;
    _connected = true;
    return 1;
}

int WiFiClientSecure::connect(const char *host, uint16_t port)
{
    (void)host;
    (void)port;
    _connected = true;
    return 1;
}

int WiFiClientSecure::connect(const char *host, uint16_t port, int32_t timeout){
    (void)host;
    (void)port;
    (void)timeout;
    _connected = true;
    return 1;
}

int WiFiClientSecure::connect(IPAddress ip, uint16_t port, const char *CA_cert, const char *cert, const char *private_key)
{
    (void)ip;
    (void)port;
    (void)CA_cert;
    (void)cert;
    (void)private_key;
    _connected = true;
    return 1;
}

int WiFiClientSecure::connect(const char *host, uint16_t port, const char *CA_cert, const char *cert, const char *private_key)
{
    (void)host;
    (void)port;
    (void)CA_cert;
    (void)cert;
    (void)private_key;
    _connected = true;
    return 1;
}

int WiFiClientSecure::connect(IPAddress ip, uint16_t port, const char *pskIdent, const char *psKey) {
    (void)ip;
    (void)port;
    (void)pskIdent;
    (void)psKey;
    _connected = true;
    return 1;
}

int WiFiClientSecure::connect(const char *host, uint16_t port, const char *pskIdent, const char *psKey) {
    (void)host;
    (void)port;
    (void)pskIdent;
    (void)psKey;
    _connected = true;
    return 1;
}

uint8_t WiFiClientSecure::connected()
{
    return _connected;
}

void WiFiClientSecure::stop()
{
    _connected = false;
}

int WiFiClientSecure::read()
{
    uint8_t data = -1;
    return data;
}

int WiFiClientSecure::read(uint8_t *buf, size_t size)
{
    (void)size;
    if(strlen(this->_nextReply) > 0) {
        strcpy((char*)buf, this->_nextReply);
        memset(this->_nextReply, 0x00, REPLY_MAX_SIZE);
        //WIFI_PRINTF("_nextReply: %s\n", buf);
        return strlen((const char*)buf);
    }
    return -1;
}

size_t WiFiClientSecure::write(unsigned char) {
    return 1;
}

size_t WiFiClientSecure::write(const uint8_t *buf, size_t size)
{
    (void)buf;
    (void)size;
    if (!_connected) {
        return 0;
    }
    return size;
}

size_t WiFiClientSecure::print(const char* data)
{
    this->mockContainer->receiveData(data, this->_nextReply);
    WIFI_PRINTF("WRITE print: next: %s\n", this->_nextReply);
    return 0;
}

int WiFiClientSecure::setTimeout(uint32_t seconds){
    (void)seconds;
    return 0;
}

void WiFiClientSecure::setCACert(const char *rootCA) {
    (void)rootCA;
}

void WiFiClientSecure::setCertificate(const char *client_ca) {
    (void)client_ca;
}

void WiFiClientSecure::setPrivateKey (const char *private_key) {
    (void)private_key;
}

int WiFiClientSecure::available() {
    WIFI_PRINTF("available [%d]\n", strlen(this->_nextReply));
    return strlen(this->_nextReply);
}

int WiFiClientSecure::peek() {
    return 0;
}
