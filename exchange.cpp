#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <atomic>
#include <chrono>

uint64_t generate_id(const std::string& ticker);

struct Order {
    int stage;
    uint64_t orderId;
    double price = 0.0;
    int quantity = 0;

    Order(const std::string& ticker, int stg, double prc, int qty)
        :stage(stg),
        orderId(generate_id(ticker)),
        price(prc),
        quantity(qty) {}
};

struct asc {
    bool operator()(const Order& a, Order& b) const {
        return a.price > b.price; // smallest price on top
    }
};

struct desc {
    bool operator()(const Order& a, const Order& b) const {
        return a.price < b.price; // largest price on top
    }
};

int exchange(std::string, double, int);
double matching(
    std::string,
    std::priority_queue<Order, std::vector<Order>, desc>& bidding_heap,
    std::priority_queue<Order, std::vector<Order>, asc>& asking_heap,
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
    std::priority_queue<Order, std::vector<Order>, asc> asking_heap;
    std::priority_queue<Order, std::vector<Order>, desc> bidding_heap;

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
                if (quantity > 0) bidding_heap.push({Ticker, 1, price, quantity});
                else std::cout << "\nFAILED\n Quantity must not be 0\n";
            }
            else if (chc == 2) {
                double price, quantity;
                std::cout << "Enter Ask Price and Quantity: ";
                std::cin >> price >> quantity;
                if (quantity > 0) asking_heap.push({Ticker, 1, price, quantity});
                else std::cout << "\nFAILED\n Quantity must not be 0\n";
            }
            else if (chc == 3) {
                Base_price = matching(Ticker, bidding_heap, asking_heap, Base_price);
                if(!bidding_heap.empty()) std::cout<< "Best Bid :: " << bidding_heap.top().price << "\n";
                if(!asking_heap.empty()) std::cout<< "Best Ask :: " << asking_heap.top().price << "\n";
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
    std::priority_queue<Order, std::vector<Order>, desc>& bidding_heap,
    std::priority_queue<Order, std::vector<Order>, asc>& asking_heap,
    double trading_price
){
    double temp_trading_price = trading_price;
    while(!bidding_heap.empty() && bidding_heap.top().stage == 5){
        bidding_heap.pop();
    }
    while(!asking_heap.empty() && asking_heap.top().stage == 5){
        asking_heap.pop();
    }
    while(!bidding_heap.empty() && !asking_heap.empty() && bidding_heap.top().price >= asking_heap.top().price){
        auto best_bid = bidding_heap.top();
        auto best_ask = asking_heap.top();
        best_bid.stage = 2;
        best_ask.stage = 2;

        int tradeQty = std::min(best_bid.quantity, best_ask.quantity);
        temp_trading_price = best_ask.price; // trade at ask price

        std::cout<<"\n--- Trade Executed ---";
        std::cout<<"\nBID Price :: " << best_bid.price << " | BID Quantity :: " << best_bid.quantity << " | ID :: " << best_bid.orderId;
        std::cout<<"\nASK Price :: " << best_ask.price << " | ASK Quantity :: " << best_ask.quantity << " | ID :: " << best_ask.orderId;;
        std::cout<<"\nTraded Quantity :: " << tradeQty << " @ Price :: " << temp_trading_price;
        std::cout<<"\n----------------------\n";

        best_bid.quantity -= tradeQty;
        best_ask.quantity -= tradeQty;

        bidding_heap.pop();
        asking_heap.pop();

        if(best_bid.quantity > 0){
            best_bid.stage = 1;
            bidding_heap.push(best_bid);
        }
        else{
            best_bid.stage = 3;
        }

        if(best_ask.quantity > 0){
            best_ask.stage = 1;
            asking_heap.push(best_ask);
        }
        else{
            best_ask.stage = 3;
        }
    }
    return temp_trading_price;
}

const uint16_t DEFAULT_ENGINE_ID = 1;

uint64_t generate_id(const std::string& ticker){
    uint64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    return (timestamp << 16)|DEFAULT_ENGINE_ID;
}