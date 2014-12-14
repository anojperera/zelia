/* Implemenetation of zTerminal class */
/* Sat Jan 28 20:20:10 GMT 2012 */

#include <ctype.h>
#include "zVar.h"

#include "zbase.h"
#include "zgeneric.h"
#include "zterminal.h"

#include "zterminals.h"

#define ZTERMINALS_SPLIT_TOKEN ",."
#define ZTERMINALS_RANGE_TOKEN "-"

/* Virtual functions */
static int _zterminals_delete(void* obj);			/* delte function */
static int _zterminals_draw(void* obj);				/* draw function */
static inline int _zterminals_parser(zterminals* obj);		/* terminals parser */


/* Constructor */
zgenerics* zterminals_new(zterminals* obj,
			  zdevice* dev,			/* device object */
			  unsigned int num_term,	/* number of terminals */
			  double x,			/* base coordinate */
			  double y,			/* base coordinate */
			  double width,			/* width */
			  double height,		/* height */
			  double ang,			/* orientation angle */
			  const char* links)		/* links */
{
    int _i;

    ZCONSTRUCTOR(obj, zterminals);

    /* create parent object */
    if(!(obj->super_cls = zgenerics_new(&obj->parent,
				       0,
					num_term)))
	{
	    /* If the object was created internally, delete it */
	    if(ZDESTRUCTOR_CHECK)
		free(obj);
	    return NULL;
	}

    /* Set device */
    zgenerics_set_device(&obj->parent, dev);

    /* set properties */
    obj->_d_counter = 0;
    obj->x = x;
    obj->y = y;
    obj->width = width;
    obj->height = height;
    obj->ang = ang;
    obj->links_flg = 0;
    obj->child = NULL;

    ZGENERIC_INIT_VTABLE(obj);

    if(links)
	{
	    strcpy(obj->term_links, links);
	    if(strlen(obj->term_links) < ZTERMINALS_LK_SZ)
		strncat(obj->term_links, ".", sizeof(char));
	}
    else
	obj->term_links[0] = '\0';

    /* create coordinate array */
    obj->x_links = (double*) calloc(num_term, sizeof(double));
    obj->y_links = (double*) calloc(num_term, sizeof(double));

    /* create objects */
    for(_i=0; _i<num_term; _i++)
	{
	    obj->parent.generics_s[_i] = zterminal_new(NULL);

	    /* set collection pointer */
	    zgeneric_set_collection_pointer(obj->parent.generics_s[_i], (void*) obj);

	    /* Set terminal number */
	    zterminal_set_terminal_number(Z_TERMINAL(obj->parent.generics_s[_i]), _i+1);

	    zbase_set_base_coords(Z_BASE(obj->parent.generics_s[_i]),
				  ang==90.0? x : x + (double) _i * width,
				  ang==90.0? y + (double) _i * width : y);
	    zbase_set_width_and_height(Z_BASE(obj->parent.generics_s[_i]),
				       ang==90.0? height : width,
				       ang==90.0? width : height);

	    zgeneric_set_device(obj->parent.generics_s[_i],
				zgenerics_get_device(&obj->parent));

	    /* Set default device context */
	    zgeneric_set_default_dev_context(obj->parent.generics_s[_i]);

	    /* add link coordinates */
	    obj->x_links[_i] = ang==90.0? x + 3 * height / 4 : x + (double) _i * width + width / 2;
	    obj->y_links[_i] = ang==90.0? y + (double) _i * width + width / 2 : y + 3 * height / 4;

	}

    /* set function pointers of parent object */
    zgeneric_set_draw(obj, _zterminals_draw);
    zgeneric_set_delete_callback(obj, _zterminals_delete);

    /* child pointer of base object */
    zgeneric_set_child_pointer(obj);

    /* add user data to parent object */
    obj->parent.usr_data = (void*) obj;

    /* return parent object */
    return obj->super_cls;
}

/* Destructor */
void zterminals_delete(zterminals* obj)
{
    /* check object */
    ZCHECK_OBJ_VOID(obj);

    if(obj->vtable.zgeneric_delete)
	{
	    obj->vtable.zgeneric_delete(obj->super_cls);
	    return;
	}

    /* call delete method of parent object */
    zgeneric_block_parent_destructor(obj);
    zgenerics_delete(&obj->parent);

    free(obj->x_links);
    free(obj->y_links);

    ZGENERIC_INIT_VTABLE(obj);

    obj->child = NULL;
    obj->x_links = NULL;
    obj->y_links = NULL;

    if(ZDESTRUCTOR_CHECK)
	free(obj);

    return;
}

/*=================================== Private Methods ===================================*/

