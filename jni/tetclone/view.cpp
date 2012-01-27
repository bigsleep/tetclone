#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstddef>

#include <GLES/gl.h>

#include <ft2build.h>
#include <freetype/freetype.h>

#include "view.h"

#include <android/log.h>
#define  LOG_TAG    "mgame.tetclone"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

namespace mgame{
    view tetclone_view = view(tetclone_model);
    std::array<int, 2> const canvas_size = {{300, 440}};
    std::array<int, 2> const field_pos = {{10, 20}};
    std::array<int, 2> const field_size = {{200, 420}};
    std::array<int, 2> const block_size = {{20, 20}};
    std::array<int, 2> const next_tetromino_pos = {{220, 360}};
    std::array<int, 2> const next_tetromino_size = {{70, 70}};
    std::array<int, 2> const score_pos = {{220, 330}};
    std::array<int, 2> const score_size = {{70, 20}};
    std::array<int, 2> const gameover_pos = {{20, 200}};
    std::array<int, 2> const gameover_size = {{180, 60}};
    double const font_size = 32.0;
    std::size_t font_reso = 600;
    std::array<GLuint, 95> texture = {{}};
    
    void set_color(color_t _c)
    {
        switch(_c)
        {
            case color_t::cyan:
                glColor4f(0, 1.0, 1.0, 1.0);
                return;
            case color_t::blue:
                glColor4f(0, 0, 1.0, 1.0);
                return;
            case color_t::orange:
                glColor4f(1.0, 0.64, 0, 1.0);
                return;
            case color_t::yellow:
                glColor4f(1.0, 1.0, 0, 1.0);
                return;
            case color_t::green:
                glColor4f(0, 1.0, 0, 1.0);
                return;
            case color_t::purple:
                glColor4f(0.5, 0, 0.5, 1.0);
                return;
            case color_t::red:
                glColor4f(1.0, 0, 0, 1.0);
                return;
            default:
                return;
        }
    }
    
    void draw_box(float _w, float _h)
    {
        std::array<float, 8> pos = {
            0.0, 0.0,
            0.0,  _h,
             _w,  _h,
             _w, 0.0};
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, &pos[0]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    
    view::view(tetclone& _tetclone)
        :
        m_frame_width(),
        m_frame_height(),
        m_tetclone(_tetclone),
        m_font_initialized(false),
        m_font()
    {
    }
    
    void view::set_frame_size(size_t w, size_t h)
    {
        //LOGI("frame size: %d, %d", w, h);
        m_frame_width = w;
        m_frame_height = h;
        
        glViewport(0,0, w, h); 
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glShadeModel(GL_SMOOTH);
        double aframe = static_cast<double>(w) / static_cast<double>(h);
        double acanvas  = static_cast<double>(canvas_size[0]) / static_cast<double>(canvas_size[1]);
        if(acanvas > aframe){
            float cw = static_cast<float>(canvas_size[0]);
            float ch = static_cast<float>(canvas_size[0]) / aframe;
            glOrthof(0.0, cw, 0.0, ch, -1.0, 1.0);
        }else{
            float cw = static_cast<float>(canvas_size[1]) * aframe;
            float ch = static_cast<float>(canvas_size[1]);
            glOrthof(0.0, cw, 0.0, ch, -1.0, 1.0);
        }
        glMatrixMode(GL_MODELVIEW);
    }
    
    void view::bind_font_texture() const
    {
        if(m_font_initialized){
            static std::string const ascii =
                " !\"#$%&'()*+,-./"
                "0123456789:;<=>?"
                "@ABCDEFGHIJKLMNO"
                "PQRSTUVWXYZ[\\]^_"
                "`abcdefghijklmno"
                "pqrstuvwxyz{|}~";
            glEnable(GL_TEXTURE_2D);
            glGenTextures(ascii.size(), &texture[0]);
            for(std::size_t i = 0, sz = ascii.size(); i < sz; ++i){
                auto const& fdata = m_font.get(ascii[i]);
                auto const& fimg = fdata.bitmap;
                texture[i] = static_cast<GLuint>(ascii[i]);
                glBindTexture(GL_TEXTURE_2D, texture[i]);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                    fimg.width(), fimg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, fimg.data());
                glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }
            glDisable(GL_TEXTURE_2D);
        }
    }
    
    void view::set_font(std::vector<char> const& data)
    {
        m_font_initialized = m_font.initialize(data, font_size, font_reso, font_reso);
    }
    
    void view::draw() const
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // background
	    glColor4f(0.5, 0.5, 0.5, 1.0);
        draw_box(canvas_size[0], canvas_size[1]);
        
	    // field
        glTranslatef(field_pos[0], field_pos[1], 0.0);
        glColor4f(0.25, 0.25, 0.25, 1.0);
        draw_box(field_size[0], field_size[1]);
	    
        // stockpile
        auto const& stockpile = m_tetclone.stockpile();
        for(std::size_t i = 0, h = stockpile.size(); i < h; ++i){
        for(std::size_t j = 0, w = stockpile[i].size(); j < w; ++j){
            if(stockpile[i][j] != color_t::non){
                glLoadIdentity();
                glTranslatef(field_pos[0], field_pos[1] + block_size[1] * m_tetclone.height, 0.0);
                glRotatef(180.0, 1.0, 0.0, 0.0);
                glTranslatef(block_size[0] * j, block_size[1] * i, 0.0);
                set_color(stockpile[i][j]);
                draw_box(block_size[0], block_size[1]);
            }
        }}
        
