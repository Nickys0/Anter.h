#ifndef _ANSI_COLOR_H
#define _ANSI_COLOR_H

#define __MAKE_ESCAPE_SEQUNCE( _cnt ) "\x1b" _cnt


// If you don't wanna have colore text you
// can define this macro
//          vvv
#ifndef NO_MSG_COLOR_TEXT
#   define COLOR_RESET                __MAKE_ESCAPE_SEQUNCE( "[0m" )

    //  Regular text
#   define BLACK_TEXT                 __MAKE_ESCAPE_SEQUNCE( "[0;30m" )
#   define RED_TEXT                   __MAKE_ESCAPE_SEQUNCE( "[0;31m" )
#   define GREEN_TEXT                 __MAKE_ESCAPE_SEQUNCE( "[0;32m" )
#   define YELLOW_TEXT                __MAKE_ESCAPE_SEQUNCE( "[0;33m" )
#   define BLUE_TEXT                  __MAKE_ESCAPE_SEQUNCE( "[0;34m" )
#   define MAGENTA_TEXT               __MAKE_ESCAPE_SEQUNCE( "[0;35m" )
#   define CYAN_TEXT                  __MAKE_ESCAPE_SEQUNCE( "[0;36m" )
#   define WHITE_TEXT                 __MAKE_ESCAPE_SEQUNCE( "[0;37m" )
    ////////////////

    //Regular bold text
#   define BOLD_BLACK_TEXT            __MAKE_ESCAPE_SEQUNCE( "[1;30m" )
#   define BOLD_RED_TEXT              __MAKE_ESCAPE_SEQUNCE( "[1;31m" )
#   define BOLD_GREEN_TEXT            __MAKE_ESCAPE_SEQUNCE( "[1;32m" )
#   define BOLD_YELLOW_TEXT           __MAKE_ESCAPE_SEQUNCE( "[1;33m" )
#   define BOLD_BLUE_TEXT             __MAKE_ESCAPE_SEQUNCE( "[1;34m" )
#   define BOLD_MAGENTA_TEXT          __MAKE_ESCAPE_SEQUNCE( "[1;35m" )
#   define BOLD_CYAN_TEXT             __MAKE_ESCAPE_SEQUNCE( "[1;36m" )
#   define BOLD_WHITE_TEXT            __MAKE_ESCAPE_SEQUNCE( "[1;37m" )
    ///////////////////

    //Regular underline text
#   define UNDE_BLACK_TEXT            __MAKE_ESCAPE_SEQUNCE( "[4;30m" )
#   define UNDE_RED_TEXT              __MAKE_ESCAPE_SEQUNCE( "[4;31m" )
#   define UNDE_GREEN_TEXT            __MAKE_ESCAPE_SEQUNCE( "[4;32m" )
#   define UNDE_YELLOW_TEXT           __MAKE_ESCAPE_SEQUNCE( "[4;33m" )
#   define UNDE_BLUE_TEXT             __MAKE_ESCAPE_SEQUNCE( "[4;34m" )
#   define UNDE_MAGENTA_TEXT          __MAKE_ESCAPE_SEQUNCE( "[4;35m" )
#   define UNDE_CYAN_TEXT             __MAKE_ESCAPE_SEQUNCE( "[4;36m" )
#   define UNDE_WHITE_TEXT            __MAKE_ESCAPE_SEQUNCE( "[4;37m" )
    ///////////////////////


    //Regular background
#   define BCKGDR_BLACK_TEXT          __MAKE_ESCAPE_SEQUNCE( "[40m" )
#   define BCKGDR_RED_TEXT            __MAKE_ESCAPE_SEQUNCE( "[41m" )
#   define BCKGDR_GREEN_TEXT          __MAKE_ESCAPE_SEQUNCE( "[42m" )
#   define BCKGDR_YELLOW_TEXT         __MAKE_ESCAPE_SEQUNCE( "[43m" )
#   define BCKGDR_BLUE_TEXT           __MAKE_ESCAPE_SEQUNCE( "[44m" )
#   define BCKGDR_MAGENTA_TEXT        __MAKE_ESCAPE_SEQUNCE( "[45m" )
#   define BCKGDR_CYAN_TEXT           __MAKE_ESCAPE_SEQUNCE( "[46m" )
#   define BCKGDR_WHITE_TEXT          __MAKE_ESCAPE_SEQUNCE( "[47m" )
    ////////////////////


    //High intensty background 
