/* Implemenetation of zTerminal class */
/* Sat Jan 28 20:20:10 GMT 2012 */

#include <ctype.h>
#include "zVar.h"
#include "zTerminals.h"
#include "zTerminal.h"
#include "zBase.h"
#include "zGeneric.h"

/* Virtual functions */
static int _zterminals_delete(zGeneric* obj, void* usr_data);		/* delte function */
static int _zterminals_draw(zGeneric* obj, void* usr_data);		/* draw function */
static inline int _zterminals_parser(zTerminals* obj);			/* terminals parser */

static int d_count = 0;

/* Constructor */
zGenerics* zTerminals_New(zTerminals* obj,
			  zDevice* dev,			/* device object */
			  unsigned int num_term,	/* number of terminals */
			  double x,			/* base coordinate */
			  double y,			/* base coordinate */
			  double width,			/* width */
			  double height,		/* height */
			  double ang,			/* orientation angle */
			  const char* links)		/* links */
{
    int i;
    
    /* check for object */
    if(obj == NULL)
	{
	    obj = (zTerminals*) malloc(sizeof(zTerminals));
	    Z_CHECK_OBJ_PTR(obj);
	    obj->z_int_flg = 1;
	}
    else
	obj->z_int_flg = 0;

    /* create parent object */
    if(zGenerics_New(&obj->z_parent,
		     0,
		     num_term))
	{
	    /* If the object was created internally, delete it */
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}

    /* Set device */
    zGenerics_Set_Device(&obj->z_parent, dev);
    
    /* set properties */
    obj->z_x = x;
    obj->z_y = y;
    obj->z_width = width;
    obj->z_height = height;
    obj->ang = ang;
    obj->z_links_flg = 0;

    if(links)
	{
	    strcpy(obj->z_term_links, links);
	    if(strlen(obj->z_term_links) < ZTERMINALS_LK_SZ)
		strncat(obj->z_term_links, ".", sizeof(char));
	}
    else
	obj->z_term_links[0] = '\0';

    /* create coordinate array */
    obj->z_x_links = (double*) calloc(num_term, sizeof(double));
    obj->z_y_links = (double*) calloc(num_term, sizeof(double));

    /* create objects */
    for(i=0; i<num_term; i++)
	{
	    obj->z_parent.z_generics_s[i] =
		zTerminal_New(NULL);

	    /* Set terminal number */
	    zTerminal_Set_Terminal_Number(Z_TERMINAL(obj->z_parent.z_generics_s[i]),
					  i+1);
	    
	    zBase_Set_Base_Coords(Z_BASE(obj->z_parent.z_generics_s[i]),
				  ang==90.0? x : x + (double) i * width,
				  ang==90.0? y + (double) i * width : y);	    
	    zBase_Set_Width_and_Height(Z_BASE(obj->z_parent.z_generics_s[i]),
				       ang==90.0? height : width,
				       ang==90.0? width : height);

	    zGeneric_Set_Device(obj->z_parent.z_generics_s[i],
				zGenerics_Get_Device(&obj->z_parent));

	    /* Set default device context */
	    zGeneric_Set_Default_Dev_Context(obj->z_parent.z_generics_s[i]);

	    /* add link coordinates */
	    obj->z_x_links[i] = ang==90.0? x + 3 * height / 4 : x + (double) i * width + width / 2;
	    obj->z_y_links[i] = ang==90.0? y + (double) i * width + width / 2 : y + 3 * height / 4;
	    
	}

    /* set function pointers of parent object */
    obj->z_parent.z_destructor_func = _zterminals_delete;
    obj->z_parent.z_draw_func = _zterminals_draw;

    /* child pointer of base object */
    obj->z_parent.z_child = (void*) obj;

    /* add user data to parent object */
    obj->z_parent.z_usr_data = (void*) obj;
    
    obj->z_obj_sz = sizeof(zTerminals);
    /* return parent object */
    return &obj->z_parent;
    
}

/* Destructor */
void zTerminals_Delete(zTerminals* obj)
{
    /* check object */
    Z_CHECK_OBJ_VOID(obj);

    /* call delete method of parent object */
    zGenerics_Delete(&obj->z_parent);

    free(obj->z_x_links);
    free(obj->z_y_links);
    
    obj->z_x_links = NULL;
    obj->z_y_links = NULL;
    

    if(obj->z_int_flg)
	free(obj);
}

