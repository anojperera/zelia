/* Implementation of junction boxes
 * Tue Jan 31 13:00:27 GMTST 2012*/

#include <math.h>
#include "zjb.h"
#include "zglands.h"
#include "zterminals.h"

#define ZJB_COORDS 4

/* Virtual function */
static int _zjb_draw(void* obj);
static int _zjb_delete(void* obj);

/* Constructor */
zgeneric* zjb_new(zjb* obj,				/* optional NULL pointer */
		  zdevice* dev,
		  double x,				/* x coord */
		  double y,				/* y coord */
		  double width,				/* width */
		  double height,			/* height */
		  double depth,				/* depth */
		  double ang)				/* angle */
{
    /* check for object pointer */
    ZCONSTRUCTOR(obj, zjb);

    /* Create parent object, if failed return NULL */
    if(!(obj->super_cls = zbase_new(&obj->parent)))
	{
	    if(ZDESTRUCTOR_CHECK)
		free(obj);
	    return NULL;
	}

    /* Set properties */
    obj->terms_ext = 1;
    obj->glands_ext = 1;

    obj->depth = depth;
    obj->ang = ang;
    obj->rad = 0.0;
    zbase_set_width_and_height(&obj->parent, width, height);
    zbase_set_base_coords(&obj->parent, x, y);

    /* Set device object if set */
    if(dev)
	{
	    zgeneric_set_device(obj->super_cls, dev);
	    zgeneric_set_default_dev_context(obj->super_cls);
	}

    obj->eterms = NULL;
    obj->terms = NULL;
    obj->glands = NULL;

    obj->child = NULL;

    /* Assign function pointer of parent object */
    zgeneric_set_draw(obj, _zjb_draw);
    zgeneric_set_delete_callback(obj, _zjb_delete);

    /* Set child pointer of parent object */
    zgeneric_set_child_pointer(obj);

    /* Return parent pointer */
    return obj->super_cls;
}

/* Destructor */
void zjb_delete(zjb* obj)
{
    ZCHECK_OBJ_VOID(obj);

    /* Call parent destructor */
    zgeneric_block_parent_destructor(obj);
    zbase_delete(&obj->parent);

    /* Call destructor of terminal collection
     * if internally set */
    if(obj->terms_ext == 0)
	zterminals_delete(Z_TERMINALS(obj->terms));

    /* Call destructor of earh terminal collection
     * if internally set */
    if(obj->eterms_ext == 0)
	zterminals_delete(Z_TERMINALS(obj->eterms));

    /* Call destructor of glands collection
     * if internally set */
    if(obj->glands_ext == 0)
	zglands_delete(Z_GLANDS(obj->glands));

    obj->terms = NULL;
    obj->eterms = NULL;
    obj->glands = NULL;
    obj->child = NULL;
    obj->super_cls = NULL;

    if(ZDESTRUCTOR_CHECK)
	free(obj);

    return;
}

/* Draw function */
int zjb_draw(zjb* obj)
{

    zbase* _base;				/* base object pointer */
    cairo_t* _dev_c;				/* cairo context */
    int _i;

    /* line arc coordinates */
    double _x1[ZJB_COORDS], _x2[ZJB_COORDS], _y1[ZJB_COORDS], _y2[ZJB_COORDS];
    double _ax[ZJB_COORDS], _ay[ZJB_COORDS];
    double _a_ang_s[ZJB_COORDS], _a_ang_e[ZJB_COORDS];

    /* Check for object */
    ZCHECK_OBJ_INT(obj);

    /* Get base and generic object */
    _base = &obj->parent;

    /* Check all objects */
    ZCHECK_OBJ_INT(_base);

    /* Device context pointer */
    _dev_c = zgeneric_get_dev_context(obj->super_cls);
    ZCHECK_OBJ_INT(_dev_c);

    /* move device context to the base coordinates and
     * draw outside rectangle */

    /* line coordinates */
    _x1[0] = _base->x + obj->rad;
    _y1[0] = _base->y;
    _x2[0] = _base->x + (obj->ang == 0.0? _base->width : _base->height) - obj->rad;
    _y2[0] = _base->y;

    _x1[1] = _x2[0] + obj->rad;
    _y1[1] = _base->y + obj->rad;
    _x2[1] = _x1[1];
    _y2[1] = _base->y + (obj->ang == 0.0? _base->height : _base->width) - obj->rad;

    _x1[2] = _x1[1] - obj->rad;
    _y1[2] = _y2[1] + obj->rad;
    _x2[2] = _x1[0];
    _y2[2] = _y1[2];

    _x1[3] = _base->x;
    _y1[3] = _y2[1];
    _x2[3] = _x1[3];
    _y2[3] = _base->y + obj->rad;

    /* arc coordinates */
    _ax[0] = _base->x + obj->rad;
    _ay[0] = _base->y + obj->rad;
    _a_ang_s[0] = M_PI;
    _a_ang_e[0] = 3 * M_PI / 2;

    _ax[1] = _base->x + (obj->ang == 0.0? _base->width : _base->height) - obj->rad;
    _ay[1] = _ay[0];
    _a_ang_s[1] = 3 * M_PI / 2;
    _a_ang_e[1] = 0.0;

    _ax[2] = _ax[1];
    _ay[2] = _base->y + (obj->ang == 0.0? _base->height : _base->width) - obj->rad;
    _a_ang_s[2] = 0.0;
    _a_ang_e[2] = M_PI / 2;

    _ax[3] = _ax[0];
    _ay[3] = _ay[2];
    _a_ang_s[3] = M_PI / 2;
    _a_ang_e[3] = M_PI;

    for(_i = 0; _i < ZJB_COORDS; _i++)
	{
	    cairo_move_to(_dev_c, ZCONV_TO_POINTS(_x1[_i]), ZCONV_TO_POINTS(_y1[_i]));
	    cairo_line_to(_dev_c, ZCONV_TO_POINTS(_x2[_i]), ZCONV_TO_POINTS(_y2[_i]));
	}

    cairo_stroke(_dev_c);

    if(obj->rad > 0.0)
    	{
    	    for(_i=0; _i<ZJB_COORDS; _i++)
    		{
    		    cairo_arc(_dev_c,
    			      ZCONV_TO_POINTS(_ax[_i]),
    			      ZCONV_TO_POINTS(_ay[_i]),
    			      ZCONV_TO_POINTS(obj->rad),
    			      _a_ang_s[_i],
    			      _a_ang_e[_i]);
		    cairo_stroke(_dev_c);
    		}
    	}

    /* call stroke as terminals and glands may have not assigned */


    /* call draw functions of glands and terminals */
    zgenerics_draw(obj->terms);
    zgenerics_draw(obj->eterms);
    zgenerics_draw(obj->glands);

    return ZELIA_OK;
}

