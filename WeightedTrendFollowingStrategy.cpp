#include "WeightedTrendFollowingStrategy.h"
#include "Utils.h"
#include <cmath>

WeightedTrendFollowingStrategy::WeightedTrendFollowingStrategy(): TrendFollowingStrategy()
{
}

WeightedTrendFollowingStrategy::WeightedTrendFollowingStrategy(const string &name, int shortWindow, int longWindow): TrendFollowingStrategy(name,shortWindow,longWindow)
{
}

double WeightedTrendFollowingStrategy::calculateExponentialWeight(int index) const
{
    double exp_weight=1.0;
    for (int i=0; i<index;i++){
        exp_weight*=1.1;
    }
    return exp_weight;
}

double WeightedTrendFollowingStrategy::calculateMovingAverage(Market *market, int index, int window) const
{
    int start_index = max(0,index-window +1 );
    int iters = index- start_index+1;
    double tot_price =0.0;
    double sum_weights =0.0;
    for (int i=0; i< iters;i++){
        double e = calculateExponentialWeight(i);
        tot_price += e*market->getPrice(start_index+i);
        sum_weights+= e;
    }
    double WMA =tot_price/sum_weights;
    return WMA;

}

WeightedTrendFollowingStrategy **WeightedTrendFollowingStrategy::generateStrategySet(const string &baseName, int minShortWindow, int maxShortWindow, int stepShortWindow, int minLongWindow, int maxLongWindow, int stepLongWindow)
{
    int num_shorts = ((maxShortWindow -minShortWindow)/stepShortWindow)+1;
    int num_longs = ((maxLongWindow-minLongWindow)/stepLongWindow)+1;
    WeightedTrendFollowingStrategy ** generated_strat =new WeightedTrendFollowingStrategy* [num_shorts*num_longs];
    int strat_count =0;
    for( int i=minLongWindow; i<=maxLongWindow; i+=stepLongWindow){
        for (int j= minShortWindow; j<=maxShortWindow; j+= stepShortWindow){
            string name = baseName+"_"+to_string(j)+"_"+to_string(i); // reversed order lmao
            generated_strat[strat_count++]= new WeightedTrendFollowingStrategy( name, j, i); // here too
        }
    }
    return generated_strat;
}
