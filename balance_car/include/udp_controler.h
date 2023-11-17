#ifndef UDP_CONTROLER_H
#define UDP_CONTROLER_H

#include <Arduino.h>
#include <WiFiUdp.h>
#include <WiFi.h>


class udp_controler
{
private:
    String ssid ;
    String password ;
    uint16_t port;
    IPAddress ip_self;
    WiFiUDP udp;
public:
    udp_controler();
    udp_controler(String ssid, String password,uint16_t port, IPAddress ip_self);
    ~udp_controler();
    void init_udp();
    void send_data(String pac, IPAddress ip, int port);
};

udp_controler::udp_controler(String ssid, String password,uint16_t port = 5555, IPAddress ip_self = IPAddress(192,168,4,1))
{
    this->ssid = ssid;
    this->password = password;
    this->port = port;
    this->ip_self = ip_self;
    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP(ssid, password,1,0,1);
    Serial.println("SoftAP Configured");
    WiFi.config(ip_self, ip_self, IPAddress(255,255,255,0));
    delay(25000);
    Serial.println("Connected to the WiFi network");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    init_udp();
}

udp_controler::~udp_controler()
{
}

void udp_controler::init_udp()
{
    udp.begin(ip_self, port);
}

void udp_controler::send_data(String pac, IPAddress ip, int port)
{
    udp.beginPacket(ip, port);
    udp.print(pac);
    udp.endPacket();
}






#endif