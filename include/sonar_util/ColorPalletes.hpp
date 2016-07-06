#ifndef ColorPalletes_hpp
#define ColorPalletes_hpp

#include <cstdio>
#include <vector>
#include <opencv2/opencv.hpp>

namespace sonar_util {

namespace color_palletes {

class PalleteBase {
public:
    PalleteBase() {}
    virtual ~PalleteBase() {}

    void init();

    cv::Vec3b color(uchar value) const;
    cv::Vec3f color(float value) const;

protected:

    virtual void create_color_map(std::vector<cv::Vec4f>& color) {
        color.push_back(cv::Vec4f(0, 0, 0, 0)); // Black
        color.push_back(cv::Vec4f(1, 1, 1, 1)); // White
    }

private:
    std::vector<cv::Vec4f> color_map_;
};

class HeatMap : public PalleteBase {
public:

    HeatMap() {}
    virtual ~HeatMap() {}

    virtual void create_color_map(std::vector<cv::Vec4f>& color) {
        color.push_back(cv::Vec4f(0, 0, 0, 0.0f));     // black
        color.push_back(cv::Vec4f(0, 0, 1, 0.125f));   // red
        color.push_back(cv::Vec4f(0, 1, 1, 0.55f));    // yellow
        color.push_back(cv::Vec4f(1, 1, 1, 1.0f));     // white
    }
};

class JetMap : public PalleteBase {
public:

    JetMap() {}
    virtual ~JetMap() {}

    virtual void create_color_map(std::vector<cv::Vec4f>& color) {
        color.push_back(cv::Vec4f(0, 0, 0, 0.0f));      // black
        color.push_back(cv::Vec4f(1, 0, 0, 0.125f));    // blue
        color.push_back(cv::Vec4f(1, 1, 0, 0.25f));     // cyan
        color.push_back(cv::Vec4f(0, 1, 0, 0.50f));     // green
        color.push_back(cv::Vec4f(0, 1, 1, 0.75f));     // yellow
        color.push_back(cv::Vec4f(0, 0, 1, 0.825f));    // red
        color.push_back(cv::Vec4f(1, 1, 1, 1.0f));      // white
    }
};

enum PalleteType {
    GRAY_PALLETE = 0,
    HEATMAP_PALLETE = 1,
    JETMAP_PALLETE = 2
};

class Palletes {
public:
    static Palletes* instance() {
        if (!instance_) {
            instance_ = new Palletes();
            instance_->load_palletes();
        }

        return instance_;
    }

    size_t count() {
        return palletes_.size();
    }

    const PalleteBase& pallete(PalleteType type) const {
        return (*palletes_[type]);
    }

private:
    Palletes() {}
    ~Palletes() {}

    void load_palletes() {
        add_palletes();
        init_palletes();
    }

    void add_palletes();

    void init_palletes();

    std::vector<PalleteBase*> palletes_;

    static Palletes *instance_;
};

} /* namespace color_palletes */

} /* namespace sonar_util */

#endif /* ColorPalletes_hpp */