#   define BCKGDR_BLACK_HIGH_TEXT     __MAKE_ESCAPE_SEQUNCE( "[0;100m" )
#   define BCKGDR_RED_HIGH_TEXT       __MAKE_ESCAPE_SEQUNCE( "[0;101m" )
#   define BCKGDR_GREEN_HIGH_TEXT     __MAKE_ESCAPE_SEQUNCE( "[0;102m" )
#   define BCKGDR_YELLOW_HIGH_TEXT    __MAKE_ESCAPE_SEQUNCE( "[0;103m" )
#   define BCKGDR_BLUE_HIGH_TEXT      __MAKE_ESCAPE_SEQUNCE( "[0;104m" )
#   define BCKGDR_MAGENTA_HIGH_TEXT   __MAKE_ESCAPE_SEQUNCE( "[0;105m" )
#   define BCKGDR_CYAN_HIGH_TEXT      __MAKE_ESCAPE_SEQUNCE( "[0;106m" )
#   define BCKGDR_WHITE_HIGH_TEXT     __MAKE_ESCAPE_SEQUNCE( "[0;107m" )
    /////////////////////////


    //High intensty text
#   define BLACK_HIGH_TEXT            __MAKE_ESCAPE_SEQUNCE( "[0;90m" )
#   define RED_HIGH_TEXT              __MAKE_ESCAPE_SEQUNCE( "[0;91m" )
#   define GREEN_HIGH_TEXT            __MAKE_ESCAPE_SEQUNCE( "[0;92m" )
#   define YELLOW_HIGH_TEXT           __MAKE_ESCAPE_SEQUNCE( "[0;93m" )
#   define BLUE_HIGH_TEXT             __MAKE_ESCAPE_SEQUNCE( "[0;94m" )
#   define MAGENTA_HIGH_TEXT          __MAKE_ESCAPE_SEQUNCE( "[0;95m" )
#   define CYAN_HIGH_TEXT             __MAKE_ESCAPE_SEQUNCE( "[0;96m" )
#   define WHITE_HIGH_TEXT            __MAKE_ESCAPE_SEQUNCE( "[0;97m" )
    ////////////////////

    //Bold high intensity text
#   define BOLD_BLACK_HIGH_TEXT       __MAKE_ESCAPE_SEQUNCE( "[1;90m" )
#   define BOLD_RED_HIGH_TEXT         __MAKE_ESCAPE_SEQUNCE( "[1;91m" )
#   define BOLD_GREEN_HIGH_TEXT       __MAKE_ESCAPE_SEQUNCE( "[1;92m" )
#   define BOLD_YELLOW_HIGH_TEXT      __MAKE_ESCAPE_SEQUNCE( "[1;93m" )
#   define BOLD_BLUE_HIGH_TEXT        __MAKE_ESCAPE_SEQUNCE( "[1;94m" )
#   define BOLD_MAGENTA_HIGH_TEXT     __MAKE_ESCAPE_SEQUNCE( "[1;95m" )
#   define BOLD_CYAN_HIGH_TEXT        __MAKE_ESCAPE_SEQUNCE( "[1;96m" )
#   define BOLD_WHITE_HIGH_TEXT       __MAKE_ESCAPE_SEQUNCE( "[1;97m" )
    //////////////////////////

        ///         MAKER          ///
#   define __MAKE_TEXT_COLOR_BASE( _color, _txt )\
           _color _txt COLOR_RESET
        
    //  Regular text
