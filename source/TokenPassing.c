#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

const int NUM_PROCESSES = 3;

struct Process {
    int id;
    bool has_token;
};

std::vector<Process> processes(NUM_PROCESSES);
std::mutex mtx;

void process_function(int id) {
    while (true) {

        std::lock_guard<std::mutex> lock(mtx);

        if (processes[id].has_token) {
            std::cout << "Process P" << id + 1 << " has the token and is requesting access to printer." << std::endl;
            std::cout << "Process P" << id + 1 << " is printing..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1)); // simulate long print time
            std::cout << "Process P" << id + 1 << " has finished printing and is releasing the token.\n" << std::endl;

            // give token to next process
            processes[id].has_token = false;
            processes[(id + 1) % NUM_PROCESSES].has_token = true;
        }
    }
}

int main()
{
    std::vector<std::thread> threads;

    // set all processes as not have token
    for (int i = 0; i < NUM_PROCESSES; ++i) {
        processes[i] = { i, false };
    }
    processes[0].has_token = true;

    for (int i = 0; i < NUM_PROCESSES; ++i) {
        threads.emplace_back(process_function, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
