/* implementation of device object
   Sat Oct 16 13:36:50 BST 2010 */

#include "zDevice.h"

/* private functions */
static inline int zdevice_rgb(zDevice** obj);
static inline int zdevice_page_dims(zDevice** obj);
static inline int zdevice_create_context(zDevice* obj)

/* constructor */
    zDevice* zDevice_New(zDevice* obj)
{
    /* declare temporary object */
    if(obj == NULL)
	{
	    obj = (zDevice*) malloc(sizeof(zDevice));
	    Z_CHECK_OBJ_PTR(obj);
	    obj->var_int_flg = 1;
	}
    else
	obj->var_int_flg = 0;

    /* set properties */
    obj->z_surface = NULL;
    obj->z_device = NULL;

    obj->z_line_color_ix = zLBlack;

    /* call to set colours */
    zdevice_rgb(obj);

    /* set page size default to A3 landscape */
    obj->z_page_width = Z_A3_WIDTH;
    obj->z_page_height = Z_A3_HEIGHT;
    obj->z_format = zFormatPDF;
    obj->z_filename[0] = NULL;
    obj->z_page_sz_str[0] = NULL;

    return obj;
}

/* Constructor 2 */
zDevice* zDevice_New2(zOutputFormat fm,
		      zSheets sh,
		      const char* fn,
		      int dev_flg,
		      zDevice* obj)
{

    Z_CHECK_OBJ_PTR(zDevice_Create(obj));

    /* check for file name */
    Z_CHECK_OBJ_PTR(fn);

    /* copy file name to local */
    strcpy(obj->z_filename, fn);

    /* set page size and dims */
    obj->z_page_sz = sh;
    zdevice_page_dims(obj);

    /* create sufrace */
    obj->z_format = zFormatPDF;
    switch(fm)
	{
	case zFormatPDF:
	    obj->z_surface =
		cairo_pdf_surface_create(obj->z_filename,
					 ConvToPoints(&obj->z_page_width),
					 ConvToPoints(&obj->z_page_height));
	    break;
	case zFormatSVG:
	    obj->z_surface =
		cairo_svg_surface_create(obj->z_filename,
					 ConvToPoints(&obj->z_page_width),
					 ConvToPoints(&obj->z_page_height));
	    break;
	default:
	    obj->z_surface =
		cairo_pdf_surface_create(obj->z_filename,
					 ConvToPoints(&obj->z_page_width),
					 ConvToPoints(&obj->z_page_height));
	    break;
	}
				
    /* set properties */
    /* check for device flag */
    if(dev_flg > 0)
	obj->z_device = cairo_create(obj->z_surface);
    else
	obj->z_device = NULL;

    obj->z_line_color_ix = zLBlack;
    zdevice_rgb(obj);
    zdevice_create_context(obj);
    return obj;
}

/* delete device */
void zDevice_Delete(zDevice* obj)
{

    /* check for NULL pointer */
    Z_CHECK_OBJ_VOID(obj);

    /* destroy surface */
    if((*obj)->z_surface != NULL)
	{
	    cairo_surface_destroy((*obj)->z_surface);
	    (*obj)->z_surface = NULL;
	}
	
    /* destroy device context */
    if((*obj)->z_device != NULL)
	{
	    cairo_destroy((*obj)->z_device);
	    (*obj)->z_device = NULL;
	}

    /* destroy file name */
    if((*obj)->z_filename != NULL)
	(*obj)->z_filename[0] = NULL;


    /* destroy page size string */
    if((*obj)->z_page_sz_str != NULL)
	(*obj)->z_page_sz_str[0] = NULL;


    if(obj->var_int_flg)
	free(*obj);
	
}

/* set surface type */
inline int zDevice_Set_SurfaceType(zDevice* obj,
				   zOutputFormat var);
{
    /* check for NULL pointer */
    Z_CHECK_OBJ(obj);

    /* check if surface object was created, if
       created function will return */
    Z_CHECK_OBJ(obj->z_surface);

    obj->z_format = var;
    switch(var)
	{
	case zFormatPDF:
	    obj->z_surface =
		cairo_pdf_surface_create(obj->z_filename,
					 ConvToPoints(&obj->z_page_width),
					 ConvToPoints(&obj->z_page_height));
	    break;
	case zFormatSVG:
	    obj->z_surface =
		cairo_svg_surface_create(obj->z_filename,
					 ConvToPoints(&obj->z_page_width),
					 ConvToPoints(&obj->z_page_height));
	    break;
	default:
	    obj->z_surface =
		cairo_pdf_surface_create(obj->z_filename,
					 ConvToPoints(&obj->z_page_width),
					 ConvToPoints(&obj->z_page_height));
	    break;
	}

    return 0;
}

