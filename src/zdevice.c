/* implementation of device object
   Sat Oct 16 13:36:50 BST 2010
*/

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <error.h>
#include <string.h>
#include "zdevice.h"

#define ZDEVICE_TEMP_FILE_NAME "fileXXXXXX"

/* private functions */
static int zdevice_rgb(zdevice* obj);
static int zdevice_page_dims(zdevice* obj);
static int zdevice_create_context(zdevice* obj);
static int zdevice_open_temp_file(zdevice* obj);
static cairo_status_t zdevice_write_callback(void* obj, unsigned char* data, unsigned int sz);

#define ZDEVICE_TEMP_FILE_ERROR						\
	_err_no = errno;							\
	ZELIA_LOG_MESSAGE("zdevice unable to get stats on temporary file"); \
	ZELIA_LOG_MESSAGE("zdevice, temporary buffer not available");	\
	ZELIA_LOG_MESSAGE_WITH_STR("zdevice reported, ", strerror(_err_no))

/* constructor */
zdevice* zdevice_new(zdevice* obj)
{
	ZELIA_LOG_MESSAGE("zdevice creating device context");
	/* call constructor helper to pre initialise the struct */
	ZCONSTRUCTOR(obj, zdevice);

	/* reference counter */
	obj->ref_cnt = 0;

	/* set properties */
	obj->surface = NULL;
	obj->device = NULL;

	memset(obj->_fname, 0, ZDEVICE_TEMP_FILE_SZ);

	obj->line_color_ix = zLBlack;
	obj->line_type_ix = zLTContinuous;
	obj->_fd = ZELIA_EMPTY_FILE;
	obj->_buff_sz = 0;
	obj->_buff = NULL;

	/* call to set colours */
	zdevice_rgb(obj);

	/* set page size default to A3 landscape */
	obj->page_width = Z_A3_WIDTH;
	obj->page_height = Z_A3_HEIGHT;

	/* open a temporary file for writing */
	ZELIA_LOG_MESSAGE("zdevice opening a temporary file for writing");
	if(zdevice_open_temp_file(obj) != ZELIA_OK)
	ZELIA_LOG_MESSAGE("zdevice failed to open a temporary file");

	return obj;
}

/* Constructor 2 */
zdevice* zdevice_new2(zSheets sh,
			  int dev_flg,
			  zdevice* obj)
{

	ZCHECK_OBJ_PTR(zdevice_new(obj));

	/* set page size and dims */
	ZELIA_LOG_MESSAGE("zdevice setting page size");
	obj->page_sz = sh;
	zdevice_page_dims(obj);

	/* if the device flag was set we create one */
	if(dev_flg > 0 && obj->_fd != ZELIA_EMPTY_FILE)
	obj->device = cairo_create(obj->surface);
	else
	obj->device = NULL;

	/* this method is called to create one if not created by above */
	zdevice_create_context(obj);

	/* set the line type */
	zdevice_set_linettype(obj, zLTContinuous);
	return obj;
}

/* delete device */
void zdevice_delete(zdevice* obj)
{

	/* check for NULL pointer */
	ZCHECK_OBJ_VOID(obj);

	/*
	 * Check reference count,
	 * if the device context is being shared between
	 * other objects, we just decrement it.
	 * When objects are no longer referenced to this
	 * we remove it.
	 */
	if(obj->ref_cnt-- > 0)
		return;
	
	/* destroy surface */
	if(obj->surface != NULL)
	{
		cairo_surface_destroy(obj->surface);
		obj->surface = NULL;
	}

	/* destroy device context */
	if(obj->device != NULL)
	{
		cairo_destroy(obj->device);
		obj->device = NULL;
	}

	/* delete the temporary buffer */
	if(obj->_buff != NULL)
	free(obj->_buff);

	obj->_buff = NULL;
	/* close temporary file if was created */
	if(obj->_fd != ZELIA_EMPTY_FILE)
	close(obj->_fd);

	/* delete object if it was created */
	if(obj->_int_flg == ZELIA_CONSTRUCTED)
	free(obj);

	return;
}

/* Set page size */
int zdevice_set_pagesize(zdevice* obj, zSheets var)
{
	/* check for NULL pointer */
	ZCHECK_OBJ_INT(obj);

	obj->page_sz = var;
	zdevice_page_dims(obj);
	return ZELIA_OK;
}

