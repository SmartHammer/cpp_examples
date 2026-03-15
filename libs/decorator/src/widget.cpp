#include "widget.hpp"

#include <iostream>

namespace BuilderPattern
{

    void Widget::draw() const
    {
        // Simple representation of the widget's properties
        std::cout << "Widget Title: " << title_ << "\n"
                  << "Position: (" << x_ << ", " << y_ << ")\n"
                  << "Size: " << width_ << "x" << height_ << std::endl;
    }

    // Implementation can be added here if needed
    void Widget::set(Widget::XPos x) { x_ = x.value; }
    void Widget::set(Widget::YPos y) { y_ = y.value; }
    void Widget::set(Widget::Width width) { width_ = width.value; }
    void Widget::set(Widget::Height height) { height_ = height.value; }
    void Widget::set(Widget::Title title) { title_ = title.value; }
} // namespace BuilderPattern