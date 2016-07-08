The idea behind Current Logger is to have a board which helps in communication with OR-WE-504. OR-WE-504 is a current meter (created by Orno) which can be commucated with RS485. Board's heart is Arduino mini pro which exchanges info with: <br>
1. MAX485 converter - part of this code has been downloaded from http://www.elektroda.pl/rtvforum/download.php?id=575592 and was just refactored <br>
2. SD Card - used for data logging <br>
3. enc28j60 ethernet module - it is going to be used to stored data outside Current Logger ( domoticz or google drive)<br>
4. DS3231 - RTC<br>
