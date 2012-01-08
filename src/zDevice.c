/* implementation of device object
   Sat Oct 16 13:36:50 BST 2010 */

#include "zDevice.h"

/* constructor */
zDevice* zDevice_Create()
{
	/* declare temporary object */
	zDevice* tmp;

	tmp = (zDevice*) malloc(sizeof(zDevice));

	/* check for NULL pointer */
	if(tmp == NULL)
		return NULL;

	/* set properties */
	tmp->z_surface = NULL;
	tmp->z_device = NULL;

	tmp->z_line_color_ix = zLBlack;

	/* call to set colours */
	zdevice_rgb(&tmp);

	/* set page size default to A3 landscape */
	tmp->z_page_width = Z_A3_WIDTH;
	tmp->z_page_height = Z_A3_HEIGHT;
	tmp->z_format = zFormatPDF;
	tmp->z_page_sz_str = NULL;
	return tmp;
	
}

zDevice* zDevice_Create2(zOutputFormat fm,
						 zSheets sh, const char* fn,
						 int dev_flg)
{
	/* create temporary */
	zDevice* tmp;

	tmp = (zDevice*) malloc(sizeof(zDevice));

	/* check for NULL pointer */
	if(tmp == NULL)
		return NULL;

	/* check for file name */
	if(fn == NULL)
		{
			free(tmp);
			return NULL;
		}

	/* copy file name to local */
	int i = strlen(fn) + 1;
	tmp->z_filename = (char*) malloc(sizeof(char) * i);
	strcpy(tmp->z_filename, fn);

	/* set page size and dims */
	tmp->z_page_sz = sh;
	zdevice_page_dims(&tmp);

	/* create sufrace */
	tmp->z_format = zFormatPDF;
	switch(fm)
		{
		case zFormatPDF:
			tmp->z_surface = cairo_pdf_surface_create(
				tmp->z_filename,
				ConvToPoints(&tmp->z_page_width),
				ConvToPoints(&tmp->z_page_height));
			break;
		case zFormatSVG:
			tmp->z_surface = cairo_svg_surface_create(
				tmp->z_filename,
				ConvToPoints(&tmp->z_page_width),
				ConvToPoints(&tmp->z_page_height));
			break;
		default:
			tmp->z_surface = cairo_pdf_surface_create(
				tmp->z_filename,
				ConvToPoints(&tmp->z_page_width),
				ConvToPoints(&tmp->z_page_height));
			break;
		}
				
	/* set properties */
	/* check for device flag */
	if(dev_flg > 0)
		tmp->z_device = cairo_create(tmp->z_surface);
	else
		tmp->z_device = NULL;

	tmp->z_line_color_ix = zLBlack;
	zdevice_rgb(&tmp);

	return tmp;
}

/* delete device */
void zDevice_Delete(zDevice** obj)
{

	/* check for NULL pointer */
	if(obj == NULL ||
	   *obj == NULL)
		return;

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
		{
			free((*obj)->z_filename);
			(*obj)->z_filename = NULL;
		}

	/* destroy page size string */
	if((*obj)->z_page_sz_str != NULL)
		{
			free((*obj)->z_page_sz_str);
			(*obj)->z_page_sz_str = NULL;
		}

	free(*obj);
	*obj = NULL;
	
}

/* set surface type */
void zDevice_Set_SurfaceType(zDevice* obj,
							 zOutputFormat var)
{
	/* check for NULL pointer */
	if(obj == NULL)
		return;

	/* check if surface object was created, if
	   created function will return */
	if(obj->z_surface != NULL)
		return;

	obj->z_format = var;
	switch(var)
		{
		case zFormatPDF:
			obj->z_surface = cairo_pdf_surface_create(
				obj->z_filename,
				ConvToPoints(&obj->z_page_width),
				ConvToPoints(&obj->z_page_height));
			break;
		case zFormatSVG:
			obj->z_surface = cairo_svg_surface_create(
				obj->z_filename,
				ConvToPoints(&obj->z_page_width),
				ConvToPoints(&obj->z_page_height));
			break;
		default:
			obj->z_surface = cairo_pdf_surface_create(
				obj->z_filename,
				ConvToPoints(&obj->z_page_width),
				ConvToPoints(&obj->z_page_height));
			break;
		}	
}

/* return the page size */
zOutputFormat zDevice_Get_SurfaceType(zDevice* obj)
{
	if(obj == NULL)
		return zFormatPDF;
	else
		return obj->z_format;
}

/* set page size */
void zDevice_Set_PageSize(zDevice* obj,
						  zSheets var)
{
	/* check for NULL pointer */
	if(obj == NULL)
		return;

	obj->z_page_sz = var;
	zdevice_page_dims(&obj);
}

