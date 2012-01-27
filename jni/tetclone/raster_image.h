#ifndef SYKES_RASTER_IMAGE_H
#define SYKES_RASTER_IMAGE_H
#include <vector>
#include <stdexcept>
#include <cstdint>
#include <cstddef>
#include <cassert>

namespace sykes{
    typedef unsigned char byte;
    
    template<std::size_t N>
    struct bytes
    {
        byte data[N];
    };
    
    template<typename T>
    struct gray_t
    {
        T g;
    };
    
    template<typename T>
    struct ga_t
    {
        T g;
        T a;
    };
    
    template<typename T>
    struct rgb_t
    {
        T r;
        T g;
        T b;
    };
    
    template<typename T>
    struct rgba_t
    {
        T r;
        T g;
        T b;
        T a;
    };
    
    typedef gray_t<byte> gray8_t;
    typedef rgb_t<byte> rgb24_t;
    typedef rgb_t<bytes<2>> rgb48_t;
    typedef rgba_t<byte> rgba32_t;
    typedef rgba_t<bytes<2>> rgba64_t;
    
    template<typename ColorType>
    class raster_image
    {
    public:
        typedef std::size_t size_type;
        typedef ColorType color_type;
    
    private:
        size_type m_width;
        size_type m_height;
        std::vector<color_type> m_data;
        
    public:
        raster_image()
            : m_width(), m_height(), m_data()
        {}
        
        raster_image(size_type _w, size_type _h)
            : m_width(_w), m_height(_h), m_data(_w * _h)
        {
        }
        
        size_type width() const
        { return m_width; }
        
        size_type height() const
        { return m_height; }
        
        void resize(size_t _w, size_t _h)
        {
            m_width = _w;
            m_height = _h;
            m_data.resize(_w * _h, color_type{});
        }
        
        color_type* data()
        {
            return &m_data[0];
        }
        
        color_type const* data() const
        {
            return &m_data[0];
        }
        
        color_type& operator()(size_t _x, size_t _y)
        {
            assert(_x < m_width && _y < m_height);
            return m_data[_y * m_width + _x];
        }
        
        color_type const& operator()(size_t _x, size_t _y) const
        {
            assert(_x < m_width && _y < m_height);
            return m_data[_y * m_width + _x];
        }
        
        color_type& at(size_t _x, size_t _y)
        {
            if(!(_x < m_width && _y < m_height))
                throw std::out_of_range("sykes::raster_image::at()");
            return m_data[_y * m_width + _x];
        }
        
        color_type const& at(size_t _x, size_t _y) const
        {
            if(!(_x < m_width && _y < m_height))
                throw std::out_of_range("sykes::raster_image::at()");
            return m_data[_y * m_width + _x];
        }
    };
    
    template<typename ColorType>
    class color_converter;
    
    template<>
    class color_converter<rgb24_t>
    {
    public:
        typedef rgb24_t color_type;
        
        inline static color_type convert(rgb24_t const& c)
        {
            return c;
        }
        
        inline static color_type convert(rgb48_t const& c)
        {
            byte r = (*(unsigned short*)(&c.r)) >> 8;
            byte g = (*(unsigned short*)(&c.g)) >> 8;
            byte b = (*(unsigned short*)(&c.b)) >> 8;
            return rgb24_t{r, g, b};
        }
        
        inline static color_type convert(rgba32_t const& c)
        {
            return rgb24_t{c.r, c.g, c.b};
        }
        
        inline static color_type convert(rgba64_t const& c)
        {
            byte r = (*(unsigned short*)(&c.r)) >> 8;
            byte g = (*(unsigned short*)(&c.g)) >> 8;
            byte b = (*(unsigned short*)(&c.b)) >> 8;
            return rgb24_t{r, g, b};
        }
    };
    
    template<>
    class color_converter<rgba32_t>
    {
    public:
        typedef rgba32_t color_type;
        
        inline static color_type convert(rgb24_t const& c)
        {
            return rgba32_t{c.r, c.g, c.b, 0};
        }
        
        inline static color_type convert(rgb48_t const& c)
        {
            byte r = (*(unsigned short*)(&c.r)) >> 8;
            byte g = (*(unsigned short*)(&c.g)) >> 8;
            byte b = (*(unsigned short*)(&c.b)) >> 8;
            return rgba32_t{r, g, b, 0};
        }
        
        inline static color_type convert(rgba32_t const& c)
        {
            return c;
        }
        
        inline static color_type convert(rgba64_t const& c)
        {
            byte r = (*(unsigned short*)(&c.r)) >> 8;
            byte g = (*(unsigned short*)(&c.g)) >> 8;
            byte b = (*(unsigned short*)(&c.b)) >> 8;
            byte a = (*(unsigned short*)(&c.a)) >> 8;
            return rgba32_t{r, g, b, a};
        }
    };
}

#endif













