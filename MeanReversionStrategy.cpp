#include "MeanReversionStrategy.h"
#include "Utils.h"
#include <cmath>
#include <iostream>

MeanReversionStrategy::MeanReversionStrategy(): Strategy()
{
}

MeanReversionStrategy::MeanReversionStrategy(const string &name, int window, int threshold): Strategy(name), window(window), threshold(threshold)
{
}

Action MeanReversionStrategy::decideAction(Market *market, int index, double currentHolding) const
{
    double MA = this->calculateMovingAverage(market,index,window);
    
    if ((market->getPrice(index)> ((1+(threshold/100))*MA)) &&(currentHolding ==1)) // sell
    {
        Action strat = SELL;
        return strat;
    }
    else if(market->getPrice(index)< ((1-(threshold/100)*MA)) &&(currentHolding ==0)) //buy
    {
        Action strat =BUY;
        return strat;
    }
    else{
        Action strat = HOLD; // what if my currentHolding =0?
        return strat;
    }
}

MeanReversionStrategy **MeanReversionStrategy::generateStrategySet(const string &baseName, int minWindow, int maxWindow, int windowStep, int minThreshold, int maxThreshold, int thresholdStep)
{
    int num_winds = ((maxWindow -minWindow)/windowStep)+1;
    int num_thresholds = ((maxThreshold-minThreshold)/thresholdStep)+1;
    MeanReversionStrategy ** generated_strat =new MeanReversionStrategy* [num_winds*num_thresholds];
    int strat_count =0;
    for( int i=minWindow; i<=maxWindow; i+=windowStep){
        for (int j= minThreshold; j<=maxThreshold; j+= thresholdStep){
            string name = baseName+"_"+to_string(i)+"_"+to_string(j);
            generated_strat[strat_count++]= new MeanReversionStrategy( name, i, j);
        }
    }
    return generated_strat;
}