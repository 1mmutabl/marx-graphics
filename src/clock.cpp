#include "marx/graphics-internal/clock.hpp"

#include <chrono>
#include <cstddef>

using Clock = std::chrono::steady_clock;

namespace marx::graphics
{
clock::clock()
{
  Before_       = Clock::now();
  DeltaTime_    = 0.f;
  AverageTime_  = 1.f;
  AverageClock_ = 0.f;
  TimePassed_   = 0.f;
  Frames_       = 0;
  LastFrames_   = 0;
}

void clock::update()
{
  auto Now = Clock::now();

  std::chrono::duration<float> Delta = Now - Before_;
  Before_                            = Now;

  DeltaTime_ = Delta.count();
  TimePassed_ += DeltaTime_;
  AverageClock_ += DeltaTime_;

  Frames_++;

  if (AverageClock_ >= AverageTime_)
  {
    LastFrames_ = Frames_;

    AverageClock_ = 0.f;
    Frames_       = 0;
  }
}

float clock::get_delta_time() const
{
  return DeltaTime_;
}

float clock::get_fps() const
{
  return 1 / DeltaTime_;
}

float clock::get_average_fps() const
{
  return LastFrames_;
}

float clock::get_time_passed() const
{
  return TimePassed_;
}

void clock::set_average_time(float AverageTime)
{
  AverageTime_ = AverageTime;
}
} // namespace marx::graphics