#   define __MAKE_BLACK_TEXT( _text )                   __MAKE_TEXT_COLOR_BASE( BLACK_TEXT, _txt )
#   define __MAKE_RED_TEXT( _txt )                      __MAKE_TEXT_COLOR_BASE( RED_TEXT, _txt )
#   define __MAKE_GREEN_TEXT( _txt )                    __MAKE_TEXT_COLOR_BASE( GREEN_TEXT, _txt )
#   define __MAKE_YELLOW_TEXT( _txt )                   __MAKE_TEXT_COLOR_BASE( YELLOW_TEXT, _txt )
#   define __MAKE_CYAN_TEXT( _txt )                     __MAKE_TEXT_COLOR_BASE( CYAN_TEXT, _txt )
#   define __MAKE_BLUE_TEXT( _txt )                     __MAKE_TEXT_COLOR_BASE( BLUE_TEXT, _txt )
#   define __MAKE_MAGENTA_TEXT( _txt )                  __MAKE_TEXT_COLOR_BASE( MAGENTA_TEXT, _txt )
#   define __MAKE_WHITE_TEXT( _txt )                    __MAKE_TEXT_COLOR_BASE( WHITE_TEXT, _txt )
    ////////////////

    //Regular bold text
#   define __MAKE_BOLD_BLACK_TEXT( _text )              __MAKE_TEXT_COLOR_BASE( BOLD_BLACK_TEXT , _text )
#   define __MAKE_BOLD_RED_TEXT( _text )                __MAKE_TEXT_COLOR_BASE( BOLD_RED_TEXT , _text )
#   define __MAKE_BOLD_GREEN_TEXT( _text )              __MAKE_TEXT_COLOR_BASE( BOLD_GREEN_TEXT , _text )
#   define __MAKE_BOLD_YELLOW_TEXT( _text )             __MAKE_TEXT_COLOR_BASE( BOLD_YELLOW_TEXT , _text )
#   define __MAKE_BOLD_BLUE_TEXT( _text )               __MAKE_TEXT_COLOR_BASE( BOLD_BLUE_TEXT , _text )
#   define __MAKE_BOLD_MAGENTA_TEXT( _text )            __MAKE_TEXT_COLOR_BASE( BOLD_MAGENTA_TEXT , _text )
#   define __MAKE_BOLD_CYAN_TEXT( _text )               __MAKE_TEXT_COLOR_BASE( BOLD_CYAN_TEXT , _text )
#   define __MAKE_BOLD_WHITE_TEXT( _text )              __MAKE_TEXT_COLOR_BASE( BOLD_WHITE_TEXT , _text )
    ////////////////////

    //Regular underline text
#   define __MAKE_UNDE_BLACK_TEXT( _text )              __MAKE_TEXT_COLOR_BASE( UNDE_BLACK_TEXT, _text )
#   define __MAKE_UNDE_RED_TEXT( _text )                __MAKE_TEXT_COLOR_BASE( UNDE_RED_TEXT, _text )
#   define __MAKE_UNDE_GREEN_TEXT( _text )              __MAKE_TEXT_COLOR_BASE( UNDE_GREEN_TEXT, _text )
#   define __MAKE_UNDE_YELLOW_TEXT( _text )             __MAKE_TEXT_COLOR_BASE( UNDE_YELLOW_TEXT, _text )
#   define __MAKE_UNDE_BLUE_TEXT( _text )               __MAKE_TEXT_COLOR_BASE( UNDE_BLUE_TEXT, _text )
#   define __MAKE_UNDE_MAGENTA_TEXT( _text )            __MAKE_TEXT_COLOR_BASE( UNDE_MAGENTA_TEXT, _text )
#   define __MAKE_UNDE_CYAN_TEXT( _text )               __MAKE_TEXT_COLOR_BASE( UNDE_CYAN_TEXT, _text )
#   define __MAKE_UNDE_WHITE_TEXT( _text )              __MAKE_TEXT_COLOR_BASE( UNDE_WHITE_TEXT, _text )
    ////////////////////////

    //Regular background
