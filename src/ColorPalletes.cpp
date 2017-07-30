#include <cstdio>
#include <cmath>
#include <stdexcept>
#include "ColorPalletes.hpp"

namespace sonar_util {

namespace color_palletes {

void PalleteBase::init() {
    create_color_map(color_map_);
}

cv::Vec3b PalleteBase::color(uchar value) const {
    cv::Vec3f colorf = color(value / 255.0f);
    return cv::Vec3b(colorf[0] * 255, colorf[1] * 255, colorf[2] * 255);
}

cv::Vec3f PalleteBase::color(float value) const {

    if (color_map_.size() == 0) {
        throw std::runtime_error("color map is empty");
    }

    for (size_t i = 0; i < color_map_.size(); i++) {
        cv::Vec4f current_color = color_map_[i];

        if (value < current_color[3]) {
            cv::Vec4f previous_color  = color_map_[MAX(0, i-1)];
            float value_diff = (current_color[3] - previous_color[3]);
            float fract_between = (value_diff == 0) ? 0 : (current_color[3] - value) / value_diff;

            cv::Vec3f result_color(0, 0, 0);
            result_color[0] = (previous_color[0] - current_color[0]) * fract_between + current_color[0];
            result_color[1] = (previous_color[1] - current_color[1]) * fract_between + current_color[1];
            result_color[2] = (previous_color[2] - current_color[2]) * fract_between + current_color[2];

            return result_color;
        }
    }

    cv::Vec4f c = color_map_.back();
    return cv::Vec3f(c[0], c[1], c[2]);
}

Palletes *Palletes::instance_ = NULL;

void Palletes::add_palletes() {
    palletes_.push_back(new PalleteBase());
    palletes_.push_back(new HeatMap());
    palletes_.push_back(new JetMap());
}

void Palletes::init_palletes() {
   for (size_t i = 0; i < palletes_.size(); i++) palletes_[i]->init();
}

} /* namespace color_palletes */

} /* namespace sonar_util */