/* Get default device context */
cairo_t* zdevice_get_context(zdevice* obj)
{
	/* check for NULL pointer */
	ZCHECK_OBJ_PTR(obj);

	zdevice_create_context(obj);

	/* increment reference counter */
	obj->ref_cnt++;
	return obj->device;
}

int zdevice_set_linecolourix(zdevice* obj, zLineColour var)
{
	/* check for NULL pointer */
	ZCHECK_OBJ_INT(obj);

	obj->line_color_ix = var;
	zdevice_rgb(obj);
	return ZELIA_OK;
}

/* get temporary buffer */
const char* zdevice_get_temp_buff(zdevice* obj)
{
	int _err_no = 0;					/* error no */
	struct stat _fstat;					/* file stats */
	off_t _c_offset;					/* offset */
	ssisze_t _r_sz = 0;					/* read buffer size */

	ZCHECK_OBJ_PTR(obj);

	ZELIA_LOG_MESSAGE("zdevice reading temp file to get content");
	/* check if the file descriptor was opened */
	if(obj->_fd == ZELIA_EMPTY_FILE)
	return NULL;

	if(fstat(_obj->_fd, &_fstat))
	{
		/* temporary file error */
		ZDEVICE_TEMP_FILE_ERROR
		return NULL;
	}

	/* get size */
	obj->_buff_sz = _fstat.st_size+1;

	/* create buffer */
	ZELIA_LOG_MESSAGE("zdevice creating temporary buffer");
	if(obj->_buff)
	free(obj->_buff);

	obj->_buff = (char*) malloc(obj->_buff_sz * sizeof(char));
	memset((void*) obj->_buff, 0, obj->_buff_sz);

	ZELIA_LOG_MESSAGE("zdevice reading file");

	/* get current file offset */
	_c_offset = lseek(obj->_fd, 0, SEEK_CUR);

	/* rewind to get every thing */
	ZELIA_LOG_MESSAGE("zdevice rewinding temp file");
	lseek(obj->_fd, 0, SEEK_SET);

	_r_sz = read(obj->_fd, (void*) obj->_buff, _fstat.st_size);

	if(_r_sz > 0)
	ZELIA_LOG_MESSAGE("zdevice buffer read successfully");
	else
	{
		ZDEVICE_TEMP_FILE_ERROR;
		lseek(obj->_fd, _r_sz+1, SEEK_SET);
		return NULL;
	}

	/* rewind the file descriptor */
	ZELIA_LOG_MESSAGE("zdevice temp file seek set to position before read");
	lseek(obj->_fd, _r_sz+1, SEEK_SET);

	return obj->_buff;
}

/* set line type */
int zdevice_set_linettype(zdevice* obj, zLineTypes var)
{
	double _array[4] = {0, 0, 0, 0};

	/* check for NULL pointer */
	ZCHECK_OBJ(obj);
	obj->line_type_ix = var;
	ZCHECK_OBJ(obj->device);

	switch(obj->line_type_ix)
		{
		case zLTHidden:
			_array[0] = Z_LT_HIDDEN_ON;
			_array[1] = Z_LT_HIDDEN_OFF;
			cairo_set_dash(obj->device,
						   _array,
						   2,
						   0.0);
			break;
		case zLTCenter:
			_array[0] = Z_LT_CENTRE_ON;
			_array[1] = Z_LT_CENTRE_OFF;
			_array[2] = Z_LT_CENTRE_LONG;
			_array[3] = Z_LT_CENTRE_OFF;
			cairo_set_dash(obj->device,
						   _array,
						   4,
						   0.0);
			break;
		case zLTContinuous:
		default:
			_array[0] = 0.0;
			cairo_set_dash(obj->device,
						   _array,
						   0,
						   0.0);
			break;
		}

	return ZELIA_OK;
}
/*=================================== Private Methods ===================================*/
static int zdevice_rgb(zdevice* obj)
{
	ZELIA_LOG_MESSAGE("zdevice setting rgb values");
	switch(obj->line_color_ix)
	{
	case zLRed:
		obj->red_rgb = 255;
		obj->green_rgb = 0;
		obj->blue_rgb = 0;
		break;
	case zLBlue:
		obj->red_rgb = 0;
		obj->green_rgb = 0;
		obj->blue_rgb = 255;
		break;
	case zLYellow:
		obj->red_rgb = 255;
		obj->green_rgb = 205;
		obj->blue_rgb = 0;
		break;
	case zLCyan:
		obj->red_rgb = 0;
		obj->green_rgb = 255;
		obj->blue_rgb = 255;
		break;
	default:
		obj->red_rgb = 0;
		obj->green_rgb = 0;
		obj->blue_rgb = 0;
		break;
	}

	return ZELIA_OK;
}