#   define __MAKE_BCKGDR_BLACK_TEXT( _text )            __MAKE_TEXT_COLOR_BASE( BCKGDR_BLACK_TEXT , _text )
#   define __MAKE_BCKGDR_RED_TEXT( _text )              __MAKE_TEXT_COLOR_BASE( BCKGDR_RED_TEXT , _text )
#   define __MAKE_BCKGDR_GREEN_TEXT( _text )            __MAKE_TEXT_COLOR_BASE( BCKGDR_GREEN_TEXT , _text )
#   define __MAKE_BCKGDR_YELLOW_TEXT( _text )           __MAKE_TEXT_COLOR_BASE( BCKGDR_YELLOW_TEXT , _text )
#   define __MAKE_BCKGDR_BLUE_TEXT( _text )             __MAKE_TEXT_COLOR_BASE( BCKGDR_BLUE_TEXT , _text )
#   define __MAKE_BCKGDR_MAGENTA_TEXT( _text )          __MAKE_TEXT_COLOR_BASE( BCKGDR_MAGENTA_TEXT , _text )
#   define __MAKE_BCKGDR_CYAN_TEXT( _text )             __MAKE_TEXT_COLOR_BASE( BCKGDR_CYAN_TEXT , _text )
#   define __MAKE_BCKGDR_WHITE_TEXT( _text )            __MAKE_TEXT_COLOR_BASE( BCKGDR_WHITE_TEXT , _text )
    ////////////////////

    //High intensty background 
#   define __MAKE_BCKGDR_BLACK_HIGH_TEXT( _text )       __MAKE_TEXT_COLOR_BASE( BCKGDR_BLACK_HIGH_TEXT , _text )
#   define __MAKE_BCKGDR_RED_HIGH_TEXT( _text )         __MAKE_TEXT_COLOR_BASE( BCKGDR_RED_HIGH_TEXT , _text )
#   define __MAKE_BCKGDR_GREEN_HIGH_TEXT( _text )       __MAKE_TEXT_COLOR_BASE( BCKGDR_GREEN_HIGH_TEXT , _text )
#   define __MAKE_BCKGDR_YELLOW_HIGH_TEXT( _text )      __MAKE_TEXT_COLOR_BASE( BCKGDR_YELLOW_HIGH_TEXT , _text )
#   define __MAKE_BCKGDR_BLUE_HIGH_TEXT( _text )        __MAKE_TEXT_COLOR_BASE( BCKGDR_BLUE_HIGH_TEXT , _text )
#   define __MAKE_BCKGDR_MAGENTA_HIGH_TEXT( _text )     __MAKE_TEXT_COLOR_BASE( BCKGDR_MAGENTA_HIGH_TEXT , _text )
#   define __MAKE_BCKGDR_CYAN_HIGH_TEXT( _text )        __MAKE_TEXT_COLOR_BASE( BCKGDR_CYAN_HIGH_TEXT , _text )
#   define __MAKE_BCKGDR_WHITE_HIGH_TEXT( _text )       __MAKE_TEXT_COLOR_BASE( BCKGDR_WHITE_HIGH_TEXT , _text )
    //////////////////////////

    //High intensty text
#   define __MAKE_BLACK_HIGH_TEXT( _text )              __MAKE_TEXT_COLOR_BASE( BLACK_HIGH_TEXT , _text  )
#   define __MAKE_RED_HIGH_TEXT( _text )                __MAKE_TEXT_COLOR_BASE( RED_HIGH_TEXT , _text  )
#   define __MAKE_GREEN_HIGH_TEXT( _text )              __MAKE_TEXT_COLOR_BASE( GREEN_HIGH_TEXT , _text  )
#   define __MAKE_YELLOW_HIGH_TEXT( _text )             __MAKE_TEXT_COLOR_BASE( YELLOW_HIGH_TEXT , _text  )
#   define __MAKE_BLUE_HIGH_TEXT( _text )               __MAKE_TEXT_COLOR_BASE( BLUE_HIGH_TEXT , _text  )
#   define __MAKE_MAGENTA_HIGH_TEXT( _text )            __MAKE_TEXT_COLOR_BASE( MAGENTA_HIGH_TEXT , _text  )
#   define __MAKE_CYAN_HIGH_TEXT( _text )               __MAKE_TEXT_COLOR_BASE( CYAN_HIGH_TEXT , _text  )
#   define __MAKE_WHITE_HIGH_TEXT( _text )              __MAKE_TEXT_COLOR_BASE( WHITE_HIGH_TEXT , _text  )
    ////////////////////

    //Bold high intensity text