/*=========================================================================*/
/* Private functions */

/* Virtual delete function */
static int _zterminals_delete(zGeneric* obj, void* usr_data)
{
    zTerminal_Delete(Z_TERMINAL(obj));
    return 0;
}


/* Virtual draw function */
static int _zterminals_draw(zGeneric* obj, void* usr_data)
{
    d_count++;
    
    if(obj == NULL || usr_data == NULL)
	return 1;

    zTerminals* zts = (zTerminals*) usr_data;    
    zTerminal_Draw(Z_TERMINAL(obj));
    
    /* if counter is reached max, draw links if required */
    if(d_count == zts->z_parent.z_count)
	{
	    _zterminals_parser(zts);
	    d_count = 0;
	}

    return 0;
}

/* Terminal links parser */
static int _zterminals_parser(zTerminals* obj)
{
    int i, j, a, b;		 /* counters */
    char** _tok;
    char* _val;
    int _tnum[2];		/* Terminal number */
    int st, ed;			/* Start and end indexes */
    zDevice* _dev;		/* Device */
    char* _tok2;		/* second token */
    
    /* Check link string */
    if(obj->z_term_links[0] == '\0')
	return 1;
    
    /* Get device */
    _dev = zGenerics_Get_Device(&obj->z_parent);

    /* split the string into tokens */
    b = 0;
    for(j=0; j < strlen(obj->z_term_links); j++)
	if(obj->z_term_links[j] == 44 ||
	   obj->z_term_links[j] == 46)
	    b++;

    _tok = (char**) calloc(b, sizeof(char*));
    _tok2 = strtok(obj->z_term_links, ",.");
    j = 0;
    while(_tok2 != NULL)
	{
	    if(j >= b)
		break;
	    _tok[j] = malloc(sizeof(char) * (strlen(_tok2) + 1));
	    strcpy(_tok[j], _tok2);
	    j++;
	    _tok2 = strtok(NULL, ",.");
	}
    
    /* split the string into tokens */    
    for(j=0; j<b; j++)
	{
	    i = 0;				/* initialise counter */
	    _tnum[0] = -1;			/* reset to -1 */
	    _tnum[1] = -1;			/* reset to -1 */
	    _val = strtok(_tok[j], "-");
	    while(_val != NULL)
	    	{
	    	    /* break out of loop if max is reached */
	    	    if(i == 2 || !isdigit(_val[0]))
	    		break;
	    	    _tnum[i] = atoi(_val) - 1;

	    	    _val = strtok(NULL, "-");
	    	    i++;
	    	}
	    	    
	    /* get the lower oder */
	    st = _tnum[0] > _tnum[1]? _tnum[1] : _tnum[0];
	    ed = _tnum[1] > _tnum[0]? _tnum[1] : _tnum[0];

	    if(ed < 0 || st < 0)
	    	continue;

	    /* check array bounds */
	    if(st >= zGenerics_Get_Count(&obj->z_parent) ||
	       ed >= zGenerics_Get_Count(&obj->z_parent))
		continue;
	    
	    /* Draw link line */
	    cairo_move_to(zDevice_Get_Context(_dev),
	    		  CONV_TO_POINTS(obj->z_x_links[st]),
	    		  CONV_TO_POINTS(obj->z_y_links[st]));
	    cairo_line_to(zDevice_Get_Context(_dev),
	    		  CONV_TO_POINTS(obj->z_x_links[ed]),
	    		  CONV_TO_POINTS(obj->z_y_links[ed]));
	    
	    cairo_stroke(zDevice_Get_Context(_dev));

	    /* link connections */
	    for(a=st; a<ed+1; a++)
	    	{
	    	    cairo_move_to(zDevice_Get_Context(_dev),
	    			  CONV_TO_POINTS(obj->z_x_links[a]),
	    			  CONV_TO_POINTS(obj->z_y_links[a]));
	    	    cairo_arc(zDevice_Get_Context(_dev),
	    		      CONV_TO_POINTS(obj->z_x_links[a]),
	    		      CONV_TO_POINTS(obj->z_y_links[a]),
	    		      CONV_TO_POINTS(2),
	    		      0.0,
	    		      2 * M_PI);
	    	    cairo_fill(zDevice_Get_Context(_dev));
	    	}
	}
    return 0;
}
