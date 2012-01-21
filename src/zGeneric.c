/* implementation of generic object
   Sat Oct 16 17:51:25 BST 2010 */

#include "zGeneric.h"


/* private functions */
inline static int zgeneric_set_lineweight(zGeneric* obj);

/* constructor */
zGeneric* zGeneric_New(zGeneric* obj)
{

    if(obj == NULL)
	{
	    obj = (zGeneric*) malloc(sizeof(zGeneric));
	    Z_CHECK_OBJ_PTR(obj);
	    obj->z_int_flg = 1;
	}
    else
	obj->z_int_flg = 0;

    
    obj->z_child = NULL;			/* set child pointer to NULL */
	
    obj->z_gdev = NULL;
    obj->z_gcairo_dev = NULL;

    obj->z_gline_color_ix = zLBlack;
    obj->z_gline_weight = zLWeight4;
    obj->z_gltype = zLTContinuous;

    return obj;

}

/* delete device */
void zGeneric_Delete(zGeneric* obj)
{

    /* check for NULL pointer */
    Z_CHECK_OBJ_VOID(obj);

    /* delete cairo context */
    if(obj->z_gcairo_dev)
	{
	    cairo_destroy(obj->z_gcairo_dev);
	    obj->z_gcairo_dev = NULL;
	}

    obj->z_gdev = NULL;
    
    if(obj->z_int_flg)
	free(obj);
}
	
/* set device */
inline int zGeneric_Set_Device(zGeneric* obj, zDevice* var)
{
    /* check for arguments */
    Z_CHECK_OBJ(obj);
    Z_CHECK_OBJ(var);

    obj->z_gdev = var;
    return 0;

}

/* get device */
inline zDevice* zGeneric_Get_Device(zGeneric* obj)
{
    /* check for NULL pointer */
    Z_CHECK_OBJ_PTR(obj);
    return obj->z_gdev;
}

/* set line colour index */
inline int zGeneric_Set_LineColour(zGeneric* obj,
			     zLineColour var)
{
    /* check for NULL pointer */
    Z_CHECK_OBJ(obj);

    obj->z_gline_color_ix = var;
    switch(obj->z_gline_color_ix)
	{
	case zLRed:
	    obj->z_gred_rgb = 255;
	    obj->z_ggreen_rgb = 0;
	    obj->z_gblue_rgb = 0;
	    break;
	case zLBlue:
	    obj->z_gred_rgb = 0;
	    obj->z_ggreen_rgb = 0;
	    obj->z_gblue_rgb = 255;
	    break;
	case zLYellow:
	    obj->z_gred_rgb = 255;
	    obj->z_ggreen_rgb = 205;
	    obj->z_gblue_rgb = 0;
	    break;
	case zLCyan:
	    obj->z_gred_rgb = 0;
	    obj->z_ggreen_rgb = 255;
	    obj->z_gblue_rgb = 255;
	    break;
	default:
	    obj->z_gred_rgb = 0;
	    obj->z_ggreen_rgb = 0;
	    obj->z_gblue_rgb = 0;
	    break;
	}

    return 0;
}

/* get line colour */
inline zLineColour zGeneric_Get_LineColour(zGeneric* obj)
{
    /* check for NULL pointer */
    if(obj == NULL)
	return zLBlack;
    else
	return obj->z_gline_color_ix;

}

/* set line weight */
inline int zGeneric_Set_LineWeight(zGeneric* obj,
				   zLineWeights var)
{
    /* check NULL pointer */
    Z_CHECK_OBJ(obj);

    obj->z_gline_weight = var;

    /* set line weight to context */
    zgeneric_set_lineweight(obj);
    return 0;

}

/* get line weights */
inline zLineWeights zGeneric_Get_LineWeight(zGeneric* obj)
{
    /* check NULL pointer */
    if(obj == NULL)
	return zLWeight1;
    else
	return obj->z_gline_weight;
}

/* set device context to default */
inline int zGeneric_Set_Default_Dev_Context(zGeneric* obj)
{
    /* check for NULL pointer */
    Z_CHECK_OBJ(obj);

    /* check for device context */
    Z_CHECK_OBJ(obj->z_gdev);

    /* check if device context was set in
       device object */
    if(obj->z_gdev->z_device)
	obj->z_gcairo_dev = obj->z_gdev->z_device;
    
    return 0;
}

/* create generic device context */
inline int zGeneric_Create_Dev_Context(zGeneric* obj)
{
    /* check for NULL pointer */
    Z_CHECK_OBJ(obj);

    /* check if device was assigned */
    Z_CHECK_OBJ(obj->z_gdev);

    /* check if surface was created */
    Z_CHECK_OBJ(obj->z_gdev->z_surface);
	
    obj->z_gcairo_dev = cairo_create(obj->z_gdev->z_surface);
    return 0;
}

/* get pointer to device context */
inline cairo_t* zGeneric_Get_Dev_Context(zGeneric* obj)
{
    /* check for NULL pointer */
    Z_CHECK_OBJ_PTR(obj);

    return obj->z_gcairo_dev;
}

inline int zGeneric_Set_Defauts(zGeneric* obj)
{
    /* check for NULL pointer */
    Z_CHECK_OBJ(obj);

    /* check if device context was
       created */
    Z_CHECK_OBJ(obj->z_gcairo_dev);

    /* set default joining */
    cairo_set_line_join (obj->z_gcairo_dev,
			 CAIRO_LINE_JOIN_MITER);

    /* set default line weight */
    zgeneric_set_lineweight(obj);
    
    return 0;
}

/* set line type */
inline int zGeneric_Set_LintType(zGeneric* obj, zLineTypes var)
{
    /* check for NULL pointer */
    Z_CHECK_OBJ(obj);
    obj->z_gltype = var;
    return 0;
}

/* get line type */
inline zLineTypes zGeneric_Get_LineType(zGeneric* obj)
{
    /* check for NULL pointer */
    if(obj == NULL)
	return zLTContinuous;
    else
	return obj->z_gltype;

}


/* Private methods */
/***********************************************************************/

/* Set generic line weight helper function */
inline int zgeneric_set_lineweight(zGeneric* obj)
{
    /* check for NULL pointer */
    Z_CHECK_OBJ(obj);

    /* check for cairo device
       context */
    Z_CHECK_OBJ(obj->z_gcairo_dev);

    switch(obj->z_gline_weight)
	{
	case zLWeight1:
	    cairo_set_line_width(obj->z_gcairo_dev,
				 Z_LINE_WEIGHT1);
	    break;
	case zLWeight2:
	    cairo_set_line_width(obj->z_gcairo_dev,
				 Z_LINE_WEIGHT2);
	    break;
	case zLWeight3:
	    cairo_set_line_width(obj->z_gcairo_dev,
				 Z_LINE_WEIGHT3);
	    break;
	case zLWeight5:
	    cairo_set_line_width(obj->z_gcairo_dev,
				 Z_LINE_WEIGHT5);
	    break;
	case zLWeight6:
	    cairo_set_line_width(obj->z_gcairo_dev,
				 Z_LINE_WEIGHT6);
	    break;
	case zLWeight7:
	    cairo_set_line_width(obj->z_gcairo_dev,
				 Z_LINE_WEIGHT7);
	    break;
	case zLWeight8:
	    cairo_set_line_width(obj->z_gcairo_dev,
				 Z_LINE_WEIGHT8);
	default:
	    cairo_set_line_width(obj->z_gcairo_dev,
				 Z_LINE_WEIGHT4);
	    break;
	}
	    
    return 0;
}
