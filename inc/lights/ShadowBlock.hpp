#ifndef __SHADOWBLOCK_HPP__
    #define __SHADOWBLOCK_HPP__

    #include <iostream>
    #include <list>
    #include <tuple>
    #include <vector>
    #include <memory>
    #include <array>
    #include "lights/ShadowCaster.hpp"

    class ShadowBlock{
        public:
            ShadowBlock();
            ~ShadowBlock();

            bool load(XMLNode *node, ShadowCaster *shadowCaster);

            const float x(void) const;
            const float y(void) const;
            const float angle(void) const;
            void pos(float *x, float *y) const;

            void x(const float x);
            void y(const float y);
            void angle(const float angle);
            void pos(const float x, const float y);

            const int size(void) const;

            void OnTick(void);

        private:

            float _x, _y;
            float _angle;

            std::unique_ptr<std::tuple<float, float, ShadowCaster::Edge*, std::array<bool, 4>>[]> edges;
            int _size;
            ShadowCaster* shadowCaster;

            bool load_edge(std::tuple<float, float, ShadowCaster::Edge*, std::array<bool, 4>> &edge, XMLNode *node);
    };

#endif