/* Copyright 2019 The Waymo Open Dataset Authors.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "waymo_open_dataset/metrics/config_util.h"

#include <memory>
#include <vector>

#include "absl/strings/str_cat.h"
#include "waymo_open_dataset/label.pb.h"
#include "waymo_open_dataset/metrics/breakdown_generator.h"
#include "waymo_open_dataset/metrics/metrics_utils.h"
#include "waymo_open_dataset/metrics/progress_bar_utils.h" // add
#include "waymo_open_dataset/protos/breakdown.pb.h"
#include "waymo_open_dataset/protos/motion_metrics.pb.h"
#include "waymo_open_dataset/protos/scenario.pb.h"

namespace waymo {
namespace open_dataset {

std::vector<std::string> GetBreakdownNamesFromConfig(const Config& config) {
  std::vector<std::string> names;

  // vvv 追加ここから vvv
  const int num_generators = config.breakdown_generator_ids_size();
  auto      start_time     = std::chrono::steady_clock::now();
  // ^^^ 追加ここまで ^^^

  for (int i = 0, sz = config.breakdown_generator_ids_size(); i < sz; ++i) {
    const std::unique_ptr<BreakdownGenerator> breakdown_generator =
        BreakdownGenerator::Create(config.breakdown_generator_ids(i));
    const int num_shards = breakdown_generator->NumShards();
    const std::vector<Label::DifficultyLevel> difficulty_levels =
        internal::GetDifficultyLevels(config, i);
    for (int shard = 0; shard < num_shards; ++shard) {
      for (auto dl : difficulty_levels) {
        names.push_back(absl::StrCat(breakdown_generator->ShardName(shard), "_",
                                     Label::DifficultyLevel_Name(dl)));
      }
    }

    // vvv 追加ここから vvv
    auto current_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = current_time - start_time;
    ShowProgressBar("Names", 
                    static_cast<float>(i + 1) / num_generators, 
                    elapsed.count(), 
                    i + 1, 
                    num_generators);
    // ^^^ 追加ここまで ^^^
  }
  // vvv 追加ここから vvv
  std::cout << std::endl;
  // ^^^ 追加ここまで ^^^
  return names;
}

std::vector<std::string> GetBreakdownNamesFromMotionConfig(
    const MotionMetricsConfig& config) {
  const std::vector<Track::ObjectType> types = {
      Track::TYPE_VEHICLE, Track::TYPE_PEDESTRIAN, Track::TYPE_CYCLIST};
  std::vector<std::string> names;
  for (const auto& object_type : types) {
    for (const auto& step : config.step_configurations()) {
      names.push_back(absl::StrCat(Track::ObjectType_Name(object_type), "_",
                                   step.measurement_step()));
    }
  }
  return names;
}

}  // namespace open_dataset
}  // namespace waymo
