#include <iostream>
#include <functional>
#include <algorithm>
#include <ctime>
#include "tetclone.h"

#include <android/log.h>
#define  LOG_TAG    "mgame.tetclone"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

namespace mgame
{
    std::size_t const level_step = 60 * 5;
    tetclone tetclone_model;
    
    std::array<point2i, 4> const& layout(tetromino_t _tet, angle_t _ang)
    {
        typedef std::array<point2i, 4> layout_t;
        switch(_tet)
        {
            case tetromino_t::I:
            {
                static layout_t const
                    i1 = {{ {{0, 1}}, {{1, 1}}, {{2, 1}}, {{3, 1}} }};
                static layout_t const
                    i2 = {{ {{1, 0}}, {{1, 1}}, {{1, 2}}, {{1, 3}} }};
                static layout_t const
                    i3 = {{ {{0, 2}}, {{1, 2}}, {{2, 2}}, {{3, 2}} }};
                static layout_t const
                    i4 = {{ {{2, 0}}, {{2, 1}}, {{2, 2}}, {{2, 3}} }};
                switch(_ang){
                    case angle_t::Zero:      return i1;
                    case angle_t::Fifteen:   return i2;
                    case angle_t::Thirty:    return i3;
                    case angle_t::FortyFive: return i4;
                }
            }
            case tetromino_t::J:
            {
                static layout_t const
                    j1 = {{ {{0, 1}}, {{1, 1}}, {{2, 1}}, {{2, 2}} }};
                static layout_t const
                    j2 = {{ {{1, 0}}, {{1, 1}}, {{1, 2}}, {{2, 0}} }};
                static layout_t const
                    j3 = {{ {{0, 0}}, {{0, 1}}, {{1, 1}}, {{2, 1}} }};
                static layout_t const
                    j4 = {{ {{1, 0}}, {{1, 1}}, {{1, 2}}, {{0, 2}} }};
                switch(_ang){
                    case angle_t::Zero:      return j1;
                    case angle_t::Fifteen:   return j2;
                    case angle_t::Thirty:    return j3;
                    case angle_t::FortyFive: return j4;
                }
            }
            case tetromino_t::L:
            {
                static layout_t const
                    l1 = {{ {{0, 1}}, {{0, 2}}, {{1, 1}}, {{2, 1}} }};
                static layout_t const
                    l2 = {{ {{1, 0}}, {{1, 1}}, {{1, 2}}, {{2, 2}} }};
                static layout_t const
                    l3 = {{ {{0, 1}}, {{1, 1}}, {{2, 0}}, {{2, 1}} }};
                static layout_t const
                    l4 = {{ {{0, 0}}, {{1, 0}}, {{1, 1}}, {{1, 2}} }};
                switch(_ang){
                    case angle_t::Zero:      return l1;
                    case angle_t::Fifteen:   return l2;
                    case angle_t::Thirty:    return l3;
                    case angle_t::FortyFive: return l4;
                }
            }
            case tetromino_t::O:
            {
                static layout_t const
                    o = {{ {{0, 0}}, {{0, 1}}, {{1, 0}}, {{1, 1}} }};
                switch(_ang){
                    case angle_t::Zero:
                    case angle_t::Fifteen:
                    case angle_t::Thirty:
                    case angle_t::FortyFive: return o;
                }
            }
            case tetromino_t::S:
            {
                static layout_t const
                    s1 = {{ {{0, 2}}, {{1, 1}}, {{1, 2}}, {{2, 1}} }};
                static layout_t const
                    s2 = {{ {{1, 0}}, {{1, 1}}, {{2, 1}}, {{2, 2}} }};
                static layout_t const
                    s3 = {{ {{0, 1}}, {{1, 0}}, {{1, 1}}, {{2, 0}} }};
                static layout_t const
                    s4 = {{ {{0, 0}}, {{0, 1}}, {{1, 1}}, {{1, 2}} }};
                switch(_ang){
                    case angle_t::Zero:      return s1;
                    case angle_t::Fifteen:   return s2;
                    case angle_t::Thirty:    return s3;
                    case angle_t::FortyFive: return s4;
                }
            }
            case tetromino_t::T:
            {
                static layout_t const
                    t1 = {{ {{0, 1}}, {{1, 1}}, {{1, 2}}, {{2, 1}} }};
                static layout_t const
                    t2 = {{ {{1, 0}}, {{1, 1}}, {{1, 2}}, {{2, 1}} }};
                static layout_t const
                    t3 = {{ {{0, 1}}, {{1, 0}}, {{1, 1}}, {{2, 1}} }};
                static layout_t const
                    t4 = {{ {{0, 1}}, {{1, 0}}, {{1, 1}}, {{1, 2}} }};
                switch(_ang){
                    case angle_t::Zero:      return t1;
                    case angle_t::Fifteen:   return t2;
                    case angle_t::Thirty:    return t3;
                    case angle_t::FortyFive: return t4;
                }
            }
            case tetromino_t::Z:
            {
                static layout_t const
                    z1 = {{ {{0, 1}}, {{1, 1}}, {{1, 2}}, {{2, 2}} }};
                static layout_t const
                    z2 = {{ {{1, 1}}, {{1, 2}}, {{2, 0}}, {{2, 1}} }};
                static layout_t const
                    z3 = {{ {{0, 0}}, {{1, 0}}, {{1, 1}}, {{2, 1}} }};
                static layout_t const
                    z4 = {{ {{0, 1}}, {{0, 2}}, {{1, 0}}, {{1, 1}} }};
                switch(_ang){
                    case angle_t::Zero:      return z1;
                    case angle_t::Fifteen:   return z2;
                    case angle_t::Thirty:    return z3;
                    case angle_t::FortyFive: return z4;
                }
            }
        }
    }
    
