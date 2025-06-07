#pragma once

namespace golxx {
    class GameObject {
    public:
        virtual ~GameObject() = default;

        virtual void init() {}

        virtual void update(float deltaTime) {}

        virtual void render() {}
    };
}
