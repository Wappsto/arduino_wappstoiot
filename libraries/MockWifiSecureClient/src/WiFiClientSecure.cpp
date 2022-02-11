#include "WiFiClientSecure.h"

#undef connect
#undef write
#undef read

WiFiClientSecure::WiFiClientSecure()
{
    _connected = false;
    this->mockContainer = new JsonMockContainer();
    this->mockContainer->addResponse("{\"jsonrpc\":\"2.0\",\"id\":45299,\"result\":{\"value\":true,\"time\":\"2022-01-25T09:13:01.858212Z\"}}");
}


WiFiClientSecure::WiFiClientSecure(int sock)
{
    (void)sock;
    _connected = false;
}

WiFiClientSecure::~WiFiClientSecure()
{
    stop();
    //delete this->mockContainer;
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
    (void)buf;
    (void)size;
    char *nextResponse = this->mockContainer->getNextResponse();
    if(nextResponse) {
        strcpy((char*)buf, nextResponse);
        Serial.println(nextResponse);
        //strcpy((char*)buf, "{\"jsonrpc\":\"2.0\",\"id\":45299,\"result\":{\"value\":true,\"time\":\"2022-01-25T09:13:01.858212Z\"}}");
        return strlen((const char*)buf);
    } else {
        Serial.println("No response to send in read");
        return -1;
    }
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
    (void)data;
    Serial.println("START socket write: !!!!!!!!!!!!!!!!!");
    Serial.println(data);
    Serial.println("END socket write: !!!!!!!!!!!!!!!!!");
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
    Serial.println("test available");
    return 0;
}

int WiFiClientSecure::peek() {
    return 0;
}