    std::size_t fall_speed(std::size_t _level)
    {
        switch(_level)
        {
            case 0:
                return 1;
            case 1:
                return 2;
            case 2:
                return 3;
            case 3:
                return 4;
            case 4:
                return 5;
            case 5:
                return 6;
            case 6:
                return 7;
            case 7:
                return 8;
            case 8:
                return 9;
            case 9:
                return 10;
            default:
                return 10;
        }
    }
    
    color_t color(tetromino_t _tet)
    {
        switch(_tet){
            case tetromino_t::I: return color_t::cyan;
            case tetromino_t::J: return color_t::blue;
            case tetromino_t::L: return color_t::orange;
            case tetromino_t::O: return color_t::yellow;
            case tetromino_t::S: return color_t::green;
            case tetromino_t::T: return color_t::purple;
            case tetromino_t::Z: return color_t::red;
        }
    }
    
    angle_t rotate(angle_t _ang)
    {
        switch(_ang){
            case angle_t::Zero:         return angle_t::Fifteen;
            case angle_t::Fifteen:      return angle_t::Thirty;
            case angle_t::Thirty:       return angle_t::FortyFive;
            case angle_t::FortyFive:    return angle_t::Zero;
        }
    }
    
    std::size_t score(std::size_t _lines)
    {
        switch(_lines){
            case 0:
                return 0;
            case 1:
                return 40;
            case 2:
                return 100;
            case 3:
                return 300;
            case 4:
                return 1200;
            default:
                return 0;
        }
    }
    
    tetromino_t tetclone::m_rand()
    {
        static std::uniform_int<> dist(0, 6);
        return static_cast<tetromino_t>(dist(m_gen) % 7);
    }
        
    tetclone::tetclone()
        :
        m_stockpile(),
        m_current(),
        m_next(),
        m_gameover(false),
        m_level(),
        m_level_count(),
        m_level_step(level_step),
        m_fall_count(),
        m_fall_speed(fall_speed(m_level)),
        m_score(),
        m_gen(std::time(0))
    {
        std::array<color_t, width> row;
        std::fill(row.begin(), row.end(), color_t::non);
        std::fill(m_stockpile.begin(), m_stockpile.end(), row);
        
        m_current.tetromino = m_rand();
        m_current.angle = angle_t::Zero;
        m_current.pos = point2i{{0, 0}};
        m_next = m_rand();
    }
    
    void tetclone::on_right()
    {
        if(!m_gameover)
            move_falling_tetromino(point2i{{1, 0}});
    }
    
    void tetclone::on_left()
    {
        if(!m_gameover)
            move_falling_tetromino(point2i{{-1, 0}});
    }
    
    void tetclone::on_down()
    {
        if(!m_gameover){
            bool bottom = !move_falling_tetromino(point2i{{0, ydivision}});
            if(bottom) on_down_bottom();
        }
    }
    
    void tetclone::on_rotate()
    {
        if(!m_gameover)
            rotate_falling_tetromino();
    }
    