/* return the page size */
inline zOutputFormat zDevice_Get_SurfaceType(zDevice* obj)
{
    if(obj == NULL)
	return zFormatPDF;
    else
	return obj->z_format;
}

/* set page size */
inline int zDevice_Set_PageSize(zDevice* obj,
				zSheets var)
{
    /* check for NULL pointer */
    Z_CHECK_OBJ(obj);

    obj->z_page_sz = var;
    zdevice_page_dims(obj);
    return 0;
}

/* get page size */
inline zSheets zDevice_Get_PageSize(zDevice* obj)
{
    if(obj == NULL)
	return zSheetA3_Landscape;
    else
	return obj->z_page_sz;
}

/* returns a pointer to page size string */
inline char* zDevice_Get_PageSizeStr(zDevice* obj)
{
    /* check for NULL pointer */
    Z_CHECK_OBJ_PTR(obj);
    return obj->z_page_sz_str;
}

/* set file name */
inline int zDevice_Set_FileName(zDevice* obj,
				const char* var)
{
    /* check for object NULL pointer */
    Z_CHECK_OBJ(obj);
    Z_CHECK_OBJ(var);

    int i = strlen(obj->z_filename) + 1;

    /* check if file name was assigned */
    if(obj->z_filename)
	free(obj->z_filename);

    strcpy(obj->z_filename, var);

    return 0;
}

/* return file name pointer */
inline char* zDevice_Get_FileName(zDevice* obj)
{
    Z_CHECK_OBJ_PTR(obj);
    return obj->z_filename;
}
			 

/* get device context */
inline cairo_t* zDevice_Get_Context(zDevice* obj)
{
    /* check for NULL pointer */
    Z_CHECK_OBJ_PTR(obj);

    zdevice_create_context(obj);
    return obj->z_device;
}

/* set line colour index */
inline int zDevice_Set_LineColourIx(zDevice* obj,
				    zLineColour var)
{
    /* check for NULL pointer */
    Z_CHECK_OBJ(obj);

    obj->z_line_color_ix = var;
    zdevice_rgb(obj);
    return 0;
}

/* get line colour index */
inline zLineColour zDevice_Get_LineColourIx(zDevice* obj)
{
    /* check for NULL pointer */
    if(obj == NULL)
	return zLBlack;
    else
	return obj->z_line_color_ix;
}
		
/******************************************************************/
/* private functions */
static inline int zdevice_rgb(zDevice* obj)
{
    switch(obj->z_line_color_ix)
	{
	case zLRed:
	    obj->z_red_rgb = 255;
	    obj->z_green_rgb = 0;
	    obj->z_blue_rgb = 0;
	    break;
	case zLBlue:
	    obj->z_red_rgb = 0;
	    obj->z_green_rgb = 0;
	    obj->z_blue_rgb = 255;
	    break;
	case zLYellow:
	    obj->z_red_rgb = 255;
	    obj->z_green_rgb = 205;
	    obj->z_blue_rgb = 0;
	    break;
	case zLCyan:
	    obj->z_red_rgb = 0;
	    obj->z_green_rgb = 255;
	    obj->z_blue_rgb = 255;
	    break;
	default:
	    obj->z_red_rgb = 0;
	    obj->z_green_rgb = 0;
	    obj->z_blue_rgb = 0;
	    break;
	}	
}

/* set page size */
static inline int zdevice_page_dims(zDevice* obj)
{
    /* check for NULL pointer */
    Z_CHECK_OBJ(obj);

    switch(obj->z_page_sz)
	{
	case zSheetA3_Landscape:
	    obj->z_page_width = Z_A3_WIDTH;
	    obj->z_page_height = Z_A3_HEIGHT;
			
	    strcpy(obj->z_page_sz_str, Z_A3_PAGE_NAME);
			
	    break;
	case zSheetA4_Portrait:
	    obj->z_page_width = Z_A4_WIDTH;
	    obj->z_page_height = Z_A4_HEIGHT;

	    strcpy(obj->z_page_sz_str, Z_A4_PAGE_NAME);
			
	    break;
	}
	
    return 0;
}

/* create device context */
static inline int zdevice_create_context(zDevice* obj)
{
    /* check for NULL pointer */
    Z_CHECK_OBJ(obj);
    
    /* check if surface object was created */
    Z_CHECK_OBJ(obj->z_surface);
    
    /* check if device context was created */
    if(!obj->z_device)
	obj->z_device = cairo_create(obj->z_surface);
    
    return 0;
}
