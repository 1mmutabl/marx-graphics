#pragma once

#include <chrono>
#include <vector>

namespace marx::graphics
{
class clock
{
public:

  clock();

  void update();

  float get_delta_time() const;
  float get_fps() const;
  float get_average_fps() const;
  float get_time_passed() const;

  void set_average_time(float AverageTime);

private:

  std::chrono::time_point<std::chrono::steady_clock> Before_;
  float                                              DeltaTime_;
  int                                                Frames_;
  int                                                LastFrames_;
  float                                              TimePassed_;
  float                                              AverageClock_;

  float AverageTime_;
};
} // namespace marx::graphics
