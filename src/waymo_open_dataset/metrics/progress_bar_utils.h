#ifndef WAYMO_OPEN_DATASET_METRICS_PROGRESS_BAR_UTILS_H_
#define WAYMO_OPEN_DATASET_METRICS_PROGRESS_BAR_UTILS_H_

#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>

namespace waymo {
namespace open_dataset {

inline void ShowProgressBar(const std::string& task_name, float progress, 
                            double elapsed_seconds, size_t processed, size_t total) {
    int barWidth = 30;
    // Left-justify the task name with a fixed width of 15 characters
    std::cout << std::left << std::setw(15) << task_name << " [";
    
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }

    std::string eta_str = "Calculating...";
    if (processed > 0 && progress > 0) {
        double total_expected_time = elapsed_seconds / progress;
        int remaining_seconds = static_cast<int>(total_expected_time - elapsed_seconds);
        int mins = remaining_seconds / 60;
        int secs = remaining_seconds % 60;
        eta_str = std::to_string(mins) + "m " + std::to_string(secs) + "s";
    }

    std::cout << "] " << int(progress * 100.0) << "% | ETA: " << eta_str << "    \r";
    std::cout.flush();
}

}  // namespace open_dataset
}  // namespace waymo

#endif