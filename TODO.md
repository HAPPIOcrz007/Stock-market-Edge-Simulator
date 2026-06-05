# Requirements
- [ ] Multiple Ticker
      multiple stocks/ etf / securities accessible and tradable
- [X] Persistent Order ID
      order Id's for tracking and audit
    - [ ] Enum for stages
    - [ ] Partial fill handling
    - [ ] Namespace Versioning
    - [ ] Benchmark different struct styles
- [ ] More types of Orders
      IOC, Limit, Market, Auction Orders
- [ ] Cancel Or Modify Order
- [ ] Thread safe Data structures
- [ ] Sharding by Instrument
      two tickers must not share their data and must be independent in orderbook
- [ ] Throughput Optimisations
- [ ] Latency Optimisations
- [ ] Data Recovery
- [ ] Recall