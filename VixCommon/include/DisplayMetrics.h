#pragma once

namespace VixCommon {

struct DisplayMetrics {
  // High resolution displays can require a lot of GPU and battery power to render.
  // High resolution phones, for example, may suffer from poor battery life if
  // games attempt to render at 60 frames per second at full fidelity.
  // The decision to render at full fidelity across all platforms and form factors
  // should be deliberate.
  DisplayMetrics(const bool support_high_resolutions,
                 const float dpi_threshold,
                 const float width_threshold,
                 const float height_threshold) :
    m_support_high_resolutions(support_high_resolutions),
    m_dpi_threshold(dpi_threshold),
    m_width_threshold(width_threshold),
    m_height_threshold(height_threshold) { }

  DisplayMetrics() : DisplayMetrics(false,
                                    192.0f,
                                    1920.0f,
                                    1080.0f) { }

  const bool m_support_high_resolutions;

  // The default thresholds that define a "high resolution" display. If the thresholds
  // are exceeded and SupportHighResolutions is false, the dimensions will be scaled
  // by 50%.
  const float m_dpi_threshold;		// 200% of standard desktop display.
  const float m_width_threshold;	// 1080p width.
  const float m_height_threshold;	// 1080p height.
};

} // VixCommon