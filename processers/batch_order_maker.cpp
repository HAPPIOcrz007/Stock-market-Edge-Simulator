// batch_order_maker.cpp
/**
 * this code reads the file in <constexpr const char* COMMANDS_DIR        = "data_holders/commands/"; >
 * and calls in the while function the order maker
 * to make the orders depending on the stuff from the file
 */
#include "./core_structure.h"
#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <string>
#include <ctime>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: "   << argv[0] << " <commands_file> [-o output_name]\n";
        std::cerr << "Example: " << argv[0] << " commands1001\n";
        std::cerr << "         " << argv[0] << " commands1001 -o 12345678\n";
        return 1;
    }
    /**
     * * checking if commands.txt file is mentioned in terminal call
    */  
    std::string cmd_filename = argv[1];
    if (cmd_filename.find(".txt") == std::string::npos) {
        cmd_filename += ".txt";
    }
    
    std::string out_filename;
    if (argc == 4 && std::string(argv[2]) == "-o") {
        out_filename = std::string(argv[3]) + ".bin";
    } else {
        time_t now = time(nullptr);
        char timestamp[32];
        strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", localtime(&now));
        out_filename = std::string("orders_") + timestamp + ".bin";
    }
    /**
     * *
     */
    std::cout << "Output: " << out_filename << "\n";
    
    std::string full_path = std::string(OrderConstants::COMMANDS_DIR) + cmd_filename;
    std::ifstream cmd_file(full_path);
    if (!cmd_file) {
        std::cerr << "Failed to open " << full_path << "\n";
        return 1;
    }
    
    uint16_t stock_id;
    cmd_file >> stock_id;
    
    std::ofstream out_file(out_filename, std::ios::binary);
    if (!out_file) {
        std::cerr << "Failed to create output file: " << out_filename << "\n";
        return 1;
    }
    
    BatchCommand cmd;
    double gradient, noise;
    int order_count;
    
    while (cmd_file >> gradient >> order_count >> noise) {
        cmd.stock_id = stock_id;
        cmd.order_count = order_count;
        cmd.set_gradient(gradient);
        cmd.set_noise(noise);
        
        std::cout << "Generating " << order_count << " orders with gradient " 
                  << gradient << ", noise " << noise << "\n";
    }
    
    std::cout << "Done! Generated orders in " << out_filename << "\n";
    return 0;
}