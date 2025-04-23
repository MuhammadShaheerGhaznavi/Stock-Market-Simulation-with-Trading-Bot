#include "Strategy.h"
#include <iostream>

Strategy::Strategy()
{
    name ="";
}

Strategy::Strategy(const string &name):name(name)
{
}

double Strategy::calculateMovingAverage(Market *market, int index, int window) const
{
    /*if ((index -window) < 0) //window falls outsode array bounds
    {
        //index unchanged ??;
        window = 0 + index; // index sey ley kar start tak
    }
    int  count=0;
    double tot_price =0.0;
    for (int i=index, j=index ; j<(index +window);i--, j++){ // i is my index variable, decre karo takey i get successive prev values
                                                            // j is my looping variable, makes sure I run the process index +window times 
        tot_price+= market->getPrice(i);
        count++;
    }
    double result = tot_price/count;

    return result;*/
    int start_index = max(0, index-window +1);
    double tot_price =0.0;
    int  count=0;
    for(int i = start_index; i<=index;i++){
        count++;
        tot_price+= market->getPrice(i);
    }

    return (tot_price/count);

}

string Strategy::getName() const
{
    return name;
}

Strategy:: ~Strategy(){ // empty?
}
