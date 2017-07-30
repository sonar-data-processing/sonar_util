#include "base/MathUtil.hpp"
#include "Plot.hpp"

namespace sonar_util {

namespace plot {

void polarshow(const std::string name,
                const std::vector<float>& bins,
                const std::vector<float>& bearings,
                uint32_t bin_count, uint32_t beam_count,
                uint32_t frame_width, uint32_t frame_height,
                std::vector<int> beam_mapping,
                color_palletes::PalleteType pallete_type) {

    cv::Mat mat = Converter::convert2polar(
        bins, 
        bearings,
        bin_count,
        beam_count,
        frame_width,
        frame_height,
        beam_mapping,
        pallete_type
    );

    cv::imshow(name.c_str(), mat);
} /* function show_polar */

void polarshow(const std::string name,
                const std::vector<float>& bins,
                const std::vector<float>& bearings,
                uint32_t bin_count, uint32_t beam_count,
                uint32_t frame_height,
                std::vector<int> beam_mapping,
                color_palletes::PalleteType pallete_type) {
    
    float angle = bearings[bearings.size()-1];
    uint32_t frame_width = base::MathUtil::aspect_ratio_width(angle, frame_height);
    polarshow(name, bins, bearings, bin_count, beam_count,
               frame_width, frame_height, beam_mapping, pallete_type);
    
} /* function show_polar */


} /* namespace plot */

} /* namespace sonar_util */
