//-----------------------------------------------------------
//    
//-----------------------------------------------------------
#ifndef tetclone_H
#define tetclone_H

#include <vector>
#include <array>
#include <cstddef>
#include <random>

namespace mgame
{
    typedef std::array<int, 2> point2i;
    
    enum class color_t
    {
        non,
        cyan,
        blue,
        orange,
        yellow,
        green,
        purple,
        red
    };
    
    enum class tetromino_t : unsigned char
    {
        I = 0,
        J = 1,
        L = 2,
        O = 3,
        S = 4,
        T = 5,
        Z = 6
    };
    
    enum class angle_t
    {
        Zero,
        Fifteen,
        Thirty,
        FortyFive
    };
    
    struct falling
    {
        point2i pos;
        angle_t angle;
        tetromino_t tetromino;
    };
    
    std::array<point2i, 4> const& layout(tetromino_t _tet, angle_t _ang);
    color_t color(tetromino_t _tet);
    angle_t rotate(angle_t _ang);
    std::size_t fall_step(std::size_t _level);
    std::size_t score(std::size_t _lines);
    
    class tetclone
    {
    public:
        static const std::size_t width = 10;
        static const std::size_t height = 21;
        static const int ydivision = 10;
        static const std::size_t score_limit = 99999990;
        typedef std::array<std::array<color_t, width>, height> matrix_t;
        
    private:
        matrix_t m_stockpile;
        falling m_current;
        tetromino_t m_next;
        bool m_gameover;
        std::size_t m_level;
        std::size_t m_level_count;
        std::size_t m_level_step;
        std::size_t m_fall_count;
        std::size_t m_fall_speed;
        std::size_t m_score;
        std::mt19937 m_gen;
        
        tetromino_t m_rand();
        tetclone(tetclone const&);
        tetclone& operator=(tetclone const&);
        
    public:
        tetclone();
        void on_right();
        void on_left();
        void on_down();
        void on_rotate();
        void on_down_bottom();
        bool move_falling_tetromino(point2i _v);
        bool rotate_falling_tetromino();
        void step();
        void restart();
        bool gameover() const;
        
        matrix_t const& stockpile() const;
        falling const& current_falling() const;
        tetromino_t next() const;
        std::size_t score() const;
    };
    
    extern tetclone tetclone_model;
}
#endif

