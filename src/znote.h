/* zNote class handles an enumerated note. Provides basic interfaces for
 * adding and numbering notes. Uses pango layout engine. Inherits the
 * zBase class.
 * Sat Mar 10 12:59:49 GMT 2012 */

#ifndef _ZNOTE_H_
#define _ZNOTE_H_

#include <stdlib.h>
#include "zgeneric.h"
#include "zbase.h"

#define Z_NOTE_BUFF 256			/* default note size fixed */

/* Forward declaration of struct */
typedef struct _znote znote;

/* Function pointer for informing final height of
 * text height */
typedef int (*znote_fptr)(zgeneric* obj, void* usr_data, int height);

struct _zNote
{
    zbase z_parent;			/* Inherited parent object */
    unsigned int z_init_flg;		/* Internal flag */
    int z_ix;				/* Note index */
    char z_note[Z_NOTE_BUFF];		/* Note content buffer */
    char* z_fnote;			/* Note and index */
    double z_indent;			/* Indent */
    size_t z_note_sz;			/* Note length */
    size_t z_obj_sz;			/* Object size */

    struct _zgeneric_vtable vtable;	/* vtable */
    znote_fptr z_height_func;		/* Inform function pointer */
    void* z_usr_data;			/* User data */
    void* z_child;			/* Child pointer */
    zgeneric* super_cls;	       	/* super class pointer */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zgeneric* znote_new(znote* obj);
    void znote_delete(znote* obj);

    /* Draw method */
    int znote_draw(znote* obj);

    /*******************************************************************/
    /* Property methods */

    /* Set and get note content */
    int znote_set_content(znote* obj, const char* content, int ix);
    inline __attribute__ ((always_inline)) static const char* znote_get_content(znote* obj)
    {
	/* check object */
	ZCHECK_OBJ_PTR(obj);

	if(obj->note[0] != '\0')
	    return obj->note;
	else
	    return NULL;
    }
    inline __attribute__ ((always_inline)) static int znote_get_ix(znote* obj)
    {
	ZCHECK_OBJ_INT(obj);
	return obj->ix;
    }
    const char* znote_get_note_with_ix(znote* obj);

    /* Set and get indent */
    inline __attribute__ ((always_inline)) static int znote_set_indent(znote* obj, double indent)
    {
	ZCHECK_OBJ_INT(obj);
	obj->indent = indent;
	return 0;
    }
    inline __attribute__ ((always_inline)) static double znote_get_indent(znote* obj)
    {
	ZCHECK_OBJ_DOUBLE(obj);
	return obj->z_indent;
    }

#ifdef __cplusplus
}
#endif

#endif /* _ZNOTE_H_ */
