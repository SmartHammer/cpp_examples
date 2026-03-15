#pragma once

#include <string>

/**
 * @file widget.hpp
 * @brief Definition of the Widget class using the Builder Pattern with named parameters.
 * @note according to Joshua Bloch's "Effective Java" Item 2: "Consider a builder when faced with many constructor parameters"
 */
namespace BuilderPattern
{    
    class Widget
    {
    public:
        struct XPos { int value; };
        struct YPos { int value; };
        struct Width { unsigned int value; };
        struct Height { unsigned int value; };
        struct Title { std::string value; };

        template<typename... Args>
        Widget(Args... args)
        {
            (set(args), ...);
        }

        void draw() const;

    private:
        void set(XPos x);
        void set(YPos y);
        void set(Width width);
        void set(Height height);
        void set(Title title);

        int x_{0};
        int y_{0};
        unsigned int width_{0};
        unsigned int height_{0};
        std::string title_{""};
    };
}