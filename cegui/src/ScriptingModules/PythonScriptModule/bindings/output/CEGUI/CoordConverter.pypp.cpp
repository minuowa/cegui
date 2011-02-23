// This file has been generated by Py++.

#include "boost/python.hpp"
#include "python_CEGUI.h"
#include "CoordConverter.pypp.hpp"

namespace bp = boost::python;

void register_CoordConverter_class(){

    { //::CEGUI::CoordConverter
        typedef bp::class_< CEGUI::CoordConverter, boost::noncopyable > CoordConverter_exposer_t;
        CoordConverter_exposer_t CoordConverter_exposer = CoordConverter_exposer_t( "CoordConverter", "*!\n\
        \n\
            Utility class that helps in converting various types of co-ordinate between\n\
            absolute screen positions and positions offset from the top-left corner of\n\
            a given Window object.\n\
        *\n", bp::no_init );
        bp::scope CoordConverter_scope( CoordConverter_exposer );
        { //::CEGUI::CoordConverter::asAbsolute
        
            typedef float ( *asAbsolute_function_type )( ::CEGUI::UDim const &,float );
            
            CoordConverter_exposer.def( 
                "asAbsolute"
                , asAbsolute_function_type( &::CEGUI::CoordConverter::asAbsolute )
                , ( bp::arg("u"), bp::arg("base") )
                , "*!\n\
            \n\
                converts given UDim to absolute value\n\
            *\n" );
        
        }
        { //::CEGUI::CoordConverter::asAbsolute
        
            typedef ::CEGUI::Vector2< float > ( *asAbsolute_function_type )( ::CEGUI::Vector2< CEGUI::UDim > const &,::CEGUI::Size< float > const & );
            
            CoordConverter_exposer.def( 
                "asAbsolute"
                , asAbsolute_function_type( &::CEGUI::CoordConverter::asAbsolute )
                , ( bp::arg("v"), bp::arg("base") )
                , "*!\n\
            \n\
                converts given Vector2<UDim> to absolute Vector2<>\n\
            *\n" );
        
        }
        { //::CEGUI::CoordConverter::asAbsolute
        
            typedef ::CEGUI::Rect< float > ( *asAbsolute_function_type )( ::CEGUI::URect const &,::CEGUI::Size< float > const & );
            
            CoordConverter_exposer.def( 
                "asAbsolute"
                , asAbsolute_function_type( &::CEGUI::CoordConverter::asAbsolute )
                , ( bp::arg("r"), bp::arg("base") ) );
        
        }
        { //::CEGUI::CoordConverter::asRelative
        
            typedef float ( *asRelative_function_type )( ::CEGUI::UDim const &,float );
            
            CoordConverter_exposer.def( 
                "asRelative"
                , asRelative_function_type( &::CEGUI::CoordConverter::asRelative )
                , ( bp::arg("u"), bp::arg("base") )
                , "*!\n\
            \n\
                converts given UDim to relative value\n\
            *\n" );
        
        }
        { //::CEGUI::CoordConverter::asRelative
        
            typedef ::CEGUI::Vector2< float > ( *asRelative_function_type )( ::CEGUI::Vector2< CEGUI::UDim > const &,::CEGUI::Size< float > const & );
            
            CoordConverter_exposer.def( 
                "asRelative"
                , asRelative_function_type( &::CEGUI::CoordConverter::asRelative )
                , ( bp::arg("v"), bp::arg("base") )
                , "*!\n\
            \n\
                converts given Vector2<UDim> to relative Vector2<>\n\
            *\n" );
        
        }
        { //::CEGUI::CoordConverter::asRelative
        
            typedef ::CEGUI::Rect< float > ( *asRelative_function_type )( ::CEGUI::URect const &,::CEGUI::Size< float > const & );
            
            CoordConverter_exposer.def( 
                "asRelative"
                , asRelative_function_type( &::CEGUI::CoordConverter::asRelative )
                , ( bp::arg("r"), bp::arg("base") ) );
        
        }
        { //::CEGUI::CoordConverter::screenToWindow
        
            typedef ::CEGUI::Vector2< float > ( *screenToWindow_function_type )( ::CEGUI::Window const &,::CEGUI::UVector2 const & );
            
            CoordConverter_exposer.def( 
                "screenToWindow"
                , screenToWindow_function_type( &::CEGUI::CoordConverter::screenToWindow )
                , ( bp::arg("window"), bp::arg("vec") )
                , "*!\n\
                \n\
                    Convert a screen relative UVector2 point to a window co-ordinate point,\n\
                    specified in pixels.\n\
            \n\
                @param window\n\
                    Window object to use as a target for the conversion.\n\
            \n\
                @param vec\n\
                    UVector2 object describing the point to be converted\n\
            \n\
                @return\n\
                    Vector2 object describing a window co-ordinate point that is equivalent\n\
                    to screen based UVector2 point  vec.\n\
                *\n" );
        
        }
        { //::CEGUI::CoordConverter::screenToWindow
        
            typedef ::CEGUI::Vector2< float > ( *screenToWindow_function_type )( ::CEGUI::Window const &,::CEGUI::Vector2< float > const & );
            
            CoordConverter_exposer.def( 
                "screenToWindow"
                , screenToWindow_function_type( &::CEGUI::CoordConverter::screenToWindow )
                , ( bp::arg("window"), bp::arg("vec") )
                , "*!\n\
                \n\
                    Convert a screen Vector2 pixel point to a window co-ordinate point,\n\
                    specified in pixels.\n\
            \n\
                @param window\n\
                    Window object to use as a target for the conversion.\n\
            \n\
                @param vec\n\
                    Vector2 object describing the point to be converted.\n\
            \n\
                @return\n\
                    Vector2 object describing a window co-ordinate point that is equivalent\n\
                    to screen based Vector2 point  vec.\n\
                *\n" );
        
        }
        { //::CEGUI::CoordConverter::screenToWindow
        
            typedef ::CEGUI::Rect< float > ( *screenToWindow_function_type )( ::CEGUI::Window const &,::CEGUI::URect const & );
            
            CoordConverter_exposer.def( 
                "screenToWindow"
                , screenToWindow_function_type( &::CEGUI::CoordConverter::screenToWindow )
                , ( bp::arg("window"), bp::arg("rect") )
                , "*!\n\
                \n\
                    Convert a URect screen area to a window area, specified in pixels.\n\
            \n\
                @param window\n\
                    Window object to use as a target for the conversion.\n\
            \n\
                @param rect\n\
                    URect object describing the area to be converted\n\
            \n\
                @return\n\
                    Rect object describing a window area that is equivalent to URect screen\n\
                    area  rect.\n\
                *\n" );
        
        }
        { //::CEGUI::CoordConverter::screenToWindow
        
            typedef ::CEGUI::Rect< float > ( *screenToWindow_function_type )( ::CEGUI::Window const &,::CEGUI::Rect< float > const & );
            
            CoordConverter_exposer.def( 
                "screenToWindow"
                , screenToWindow_function_type( &::CEGUI::CoordConverter::screenToWindow )
                , ( bp::arg("window"), bp::arg("rect") )
                , "*!\n\
                \n\
                    Convert a Rect screen pixel area to a window area, specified in pixels.\n\
            \n\
                @param window\n\
                    Window object to use as a target for the conversion.\n\
            \n\
                @param rect\n\
                    Rect object describing the area to be converted.\n\
            \n\
                @return\n\
                    Rect object describing a window area that is equivalent to Rect screen\n\
                    area  rect.\n\
                *\n" );
        
        }
        { //::CEGUI::CoordConverter::screenToWindowX
        
            typedef float ( *screenToWindowX_function_type )( ::CEGUI::Window const &,::CEGUI::UDim const & );
            
            CoordConverter_exposer.def( 
                "screenToWindowX"
                , screenToWindowX_function_type( &::CEGUI::CoordConverter::screenToWindowX )
                , ( bp::arg("window"), bp::arg("x") )
                , "*!\n\
                \n\
                    Convert a screen relative UDim co-ordinate value to a window co-ordinate\n\
                    value, specified in pixels.\n\
            \n\
                @param window\n\
                    Window object to use as a target for the conversion.\n\
            \n\
                @param x\n\
                    UDim x co-ordinate value to be converted\n\
            \n\
                @return\n\
                    float value describing a window co-ordinate value that is equivalent to\n\
                    screen UDim co-ordinate  x.\n\
                *\n" );
        
        }
        { //::CEGUI::CoordConverter::screenToWindowX
        
            typedef float ( *screenToWindowX_function_type )( ::CEGUI::Window const &,float const );
            
            CoordConverter_exposer.def( 
                "screenToWindowX"
                , screenToWindowX_function_type( &::CEGUI::CoordConverter::screenToWindowX )
                , ( bp::arg("window"), bp::arg("x") )
                , "*!\n\
                \n\
                    Convert a screen pixel co-ordinate value to a window co-ordinate\n\
                    value, specified in pixels.\n\
            \n\
                @param window\n\
                    Window object to use as a target for the conversion.\n\
            \n\
                @param x\n\
                    float x co-ordinate value to be converted.\n\
            \n\
                @return\n\
                    float value describing a window co-ordinate value that is equivalent to\n\
                    screen co-ordinate  x.\n\
                *\n" );
        
        }
        { //::CEGUI::CoordConverter::screenToWindowY
        
            typedef float ( *screenToWindowY_function_type )( ::CEGUI::Window const &,::CEGUI::UDim const & );
            
            CoordConverter_exposer.def( 
                "screenToWindowY"
                , screenToWindowY_function_type( &::CEGUI::CoordConverter::screenToWindowY )
                , ( bp::arg("window"), bp::arg("y") )
                , "*!\n\
                \n\
                    Convert a screen relative UDim co-ordinate value to a window co-ordinate\n\
                    value, specified in pixels.\n\
            \n\
                @param window\n\
                    Window object to use as a target for the conversion.\n\
            \n\
                @param y\n\
                    UDim y co-ordinate value to be converted\n\
            \n\
                @return\n\
                    float value describing a window co-ordinate value that is equivalent to\n\
                    screen UDim co-ordinate  y.\n\
                *\n" );
        
        }
        { //::CEGUI::CoordConverter::screenToWindowY
        
            typedef float ( *screenToWindowY_function_type )( ::CEGUI::Window const &,float const );
            
            CoordConverter_exposer.def( 
                "screenToWindowY"
                , screenToWindowY_function_type( &::CEGUI::CoordConverter::screenToWindowY )
                , ( bp::arg("window"), bp::arg("y") )
                , "*!\n\
                \n\
                    Convert a screen pixel co-ordinate value to a window co-ordinate\n\
                    value, specified in pixels.\n\
            \n\
                @param window\n\
                    Window object to use as a target for the conversion.\n\
            \n\
                @param y\n\
                    UDim y co-ordinate value to be converted.\n\
            \n\
                @return\n\
                    float value describing a window co-ordinate value that is equivalent to\n\
                    screen co-ordinate  y.\n\
                *\n" );
        
        }
        { //::CEGUI::CoordConverter::windowToScreen
        
            typedef ::CEGUI::Vector2< float > ( *windowToScreen_function_type )( ::CEGUI::Window const &,::CEGUI::UVector2 const & );
            
            CoordConverter_exposer.def( 
                "windowToScreen"
                , windowToScreen_function_type( &::CEGUI::CoordConverter::windowToScreen )
                , ( bp::arg("window"), bp::arg("vec") )
                , "*!\n\
                \n\
                    Convert a window co-ordinate point, specified as a UVector2, to a\n\
                    screen relative pixel co-ordinate point.\n\
            \n\
                @param window\n\
                    Window object to use as a base for the conversion.\n\
            \n\
                @param vec\n\
                    UVector2 object describing the point to be converted\n\
            \n\
                @return\n\
                    Vector2 object describing a screen co-ordinate position that is\n\
                    equivalent to window based UVector2  vec.\n\
                *\n" );
        
        }
        { //::CEGUI::CoordConverter::windowToScreen
        
            typedef ::CEGUI::Vector2< float > ( *windowToScreen_function_type )( ::CEGUI::Window const &,::CEGUI::Vector2< float > const & );
            
            CoordConverter_exposer.def( 
                "windowToScreen"
                , windowToScreen_function_type( &::CEGUI::CoordConverter::windowToScreen )
                , ( bp::arg("window"), bp::arg("vec") )
                , "*!\n\
                \n\
                    Convert a window pixel co-ordinate point, specified as a Vector2, to a\n\
                    screen pixel co-ordinate point.\n\
            \n\
                @param window\n\
                    Window object to use as a base for the conversion.\n\
            \n\
                @param vec\n\
                    Vector2 object describing the point to be converted.\n\
            \n\
                @return\n\
                    Vector2 object describing a screen co-ordinate position that is\n\
                    equivalent to window based Vector2  vec.\n\
                *\n" );
        
        }
        { //::CEGUI::CoordConverter::windowToScreen
        
            typedef ::CEGUI::Rect< float > ( *windowToScreen_function_type )( ::CEGUI::Window const &,::CEGUI::URect const & );
            
            CoordConverter_exposer.def( 
                "windowToScreen"
                , windowToScreen_function_type( &::CEGUI::CoordConverter::windowToScreen )
                , ( bp::arg("window"), bp::arg("rect") )
                , "*!\n\
                \n\
                    Convert a window area, specified as a URect, to a screen area.\n\
            \n\
                @param rect\n\
                    URect object describing the area to be converted\n\
            \n\
                @return\n\
                    Rect object describing a screen area that is equivalent to window\n\
                    area  rect.\n\
                *\n" );
        
        }
        { //::CEGUI::CoordConverter::windowToScreen
        
            typedef ::CEGUI::Rect< float > ( *windowToScreen_function_type )( ::CEGUI::Window const &,::CEGUI::Rect< float > const & );
            
            CoordConverter_exposer.def( 
                "windowToScreen"
                , windowToScreen_function_type( &::CEGUI::CoordConverter::windowToScreen )
                , ( bp::arg("window"), bp::arg("rect") )
                , "*!\n\
                \n\
                    Convert a pixel window area, specified as a Rect, to a screen area.\n\
            \n\
                @param window\n\
                    Window object to use as a base for the conversion.\n\
            \n\
                @param rect\n\
                    Rect object describing the area to be converted.\n\
            \n\
                @return\n\
                    Rect object describing a screen area that is equivalent to window\n\
                    area  rect.\n\
                *\n" );
        
        }
        { //::CEGUI::CoordConverter::windowToScreenX
        
            typedef float ( *windowToScreenX_function_type )( ::CEGUI::Window const &,::CEGUI::UDim const & );
            
            CoordConverter_exposer.def( 
                "windowToScreenX"
                , windowToScreenX_function_type( &::CEGUI::CoordConverter::windowToScreenX )
                , ( bp::arg("window"), bp::arg("x") )
                , "*!\n\
                \n\
                    Convert a window co-ordinate value, specified as a UDim, to a screen\n\
                    relative pixel co-ordinate.\n\
            \n\
                @param window\n\
                    Window object to use as a base for the conversion.\n\
            \n\
                @param x\n\
                    UDim x co-ordinate value to be converted\n\
            \n\
                @return\n\
                    float value describing a pixel screen co-ordinate that is equivalent to\n\
                    window UDim co-ordinate  x.\n\
                *\n" );
        
        }
        { //::CEGUI::CoordConverter::windowToScreenX
        
            typedef float ( *windowToScreenX_function_type )( ::CEGUI::Window const &,float const );
            
            CoordConverter_exposer.def( 
                "windowToScreenX"
                , windowToScreenX_function_type( &::CEGUI::CoordConverter::windowToScreenX )
                , ( bp::arg("window"), bp::arg("x") )
                , "*!\n\
                \n\
                    Convert a window pixel co-ordinate value, specified as a float, to a\n\
                    screen pixel co-ordinate.\n\
            \n\
                @param window\n\
                    Window object to use as a base for the conversion.\n\
            \n\
                @param x\n\
                    float x co-ordinate value to be converted.\n\
            \n\
                @return\n\
                    float value describing a pixel screen co-ordinate that is equivalent to\n\
                    window co-ordinate  x.\n\
                *\n" );
        
        }
        { //::CEGUI::CoordConverter::windowToScreenY
        
            typedef float ( *windowToScreenY_function_type )( ::CEGUI::Window const &,::CEGUI::UDim const & );
            
            CoordConverter_exposer.def( 
                "windowToScreenY"
                , windowToScreenY_function_type( &::CEGUI::CoordConverter::windowToScreenY )
                , ( bp::arg("window"), bp::arg("y") )
                , "*!\n\
                \n\
                    Convert a window co-ordinate value, specified as a UDim, to a screen\n\
                    relative pixel co-ordinate.\n\
            \n\
                @param window\n\
                    Window object to use as a base for the conversion.\n\
            \n\
                @param y\n\
                    UDim y co-ordinate value to be converted\n\
            \n\
                @return\n\
                    float value describing a screen co-ordinate that is equivalent to\n\
                    window UDim co-ordinate  y.\n\
                *\n" );
        
        }
        { //::CEGUI::CoordConverter::windowToScreenY
        
            typedef float ( *windowToScreenY_function_type )( ::CEGUI::Window const &,float const );
            
            CoordConverter_exposer.def( 
                "windowToScreenY"
                , windowToScreenY_function_type( &::CEGUI::CoordConverter::windowToScreenY )
                , ( bp::arg("window"), bp::arg("y") )
                , "*!\n\
                \n\
                    Convert a window pixel co-ordinate value, specified as a float, to a\n\
                    screen pixel co-ordinate.\n\
            \n\
                @param window\n\
                    Window object to use as a base for the conversion.\n\
            \n\
                @param y\n\
                    float y co-ordinate value to be converted.\n\
            \n\
                @return\n\
                    float value describing a screen co-ordinate that is equivalent to\n\
                    window co-ordinate  y.\n\
                *\n" );
        
        }
        CoordConverter_exposer.staticmethod( "asAbsolute" );
        CoordConverter_exposer.staticmethod( "asRelative" );
        CoordConverter_exposer.staticmethod( "screenToWindow" );
        CoordConverter_exposer.staticmethod( "screenToWindowX" );
        CoordConverter_exposer.staticmethod( "screenToWindowY" );
        CoordConverter_exposer.staticmethod( "windowToScreen" );
        CoordConverter_exposer.staticmethod( "windowToScreenX" );
        CoordConverter_exposer.staticmethod( "windowToScreenY" );
    }

}
