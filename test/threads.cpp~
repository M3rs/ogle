#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

std::atomic<bool> done;

int main() {
  done = false;

  std::thread t([]() {
    std::cout << "Working!" << '\n';
    std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(9600));
    std::cout << "Finished" << '\n';
    done = true;
  });

  while (1) {
    std::cout << "Waiting..." << '\n';
    std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(1000));
    if (done)
      break;
  }

  t.join();
  std::cout << "done" << std::endl;
}
