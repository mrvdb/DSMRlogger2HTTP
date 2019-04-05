/*
***************************************************************************  
**  Program  : TimeStuff, part of DSMRlogger2HTTP
**  Version  : v0.7.0
**
**  Copyright (c) 2018 Willem Aandewiel
**
**  TERMS OF USE: MIT License. See bottom of file.                                                            
***************************************************************************      
*/

//===========================================================================================
String buildDateTimeString(String timeStamp) {
//===========================================================================================
  String DateTime = "";

  DateTime   = "20" + timeStamp.substring(0, 2);
  DateTime  += "-"  + timeStamp.substring(2, 4);
  DateTime  += "-"  + timeStamp.substring(4, 6);
  DateTime  += " "  + timeStamp.substring(6, 8);
  DateTime  += ":"  + timeStamp.substring(8, 10);
  DateTime  += ":"  + timeStamp.substring(10, 12);
  return DateTime;
    
} // buildDateTimeString()

//===========================================================================================
String getDayName(int weekDayNr) {
//===========================================================================================
  if (weekDayNr >=0 && weekDayNr <= 6)
      return weekDayName[weekDayNr];
      
  return weekDayName[7];    
    
} // getDayName()


//===========================================================================================
int8_t SecondFromTimestamp(String timeStamp) {
//===========================================================================================
  return timeStamp.substring(10, 12).toInt();
    
} // SecondFromTimestamp()

//===========================================================================================
int8_t MinuteFromTimestamp(String timeStamp) {
//===========================================================================================
  return timeStamp.substring(8, 10).toInt();
    
} // MinuteFromTimestamp()

//===========================================================================================
int8_t HourFromTimestamp(String timeStamp) {
//===========================================================================================
  return timeStamp.substring(6, 8).toInt();
    
} // HourFromTimestamp()

//===========================================================================================
int8_t DayFromTimestamp(String timeStamp) {
//===========================================================================================
  return timeStamp.substring(4, 6).toInt();
    
} // DayFromTimestamp()

//===========================================================================================
int8_t MonthFromTimestamp(String timeStamp) {
  return timeStamp.substring(2, 4).toInt();
    
} // MonthFromTimestamp()

//===========================================================================================
int8_t YearFromTimestamp(String timeStamp) {
  return timeStamp.substring(0, 2).toInt();
    
} // YearFromTimestamp()

//===========================================================================================
time_t epoch(String timeStamp) {
//===========================================================================================
/**  
  TelnetStream.printf("DateTime: [%02d]-[%02d]-[%02d] [%02d]:[%02d]:[%02d]\n"
                                                                 ,DayFromTimestamp(timeStamp)
                                                                 ,MonthFromTimestamp(timeStamp)
                                                                 ,YearFromTimestamp(timeStamp)
                                                                 ,HourFromTimestamp(timeStamp)
                                                                 ,MinuteFromTimestamp(timeStamp)
                                                                 ,0
                       );
**/  
  // setTime(HourFromTimestamp(timeStamp)
  //        ,MinuteFromTimestamp(timeStamp)
  //        ,0
  //        ,DayFromTimestamp(timeStamp)
  //        ,MonthFromTimestamp(timeStamp)
  //        ,YearFromTimestamp(timeStamp));

  return now();

} // epoch()

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  TelnetStream.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  TelnetStream.print(ntpServerName);
  TelnetStream.print(": ");
  TelnetStream.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      TelnetStream.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  TelnetStream.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

/***************************************************************************
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the
* following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
* OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
* THE USE OR OTHER DEALINGS IN THE SOFTWARE.
* 
***************************************************************************/