#   define __MAKE_BOLD_BLACK_HIGH_TEXT( _text )         __MAKE_TEXT_COLOR_BASE( BOLD_BLACK_HIGH_TEXT, _text )
#   define __MAKE_BOLD_RED_HIGH_TEXT( _text )           __MAKE_TEXT_COLOR_BASE( BOLD_RED_HIGH_TEXT, _text )
#   define __MAKE_BOLD_GREEN_HIGH_TEXT( _text )         __MAKE_TEXT_COLOR_BASE( BOLD_GREEN_HIGH_TEXT, _text )
#   define __MAKE_BOLD_YELLOW_HIGH_TEXT( _text )        __MAKE_TEXT_COLOR_BASE( BOLD_YELLOW_HIGH_TEXT, _text )
#   define __MAKE_BOLD_BLUE_HIGH_TEXT( _text )          __MAKE_TEXT_COLOR_BASE( BOLD_BLUE_HIGH_TEXT, _text )
#   define __MAKE_BOLD_MAGENTA_HIGH_TEXT( _text )       __MAKE_TEXT_COLOR_BASE( BOLD_MAGENTA_HIGH_TEXT, _text )
#   define __MAKE_BOLD_CYAN_HIGH_TEXT( _text )          __MAKE_TEXT_COLOR_BASE( BOLD_CYAN_HIGH_TEXT, _text )
#   define __MAKE_BOLD_WHITE_HIGH_TEXT( _text )         __MAKE_TEXT_COLOR_BASE( BOLD_WHITE_HIGH_TEXT, _text )
    ////////////////////////
        
///////////////////////////////////////////////////////////

#else
#   define COLOR_RESET
#   define BLACK_TEXT
#   define RED_TEXT
#   define GREEN_TEXT
#   define YELLOW_TEXT
#   define BLUE_TEXT
#   define MAGENTA_TEXT
#   define CYAN_TEXT
#   define WHITE_TEXT

#   define BOLD_BLACK_TEXT
#   define BOLD_RED_TEXT
#   define BOLD_GREEN_TEXT
#   define BOLD_YELLOW_TEXT
#   define BOLD_BLUE_TEXT
#   define BOLD_MAGENTA_TEXT
#   define BOLD_CYAN_TEXT
#   define BOLD_WHITE_TEXT

#   define UNDE_BLACK_TEXT
#   define UNDE_RED_TEXT
#   define UNDE_GREEN_TEXT
#   define UNDE_YELLOW_TEXT
#   define UNDE_BLUE_TEXT
#   define UNDE_MAGENTA_TEXT
#   define UNDE_CYAN_TEXT
#   define UNDE_WHITE_TEXT

#   define BCKGDR_BLACK_TEXT
#   define BCKGDR_RED_TEXT
#   define BCKGDR_GREEN_TEXT
#   define BCKGDR_YELLOW_TEXT
#   define BCKGDR_BLUE_TEXT
#   define BCKGDR_MAGENTA_TEXT
#   define BCKGDR_CYAN_TEXT
#   define BCKGDR_WHITE_TEXT

#   define BCKGDR_BLACK_HIGH_TEXT
#   define BCKGDR_RED_HIGH_TEXT
#   define BCKGDR_GREEN_HIGH_TEXT
#   define BCKGDR_YELLOW_HIGH_TEXT
#   define BCKGDR_BLUE_HIGH_TEXT
#   define BCKGDR_MAGENTA_HIGH_TEXT
#   define BCKGDR_CYAN_HIGH_TEXT
#   define BCKGDR_WHITE_HIGH_TEXT

