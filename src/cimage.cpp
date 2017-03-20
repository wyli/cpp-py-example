#include "cimage.h"


Cimage::Cimage(size_t width, size_t height)
    : _width(width)
    , _height(height)
{

}

Cimage::~Cimage()
{

}

size_t Cimage::width()
{
    return _width;
}

size_t Cimage::height()
{
    return _height;
}