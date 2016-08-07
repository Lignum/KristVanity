#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <thread>

struct miner_params_t {
    bool do_v1;
    unsigned int thread_count;
    uint64_t base_pass;
    bool clean_output;
    bool no_numbers;
};

struct miner_context_t {
    std::vector<std::string> terms;
    bool running;

    std::vector<std::shared_ptr<std::thread>> threads;

    miner_params_t params;
};

extern miner_context_t g_miner_ctx;