#   define BLACK_HIGH_TEXT
#   define RED_HIGH_TEXT
#   define GREEN_HIGH_TEXT
#   define YELLOW_HIGH_TEXT
#   define BLUE_HIGH_TEXT
#   define MAGENTA_HIGH_TEXT
#   define CYAN_HIGH_TEXT
#   define WHITE_HIGH_TEXT

#   define BOLD_BLACK_HIGH_TEXT
#   define BOLD_RED_HIGH_TEXT
#   define BOLD_GREEN_HIGH_TEXT
#   define BOLD_YELLOW_HIGH_TEXT
#   define BOLD_BLUE_HIGH_TEXT
#   define BOLD_MAGENTA_HIGH_TEXT
#   define BOLD_CYAN_HIGH_TEXT
#   define BOLD_WHITE_HIGH_TEXT

#   define __MAKE_BLACK_TEXT( _text )                   _txt
#   define __MAKE_RED_TEXT( _txt )                      _txt
#   define __MAKE_GREEN_TEXT( _txt )                    _txt
#   define __MAKE_YELLOW_TEXT( _txt )                   _txt
#   define __MAKE_CYAN_TEXT( _txt )                     _txt
#   define __MAKE_BLUE_TEXT( _txt )                     _txt
#   define __MAKE_MAGENTA_TEXT( _txt )                  _txt
#   define __MAKE_WHITE_TEXT( _txt )                    _txt

#   define __MAKE_BOLD_BLACK_TEXT( _text )              _text
#   define __MAKE_BOLD_RED_TEXT( _text )                _text
#   define __MAKE_BOLD_GREEN_TEXT( _text )              _text
#   define __MAKE_BOLD_YELLOW_TEXT( _text )             _text
#   define __MAKE_BOLD_BLUE_TEXT( _text )               _text
#   define __MAKE_BOLD_MAGENTA_TEXT( _text )            _text
#   define __MAKE_BOLD_CYAN_TEXT( _text )               _text
#   define __MAKE_BOLD_WHITE_TEXT( _text )              _text

#   define __MAKE_UNDE_BLACK_TEXT( _text )              _text
#   define __MAKE_UNDE_RED_TEXT( _text )                _text
#   define __MAKE_UNDE_GREEN_TEXT( _text )              _text
#   define __MAKE_UNDE_YELLOW_TEXT( _text )             _text
#   define __MAKE_UNDE_BLUE_TEXT( _text )               _text
#   define __MAKE_UNDE_MAGENTA_TEXT( _text )            _text
#   define __MAKE_UNDE_CYAN_TEXT( _text )               _text
#   define __MAKE_UNDE_WHITE_TEXT( _text )              _text

#   define __MAKE_BCKGDR_BLACK_TEXT( _text )            _text
#   define __MAKE_BCKGDR_RED_TEXT( _text )              _text
#   define __MAKE_BCKGDR_GREEN_TEXT( _text )            _text
#   define __MAKE_BCKGDR_YELLOW_TEXT( _text )           _text
#   define __MAKE_BCKGDR_BLUE_TEXT( _text )             _text
#   define __MAKE_BCKGDR_MAGENTA_TEXT( _text )          _text
#   define __MAKE_BCKGDR_CYAN_TEXT( _text )             _text
#   define __MAKE_BCKGDR_WHITE_TEXT( _text )            _text

#   define __MAKE_BCKGDR_BLACK_HIGH_TEXT( _text )       _text
#   define __MAKE_BCKGDR_RED_HIGH_TEXT( _text )         _text
#   define __MAKE_BCKGDR_GREEN_HIGH_TEXT( _text )       _text
#   define __MAKE_BCKGDR_YELLOW_HIGH_TEXT( _text )      _text
#   define __MAKE_BCKGDR_BLUE_HIGH_TEXT( _text )        _text
#   define __MAKE_BCKGDR_MAGENTA_HIGH_TEXT( _text )     _text
#   define __MAKE_BCKGDR_CYAN_HIGH_TEXT( _text )        _text
#   define __MAKE_BCKGDR_WHITE_HIGH_TEXT( _text )       _text

