#ifndef SYKES_BITMAP_FONT_H
#define SYKES_BITMAP_FONT_H

#include <map>
#include <cstddef>
#include "raster_image.h"

namespace sykes{
    struct bitmap_font_data
    {
        raster_image<rgba32_t> bitmap;
        double width;
        double height;
        double horizontal_bearing_x;
        double horizontal_bearing_y;
        double horizontal_advance;
        double vertical_bearing_x;
        double vertical_bearing_y;
        double vertical_advance;
    };
    
    class bitmap_font_set
    {
    public:
        typedef char char_type;
        typedef unsigned int size_type;
        typedef raster_image<rgba32_t> bitmap_type;
        
        bitmap_font_set();
        
        bitmap_font_set(std::vector<char> const& data,
            double height, size_type xreso, size_type yreso);
        
        bool initialize(std::vector<char> const& data,
            double height, size_type xreso, size_type yreso);
        
        bitmap_font_data const& get(char_type c) const;
        
        inline size_type xresolution() const
        { return m_xresolution; }
        
        inline size_type yresolution() const
        { return m_yresolution; }
        
        inline double height() const
        { return m_height; }
        
        inline double ascender() const
        { return m_ascender; }
        
        inline double descender() const
        { return m_descender; }
        
        inline double max_advance() const
        { return m_max_advance; }
        
        inline double pt_to_pixx(double pt) const
        {
            return pt * double(m_xresolution) / 72.0;
        }
        
        inline double pt_to_pixy(double pt) const
        {
            return pt * double(m_yresolution) / 72.0;
        }
        
    private:
        std::map<char_type, bitmap_font_data> m_map;
        size_type m_xresolution;
        size_type m_yresolution;
        double m_height;
        double m_ascender;
        double m_descender;
        double m_max_advance;
    };
}
#endif