/* Virtual delete function */
static int _zterminals_delete(void* obj)
{
    zgenerics* _zgs;

    ZCHECK_OBJ_INT(obj);
    _zgs = (zgenerics*) obj;

    zterminals_delete(Z_TERMINALS(_zgs));

    return ZELIA_OK;
}


/* Virtual draw function */
static int _zterminals_draw(void* obj)
{
    zgeneric* _zg = NULL;
    zterminals* _zts = NULL;

    ZCHECK_OBJ_INT(obj);

    _zg = (zgeneric*) obj;

    _zts = (zterminals*) zgeneric_get_collection_pointer(_zg);
    zterminal_draw(Z_TERMINAL(_zg));

    /* if counter is reached max, draw links if required */
    if(_zts->_d_counter == _zts->parent.count)
	{
	    _zterminals_parser(_zts);
	    _zts->_d_counter = 0;
	}

    return 0;
}

/* Terminal links parser */
static int _zterminals_parser(zterminals* obj)
{
    int _i, _j, _a, _b;		 /* counters */
    char** _tok;
    char* _val;
    int _tnum[2];		/* Terminal number */
    int _st, _ed;		/* Start and end indexes */
    zdevice* _dev;		/* Device */
    char* _tok2;		/* second token */

    /* Check link string */
    if(obj->term_links[0] == '\0')
	return ZELIA_TERINALS_ERROR;

    /* Get device */
    _dev = zgenerics_get_device(&obj->parent);

    /* split the string into tokens */
    _b = 0;
    for(_j=0; _j < strlen(obj->term_links); _j++)
	if(obj->term_links[_j] == 44 ||
	   obj->term_links[_j] == 46)
	    _b++;

    _tok = (char**) calloc(_b, sizeof(char*));
    _tok2 = strtok(obj->term_links, ZTERMINALS_SPLIT_TOKEN);
    _j = 0;
    while(_tok2 != NULL)
	{
	    if(_j >= _b)
		break;
	    _tok[_j] = (char*) malloc(sizeof(char) * (strlen(_tok2) + 1));
	    strcpy(_tok[_j], _tok2);
	    _j++;
	    _tok2 = strtok(NULL, ZTERMINALS_SPLIT_TOKEN);
	}

    /* split the string into tokens */
    for(_j=0; _j<_b; _j++)
	{
	    _i = 0;				/* initialise counter */
	    _tnum[0] = -1;			/* reset to -1 */
	    _tnum[1] = -1;			/* reset to -1 */
	    _val = strtok(_tok[_j], ZTERMINALS_RANGE_TOKEN);
	    while(_val != NULL)
	    	{
	    	    /* break out of loop if max is reached */
	    	    if(_i == 2 || !isdigit(_val[0]))
	    		break;
	    	    _tnum[_i] = atoi(_val) - 1;

	    	    _val = strtok(NULL, ZTERMINALS_RANGE_TOKEN);
	    	    _i++;
	    	}

	    /* get the lower oder */
	    _st = _tnum[0] > _tnum[1]? _tnum[1] : _tnum[0];
	    _ed = _tnum[1] > _tnum[0]? _tnum[1] : _tnum[0];

	    if(_ed < 0 || _st < 0)
	    	continue;

	    /* check array bounds */
	    if(_st >= zgenerics_get_count(&obj->parent) ||
	       _ed >= zgenerics_get_count(&obj->parent))
		continue;

	    /* Draw link line */
	    cairo_move_to(zdevice_get_context(_dev),
	    		  ZCONV_TO_POINTS(obj->x_links[_st]),
	    		  ZCONV_TO_POINTS(obj->y_links[_st]));
	    cairo_line_to(zdevice_get_context(_dev),
	    		  ZCONV_TO_POINTS(obj->x_links[_ed]),
	    		  ZCONV_TO_POINTS(obj->y_links[_ed]));

	    cairo_stroke(zdevice_get_context(_dev));

	    /* link connections */
	    for(_a=_st; _a<_ed+1; _a++)
	    	{
	    	    cairo_move_to(zdevice_get_context(_dev),
	    			  ZCONV_TO_POINTS(obj->x_links[_a]),
	    			  ZCONV_TO_POINTS(obj->y_links[_a]));
	    	    cairo_arc(zdevice_get_context(_dev),
	    		      ZCONV_TO_POINTS(obj->x_links[_a]),
	    		      ZCONV_TO_POINTS(obj->y_links[_a]),
	    		      ZCONV_TO_POINTS(2),
	    		      0.0,
	    		      2 * M_PI);
	    	    cairo_fill(zdevice_get_context(_dev));
	    	}
	}

    for(_j=0; _j<_b; _j++)
	{
	    if(_tok[_j])
		free(_tok[_j]);
	    _tok[_j] = NULL;
	}

    if(_tok)
	free(_tok);
    _tok = NULL;
    return ZELIA_OK;
}