#   define __MAKE_BLACK_HIGH_TEXT( _text )              _text
#   define __MAKE_RED_HIGH_TEXT( _text )                _text
#   define __MAKE_GREEN_HIGH_TEXT( _text )              _text
#   define __MAKE_YELLOW_HIGH_TEXT( _text )             _text
#   define __MAKE_BLUE_HIGH_TEXT( _text )               _text
#   define __MAKE_MAGENTA_HIGH_TEXT( _text )            _text
#   define __MAKE_CYAN_HIGH_TEXT( _text )               _text
#   define __MAKE_WHITE_HIGH_TEXT( _text )              _text

#   define __MAKE_BOLD_BLACK_HIGH_TEXT( _text )         _text
#   define __MAKE_BOLD_RED_HIGH_TEXT( _text )           _text
#   define __MAKE_BOLD_GREEN_HIGH_TEXT( _text )         _text
#   define __MAKE_BOLD_YELLOW_HIGH_TEXT( _text )        _text
#   define __MAKE_BOLD_BLUE_HIGH_TEXT( _text )          _text
#   define __MAKE_BOLD_MAGENTA_HIGH_TEXT( _text )       _text
#   define __MAKE_BOLD_CYAN_HIGH_TEXT( _text )          _text
#   define __MAKE_BOLD_WHITE_HIGH_TEXT( _text )         _text
#endif


//  Regular text
#   define MK_BLACK(_text)                      __MAKE_BLACK_TEXT(_txt)
#   define MK_RED(_txt)                         __MAKE_RED_TEXT(_txt)
#   define MK_GREEN(_txt)                       __MAKE_GREEN_TEXT(_txt)
#   define MK_YELLOW(_txt)                      __MAKE_YELLOW_TEXT(_txt)
#   define MK_CYAN(_txt)                        __MAKE_CYAN_TEXT(_txt)
#   define MK_BLUE(_txt)                        __MAKE_BLUE_TEXT(_txt)
#   define MK_MAGENTA(_txt)                     __MAKE_MAGENTA_TEXT(_txt)
#   define MK_WHITE(_txt)                       __MAKE_WHITE_TEXT(_txt)
    ////////////////

    //Regular bold text
#   define MK_BBLACK(_text)                     __MAKE_BOLD_BLACK_TEXT(_text)
#   define MK_BRED(_text)                       __MAKE_BOLD_RED_TEXT(_text)
#   define MK_BGREEN(_text)                     __MAKE_BOLD_GREEN_TEXT(_text)
#   define MK_BYELLOW(_text)                    __MAKE_BOLD_YELLOW_TEXT(_text)
#   define MK_BBLUE(_text)                      __MAKE_BOLD_BLUE_TEXT(_text)
#   define MK_BMAGENTA(_text)                   __MAKE_BOLD_MAGENTA_TEXT(_text)
#   define MK_BCYAN(_text)                      __MAKE_BOLD_CYAN_TEXT(_text)
#   define MK_BWHITE(_text)                     __MAKE_BOLD_WHITE_TEXT(_text)
    ////////////////////

    //Regular underline text
#   define MK_UBLACK(_text)                     __MAKE_UNDE_BLACK_TEXT(_text)
#   define MK_URED(_text)                       __MAKE_UNDE_RED_TEXT(_text)
#   define MK_UGREEN(_text)                     __MAKE_UNDE_GREEN_TEXT(_text)
#   define MK_UYELLOW(_text)                    __MAKE_UNDE_YELLOW_TEXT(_text)
#   define MK_UBLUE(_text)                      __MAKE_UNDE_BLUE_TEXT(_text)
#   define MK_UMAGENTA(_text)                   __MAKE_UNDE_MAGENTA_TEXT(_text)
#   define MK_UCYAN(_text)                      __MAKE_UNDE_CYAN_TEXT(_text)
#   define MK_UWHITE(_text)                     __MAKE_UNDE_WHITE_TEXT(_text)
    ////////////////////////

    //Regular background
