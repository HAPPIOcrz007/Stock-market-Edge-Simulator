# Stock Market Edge Simulator
A high-performance C++ order book simulation system for stock market behavior analysis and market microstructure research. This simulator generates realistic order flows, processes them through a matching engine, and analyzes market movements under various conditions.

## 📋 Overview

The Stock Market Edge Simulator is a C++-based trading system simulation that models order book dynamics, order matching, and price formation. It's designed to help understand market behavior, test trading strategies, and analyze market microstructure effects under different scenarios.

### Key Features

- **Binary Order Book Storage**: Efficient 16-byte order structures for optimal performance
- **Multiple Order Types**: Supports GTC (Good 'til Cancelled), FOK (Fill-or-Kill), and IOC (Immediate-or-Cancel) orders
- **Batch Order Generation**: Generate thousands of orders with configurable price gradients and noise
- **Fixed-Point Arithmetic**: High precision price and volume representation using integer scaling
- **Market State Tracking**: Real-time tracking of bid/ask spreads, day high/low, and trading volumes
- **Modular Architecture**: Separated concerns between order generation, matching, and state management

## 🏗️ Architecture

```
┌─────────────────┐
│  Command.txt    │
│  (Human Input)  │
└────────┬────────┘
         ▼
┌─────────────────────────┐
│  Batch Order Generator  │
│  (batch_order_maker)    │
└────────┬────────────────┘
         ▼
┌─────────────────────────┐
│    Order Maker          │
│  (Creates .bin files)   │
└────────┬────────────────┘
         ▼
┌─────────────────────────┐
│   Matching Engine       │
│  (Processes orders)     │
└────────┬────────────────┘
         ▼
┌─────────────────────────┐
│   Price Changes         │
│   (Final Output)        │
└─────────────────────────┘
```

### Directory Structure

```
Project_Stock_Simulation/
├── src/
│   ├── core_structure.h        # Core data structures and constants
│   ├── batch_order_maker.cpp   # Order generation from commands
│   ├── matching_engine.cpp     # Order matching logic (planned)
│   └── order_maker.cpp         # Binary order file creation
├── data_holders/
│   ├── commands/                # Command files for order generation
│   │   └── commands1001.txt    # Example command file
│   ├── orders/                  # Generated order books
│   │   ├── bin/                # Binary order files
│   │   └── text/               # Human-readable order files
│   ├── states/                 # Market state snapshots
│   │   ├── currents/           # Current market states
│   │   └── snapshots/          # Historical state snapshots
│   └── matching/               # Matching results
└── Makefile                    # Build configuration
```

## 📦 Data Structures

### Order Structure (16 bytes)
```cpp
struct Order {
    uint32_t price;      // Price scaled by 1000 (max: $4,294,967.295)
    uint32_t volume;     // Volume scaled by 10,000 (max: 429,496.7295 units)
    uint32_t trader_id;  // Trader identifier (0-4.29B)
    uint16_t stock_id;   // Stock identifier (0-65,535)
    uint8_t  flags;      // Order type flags (GTC/FOK/IOC, BUY/SELL)
    uint8_t  reserved;   // Reserved for future use
};
```

### Order Types
| Flag | Type | Description |
|------|------|-------------|
| 0 | SELL + GTC | Sell order, Good 'til Cancelled |
| 1 | BUY + GTC | Buy order, Good 'til Cancelled |
| 2 | SELL + FOK | Sell order, Fill-or-Kill |
| 3 | BUY + FOK | Buy order, Fill-or-Kill |
| 4 | SELL + IOC | Sell order, Immediate-or-Cancel |
| 5 | BUY + IOC | Buy order, Immediate-or-Cancel |

### Batch Command Format (8 bytes)
```cpp
struct BatchCommand {
    uint16_t stock_id;      // Stock identifier
    uint16_t order_count;   // Number of orders to generate
    int16_t gradient_x100;  // Price gradient ×100 (-327.68 to +327.67)
    uint16_t noise_x100;    // Noise factor ×100 (0 to 655.35)
};
```

### Market State (32 bytes)
```cpp
struct StockState {
    uint32_t last_price;   // Last traded price
    uint32_t last_volume;  // Last traded volume
    uint32_t bid_price;    // Current best bid
    uint32_t ask_price;    // Current best ask
    uint32_t bid_volume;   // Volume at best bid
    uint32_t ask_volume;   // Volume at best ask
    uint32_t day_high;     // Day's highest price
    uint32_t day_low;      // Day's lowest price
};
```

### Match Structure (16 bytes)
```cpp
struct Match {
    uint64_t buyer_ref;    // Buyer: 32-bit trader_id + 32-bit order_seq
    uint64_t seller_ref;   // Seller: 32-bit trader_id + 32-bit order_seq
};
```

## 🚀 Getting Started

### Prerequisites

- **C++ Compiler** with C++17 support
  - GCC 7+ 
  - Clang 5+
  - MSVC 2017+ (Windows)
- **Make** (or CMake alternative)

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/Stock-market-Edge-Simulator.git
   cd Stock-market-Edge-Simulator/Project_Stock_Simulation
   ```

2. Build the project:
   ```bash
   make
   ```

   Or manually compile:
   ```bash
   g++ -std=c++17 -O3 -Wall -o batch_order_maker src/batch_order_maker.cpp
   ```

3. Run the simulator:
   ```bash
   ./batch_order_maker commands1001.txt
   ```

### Creating Command Files

Create a command file in `data_holders/commands/` (e.g., `commands1001.txt`):

```txt
# Format: stock_id gradient order_count noise
1001 0.5 1000 0.1
1001 -0.3 500 0.05
1001 0.8 2000 0.15
```

**Command File Format:**
- First line: `stock_id` (stock identifier)
- Following lines: `gradient order_count noise`
- Gradient: Expected price movement per order (positive = upward, negative = downward)
- Noise: Market randomness factor

## 🔧 Usage

### Generate Orders

```bash
# Generate orders from command file (auto-generates timestamped output)
./batch_order_maker commands1001.txt

# Generate with custom output name
./batch_order_maker commands1001.txt -o my_orders

# Example output
Output: orders_20240315_143022.bin
Generating 1000 orders with gradient 0.5, noise 0.1
Generating 500 orders with gradient -0.3, noise 0.05
Generating 2000 orders with gradient 0.8, noise 0.15
Done! Generated orders in orders_20240315_143022.bin
```

### Output Files

- **Binary Order Files**: Compact 16-byte per order format for efficient processing
- **Naming Convention**: `orders_YYYYMMDD_HHMMSS.bin` or custom name with `.bin` extension

## 📊 Performance Characteristics

- **Order Size**: 16 bytes per order (compact storage)
- **Price Precision**: 0.001 units ($0.001)
- **Volume Precision**: 0.0001 units
- **Max Orders per Batch**: 65,535
- **Max Stocks**: 65,536
- **Max Traders**: 4.29 billion
- **Memory Efficiency**: ~16MB for 1 million orders

## 🧪 Testing Scenarios

### 1. Bull Market Simulation
```txt
1001 0.8 5000 0.1   # Strong upward trend
1001 0.5 3000 0.08  # Continued growth
1001 0.2 2000 0.05  # Slowing growth
```

### 2. Bear Market Simulation
```txt
1001 -0.7 4000 0.12 # Gradual decline
1001 -1.2 3000 0.15 # Accelerating decline
1001 -0.3 2000 0.08 # Stabilization
```

### 3. Volatile Market
```txt
1001 1.2 1000 0.3   # Sharp increase with high noise
1001 -1.5 1000 0.4  # Sharp reversal with extreme noise
1001 0.0 2000 0.2   # Sideways movement
```

### 4. Market Crash
```txt
1001 -2.0 5000 0.25 # Rapid decline
1001 -1.5 4000 0.2  # Continued selling pressure
1001 -0.5 3000 0.15 # Stabilization
```

## 🛠️ Development

### Building from Source

```bash
# Clean build
make clean
make

# Debug build
make debug

# Release build (optimized)
make release
```

### Adding New Features

1. **New Order Types**: Extend the `flags` field in `Order` struct
2. **Additional Constants**: Add to `OrderConstants` namespace
3. **Custom Matching Logic**: Implement in `matching_engine.cpp`

### Performance Optimization Tips

- Compile with `-O3` optimization flag
- Use `-march=native` for CPU-specific optimizations
- Consider memory-mapped I/O for large order books
- Batch process orders for better throughput
- Use `#pragma pack` for optimal structure packing (already implemented)

## 🔍 Debugging

Enable debug output:
```bash
# Compile with debug symbols
g++ -std=c++17 -g -Wall -o batch_order_maker src/batch_order_maker.cpp

# Run with debugger
gdb ./batch_order_maker
```

## 📝 File Formats

### Binary Order Format
Each order is 16 bytes in little-endian format:
```
Offset | Size | Field
-------+------+--------
0      | 4    | price (scaled by 1000)
4      | 4    | volume (scaled by 10000)
8      | 4    | trader_id
12     | 2    | stock_id
14     | 1    | flags
15     | 1    | reserved
```

### Command File Format
Plain text with space-separated values:
```
stock_id
gradient order_count noise
gradient order_count noise
...
```

## 🤝 Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Coding Standards
- Use C++17 features where appropriate
- Follow existing naming conventions
- Add comments for complex logic
- Update documentation for new features

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Inspired by real-world exchange matching engines
- Built with performance and precision in mind
- Thanks to all contributors who help improve the simulator

---

**⚠️ Disclaimer**: This is a simulation tool for research and educational purposes. Not intended for actual trading or financial decision-making without proper validation and testing.
```

This README is now properly tailored for your C++ project, removing all Rust references and focusing on the actual architecture with `.cpp` and `.h` files. It includes all the structures from your `core_structure.h` and explains the batch order maker functionality accurately.