/* set page size */
static int zdevice_page_dims(zdevice* obj)
{
	ZELIA_LOG_MESSAGE("zdevice setting page size based on label");
	switch(obj->page_sz)
	{
	case zSheetA3_Landscape:
		obj->page_width = Z_A3_WIDTH;
		obj->page_height = Z_A3_HEIGHT;
		break;
	case zSheetA4_Portrait:
		obj->page_width = Z_A4_WIDTH;
		obj->page_height = Z_A4_HEIGHT;
		break;
	}

	/* if surface is not created we create it here */
	if(obj->device == NULL)
	{
		ZELIA_LOG_MESSAGE("zdevice creating cairo surface from stream");
		obj->surface = cairo_svg_surface_create_for_stream(zdevice_write_callback,
								  (void*) obj,
								  ZCONV_TO_POINTS(obj->page_width),
								  ZCONV_TO_POINTS(obj->page_height));
	}

	/* check for errors */
	ZELIA_LOG_MESSAGE("zdevice checking if surface was properly created");
	if(cairo_surface_status(obj->surface) != CAIRO_STATUS_SUCCESS)
	{
		ZELIA_LOG_MESSAGE("zdevice was not created properly, setting the surface pointer to NULL");
		obj->surface = NULL;
	}
	return ZELIA_OK;
}

/* create device context */
static int zdevice_create_context(zdevice* obj)
{
	/* check if device context was created */
	if(!obj->device && obj->surface)
	obj->device = cairo_create(obj->surface);

	return ZELIA_OK;
}

/*
 * Open a temporary file in typically temporary folder,
 * the folder depends on the label defind in zvar.h header.
 */
static int zdevice_open_temp_file(zdevice* obj)
{
	char* _fname_ptr = NULL;					/* pointer to copied destination */

	size_t _dir_sz = strlen(Z_TEMP_FILE_DIR);			/* temporary directory length */
	size_t _fname_sz = strlen(ZDEVICE_TEMP_FILE_NAME);		/* temporary file name length */

	/* initialise the file name buffer */
	memset((void*) obj->_fname, 0, ZDEVICE_TEMP_FILE_SZ);

	/*
	 * we set the destination pointer to initial,
	 * once the first string is copied, we increment it.
	 */
	_fname_ptr = obj->_fname;

	/* create a file name including the path */
	ZELIA_LOG_MESSAGE("zdevice creating file name");
	strncat(obj->_fname, Z_TEMP_FILE_DIR, _dir_sz);

	/* increment pointer */
	_fname_ptr += _dir_sz;
	strncat(_fname_ptr, ZDEVICE_TEMP_FILE_NAME, _fname_sz);

	/* create a temporay file for read write */
	obj->_fd = mkostemp(obj->_fname, O_CREAT | O_APPEND | O_RDWR | O_SYNC);

	return ZELIA_OK;
}

/* callback method for writing to file */
static cairo_status_t zdevice_write_callback(void* obj, unsigned char* data, unsigned int sz)
{
	zdevice* _obj = NULL;
	struct stat _fstat;
	int _err_no = 0;
	ssize_t _w_sz = 0;

	/* check arguments */
	if(obj == NULL || data == NULL || sz <= 0)
	return CAIRO_STATUS_WRITE_ERROR;

	/* cast object to the correct type */
	_obj = (zdevice*) obj;

	/* check if the file was opened */
	if(fstat(_obj->_fd, &_fstat))
	{
		/* macro for handling error */
		ZDEVICE_TEMP_FILE_ERROR;
		return CAIRO_STATUS_WRITE_ERROR;
	}

	ZELIA_LOG_MESSAGE("zdevice writing to temporary buffer");
	_w_sz = write(_obj->_fd, (void*) data, (size_t) sz);

	if(_w_sz > 0)
	ZELIA_LOG_MESSAGE_WITH_INT("zdevice succesfully written", (int) _w_sz);

	/* return to indicate there were no errors */
	return CAIRO_STATUS_SUCCESS;
}
