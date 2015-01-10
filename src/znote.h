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

struct _znote
{
    zbase parent;			/* Inherited parent object */
    unsigned int _init_flg;		/* Internal flag */
    unsigned int ref_flg;		/* reference flag */
    int ix;				/* Note index */
    char* note;				/* Note content buffer */
    char* fnote;			/* Note and index */
    double indent;			/* Indent */
    size_t note_sz;			/* Note length */
    size_t obj_sz;			/* Object size */

    znote_fptr height_func;		/* Inform function pointer */
    void* usr_data;			/* User data */
    void* cols;				/* collection object pointer */
    void* child;			/* Child pointer */
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
    int znote_set_content_with_ref(znote* obj, const char* content, int ix);
    inline __attribute__ ((always_inline)) static const char* znote_get_content(znote* obj)
    {
	/* check object */
	ZCHECK_OBJ_PTR(obj);

	return obj->note;
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
	return obj->indent;
    }

    /* enable and disable flags reference */
#define znote_enable_ref(obj)			\
    (obj)->ref_flg = 1
#define znote_disable_ref(obj)			\
    (obj)->ref_flg = 0

#ifdef __cplusplus
}
#endif

#endif /* _ZNOTE_H_ */
