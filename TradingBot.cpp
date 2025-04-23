#include "TradingBot.h"
#include <limits>

TradingBot::TradingBot(Market *market, int initialCapacity): market(market), strategyCapacity(initialCapacity)
{
    strategyCount=0;
    availableStrategies = new Strategy* [strategyCapacity];
    for (int i=0; i<strategyCapacity;i++){
        availableStrategies[i] =nullptr;
    }
}

TradingBot::~TradingBot()
{
    for (int i=0;i<strategyCapacity;i++){ // do I need to deallocate startegies? CHECK!!
        delete availableStrategies[i];
    }
    delete[] availableStrategies;
}

void TradingBot::addStrategy(Strategy *strategy)
{
    if(strategyCount >= strategyCapacity)// allocate more memory
    {
        Strategy** temp = new Strategy* [strategyCapacity*2];
        int i=0;
        for (; i<strategyCapacity;i++){
            temp[i] = availableStrategies[i];
        }
        for (int j=i; j<(strategyCapacity*2); j++){
            temp[j] = nullptr;
        }
        delete []availableStrategies;
        strategyCapacity*=2;
        availableStrategies = temp;
    }
    availableStrategies[strategyCount] = strategy; // added new strategy
    strategyCount++;

}

// LEFTT
SimulationResult TradingBot::runSimulation() //we try one strategy in the whole process,
                                            // and choose the one with highest profit
{
    SimulationResult strat;
    strat.bestStrategy=  nullptr;
    strat.totalReturn=0.0;

    for (int i=0; i< strategyCount; i++)
    {
        Strategy* curr_strat = availableStrategies[i];
        double cumul_prof =0.0;
        //double last_price =market->getLastPrice();
        int numTradingDays = market->getNumTradingDays();
        int start_index = numTradingDays-101;
        int current_holding =0; // initially dont have anything
        Action action;
        double purchase_price=0.0;
        double sell_price = 0.0;

        for (int j=start_index; j<numTradingDays; j++) // should iterate 101 times

        {
            action = curr_strat->decideAction(market,j,current_holding);
            if (action == BUY)// store price
            {
                purchase_price = market->getPrice(j);
                current_holding =1;
            }
            else if(action == SELL) // find profit
            {
                sell_price = market->getPrice(j);
                cumul_prof += (sell_price-purchase_price);
                current_holding=0;
            }
            else {continue;}
        }
        if (current_holding == 1)
        {
            double sell_price = market->getLastPrice();
            cumul_prof += (sell_price - purchase_price);
        }
        
        
        if (cumul_prof> strat.totalReturn) // found a better strat
        {
            strat.bestStrategy =curr_strat;
            strat.totalReturn = cumul_prof;
        }
    }

    return strat;
}
