/* A basic exponential moving average filter.
 * See InfiniteImpulseResponse.h for more info on IIR filters.
 * http://en.wikipedia.org/wiki/Moving_average#Exponential_moving_average
 */

#include "InfiniteImpulseResponse.h"
#include "../../core/DeviceListenerWrapper.h"

namespace features {
namespace filters {
class ExponentialMovingAverage : public InfiniteImpulseResponse {
 public:
  explicit ExponentialMovingAverage(core::DeviceListenerWrapper& parent_feature,
                                    DataFlags flags, float alpha);

 private:
  virtual float Update(float new_value, float old_value) override;

  float alpha_;
};

ExponentialMovingAverage::ExponentialMovingAverage(
    core::DeviceListenerWrapper& parent_feature, DataFlags flags, float alpha)
    : InfiniteImpulseResponse(parent_feature, flags), alpha_(alpha) {}

float ExponentialMovingAverage::Update(float new_value, float old_value) {
  return (alpha_ * new_value) + ((1 - alpha_) * old_value);
}

ExponentialMovingAverage make_exponential_moving_average(
    core::DeviceListenerWrapper& parent_feature,
    ExponentialMovingAverage::DataFlags flags, float alpha) {
  return ExponentialMovingAverage(parent_feature, flags, alpha);
}
}
}
