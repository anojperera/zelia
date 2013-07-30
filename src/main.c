/* Temporary function to opern and draw a box and save */

#include <stdlib.h>
#include <string.h>
#include "zVar.h"
#include "zSheet.h"
#include <cairo/cairo.h>
#include <cairo/cairo-svg.h>
#include <librsvg-2.0/librsvg/rsvg.h>
#include <librsvg-2.0/librsvg/rsvg-cairo.h>


#define Z_FILE_PATH "/home/pyrus/Prog/C++/zelia/resources/wozair_border_a4.svg"

int main(int argc, char** argv)
{
    GError* _err;
    RsvgHandle* _svg;
    cairo_t* _cr;
    cairo_surface_t* _surface;
    RsvgDimensionData _dims;

    g_type_init();
    _svg = rsvg_handle_new_from_file(Z_FILE_PATH, &_err);
    if(_svg == NULL)
	{
	    fprintf(stderr, "Unable to open file\n");
	    return -1;
	}

    /* get dimensions */
    rsvg_handle_get_dimensions(_svg, &_dims);
    fprintf(stdout, "SVG width: %i\nSVG height: %i\n", _dims.width, _dims.height);
    /* create cairo surface */
    _surface = cairo_svg_surface_create("temp.svg",
					CONV_TO_POINTS(Z_A4_WIDTH),
					CONV_TO_POINTS(Z_A4_HEIGHT));

    if(cairo_surface_status(_surface) != CAIRO_STATUS_SUCCESS)
	{
	    fprintf(stderr, "Errors occured while creating surface\n");
	    rsvg_handle_close(_svg, &_err);
	    return -1;
	}

    /* create cairo dc and destroy the surface */
    _cr = cairo_create(_surface);
    cairo_scale(_cr, 0.8, 0.8);
    rsvg_handle_render_cairo(_svg, _cr);

    rsvg_handle_close(_svg, &_err);

    cairo_surface_destroy(_surface);
    if(cairo_status(_cr) != CAIRO_STATUS_SUCCESS)
	{
	    fprintf(stderr, "Device context creation failed");
	    rsvg_handle_close(_svg, &_err);
	    return -1;	    
	}
    
    cairo_destroy(_cr);
    return 0;
}


