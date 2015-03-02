/* A basic exponential moving average filter.
 * http://en.wikipedia.org/wiki/Moving_average#Exponential_moving_average
 */

#include "InfiniteImpulseResponseFilter.h"

class ExponentialMovingAverageFilter : public InfiniteImpulseResponseFilter {
 public:
  explicit ExponentialMovingAverageFilter(DeviceListenerWrapper& parent_feature,
                                          DataFlags flags, float alpha);

 private:
  virtual float Update(float new_value, float old_value) override;

  float alpha_;
};

ExponentialMovingAverageFilter::ExponentialMovingAverageFilter(
    DeviceListenerWrapper& parent_feature, DataFlags flags, float alpha)
    : InfiniteImpulseResponseFilter(parent_feature, flags), alpha_(alpha) {}

float ExponentialMovingAverageFilter::Update(float new_value, float old_value) {
  return (alpha_ * new_value) + ((1 - alpha_) * old_value);
}

ExponentialMovingAverageFilter make_exponential_moving_average_filter(
    DeviceListenerWrapper& parent_feature,
    ExponentialMovingAverageFilter::DataFlags flags, float alpha) {
  return ExponentialMovingAverageFilter(parent_feature, flags, alpha);
}
