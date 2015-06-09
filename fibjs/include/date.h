/*
 * date.h
 *
 *  Created on: Jul 27, 2012
 *      Author: lion
 */

#include <string>

#ifndef DATE_H_
#define DATE_H_

namespace v8
{
namespace base
{

class OS
{
public:
    static double TimeCurrentMillis();
};

}
}

namespace fibjs
{

class date_t
{
public:
    enum
    {
        _YEAR = 0,
        _MONTH = 1,
        _DAY = 2,
        _HOUR = 3,
        _MINUTE = 4,
        _SECOND = 5
    };

public:
    date_t(double v = NAN) :
        d(v)
    {
    }

    date_t(const date_t &v) :
        d(v.d)
    {
    }

    date_t(v8::Local<v8::Value> v) :
        d(NAN)
    {
        operator=(v);
    }

    void clear()
    {
        d = NAN;
    }

    void now()
    {
        d = v8::base::OS::TimeCurrentMillis();
    }

    void create(int Y, int M, int D, int h, int m, int s, int ms);

    date_t &operator=(double v)
    {
        d = v;
        return *this;
    }

    date_t &operator=(const date_t &v)
    {
        d = v.d;
        return *this;
    }

    date_t &operator=(v8::Local<v8::Value> v)
    {
        d = v->NumberValue();
        return *this;
    }

    operator v8::Local<v8::Value>() const
    {
        return v8::Date::New(Isolate::now().isolate, d);
    }

    bool empty() const
    {
        return isnan(d);
    }

    double diff(date_t d1)
    {
        return d - d1.d;
    }

    void add(int num, int part = _SECOND);
    void fix(int part = _SECOND);

    void toLocal();
    void toUTC();

    void parse(const char *str, int len = -1);
    void toGMTString(std::string &retVal);
    void toX509String(std::string &retVal);
    void sqlString(std::string &retVal);
    void stamp(std::string &retVal);

private:
    double d;
};

}

#endif /* DATE_H_ */
