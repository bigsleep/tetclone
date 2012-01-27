#ifndef VIEW_H
#define VIEW_H

#include <vector>
#include <array>
#include <string>
#include <cstddef>
#include "tetclone.h"
#include "bitmap_font.h"

namespace mgame{
    class view
    {
    public:
        view(tetclone& _tetclone);
        void set_frame_size(std::size_t w, std::size_t h);
        void set_font(std::vector<char> const& data);
        void draw() const;
        void step();
        std::size_t width() const;
        std::size_t height() const;
        void bind_font_texture() const;
        
    private:
        std::size_t m_frame_width;
        std::size_t m_frame_height;
        tetclone& m_tetclone;
        bool m_font_initialized;
        sykes::bitmap_font_set m_font;
        
        void draw_character(char c, double w, double h) const;
        void draw_string(std::string const& str, double w, double h) const;
    };
    
    extern view tetclone_view;
    extern std::array<int, 2> const canvas_size;
    extern std::array<int, 2> const block_size;
}//---- namespace

#endif

