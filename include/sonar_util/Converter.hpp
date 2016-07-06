#ifndef Converter_hpp
#define Converter_hpp

#include <opencv2/opencv.hpp>
#include "sonar_util/ColorPalletes.hpp"

namespace sonar_util {

class Converter {
public:
    static std::vector<int> generate_beam_mapping(const std::vector<float>& bins,
                                                  const std::vector<float>& bearings,
                                                  uint32_t bin_count, uint32_t beam_count,
                                                  uint32_t frame_width, uint32_t frame_height);
                                                  
    static std::vector<int> generate_beam_mapping_from_cartesian(const std::vector<float>& bins,
                                                                 const std::vector<float>& bearings,
                                                                 uint32_t bin_count, uint32_t beam_count,
                                                                 uint32_t frame_width, uint32_t frame_height);

    static cv::Mat convert2polar(std::vector<uint8_t> data,
                                 uint16_t number_of_beams,
                                 uint16_t number_of_bins,
                                 float start_bearing,
                                 float angular_resolution,
                                 uint32_t frame_height = 512,
                                 sonar_util::color_palletes::PalleteType pallete_type = sonar_util::color_palletes::GRAY_PALLETE);

    static cv::Mat convert2polar(const std::vector<float>& bins,
                                 const std::vector<float>& bearings,
                                 uint32_t bin_count, uint32_t beam_count,
                                 uint32_t frame_width = 512, uint32_t frame_height = 512,
                                 std::vector<int> beam_mapping = std::vector<int>(),
                                 sonar_util::color_palletes::PalleteType pallete_type = sonar_util::color_palletes::GRAY_PALLETE);

    static cv::Mat convert2raw(std::vector<float> data, 
                               int number_of_beams, int number_of_bins,
                               int frame_width, int frame_height,
                               sonar_util::color_palletes::PalleteType pallete_type);

};

}

#endif /* Converter_hpp */
