#include <iostream>
#include <queue>
#include <string>
#include <vector>

struct asc {
    bool operator()(const std::pair<double,int>& a, const std::pair<double,int>& b) const {
        return a.first > b.first; // smallest price on top
    }
};

struct desc {
    bool operator()(const std::pair<double,int>& a, const std::pair<double,int>& b) const {
        return a.first < b.first; // largest price on top
    }
};

int exchange(std::string, double, int);
double matching(
    std::string,
    std::priority_queue<std::pair<double,int>, std::vector<std::pair<double,int>>, desc>& bidding_heap,
    std::priority_queue<std::pair<double,int>, std::vector<std::pair<double,int>>, asc>& asking_heap,
    double trading_price
);

int main()
{
    std::cout << "Welcome To the interface\n";
    std::string Ticker;
    std::cout << "\nTicker :: ";
    std::cin >> Ticker;
    double Base_price = 0.0;
    std::cout << "\nBase Price :: ";
    std::cin >> Base_price;

    return exchange(Ticker, Base_price, 1); // start state at 1 so loop runs
}

int exchange(std::string Ticker, double Base_price, int State)
{
    std::cout << "\n\nWelcome To the exchange\n";
    std::priority_queue<std::pair<double,int>, std::vector<std::pair<double,int>>, asc> asking_heap;
    std::priority_queue<std::pair<double,int>, std::vector<std::pair<double,int>>, desc> bidding_heap;

    while (State == 1)
    {
        std::cout << "\n1. Bid \n2. Ask \n3. Check Price \n4. EXIT \n\n";
        std::string choice;
        std::cin >> choice;
        if (isdigit(choice[0]))
        {
            int chc = choice[0] - '0'; // FIXED conversion
            if (chc == 4) {
                State = 2;
                std::cout << "Exiting...\n";
                return 0;
            }
            else if (chc == 1) {
                double price, quantity;
                std::cout << "Enter Bid Price and Quantity: ";
                std::cin >> price >> quantity;
                if (quantity > 0) bidding_heap.push({price, (int)quantity});
                else std::cout << "\nFAILED\n Quantity must not be 0\n";
            }
            else if (chc == 2) {
                double price, quantity;
                std::cout << "Enter Ask Price and Quantity: ";
                std::cin >> price >> quantity;
                if (quantity > 0) asking_heap.push({price, (int)quantity});
                else std::cout << "\nFAILED\n Quantity must not be 0\n";
            }
            else if (chc == 3) {
                Base_price = matching(Ticker, bidding_heap, asking_heap, Base_price);
                if(!bidding_heap.empty()) std::cout<< "Best Bid :: " << bidding_heap.top().first << "\n";
                if(!asking_heap.empty()) std::cout<< "Best Ask :: " << asking_heap.top().first << "\n";
                std::cout << "Price of Ticker ::" << Ticker << " is :: " << Base_price << "\n";
            }
        }
        else {
            State = 2;
            return 1;
        }
    }
    return 0;
}

double matching(
    std::string Ticker,
    std::priority_queue<std::pair<double,int>, std::vector<std::pair<double,int>>, desc>& bidding_heap,
    std::priority_queue<std::pair<double,int>, std::vector<std::pair<double,int>>, asc>& asking_heap,
    double trading_price
){
    double temp_trading_price = trading_price;
    while(!bidding_heap.empty() && !asking_heap.empty() && bidding_heap.top().first >= asking_heap.top().first){
        auto best_bid = bidding_heap.top();
        auto best_ask = asking_heap.top();

        int tradeQty = std::min(best_bid.second, best_ask.second);
        temp_trading_price = best_ask.first; // trade at ask price

        std::cout<<"\n--- Trade Executed ---";
        std::cout<<"\nBID Price :: " << best_bid.first << " | BID Quantity :: " << best_bid.second;
        std::cout<<"\nASK Price :: " << best_ask.first << " | ASK Quantity :: " << best_ask.second;
        std::cout<<"\nTraded Quantity :: " << tradeQty << " @ Price :: " << temp_trading_price;
        std::cout<<"\n----------------------\n";

        best_bid.second -= tradeQty;
        best_ask.second -= tradeQty;

        bidding_heap.pop();
        asking_heap.pop();

        if(best_bid.second > 0) bidding_heap.push(best_bid);
        if(best_ask.second > 0) asking_heap.push(best_ask);
    }
    return temp_trading_price;
}
