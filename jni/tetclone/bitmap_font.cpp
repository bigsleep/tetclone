#include <vector>
#include <iostream>
#include <cstddef>
#include <cmath>
#include <ft2build.h>
#include <freetype/freetype.h>
#include "bitmap_font.h"

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)
#include <android/log.h>
#define  LOG_TAG    "mgame.tetclone"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

namespace sykes{
    template<typename T>
    T to_fixed(double a)
    {
        return static_cast<T>(a * 64.0);
    }
    
    template<typename T>
    double fixed_to_d(T const& a)
    {
        return static_cast<double>(a) / 64.0;
    }
    
    std::size_t larger_2n(std::size_t a)
    {
        std::size_t tmp = 1;
        while(tmp < a){
            tmp *= 2;
        }
        return tmp;
    }
    
    bitmap_font_set::bitmap_font_set()
        :
        m_map(),
        m_xresolution(),
        m_yresolution(),
        m_height(),
        m_ascender(),
        m_descender(),
        m_max_advance()
    {}
    
    bitmap_font_set::bitmap_font_set(std::vector<char> const& data,
        double height, size_type xreso, size_type yreso)
        :
        m_map(),
        m_xresolution(xreso),
        m_yresolution(yreso),
        m_height(),
        m_ascender(),
        m_descender(),
        m_max_advance()
    {
        initialize(data, height, xreso, yreso);
    }
    
    
    bool bitmap_font_set::initialize(std::vector<char> const& data,
        double height, size_type xreso, size_type yreso)
    {
        FT_Library library;
        FT_Face face;
        FT_UInt glyph_index;
        
        FT_Error init = FT_Init_FreeType( &library );
        if(init){
            FT_Done_FreeType(library);
            return false;
        }
        
        FT_Error load = FT_New_Memory_Face(library, (unsigned char const*)&data[0], data.size(), 0, &face);
        if(load){
            FT_Done_FreeType(library);
            return false;
        }
        
        FT_GlyphSlot slot = face->glyph;
        FT_Error setsize = FT_Set_Char_Size( face, 0, to_fixed<unsigned int>(height), xreso, yreso);
        if(setsize){
            FT_Done_FreeType(library);
            return false;
        }
        
        m_height = fixed_to_d(face->size->metrics.height);
        m_ascender = fixed_to_d(face->size->metrics.ascender);
        m_descender = fixed_to_d(face->size->metrics.descender);
        m_max_advance = fixed_to_d(face->size->metrics.max_advance);
        
        LOGI("font height: %f", m_height);
        LOGI("font ascender: %f", m_ascender);
        LOGI("font descender: %f", m_descender);
        LOGI("font max_advance: %f", m_max_advance);
        
        sykes::raster_image<sykes::rgba32_t> img;
        static std::string const ascii =
            " !\"#$%&'()*+,-./"
            "0123456789:;<=>?"
            "@ABCDEFGHIJKLMNO"
            "PQRSTUVWXYZ[\\]^_"
            "`abcdefghijklmno"
            "pqrstuvwxyz{|}~";
        for(int c = 0, sz = ascii.size(); c < sz; ++c){
            FT_Error loadchar = FT_Load_Char(face, ascii[c], FT_LOAD_RENDER);
            if(loadchar){
                FT_Done_FreeType(library);
                return false;
            }
            
            std::size_t w = slot->bitmap.width;
            std::size_t h = slot->bitmap.rows;
            std::size_t w_ = std::max(larger_2n(w), larger_2n(h)), h_ = w_;
            img.resize(w_, h_);
            for(std::size_t i = 0; i < w_; ++i){
            for(std::size_t j = 0; j < h_; ++j){
                if(i < w && j < h){
                    sykes::byte gray = slot->bitmap.buffer[i + j * w];
                    sykes::byte transparent = (gray == 0) ? 0 : 255;
                    img(i, h - j - 1) = sykes::rgba32_t{0, 0, 0, transparent};
                }else{
                    img(i, j) = sykes::rgba32_t{0, 0, 0, 0};
                }
            }}
            bitmap_font_data d;
            d.width = fixed_to_d(slot->metrics.width);
            d.height = fixed_to_d(slot->metrics.height);
            d.horizontal_bearing_x = fixed_to_d(slot->metrics.horiBearingX);
            d.horizontal_bearing_y = fixed_to_d(slot->metrics.horiBearingY);
            d.horizontal_advance = fixed_to_d(slot->metrics.horiAdvance);
            d.vertical_bearing_x = fixed_to_d(slot->metrics.vertBearingX);
            d.vertical_bearing_y = fixed_to_d(slot->metrics.vertBearingY);
            d.vertical_advance = fixed_to_d(slot->metrics.vertAdvance);
            m_map[ascii[c]] = d;
            m_map[ascii[c]].bitmap = img;
        }
        
        FT_Done_FreeType(library);
        return true;
    }
    
    bitmap_font_data const& bitmap_font_set::get(char_type c) const
    {
        return m_map.at(c);
    }
}


