#include "miner.h"
#include "utils.h"
#include "krist.h"

#include <tclap/CmdLine.h>

#include <memory>
#include <thread>
#include <random>
#include <fstream>
#include <cstring>
#include <ctime>

miner_context_t g_miner_ctx;

bool load_terms(const std::string &file) {
    std::ifstream in(file, std::ios::in);

    if (!in.is_open()) {
        return false;
    }

    std::string line;
    std::vector<std::string> terms;

    while (std::getline(in, line)) {
        if (!line.empty()) {
            terms.push_back(line);
        }
    }

    g_miner_ctx.term_count = terms.size();
    g_miner_ctx.terms = new char*[g_miner_ctx.term_count];

    for (size_t i = 0; i < g_miner_ctx.term_count; ++i) {
        g_miner_ctx.terms[i] = new char[terms[i].size()];
        strcpy(g_miner_ctx.terms[i], terms[i].c_str());
    }

    return true;
}

uint64_t gen_base_pass() {
    std::random_device dev;
    std::mt19937 mt(dev());
    std::uniform_int_distribution<uint64_t> dist(0, INT64_MAX);
    return dist(mt);
}

void mine_speed_count_thread() {
    while (g_miner_ctx.running) {
        g_miner_ctx.miner_mutex.lock();
        std::cout << "Mining at " << g_miner_ctx.addresses_mined << " A/s" << std::endl;
        g_miner_ctx.miner_mutex.unlock();

        g_miner_ctx.addresses_mined = 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void mine_address_thread(int thread_id, uint64_t base_pass, uint64_t addr_count, bool do_v1, bool clean_output, bool no_numbers) {
    if (!g_miner_ctx.params.clean_output) {
        g_miner_ctx.miner_mutex.lock();
        {
            std::ostringstream greeting;
            greeting << "Started thread " << thread_id << ", working from " << std::hex << base_pass << " to " << std::hex
                     << (base_pass + addr_count);
            std::cout << greeting.str() << std::endl;
        }
        g_miner_ctx.miner_mutex.unlock();
    }

    for (uint64_t current = base_pass; g_miner_ctx.running && current < base_pass + addr_count; ++current) {
        char current_hex[17];
        to_hex_string(current, current_hex);

        char pkey[128];
        make_private_key(current_hex, pkey);

        char address[11];

        if (do_v1) {
            make_v1_address(pkey, address);
        } else {
            make_v2_address(pkey, address);
        }

        g_miner_ctx.addresses_mined++;

        if (no_numbers) {
            bool has_number = false;

            for (size_t i = 0; i < strlen(address); ++i) {
                if (isdigit(address[i])) {
                    has_number = true;
                    break;
                }
            }

            if (has_number) {
                continue;
            }
        }

        for (size_t i = 0; i < g_miner_ctx.term_count; ++i) {
            char *term = g_miner_ctx.terms[i];

            if (strstr(address, term) != nullptr) {
                g_miner_ctx.miner_mutex.lock();
                {
                    std::ostringstream msg;
                    msg << address << ":" << current_hex;

                    if (clean_output) {
                        std::cout << msg.str() << std::endl;
                    } else {
                        std::ostringstream msg;
                        msg << thread_id << " => " << address << " with pw " << current_hex;
                        std::cout << msg.str() << std::endl;
                    }

                    if (g_miner_ctx.params.do_logging) {
                        std::ofstream log(g_miner_ctx.params.log_file, std::ios::app);
                        log << msg.str() << std::endl;
                    }
                }
                g_miner_ctx.miner_mutex.unlock();
            }
        }
    }
}

void cleanup() {
    for (unsigned int i = 0; i < g_miner_ctx.term_count; ++i) {
        delete[] g_miner_ctx.terms[i];
    }

    delete[] g_miner_ctx.terms;
}

void start_miner() {
    if (!g_miner_ctx.params.clean_output) {
        char base_pass_str[17];
        to_hex_string(g_miner_ctx.params.base_pass, base_pass_str);
        std::cout << "Using \"" << base_pass_str << "\" as base\n";
    }

    g_miner_ctx.running = true;
    g_miner_ctx.threads.reserve((size_t)g_miner_ctx.params.thread_count);

    uint64_t work_size = UINT64_MAX - g_miner_ctx.params.base_pass;
    uint64_t work_size_per_thread = work_size / g_miner_ctx.params.thread_count;

    uint64_t pass = g_miner_ctx.params.base_pass;

    for (unsigned int i = 0; i < g_miner_ctx.params.thread_count; ++i) {
        std::shared_ptr<std::thread> thread(new std::thread(
                mine_address_thread, i, pass, work_size_per_thread,
                g_miner_ctx.params.do_v1, g_miner_ctx.params.clean_output, g_miner_ctx.params.no_numbers
        ));

        g_miner_ctx.threads.push_back(thread);
        pass += work_size_per_thread;
    }

    if (!g_miner_ctx.params.clean_output) {
        std::shared_ptr<std::thread> speed_counter(new std::thread(
                mine_speed_count_thread
        ));

        g_miner_ctx.threads.push_back(speed_counter);
    }

    for (auto thread : g_miner_ctx.threads) {
        thread->join();
    }

    cleanup();
}

int main(int argc, char **argv) {
    TCLAP::CmdLine cmd("Generates Krist addresses from a set of input strings.", ' ', "0.1");
    TCLAP::ValueArg<std::string> file_arg("i", "input", "A simple text file containing terms to look for in each line.", false, "terms.txt", "string");
    cmd.add(file_arg);

    TCLAP::SwitchArg gen_v1_addresses_arg("1", "v1", "Generate Krist v1 addresses instead of v2.", false);
    cmd.add(gen_v1_addresses_arg);

    unsigned int default_thread_count = std::thread::hardware_concurrency();
    TCLAP::ValueArg<unsigned int> thread_count_arg("t", "threads", "How many threads to use for finding addresses.", false, default_thread_count, "number");
    cmd.add(thread_count_arg);

    TCLAP::SwitchArg clean_output_arg("c", "clean", "Clean output that can be piped into a file.", false);
    cmd.add(clean_output_arg);

    TCLAP::SwitchArg no_numbers_arg("n", "nonumbers", "Ignores addresses that contain numbers.", false);
    cmd.add(no_numbers_arg);

    TCLAP::ValueArg<std::string> log_file_arg("l", "log", "Tells the program to log to a file.", false, "", "string");
    cmd.add(log_file_arg);

    cmd.parse(argc, argv);

    if (!load_terms(file_arg.getValue())) {
        std::cerr << "Failed to load terms from \"" << file_arg.getValue() << "\"!! Check or set -i." << std::endl;
        return 1;
    }

    g_miner_ctx.running = false;
    g_miner_ctx.params.base_pass = gen_base_pass();
    g_miner_ctx.params.clean_output = clean_output_arg.getValue();
    g_miner_ctx.params.do_v1 = gen_v1_addresses_arg.getValue();
    g_miner_ctx.params.thread_count = thread_count_arg.getValue();
    g_miner_ctx.params.no_numbers = no_numbers_arg.getValue();
    g_miner_ctx.params.do_logging = log_file_arg.isSet() && !log_file_arg.getValue().empty();
    g_miner_ctx.params.log_file = log_file_arg.getValue();

    if (!g_miner_ctx.params.clean_output) {
        std::cout << "Mining on " << g_miner_ctx.params.thread_count << " threads..." << std::endl;

        if (g_miner_ctx.params.no_numbers) {
            std::cout << "Ignoring addresses with numbers." << std::endl;
        }
    }

    start_miner();
    cleanup();
    return 0;
}