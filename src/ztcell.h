/* Header file for declaring interfaces for Cell object of
 * table. Inherites the base class and this class shall
 * derive column and row collections.
 * Sat Mar  3 13:00:22 GMT 2012 */

#ifndef _ZTCELL_H_
#define _ZTCELL_H_

#include <stdlib.h>
#include "zgeneric.h"
#include "zbase.h"

/* Content buffer size */
#define Z_TCELL_BUFF 256

/* Forward declaration of struct */
typedef struct _ztcell ztcell;

struct _ztcell
{
    zbase parent;					/* Inherited parent object */
    unsigned int _init_flg;				/* Internal flag */
    size_t content_sz;					/* content size */
    char* content;					/* Content buffer */
    unsigned int ref_flg;				/* reference flag */
    unsigned int _row_ix;				/* Internal row index */
    unsigned int _col_ix;				/* Internal column index */
    unsigned int line_flg;				/* Line flag */

    zgeneric* super_cls;
    void* child;					/* Child pointer */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zgeneric* ztcell_new(ztcell* obj);
    void ztcell_delete(ztcell* obj);

    /* Draw function */
    int ztcell_draw(ztcell* obj);

    /* Properties methods */

    /* Set and get cell content */
    inline __attribute__ ((always_inline)) static int ztcell_set_content(ztcell* obj, const char* content)
    {
	ZCHECK_OBJ_INT(obj);
	ZCHECK_OBJ_INT(content);
	
	obj->content_sz = strlen(content);
	
	if(obj->content)
	    free(obj->content);

	/*
	 * if reference flag is set we just set the
	 * reference to the content to be deleted in destructor
	*/
	if(obj->ref_flg > 0)
	    obj->content = (char*) content;
	else
	    {
		obj->content = (char*) malloc(sizeof(char) * (obj->content_sz + 1));
		strncpy(obj->content, content, obj->content_sz);
		obj->content[obj->content_sz] = '\0';
	    }

	return ZELIA_OK;
    }
    inline __attribute__ ((always_inline)) static const char* ztcell_get_content(ztcell* obj)
    {
	ZCHECK_OBJ_PTR(obj);
	return obj->content;
    }

    /* Get row and column index */
    inline __attribute__ ((always_inline)) static unsigned int ztcell_get_row_ix(ztcell* obj)
    {
	ZCHECK_OBJ_INT(obj);
	return obj->_row_ix;
    }
    inline __attribute__ ((always_inline)) static unsigned int ztcell_get_col_ix(ztcell* obj)
    {
	ZCHECK_OBJ_INT(obj);
	return obj->_col_ix;
    }

    /* Set and get line flag */
    inline __attribute__ ((always_inline)) static int ztcell_set_line_flg(ztcell* obj, unsigned int flg)
    {
	ZCHECK_OBJ_INT(obj);
	obj->line_flg = flg;
	return ZELIA_OK;
    }
    inline __attribute__ ((always_inline)) static unsigned int ztcell_get_line_flg(ztcell* obj)
    {
	if(obj == NULL)
	    return 0;
	else
	    return obj->line_flg;
    }

#ifdef __cplusplus
}
#endif

#endif /* _ZTCELL_H_ */
