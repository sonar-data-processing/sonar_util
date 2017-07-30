#ifndef SonarUtil_Plot_hpp
#define SonarUtil_Plot_hpp

#include <vector>
#include <opencv2/opencv.hpp>
#include "Converter.hpp"

namespace sonar_util {

namespace plot {

void polarshow(const std::string name,
                const std::vector<float>& bins,
                const std::vector<float>& bearings,
                uint32_t bin_count, uint32_t beam_count,
                uint32_t frame_width, uint32_t frame_height,
                std::vector<int> beam_mapping = std::vector<int>(),
                color_palletes::PalleteType pallete_type = color_palletes::GRAY_PALLETE);

void polarshow(const std::string name,
                const std::vector<float>& bins,
                const std::vector<float>& bearings,
                uint32_t bin_count, uint32_t beam_count,
                uint32_t frame_height = 512,
                std::vector<int> beam_mapping = std::vector<int>(),
                color_palletes::PalleteType pallete_type = color_palletes::GRAY_PALLETE);

} /* namespace plot */

}  /* namespace sonar_util */


#endif /* Plot_hpp */