#   define MK_BCK_BLACK(_text)                  __MAKE_BCKGDR_BLACK_TEXT(_text )
#   define MK_BCK_RED(_text)                    __MAKE_BCKGDR_RED_TEXT(_text)
#   define MK_BCK_GREEN(_text)                  __MAKE_BCKGDR_GREEN_TEXT(_text)
#   define MK_BCK_YELLOW(_text)                 __MAKE_BCKGDR_YELLOW_TEXT(_text)
#   define MK_BCK_BLUE(_text)                   __MAKE_BCKGDR_BLUE_TEXT(_text)
#   define MK_BCK_MAGENTA(_text)                __MAKE_BCKGDR_MAGENTA_TEXT(_text)
#   define MK_BCK_CYAN(_text)                   __MAKE_BCKGDR_CYAN_TEXT(_text)
#   define MK_BCK_WHITE(_text)                  __MAKE_BCKGDR_WHITE_TEXT(_text)
    ////////////////////

    //High intensty background 
#   define MK_BCK_HBLACK( _text )               __MAKE_BCKGDR_BLACK_HIGH_TEXT(_text)
#   define MK_BCK_HRED( _text )                 __MAKE_BCKGDR_RED_HIGH_TEXT(_text)
#   define MK_BCK_HGREEN( _text )               __MAKE_BCKGDR_GREEN_HIGH_TEXT(_text)
#   define MK_BCK_HYELLOW( _text )              __MAKE_BCKGDR_YELLOW_HIGH_TEXT(_text)
#   define MK_BCK_HBLUE( _text )                __MAKE_BCKGDR_BLUE_HIGH_TEXT(_text)
#   define MK_BCK_HMAGENTA( _text )             __MAKE_BCKGDR_MAGENTA_HIGH_TEXT(_text)
#   define MK_BCK_HCYAN( _text )                __MAKE_BCKGDR_CYAN_HIGH_TEXT(_text)
#   define MK_BCK_HWHITE( _text )               __MAKE_BCKGDR_WHITE_HIGH_TEXT(_text)
    //////////////////////////


#   define MK_BHBLACK(_txt)                     __MAKE_BOLD_BLACK_HIGH_TEXT(_text)
#   define MK_BHRED(_text)                      __MAKE_BOLD_RED_HIGH_TEXT(_text)
#   define MK_BHGREEN(_text)                    __MAKE_BOLD_GREEN_HIGH_TEXT(_text)
#   define MK_BHYELLOW(_text)                   __MAKE_BOLD_YELLOW_HIGH_TEXT(_text)
#   define MK_BHBLUE(_text)                     __MAKE_BOLD_BLUE_HIGH_TEXT(_text)
#   define MK_BHMAGENTA(_text)                  __MAKE_BOLD_MAGENTA_HIGH_TEXT(_text)
#   define MK_BHCYAN(_text)                     __MAKE_BOLD_CYAN_HIGH_TEXT(_text)
#   define MK_BHWHITE(_text)                    __MAKE_BOLD_WHITE_HIGH_TEXT(_text)

#   define MK_HBLACK(_text)                     __MAKE_BLACK_HIGH_TEXT(_text)
#   define MK_HRED(_text)                       __MAKE_RED_HIGH_TEXT(_text)
#   define MK_HGREEN(_text)                     __MAKE_GREEN_HIGH_TEXT(_text)
#   define MK_HYELLOW(_text)                    __MAKE_YELLOW_HIGH_TEXT(_text)
#   define MK_HBLUE(_text)                      __MAKE_BLUE_HIGH_TEXT(_text)
#   define MK_HMAGENTA(_text)                   __MAKE_MAGENTA_HIGH_TEXT(_text)
#   define MK_HCYAN(_text)                      __MAKE_CYAN_HIGH_TEXT(_text)
#   define MK_HWHITE(_text)                     __MAKE_WHITE_HIGH_TEXT(_text)


#endif
