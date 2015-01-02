/* zNotes class is a collection of zNote.
 * Uses a dynamically expanding array of zGenerics class.
 * Sat Mar 10 15:23:54 GMT 2012 */

#ifndef _ZNOTES_H_
#define _ZNOTES_H_

#include <stdlib.h>
#include <string.h>
#include "zdevice.h"
#include "zgenerics.h"
#include "znote.h"

#define Z_NOTES_TITLE_BUFF 256

/* Forward declaration of struct */
typedef struct _znotes znotes;

struct _znotes
{
    zgenerics parent;				/* Parent object */
    unsigned int _init_flg;			/* Internal flag */
    double _height;				/* Height */
    double width;				/* Width */
    double x;					/* x coord */
    double y;					/* y coord */
    double _note_height;			/* Note height */
    char* title;					/* Title buffer */
    unsigned int uline_flg;			/* Underline flag */
    unsigned int _znotes_counter;
    unsigned int counter;			/* Counter */

    struct _zgeneric_vtable vtable;		/* vtable */
    zgenerics* super_cls;			/* super class pointer */
    void* child;				/* Child pointer */
};

#ifdef __cplusplus
extern "C" {
#endif
    
    /* Constructor & Destructor */
    zgenerics* znotes_new(znotes* obj,		/* optional argument */
			  zdevice* dev,
			  double x,
			  double y,
			  double width);
    void znotes_delete(znotes* obj);

    /* Add note */
    int znotes_add(znotes* obj, const char* note);

    /***********************************************/    
    /* Property methods */
    
    /* Get Note */
    znote* znotes_get_note(znotes* obj, unsigned int ix);

    /* Get Height */
    inline __attribute__ ((always_inline)) static double znoets_get_height(znotes* obj)
    {
	ZCHECK_OBJ_DOUBLE(obj);
	return obj->_height;
    }

    /* Set and get title text */
    inline __attribute__ ((always_inline)) static int znotes_set_title(znotes* obj, const char* title)
    {
	size_t _sz = 0;
	ZCHECK_OBJ_INT(obj);

	if(obj->title)
	    free(obj->title);
	
	_sz = strlen(title);
	if(zgenerics_get_ref_flg(obj->super_cls))
	    obj->title = (char*) title;
	else
	    {
		obj->title = (char*) malloc(sizeof(char) * (_sz + 1));
	
		/* copy to internal buffer */
		strcpy(obj->title, title);
	    }
	return ZELIA_OK;
    }
    inline __attribute__ ((always_inline)) static const char* znotes_get_title(znotes* obj)
    {
	/* check object */
	ZCHECK_OBJ_PTR(obj);

	return obj->title[0] == '\0'? NULL : obj->title;
    }

    /* Set underline flag */
    inline __attribute__ ((always_inline)) static int znotes_set_uline_flg(znotes* obj, unsigned int flg)
    {
	ZCHECK_OBJ_INT(obj);
	obj->uline_flg = flg;
	return ZELIA_OK;
    }
    
#ifdef __cplusplus
}
#endif
#endif /* _ZNOTES_H_ */