int zjb_add_with_earth_terminals(zjb* obj,
		      unsigned int num_term,			/* number of terminals */
		      unsigned int num_eterm,			/* number of earth terminals */
		      double width,				/* terminal width */
		      double height,				/* terminal height */
		      const char* e_annot,			/* earth terminal annotation */
		      const char* links)			/* terminal links */
{
    double _w, _h, _bw, _bh;
    zbase* _base;

    /* Check for object */
    ZCHECK_OBJ_INT(obj);

    /* Get base object */
    _base = &obj->parent;

    ZCHECK_OBJ_INT(_base);

    _w = (obj->ang == 0.0? width * (double) (num_term + num_eterm) : height);
    _h = (obj->ang == 0.0? height : width * (double) (num_term + num_eterm));

    _bw = (obj->ang == 0.0? _base->width : _base->height);
    _bh = (obj->ang == 0.0? _base->height : _base->width);

    /* Create internal terminals collection */
    obj->terms = zterminals_new(NULL,				/* object pointer */
				zgeneric_get_device(obj->super_cls),
				num_term,
				_base->x + (_bw - _w) / 2 + width * num_eterm,
				_base->y + (_bh - _h) / 2,
				width,
				height,
				obj->ang,
				links);

    if(num_eterm > 0)
	{
	    obj->eterms = zterminals_new_with_annot(NULL,				/* object pointer */
						    zgeneric_get_device(obj->super_cls),
						    num_eterm,
						    _base->x + (_bw - _w) / 2,
						    _base->y + (_bh - _h) / 2,
						    width,
						    height,
						    obj->ang,
						    e_annot,
						    links);
	}

    /* Check for object */
    ZCHECK_OBJ_INT(obj->terms);

    /* Set flag to indicate terminal collection created interanlly
     * so that class destructor will call terminal collections
     * destructor method */
    obj->terms_ext = 0;
    obj->eterms_ext = 0;

    return ZELIA_OK;
}


/* Adds a gland into the internal collection */
int zjb_add_glands(zjb* obj,
		   double x,					/* relative coordinate */
		   double y,					/* relative coordinate */
		   zGlandSize sz,				/* gland size */
		   unsigned int hex_flg)			/* hex flag */
{
    zdevice* _dev;
    zbase* _base;

    /* check for object */
    ZCHECK_OBJ_INT(obj);

    /* if the glands collection is not initialised, create */
    if(obj->glands == NULL)
	obj->glands = zglands_new(NULL);

    _dev = zgeneric_get_device(obj->super_cls);
    _base = &obj->parent;

    obj->glands_ext = 0;
    /* Adds a gland into the collection */
    return zglands_add(Z_GLANDS(obj->glands),
		       _dev,
		       _base->x + x,
		       _base->y + y,
		       sz,
		       hex_flg);
}


/*=================================== Private Methods ===================================*/

static int _zjb_draw(void* obj)
{
    zgeneric* _zg = NULL;
    zjb* _zjb = NULL;

    /* Check for object */
    ZCHECK_OBJ_INT(obj);
    _zg = (zgeneric*) obj;

    _zjb = Z_JB(_zg);

    return zjb_draw(_zjb);

}

static int _zjb_delete(void* obj)
{
    zgeneric* _zg = NULL;

    ZCHECK_OBJ_INT(obj);

    _zg = (zgeneric*) obj;

    zjb_delete(Z_JB(_zg));
    return  ZELIA_OK;
}