        // falling tetromino
        {
            auto const& current = m_tetclone.current_falling();
            auto const& lay = layout(current.tetromino, current.angle);
            auto c = color(current.tetromino);
            for(std::size_t i = 0, sz = lay.size(); i < sz; ++i){
                glLoadIdentity();
                glTranslatef(field_pos[0], field_pos[1] + block_size[1] * m_tetclone.height, 0.0);
                glRotatef(180.0, 1.0, 0.0, 0.0);
                float x = static_cast<float>((current.pos[0] + lay[i][0]) * block_size[0]);
                float y = static_cast<float>((current.pos[1] + lay[i][1] * m_tetclone.ydivision) * block_size[1])
                        / static_cast<float>(m_tetclone.ydivision);
                glTranslatef(x, y, 0.0);
                set_color(c);
                draw_box(block_size[0], block_size[1]);
            }
        }
        
        // next tetromino
        {
            glLoadIdentity();
            glTranslatef(next_tetromino_pos[0], next_tetromino_pos[1], 0.0);
	        glColor4f(0.25, 0.25, 0.25, 1.0);
	        draw_box(next_tetromino_size[0], next_tetromino_size[1]);
            tetromino_t next = m_tetclone.next();
            auto const& lay = layout(next, angle_t::Zero);
            auto c = color(next);
            for(std::size_t i = 0, sz = lay.size(); i < sz; ++i){
                glLoadIdentity();
                glTranslatef(next_tetromino_pos[0],
                    next_tetromino_pos[1] + next_tetromino_size[1], 0.0);
                glRotatef(180.0, 1.0, 0.0, 0.0);
                glScalef(0.5, 0.5, 1.0);
                glTranslatef(40.0, 40.0, 0.0);
                double x = double(lay[i][0]) * 20.0;
                double y = double(lay[i][1]) * 20.0;
                glTranslatef(x, y, 0.0);
                set_color(c);
                draw_box(block_size[0], block_size[1]);
            }
        }
        
        // score
        {
            glLoadIdentity();
            glTranslatef(score_pos[0], score_pos[1], 0.0);
	        glColor4f(0.25, 0.25, 0.25, 1.0);
	        draw_box(score_size[0], score_size[1]);
	        
	        int score = m_tetclone.score();
	        std::ostringstream o;
	        o << std::setw(8) << score;
	        glLoadIdentity();
	        glTranslatef(score_pos[0], score_pos[1], 0.0);
            glColor4f(1.0, 1.0, 1.0, 1.0);
	        draw_string(o.str(), score_size[0], score_size[1]);
        }
        
        if(m_tetclone.gameover()){
            glLoadIdentity();
            glTranslatef(gameover_pos[0], gameover_pos[1], 0.0);
            glColor4f(0.0, 0.0, 0.0, 1.0);
            draw_string(std::string("GAME OVER"), gameover_size[0], gameover_size[1]);
        }
    }
    void view::draw_character(char c, double w, double h) const
    {
    
        if(!m_font_initialized) return;
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        
        
        auto const& fdata = m_font.get(c);
        auto const& fimg = fdata.bitmap;
        double height = m_font.height();
        double th = double(fimg.height());
        double tw = double(fimg.width());
        double fh = fdata.height;
        double fw = fdata.width;
        double as = m_font.ascender();
        double de = m_font.descender();
        double advance = fdata.horizontal_advance;
        double bx = fdata.horizontal_bearing_x;
        double by = fdata.horizontal_bearing_y;
        
        float OX = float(w * bx / advance);
        float OY = float(h * (by - de - fh) / height);
        float X = OX + w * fw / advance;
        float Y = OY + h * fh / height;
        float w_ = (float)w, h_ = (float)h;
        float vertices[] = {
            X,   Y, 0.0,
            OX,  Y, 0.0,
            OX, OY, 0.0,
             X,  Y, 0.0,
            OX, OY, 0.0,
             X, OY, 0.0};
        
        float ox = 0.0;
        float oy = 0.0;
        float x = float(fw / tw);
        float y = float(fh / th);
        float tvertices[] = {
             x,  y,
            ox,  y,
            ox, oy,
             x,  y,
            ox, oy,
             x, oy};
        glBindTexture(GL_TEXTURE_2D, c);
        glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
        glTexCoordPointer(2, GL_FLOAT, 0, &tvertices[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glTranslatef(w_, 0.0, 0.0);
    }
    
    void view::draw_string(std::string const& str, double w, double h) const
    {
        if(!m_font_initialized) return;
        std::vector<double> advance(str.size());
        double asum = 0.0;
        for(std::size_t i = 0, sz = str.size(); i < sz; ++i){
            advance[i] = m_font.get(str[i]).horizontal_advance;
            asum += advance[i];
        }
        for(std::size_t i = 0, sz = str.size(); i < sz; ++i){
            draw_character(str[i], w * advance[i] / asum, h);
        }
    }
    
    std::size_t view::width() const
    {
        return m_frame_width;
    }
    
    std::size_t view::height() const
    {
        return m_frame_height;
    }
}//---- namespace



