#pragma once
#include <string>
#include <stdexcept>


class Data_Source
{
    public:
    virtual std::string Get_Data();
    //Call Get_Data() using the >> operator
    void operator >>(std::string &Data);
    void operator >>(int &Data);
    void operator >>(float &Data);
    void operator >>(double &Data);
    void operator >>(bool &Data);

    // Keep this interface in sync with Appeal's Source/Data_Source.h.
    // A normal command discards results; an expression context collects them.
    virtual void Return_Value(std::string Value) {}
    virtual bool End_Of_Source() const { return false; }
};

