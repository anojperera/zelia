/* implementation of generic object
   Sat Oct 16 17:51:25 BST 2010 */

#include "zGeneric.h"

/* constructor */
zGeneric* zGeneric_Create()
{
    /* temporary object */
    zGeneric* tmp;

    tmp = (zGeneric*) malloc(sizeof(zGeneric));

    /* check for NULL pointer */
    if(tmp == NULL)
	return NULL;

    tmp->z_gsheet_child = NULL;			/* child object
						   set to NULL */
	
    tmp->z_gdev = NULL;
    tmp->z_gcairo_dev = NULL;

    tmp->z_gline_color_ix = zLBlack;
    tmp->z_gline_weight = zLWeight4;
    tmp->z_gltype = zLTContinuous;

    return tmp;

}

/* delete device */
void zGeneric_Delete(zGeneric** obj)
{

	/* check for NULL pointer */
	if(obj == NULL ||
	   *obj == NULL)
		return;

	/* delete cairo context */
	if((*obj)->z_gcairo_dev)
		{
		cairo_destroy((*obj)->z_gcairo_dev);
		(*obj)->z_gcairo_dev = NULL;
		}

	free(*obj);
	*obj = NULL;

}
	
/* set device */
void zGeneric_Set_Device(zGeneric* obj, zDevice** var)
{
	/* check for NULL pointer */
	if(obj == NULL)
		return;

	/* check for device NULL pointer */
	if(var != NULL && *var != NULL)
		obj->z_gdev = var;

}

/* get device */
zDevice* zGeneric_Get_Device(zGeneric* obj)
{
	/* check for NULL pointer */
	if(obj == NULL)
		return NULL;
	else
		return *(obj->z_gdev);
}

/* set line colour index */
void zGeneric_Set_LineColour(zGeneric* obj,
								zLineColour var)
{
	/* check for NULL pointer */
	if(obj == NULL)
		return;

	obj->z_gline_color_ix = var;
	switch(obj->z_gline_color_ix)
		{
		case zLRed:
		{
			obj->z_gred_rgb = 255;
			obj->z_ggreen_rgb = 0;
			obj->z_gblue_rgb = 0;
			break;
		}
		case zLBlue:
		{
			obj->z_gred_rgb = 0;
			obj->z_ggreen_rgb = 0;
			obj->z_gblue_rgb = 255;
			break;
		}
		case zLYellow:
		{
			obj->z_gred_rgb = 255;
			obj->z_ggreen_rgb = 205;
			obj->z_gblue_rgb = 0;
			break;
		}
		case zLCyan:
		{
			obj->z_gred_rgb = 0;
			obj->z_ggreen_rgb = 255;
			obj->z_gblue_rgb = 255;
			break;
		}
		default:
		{
			obj->z_gred_rgb = 0;
			obj->z_ggreen_rgb = 0;
			obj->z_gblue_rgb = 0;
			break;
		}
		}
}

/* get line colour */
zLineColour zGeneric_Get_LineColour(zGeneric* obj)
{
	/* check for NULL pointer */
	if(obj == NULL)
		return zLBlack;
	else
		return obj->z_gline_color_ix;

}

/* set line weight */
void zGeneric_Set_LineWeight(zGeneric* obj,
			     zLineWeights var)
{
    /* check NULL pointer */
    if(obj == NULL)
	return;

    obj->z_gline_weight = var;

    /* set line weight to context */
    zgeneric_set_lineweight(&obj);

}

/* get line weights */
zLineWeights zGeneric_Get_LineWeight(zGeneric* obj)
{
	/* check NULL pointer */
	if(obj == NULL)
		return zLWeight1;
	else
		return obj->z_gline_weight;
}

/* set device context to default */
void zGeneric_Set_Default_Dev_Context(zGeneric* obj)
{
    /* check for NULL pointer */
    if(obj == NULL)
	return;

    /* check for device context */
    if(obj->z_gdev == NULL ||
       *(obj->z_gdev) == NULL)
	return;

    /* check if device context was set in
       device object */
    if((*obj->z_gdev)->z_device)
	obj->z_gcairo_dev = (*obj->z_gdev)->z_device;


}

/* create generic device context */
void zGeneric_Create_Dev_Context(zGeneric* obj)
{
	/* check for NULL pointer */
	if(obj == NULL)
		return;

	/* check if device was assigned */
	if(obj->z_gdev == NULL ||
	   *obj->z_gdev == NULL)
		return;

	/* check if surface was created */
	if(!(*obj->z_gdev)->z_surface)
		return;
	
	obj->z_gcairo_dev = cairo_create((*obj->z_gdev)->z_surface);
	
}

/* get pointer to device context */
cairo_t** zGeneric_Get_Dev_Context(zGeneric* obj)
{
	/* check for NULL pointer */
	if(obj == NULL)
		return NULL;

	if(obj->z_gcairo_dev == NULL)
		return NULL;
	else
		return &obj->z_gcairo_dev;

}

void zGeneric_Set_Defauts(zGeneric* obj)
{
	/* check for NULL pointer */
	if(obj == NULL)
		return;

	/* check if device context was
	   created */
	if(!obj->z_gcairo_dev)
		return;

	/* set default joining */
	cairo_set_line_join (obj->z_gcairo_dev,
						 CAIRO_LINE_JOIN_MITER);

	/* set default line weight */
	zgeneric_set_lineweight(&obj);
}

/* set line type */
void zGeneric_Set_LintType(zGeneric* obj, zLineTypes var)
{
	/* check for NULL pointer */
	if(obj == NULL)
		return;

	obj->z_gltype = var;
}

/* get line type */
zLineTypes zGeneric_Get_LineType(zGeneric* obj)
{
	/* check for NULL pointer */
	if(obj == NULL)
		return zLTContinuous;
	else
		return obj->z_gltype;

}

void zgeneric_set_lineweight(zGeneric** obj)
{
    /* check for NULL pointer */
    if(obj == NULL ||
       *obj == NULL)
	return;

    /* check for cairo device
       context */
    if((*obj)->z_gcairo_dev == NULL)
	return;

    switch((*obj)->z_gline_weight)
	{
	case zLWeight1:
	    cairo_set_line_width((*obj)->z_gcairo_dev,
				 Z_LINE_WEIGHT1);
	    break;
	case zLWeight2:
	    cairo_set_line_width((*obj)->z_gcairo_dev,
				 Z_LINE_WEIGHT2);
	    break;
	case zLWeight3:
	    cairo_set_line_width((*obj)->z_gcairo_dev,
				 Z_LINE_WEIGHT3);
	    break;
	case zLWeight5:
	    cairo_set_line_width((*obj)->z_gcairo_dev,
				 Z_LINE_WEIGHT5);
	    break;
	case zLWeight6:
	    cairo_set_line_width((*obj)->z_gcairo_dev,
				 Z_LINE_WEIGHT6);
	    break;
	case zLWeight7:
	    cairo_set_line_width((*obj)->z_gcairo_dev,
				 Z_LINE_WEIGHT7);
	    break;
	case zLWeight8:
	    cairo_set_line_width((*obj)->z_gcairo_dev,
				 Z_LINE_WEIGHT8);
	default:
	    cairo_set_line_width((*obj)->z_gcairo_dev,
				 Z_LINE_WEIGHT4);
	    break;
	}
	    
    return;
}
