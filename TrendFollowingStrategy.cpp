#include "TrendFollowingStrategy.h"
#include "Utils.h"
#include <iostream>

TrendFollowingStrategy::TrendFollowingStrategy(): Strategy()
{
}

TrendFollowingStrategy::TrendFollowingStrategy(const std::string &name, int shortWindow, int longWindow): Strategy(name), shortMovingAverageWindow(shortWindow), longMovingAverageWindow(longWindow)
{
}

Action TrendFollowingStrategy::decideAction(Market *market, int index, double currentHolding) const
{
    double long_MA = this->calculateMovingAverage(market,index,longMovingAverageWindow);
    double short_MA = this->calculateMovingAverage(market,index,shortMovingAverageWindow);
    Action strat = HOLD;
    if (short_MA>long_MA && currentHolding==0){
        strat = BUY;
    }
    else if(short_MA<long_MA && currentHolding==1){
        strat = SELL;
    }
    else{
        strat = HOLD;
    }
    return strat;

}

TrendFollowingStrategy **TrendFollowingStrategy::generateStrategySet(const string &baseName, int minShortWindow, int maxShortWindow, int stepShortWindow, int minLongWindow, int maxLongWindow, int stepLongWindow)
{
    int num_shorts = ((maxShortWindow -minShortWindow)/stepShortWindow)+1;
    int num_longs = ((maxLongWindow-minLongWindow)/stepLongWindow)+1;
    TrendFollowingStrategy ** generated_strat =new TrendFollowingStrategy* [num_shorts*num_longs];
    int strat_count =0;
    for( int i=minLongWindow; i<=maxLongWindow; i+=stepLongWindow){
        for (int j= minShortWindow; j<=maxShortWindow; j+= stepShortWindow){
            string name = baseName+"_"+to_string(j)+"_"+to_string(i); // reversed order lmao
            generated_strat[strat_count++]= new TrendFollowingStrategy( name, j, i); // here too
        }
    }
    return generated_strat;
}