    void tetclone::on_down_bottom()
    {
        color_t c = color(m_current.tetromino);
        point2i const& p = m_current.pos;
        auto const& lay = layout(m_current.tetromino, m_current.angle);
        for(std::size_t i = 0; i < 4; ++i){
            int x = p[0] + lay[i][0];
            int y = p[1] + lay[i][1] * ydivision;
            int yp = y / ydivision + ((y % ydivision != 0) ? 1 : 0);
            m_stockpile[yp][x] = c;
        }
        
        namespace ph = std::placeholders;
        std::size_t lines = 0;
        auto it = m_stockpile.rbegin();
        auto end = m_stockpile.rend();
        static std::function<bool(color_t)> const is_color_non
            = std::bind(std::not_equal_to<color_t>(), color_t::non, ph::_1);
        while(it != end){
            if(std::all_of((*it).begin(), (*it).end(), is_color_non)){
                std::copy(it + 1, m_stockpile.rend(), it);
                ++lines;
            }else{
                ++it;
            }
        }
        m_score += mgame::score(lines);
        if(m_score >= score_limit) m_score = score_limit;
        
        m_current.tetromino = m_next;
        m_current.pos = point2i{{0, 0}};
        m_current.angle = angle_t::Zero;
        m_next = m_rand();
        
        auto const& lay2 = layout(m_current.tetromino, m_current.angle);
        for(std::size_t i = 0; i < 4; ++i){
            int x = lay2[i][0];
            int y = lay2[i][1];
            if(m_stockpile[y][x] != color_t::non){
                m_gameover = true;
                return;
            }
        }
    }
    
    bool tetclone::move_falling_tetromino(point2i _v)
    {
        point2i const& p = m_current.pos;
        bool success = true;
        auto const& lay = layout(m_current.tetromino, m_current.angle);
        for(std::size_t i = 0; i < 4; ++i){
            int x = p[0] + lay[i][0] + _v[0];
            int y = p[1] + lay[i][1] * ydivision + _v[1];
            int yp1 = y / ydivision;
            int yp2 = yp1 + ((y % ydivision != 0) ? 1 : 0);
            success &= x >= (int)0 && x < width;
            success &= y >= (int)0 && y <= (height - 1) * ydivision;
            if(!success){
                return false;
            }
            success &= m_stockpile[yp1][x] == color_t::non;
            success &= m_stockpile[yp2][x] == color_t::non;
            if(!success) return false;
        }
        m_current.pos[0] += _v[0];
        m_current.pos[1] += _v[1];
        return true;
    }
    
    bool tetclone::rotate_falling_tetromino()
    {
        point2i const& p = m_current.pos;
        bool success = true;
        angle_t rotated = rotate(m_current.angle);
        auto const& lay = layout(m_current.tetromino, rotated);
        for(std::size_t i = 0; i < 4; ++i){
            int x = p[0] + lay[i][0];
            int y = p[1] + lay[i][1] * ydivision;
            int yp1 = y / ydivision;
            int yp2 = yp1 + ((y % ydivision != 0) ? 1 : 0);
            success &= x >= (int)0 && x < width;
            success &= y >= (int)0 && y < height * ydivision;
            if(!success) return false;
            success &= m_stockpile[yp1][x] == color_t::non;
            success &= m_stockpile[yp2][x] == color_t::non;
            if(!success) return false;
        }
        m_current.angle = rotated;
        return true;
    }
    
    void tetclone::step()
    {
        if(!m_gameover){
            ++m_level_count;
            bool bottom = !move_falling_tetromino(point2i{{0, m_fall_speed}});
            if(bottom) on_down_bottom();
            if(m_level_count >= m_level_step){
                if(m_level < 10)
                    ++m_level;
                m_fall_speed = fall_speed(m_level);
                m_level_count = 0;
            }
        }
    }
    
    void tetclone::restart()
    {
        m_gameover = false;
        std::array<color_t, width> row;
        std::fill(row.begin(), row.end(), color_t::non);
        std::fill(m_stockpile.begin(), m_stockpile.end(), row);
        
        m_current.tetromino = m_rand();
        m_current.angle = angle_t::Zero;
        m_current.pos = point2i{{0, 0}};
        m_next = m_rand();
        m_score = 0;
        m_level = 0;
        m_fall_speed = fall_speed(m_level);
    }
    
    tetclone::matrix_t const& tetclone::stockpile() const
    {
        return m_stockpile;
    }
    
    falling const& tetclone::current_falling() const
    {
        return m_current;
    }
    
    tetromino_t tetclone::next() const
    {
        return m_next;
    }
    
    std::size_t tetclone::score() const
    {
        return m_score;
    }
    
    bool tetclone::gameover() const
    {
        return m_gameover;
    }
}

