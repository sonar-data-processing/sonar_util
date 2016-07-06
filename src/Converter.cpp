#include "base/MathUtil.hpp"
#include "sonar_util/Converter.hpp"

using namespace base;

namespace sonar_util  {

std::vector<int> Converter::generate_beam_mapping(const std::vector<float>& bins,
                                                             const std::vector<float>& bearings,
                                                             uint32_t bin_count, uint32_t beam_count,
                                                             uint32_t frame_width, uint32_t frame_height) {

    unsigned int max_radius = bin_count;

    float height_to_bin_factor = (float)max_radius / frame_height;

    float first_bearing = bearings[0];
    float last_bearing = bearings[bearings.size()-1];

    int ref_width = MathUtil::aspect_ratio_width(bearings[bearings.size()-1], frame_height);
    float scale_x = frame_width / (float)ref_width;

    int cy = frame_height;
    int cx = ref_width >> 1;

    std::vector<int> beam_mapping(frame_height * frame_width, -1);
    
    for (unsigned int y = 0; y < frame_height; y++) {
        for (unsigned int beam = 0; beam < beam_count-1; beam++) {
            
            float r0 = y;
            float r1 = (y + 1);
            
            float min_angle = bearings[beam];
            float max_angle = bearings[beam+1];
            float theta0 = min_angle - M_PI_2;
            float theta1 = max_angle - M_PI_2;
            
            cv::Rect rc = MathUtil::arc_bounding_box(theta0, theta1, r0, r1, cv::Point(cx, cy));
            
            for (int y = rc.tl().y; y <= rc.br().y && y < frame_height; y++) {
                for (int x = rc.tl().x; x <= rc.br().x && x < ref_width; x++) {
                    int *beam_mapping_ptr = &beam_mapping[y * frame_width + x * scale_x];
                    
                    if (*(beam_mapping_ptr) == -1) {
                        float dx = cx - x;
                        float dy = cy - y;
                        float r = sqrt(dx * dx + dy * dy);
                        float t = atan2(dy, dx) - M_PI_2;
                        
                        if (r <= r1 && r >= r0 && t >= min_angle && t < max_angle) {
                            int bin = r * height_to_bin_factor;
                            *(beam_mapping_ptr) = beam  * bin_count + bin;
                        }
                    }
                }
            }
        }
    }

    return beam_mapping;
}

std::vector<int> Converter::generate_beam_mapping_from_cartesian(const std::vector<float>& bins,
                                                                            const std::vector<float>& bearings,
                                                                            uint32_t bin_count, uint32_t beam_count,
                                                                            uint32_t frame_width, uint32_t frame_height) {

    float min_angle = bearings[0];
    float max_angle = bearings[bearings.size()-1];

    float height_to_bin_factor =  (float)bin_count / frame_height;

    int max_radius = frame_height;
    
    int ref_width = MathUtil::aspect_ratio_width(max_angle, frame_height);
    float scale_x = frame_width / (float)ref_width;
    
    int cy = frame_height;
    int cx = ref_width >> 1;
    
    std::vector<int> beam_mapping(frame_height * frame_width, -1);

    for (int y = 0; y < cy; y++) {
        for (int x = 0; x < ref_width; x++) {
            int *beam_mapping_ptr = &beam_mapping[y * frame_width + x * scale_x];
            if (*(beam_mapping_ptr) == -1) {             
                float dx = cx - x;
                float dy = cy - y;
                float r = sqrt(dx * dx + dy * dy);
                float t = atan2(dy, dx) - M_PI_2;
                
                if (r <= max_radius && r >= 0 && t >= min_angle && t <= max_angle) {
                    int bin = r * height_to_bin_factor;
                    std::vector<float>::const_iterator it = std::upper_bound(bearings.begin(), bearings.end(), t);
                    int beam = it - bearings.begin() - 1;
                    *(beam_mapping_ptr) = beam  * bin_count + bin;
                }
            }
        }
    }

    return beam_mapping;
}

cv::Mat Converter::convert2raw(std::vector<float> data, int number_of_beams, int number_of_bins, int frame_width, int frame_height, color_palletes::PalleteType pallete_type) {

    cv::Mat sonar_image = cv::Mat::zeros(cv::Size(frame_width, frame_height), ((pallete_type == color_palletes::GRAY_PALLETE) ? CV_8UC1 : CV_8UC3));

    const color_palletes::PalleteBase& pallete = color_palletes::Palletes::instance()->pallete(pallete_type);

    for (int y = 0; y < sonar_image.rows; y++) {
        for (int x = 0; x < sonar_image.cols; x++) {
            int sonar_beam = x * number_of_beams / frame_width;
            int sonar_bin = y * number_of_bins / frame_height;

            uint8_t bin = data[sonar_beam * number_of_bins + sonar_bin] * 255;

            if (pallete_type == color_palletes::GRAY_PALLETE) {
                sonar_image.at<uchar>(sonar_image.rows - y - 1, x) = bin;
            } else {
                sonar_image.at<cv::Vec3b>(sonar_image.rows - y - 1, x) = pallete.color(bin);
            }
        }
    }

    return sonar_image;
}

cv::Mat Converter::convert2polar(std::vector<uint8_t> data,
                                            uint16_t number_of_beams,
                                            uint16_t number_of_bins,
                                            float start_bearing,
                                            float angular_resolution,
                                            uint32_t frame_height,
                                            sonar_util::color_palletes::PalleteType pallete_type) {

    float max_angle = start_bearing + angular_resolution * number_of_beams;
    float min_angle = start_bearing;

    int frame_width = MathUtil::aspect_ratio_width(max_angle, frame_height);

    int cy = frame_height;
    int cx = ceil(frame_width / 2.0);
    int max_radius = frame_height;

    cv::Mat sonar_image = cv::Mat::zeros(cv::Size(frame_width, frame_height), ((pallete_type == color_palletes::GRAY_PALLETE) ? CV_8UC1 : CV_8UC1));

    for (int y = 0; y < frame_height; y++) {
        for (int x = 0; x < frame_width; x++) {
            float dx = cx - x;
            float dy = cy - y;
            float r = sqrt(dx * dx + dy * dy);
            float t = atan2(dy, dx) - M_PI_2;

            if (r <= max_radius && r >= 0 && t >= min_angle && t <= max_angle) {
                int sonar_bin = r * number_of_bins / max_radius;
                int sonar_beam = (t - start_bearing) / angular_resolution;
                sonar_image.at<uchar>(y, x) = data[sonar_beam * number_of_bins + sonar_bin];
            }
        }
    }

    return sonar_image;
}


cv::Mat Converter::convert2polar(const std::vector<float>& bins,
                                            const std::vector<float>& bearings,
                                            uint32_t bin_count, uint32_t beam_count,
                                            uint32_t frame_width, uint32_t frame_height,
                                            std::vector<int> beam_mapping,
                                            color_palletes::PalleteType pallete_type) {

    if (beam_mapping.empty()) {
        beam_mapping = generate_beam_mapping_from_cartesian(bins, bearings, bin_count, beam_count, frame_width, frame_height);
    }

    cv::Mat sonar_image = cv::Mat::zeros(cv::Size(frame_width, frame_height), ((pallete_type == color_palletes::GRAY_PALLETE) ? CV_8UC1 : CV_8UC3));

    const color_palletes::PalleteBase& pallete = color_palletes::Palletes::instance()->pallete(pallete_type);

    for (int i = 0; i < beam_mapping.size(); i < i++){
        int y = i / frame_width;
        int x = i % frame_width;

        if (beam_mapping[i] > -1) {
            uint8_t bin = bins[beam_mapping[i]] * 255;

            if (pallete_type == color_palletes::GRAY_PALLETE) {
                sonar_image.at<uchar>(y, x) = bin;
            } else {
                sonar_image.at<cv::Vec3b>(y, x) = pallete.color(bin);
            }

        }
    }

    return sonar_image;
}

} /* namespace sonar_util */
                                                                 
