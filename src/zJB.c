/* Implementation of junction boxes
 * Tue Jan 31 13:00:27 GMTST 2012*/

#include <math.h>
#include "zJB.h"
#include "zGlands.h"
#include "zTerminals.h"

/* Virtual function */
static int _zjb_draw(zGeneric* obj);


/* Constructor */
zGeneric* zJB_New(zJB* obj,				/* optional NULL pointer */
		  zDevice* dev,
		  double x,				/* x coord */
		  double y,				/* y coord */
		  double width,				/* width */
		  double height,			/* height */
		  double depth,				/* depth */
		  double ang)				/* angle */
{
    /* check for object pointer */
    if(obj == NULL)
	{
	    obj = (zJB*) malloc(sizeof(zJB));
	    Z_CHECK_OBJ_PTR(obj);
	    obj->z_int_flg = 1;
	}
    else
	obj->z_int_flg = 0;

    /* Create parent object, if failed return NULL */
    if(!zBase_New(&obj->z_parent))
	{
	    if(obj->z_int_flg == 1)
		free(obj);

	    return NULL;
	}

    /* Set properties */
    obj->z_terms_ext = 1;
    obj->z_glands_ext = 1;

    obj->z_depth = depth;
    obj->z_ang = ang;
    obj->z_rad = 0.0;
    zBase_Set_Width_and_Height(&obj->z_parent, width, height);
    zBase_Set_Base_Coords(&obj->z_parent, x, y);

    /* Set device object if set */
    if(dev)
	{
	    zGeneric_Set_Device(&obj->z_parent.z_sgeneric, dev);
	    zGeneric_Set_Default_Dev_Context(&obj->z_parent.z_sgeneric);
	}
    obj->z_terms = NULL;
    obj->z_glands = NULL;
    obj->z_obj_sz = sizeof(zJB);
    obj->z_child = NULL;

    /* Assign function pointer of parent object */
    obj->z_parent.z_draw_func = _zjb_draw;

    /* Set child pointer of parent object */
    obj->z_parent.z_child = (void*) obj;
    obj->z_obj_sz = sizeof(zJB);

    /* Return parent pointer */
    return &obj->z_parent.z_sgeneric;
}

/* Destructor */
void zJB_Delete(zJB* obj)
{
    Z_CHECK_OBJ_VOID(obj);

    /* Call parent destructor */
    zBase_Delete(&obj->z_parent);

    /* Call destructor of terminal collection
     * if internally set */
    if(obj->z_terms_ext == 0)
	zTerminals_Delete(Z_TERMINALS(obj->z_terms));

    /* Call destructor of glands collection
     * if internally set */
    if(obj->z_glands_ext == 0)
	zGlands_Delete(Z_GLANDS(obj->z_glands));
    
    obj->z_terms = NULL;
    obj->z_glands = NULL;
    obj->z_draw_func = NULL;
    obj->z_child = NULL;

    if(obj->z_int_flg)
	free(obj);
    
}

/* Draw function */
int zJB_Draw(zJB* obj)
{
#define COORDS 4
    zBase* _base;				/* base object pointer */
    zGeneric* _genric;				/* generic object pointer */
    cairo_t* _dev_c;				/* cairo context */
    int i;

    /* line arc coordinates */
    double x1[COORDS], x2[COORDS], y1[COORDS], y2[COORDS];
    double ax[COORDS], ay[COORDS];
    double a_ang_s[COORDS], a_ang_e[COORDS];
    
    /* Check for object */
    Z_CHECK_OBJ(obj);

    /* Get base and generic object */
    _base = &obj->z_parent;
    _genric = &obj->z_parent.z_sgeneric;

    /* Check all objects */
    Z_CHECK_OBJ(_base);
    Z_CHECK_OBJ(_genric);

    /* Device context pointer */
    _dev_c = zGeneric_Get_Dev_Context(_genric);
    Z_CHECK_OBJ(_dev_c);

    /* move device context to the base coordinates and
     * draw outside rectangle */
    
    /* line coordinates */
    x1[0] = _base->z_x + obj->z_rad;
    y1[0] = _base->z_y;
    x2[0] = _base->z_x + (obj->z_ang == 0.0? _base->z_width : _base->z_height) - obj->z_rad;
    y2[0] = _base->z_y;

    x1[1] = x2[0] + obj->z_rad;
    y1[1] = _base->z_y + obj->z_rad;
    x2[1] = x1[1];
    y2[1] = _base->z_y + (obj->z_ang == 0.0? _base->z_height : _base->z_width) - obj->z_rad;

    x1[2] = x1[1] - obj->z_rad;
    y1[2] = y2[1] + obj->z_rad;
    x2[2] = x1[0];
    y2[2] = y1[2];

    x1[3] = _base->z_x;
    y1[3] = y2[1];
    x2[3] = x1[3];
    y2[3] = _base->z_y + obj->z_rad;
    
    /* arc coordinates */
    ax[0] = _base->z_x + obj->z_rad;
    ay[0] = _base->z_y + obj->z_rad;
    a_ang_s[0] = M_PI;
    a_ang_e[0] = 3 * M_PI / 2;

    ax[1] = _base->z_x + (obj->z_ang == 0.0? _base->z_width : _base->z_height) - obj->z_rad;
    ay[1] = ay[0];
    a_ang_s[1] = 3 * M_PI / 2;
    a_ang_e[1] = 0.0;

    ax[2] = ax[1];
    ay[2] = _base->z_y + (obj->z_ang == 0.0? _base->z_height : _base->z_width) - obj->z_rad;
    a_ang_s[2] = 0.0;
    a_ang_e[2] = M_PI / 2;

    ax[3] = ax[0];
    ay[3] = ay[2];
    a_ang_s[3] = M_PI / 2;
    a_ang_e[3] = M_PI;
    
    for(i = 0; i < COORDS; i++)
	{
	    cairo_move_to(_dev_c, CONV_TO_POINTS(x1[i]), CONV_TO_POINTS(y1[i]));
	    cairo_line_to(_dev_c, CONV_TO_POINTS(x2[i]), CONV_TO_POINTS(y2[i]));
	}

    cairo_stroke(_dev_c);
    
    if(obj->z_rad > 0.0)
    	{
    	    for(i = 0; i < COORDS; i++)
    		{
    		    cairo_arc(_dev_c,
    			      CONV_TO_POINTS(ax[i]),
    			      CONV_TO_POINTS(ay[i]),
    			      CONV_TO_POINTS(obj->z_rad),
    			      a_ang_s[i],
    			      a_ang_e[i]);
		    cairo_stroke(_dev_c);
    		}
    	}

    /* call stroke as terminals and glands may have not assigned */


    /* call draw functions of glands and terminals */
    zGenerics_Draw(obj->z_terms);
    zGenerics_Draw(obj->z_glands);

    /* call child drawing function pointer if assigned */
    if(obj->z_draw_func)
	return obj->z_draw_func(_genric);

    return 0;
}

