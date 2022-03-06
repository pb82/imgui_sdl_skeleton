#ifndef SKELETON_PERFORMANCE_H
#define SKELETON_PERFORMANCE_H

#include <deque>

namespace overlays {
    class Performance {
    public:
        Performance(int recordSeconds);

        void add(float f);

        void render();
    private:
        float targetFrametime = 0.0f;
        std::deque<float> frametimes;
    };
}

#endif //SKELETON_PERFORMANCE_H
