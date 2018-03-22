#ifndef DATE_TIME_H_
#define DATE_TIME_H_

#include "headers.h"
#include <sstream>


///************Number to String***************\\\

template <typename T>
  std::string NumberToString ( T Number )
  {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
  }

///************FINDING DATE***************\\\


string find_Date() {
    time_t     now;
    struct tm nowlocal;

    now = time(NULL);
    nowlocal = *localtime(&now);


    string st = "";
    string day = NumberToString(nowlocal.tm_mday);
    string mon = NumberToString(nowlocal.tm_mon+1);
    string year = NumberToString(nowlocal.tm_year + 1900);

    st += (string)(day + "-" + mon + "-" + year);

    return st;

}


///************FINDING TIME***************\\\


string find_Time() {
    time_t     now;
    struct tm nowlocal;

    now = time(NULL);
    nowlocal = *localtime(&now);

    int hour = nowlocal.tm_hour;
    string state="am";
    if(hour>=12)
        {
            state = "pm";
            if(hour!=12)
            hour -= 12;
        }

    string h = NumberToString(hour);
    string m = NumberToString(nowlocal.tm_min);
    string s = NumberToString(nowlocal.tm_sec);
    string st = "";
    st += (string)(h + ":" + m + ":" + s + state);
    return st;
}


#endif // DATE_TIME_H_
