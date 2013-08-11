/* zNote class handles an enumerated note. Provides basic interfaces for
 * adding and numbering notes. Uses pango layout engine. Inherits the
 * zBase class.
 * Sat Mar 10 12:59:49 GMT 2012 */

#ifndef _ZNOTE_H_
#define _ZNOTE_H_

#include <stdlib.h>
#include "zGeneric.h"
#include "zBase.h"

#define Z_NOTE_BUFF 256			/* default note size fixed */
/* Forward declaration of struct */
typedef struct _zNote zNote;

/* Function pointer for informing final height of
 * text height */
typedef int (*znote_fptr)(zGeneric* obj, void* usr_data, int height);

struct _zNote
{
    zBase z_parent;			/* Inherited parent object */
    unsigned int z_int_flg;		/* Internal flag */
    int z_ix;				/* Note index */
    char z_note[Z_NOTE_BUFF];		/* Note content buffer */
    char* z_fnote;			/* Note and index */
    double z_indent;			/* Indent */
    size_t z_note_sz;			/* Note length */
    size_t z_obj_sz;			/* Object size */
    zgeneric_fptr z_draw_func;		/* Draw function pointer */
    znote_fptr z_height_func;		/* Inform function pointer */
    void* z_usr_data;			/* User data */
    void* z_child;			/* Child pointer */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zGeneric* zNote_New(zNote* obj);
    void zNote_Delete(zNote* obj);

    /* Draw method */
    int zNote_Draw(zNote* obj);

    /*******************************************************************/
    /* Property methods */

    /* Set and get note content */
    inline int zNote_Set_Content(zNote* obj, const char* content, int ix);
    inline const char* zNote_Get_Content(zNote* obj);
    inline int zNote_Get_Ix(zNote* obj);
    inline const char* zNote_Get_Note_With_Ix(zNote* obj);

    /* Set and get indent */
    inline int zNote_Set_Indent(zNote* obj, double indent);
    inline double zNote_Get_Indent(zNote* obj);
    
#ifdef __cplusplus
}
#endif

#endif /* _ZNOTE_H_ */
