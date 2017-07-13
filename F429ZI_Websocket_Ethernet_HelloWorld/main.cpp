#if !FEATURE_LWIP
    #error [NOT_SUPPORTED] LWIP not supported for this target
#endif
 
#include "mbed.h"
#include "EthernetInterface.h"
#include "TCPServer.h"
#include "TCPSocket.h"
#include "BMP180_Sensor.h"
 
#define HTTP_STATUS_LINE "HTTP/1.0 200 OK"
#define HTTP_HEADER_FIELDS "Content-Type: text/html; charset=utf-8"
#define HTTP_MESSAGE_BODY ""                                     \
"<html>" "\r\n"                                                  \
"  <body style=\"display:flex;text-align:center\">" "\r\n"       \
"    <div style=\"margin:auto\">" "\r\n"                         \
"      <h1>Hello World</h1>" "\r\n"                              \
"      <p>It works !</p>" "\r\n"                                 \
"    </div>" "\r\n"                                              \
"  </body>" "\r\n"                                               \
"</html>" "\r\n"
    
#define HTTP_RESPONSE HTTP_STATUS_LINE "\r\n"   \
                      HTTP_HEADER_FIELDS "\r\n" \
                      "\r\n"                    \
                      HTTP_MESSAGE_BODY "\r\n"
#define HTTP_TITLE     "<head><title> Plymouth Uni Weather Page </title></head>" "\r\n"
#define HTTP_FORMAT_1 "<body style=\"display:flex;text-align:center\">" "\r\n" \
                      "<div style=\"margin:auto\">" "\r\n"
#define HTTP_BOTTOM "</html>" "\r\n"

#define IP        "192.168.1.2"
#define NETMASK   "255.255.255.0"
#define GATEWAY   "192.168.1.1"


void ReadFlag_ISR(void);
bool ReadFlag=true;

int main()
{
    printf("Basic HTTP server example\r\n");
    //generic error code handle
    int err=0;
  
    //BMP180 Stuff here
    int T_Temp,T_Press, msTempNumber, msTempDecimal, msPressNumber, msPressDecimal;
    char WeatherData[400]={0};
    BMP180_Initialise();
    
    Ticker ticktock;
    ticktock.attach(ReadFlag_ISR,1);
    
    EthernetInterface eth;
    eth.set_network(IP, NETMASK, GATEWAY);
		
    eth.connect();
    printf("The target IP address is '%s'\r\n", eth.get_ip_address());
    
    TCPServer srv;
    TCPSocket clt_sock;
    SocketAddress clt_addr;

    
    /* Open the server on ethernet stack */
    srv.open(&eth);
    
    /* Bind the HTTP port (TCP 80) to the server */
    srv.bind(eth.get_ip_address(), 80);
    
    /* Can handle 5 simultaneous connections */
    srv.listen(5);

    while (true) {

        srv.accept(&clt_sock, &clt_addr);
        printf("accept %s:%d\r\n", clt_addr.get_ip_address(), clt_addr.get_port());

      //clt_sock.send(HTTP_RESPONSE, strlen(HTTP_RESPONSE));
        
        if (ReadFlag==true){
            T_Temp=Temperature();T_Press=Pressure();// Get the temperature and pressure from BMP180_Sensor.c
            msTempNumber=T_Temp/10;                    //convert into two parts by divide and mod X
            msTempDecimal=T_Temp%10;                    //For the purpose of making a char string constructor sprintf see below
            msPressNumber=T_Press/100;                 //convert into two parts by divide and mod X
            msPressDecimal=T_Press%100;                 //For the purpose of making a char string constructor sprintf see below
            sprintf(WeatherData,"\
  \r\n%s\r\n%s\r\n\r\n<html>\n\r%s%s\r\n\
  <h1>Temp:%d.%d&#8451</h1>\r\n<h1>Pressure:%d.%dmBar</h1>\
  \r\n</div>\r\n</body>\r\n</html>\r\n\r\n"\
  ,HTTP_STATUS_LINE, HTTP_HEADER_FIELDS, HTTP_TITLE, HTTP_FORMAT_1\
  , msTempNumber , msTempDecimal, msPressNumber, msPressDecimal);
            ReadFlag=false;
            }
        printf("String Length/Error Number:%d\r\nWeather Data String is:%s\r\n",err,WeatherData);
        err=clt_sock.send(WeatherData, strlen(WeatherData));
        //printf("String Length/Error Number:%d\r\nWeather Data String is:%s\r\n",err,WeatherData);
         }
}
void ReadFlag_ISR(void){
    ReadFlag=true;
    }