/* Set external terminal collection */
inline int zJB_Set_Terminals(zJB* obj, zGenerics* terms)
{
    /* check for objects */
    Z_CHECK_OBJ(obj);
    Z_CHECK_OBJ(terms);

    if(obj->z_terms_ext == 0)
	return 1;
    
    obj->z_terms = terms;

    /* indicate external flag was set */
    obj->z_terms_ext = 1;
    return 0;
}

/* Get terminals collection */
inline zGenerics* zJB_Get_Terminals(zJB* obj)
{
    Z_CHECK_OBJ_PTR(obj);
    return obj->z_terms;
}

/* Set fillet radius */
inline int zJB_Set_Fillet_Radius(zJB* obj, double rad)
{
    /* Check for object */
    Z_CHECK_OBJ(obj);
    obj->z_rad = rad;
    return 0;
}

/* Get fillet radius */
inline double zJB_Get_Fillet_Radius(zJB* obj)
{
    Z_CHECK_OBJ(obj);
    return obj->z_rad;
}

/* Add terminal collection*/
inline int zJB_Add_Terminals(zJB* obj,
			     unsigned int num_term,			/* number of terminals */
			     double width,				/* terminal width */
			     double height,				/* terminal height */
			     const char* links)
{
    double _w, _h, _bw, _bh;
    zBase* _base;
    zGeneric* _genric;
    
    /* Check for object */
    Z_CHECK_OBJ(obj);

    /* Get base object */
    _base = &obj->z_parent;
    _genric = &obj->z_parent.z_sgeneric;

    Z_CHECK_OBJ(_base);
    Z_CHECK_OBJ(_genric);

    _w = (obj->z_ang == 0.0? width * (double) num_term : height);
    _h = (obj->z_ang == 0.0? height : width * (double) num_term);

    _bw = (obj->z_ang == 0.0? _base->z_width : _base->z_height);
    _bh = (obj->z_ang == 0.0? _base->z_height : _base->z_width);

    /* Create internal terminals collection */
    obj->z_terms = zTerminals_New(NULL,				/* object pointer */
				  zGeneric_Get_Device(_genric),
				  num_term,
				  _base->z_x + (_bw - _w) / 2,
				  _base->z_y + (_bh - _h) / 2,
				  width,
				  height,
				  obj->z_ang,
				  links);

    /* Check for object */
    Z_CHECK_OBJ(obj->z_terms);

    /* Set flag to indicate terminal collection created interanlly
     * so that class destructor will call terminal collections
     * destructor method */
    obj->z_terms_ext = 0;

    return 0;
}

/* Set external cable glands collection */
inline int zJB_Set_Glands(zJB* obj, zGenerics* glands)
{
    Z_CHECK_OBJ(obj);
    Z_CHECK_OBJ(glands);

    if(obj->z_glands == 0)
	return 1;
    
    obj->z_glands = glands;
    obj->z_glands_ext = 1;

    return 0;
}

/* Get cable gland collection */
inline zGenerics* zJB_Get_Glands(zJB* obj)
{
    /* Check for object */
    Z_CHECK_OBJ_PTR(obj);
    return obj->z_glands;
}

/* Adds a gland into the internal collection */
inline int zJB_Add_Glands(zJB* obj,
			  double x,					/* relative coordinate */
			  double y,					/* relative coordinate */
			  zGlandSize sz,				/* gland size */
			  unsigned int hex_flg)			/* hex flag */
{
    zDevice* _dev;
    zBase* _base;
    /* check for object */
    Z_CHECK_OBJ(obj);

    /* if the glands collection is not initialised, create */
    if(obj->z_glands == NULL) 
	obj->z_glands = zGlands_New(NULL);

    _dev = zGeneric_Get_Device(&obj->z_parent.z_sgeneric);
    _base = &obj->z_parent;

    obj->z_glands_ext = 0;
    /* Adds a gland into the collection */
    return zGlands_Add(Z_GLANDS(obj->z_glands),
		       _dev,
		       _base->z_x + x,
		       _base->z_y + y,
		       sz,
		       hex_flg);
}


/*==========================================================================*/
/* Private Functions */

static int _zjb_draw(zGeneric* obj)
{
    int rt_val;
    zJB* z_jb;
    /* Check for object */
    Z_CHECK_OBJ(obj);
    z_jb = Z_JB(obj);
    rt_val = zJB_Draw(Z_JB(obj));
    if(z_jb->z_draw_func)
	return z_jb->z_draw_func(obj);
    else
	return rt_val;
}