/* get page size */
zSheets zDevice_Get_PageSize(zDevice* obj)
{
	if(obj == NULL)
		return zSheetA3_Landscape;
	else
		return obj->z_page_sz;
}

	/* returns a pointer to page size string */
char* zDevice_Get_PageSizeStr(zDevice* obj)
{
	/* check for NULL pointer */
	if(obj == NULL)
		return NULL;
	else
		return obj->z_page_sz_str;
}

/* set file name */
void zDevice_Set_FileName(zDevice* obj,
						  const char* var)
{
	/* check for object NULL pointer */
	if(obj == NULL)
		return;

	/* check for file name pointer */
	if(var == NULL)
		return;

	int i = strlen(obj->z_filename) + 1;

	/* check if file name was assigned */
	if(obj->z_filename)
		free(obj->z_filename);

	obj->z_filename = (char*) malloc(sizeof(char) * i);
	strcpy(obj->z_filename, var);

	return;
}

/* return file name pointer */
char* zDevice_Get_FileName(zDevice* obj)
{
	if(obj == NULL)
		return NULL;
	else
		return obj->z_filename;
}
			 
/* create device context */
void zDevice_Create_Context(zDevice* obj)
{
	/* check for NULL pointer */
	if(obj == NULL)
		return;

	/* check if device context was created */
	if(obj->z_device != NULL)
		return;

	/* check if surface object was created */
	if(obj->z_surface == NULL)
		return;
	
	obj->z_device = cairo_create(obj->z_surface);
}

/* get device context */
cairo_t** zDevice_Get_Context(zDevice* obj)
{
	/* check for NULL pointer */
	if(obj == NULL)
		return NULL;

	/* check for device context */
	if(obj->z_device)
		return &obj->z_device;
	else
		return NULL;
}

/* set line colour index */
void zDevice_Set_LineColourIx(zDevice* obj,
							  zLineColour var)
{
	/* check for NULL pointer */
	if(obj == NULL)
		return;

	obj->z_line_color_ix = var;
	zdevice_rgb(&obj);
}

/* get line colour index */
zLineColour zDevice_Get_LineColourIx(zDevice* obj)
{
	/* check for NULL pointer */
	if(obj == NULL)
		return zLBlack;
	else
		return obj->z_line_color_ix;
}
		

/* private functions */
void zdevice_rgb(zDevice** obj)
{
	/* check for NULL poiner */
	if(obj == NULL ||
	   *obj == NULL)
		return;

	switch((*obj)->z_line_color_ix)
		{
		case zLRed:
		{
			(*obj)->z_red_rgb = 255;
			(*obj)->z_green_rgb = 0;
			(*obj)->z_blue_rgb = 0;
			break;
		}
		case zLBlue:
		{
			(*obj)->z_red_rgb = 0;
			(*obj)->z_green_rgb = 0;
			(*obj)->z_blue_rgb = 255;
			break;
		}
		case zLYellow:
		{
			(*obj)->z_red_rgb = 255;
			(*obj)->z_green_rgb = 205;
			(*obj)->z_blue_rgb = 0;
			break;
		}
		case zLCyan:
		{
			(*obj)->z_red_rgb = 0;
			(*obj)->z_green_rgb = 255;
			(*obj)->z_blue_rgb = 255;
			break;
		}
		default:
		{
			(*obj)->z_red_rgb = 0;
			(*obj)->z_green_rgb = 0;
			(*obj)->z_blue_rgb = 0;
			break;
		}
		
		}	
}

/* set page size */
void zdevice_page_dims(zDevice** obj)
{
	/* check for NULL pointer */
	if(obj == NULL ||
	   *obj == NULL)
		return;

	int i = 0;
	switch((*obj)->z_page_sz)
		{
		case zSheetA3_Landscape:
			(*obj)->z_page_width = Z_A3_WIDTH;
			(*obj)->z_page_height = Z_A3_HEIGHT;
			
			i = strlen(Z_A3_PAGE_NAME) + 1;
			(*obj)->z_page_sz_str = (char*) malloc(sizeof(char) * i);
			strcpy((*obj)->z_page_sz_str, Z_A3_PAGE_NAME);
			
			break;
		case zSheetA4_Portrait:
			(*obj)->z_page_width = Z_A4_WIDTH;
			(*obj)->z_page_height = Z_A4_HEIGHT;

			i = strlen(Z_A4_PAGE_NAME) + 1;
			(*obj)->z_page_sz_str = (char*) malloc(sizeof(char) * i);
			strcpy((*obj)->z_page_sz_str, Z_A4_PAGE_NAME);
			
			break;
		}
	
	return;
}
