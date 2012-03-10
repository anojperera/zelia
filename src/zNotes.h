/* zNotes class is a collection of zNote.
 * Uses a dynamically expanding array of zGenerics class.
 * Sat Mar 10 15:23:54 GMT 2012 */

#ifndef _ZNOTES_H_
#define _ZNOTES_H_

#include <stdlib.h>
#include "zDevice.h"
#include "zGenerics.h"
#include "zNote.h"

#define Z_NOTES_TITLE_BUFF 256

/* Forward declaration of struct */
typedef struct _zNotes zNotes;

struct _zNotes
{
    zGenerics z_parent;				/* Parent object */
    unsigned int z_int_flg;			/* Internal flag */
    double _z_height;				/* Height */
    double z_width;				/* Width */
    double z_x;					/* x coord */
    double z_y;					/* y coord */
    char z_title[Z_NOTES_TITLE_BUFF];		/* Title buffer */
    unsigned int z_uline_flg;			/* Underline flag */
    unsigned int z_counter;			/* Counter */
    size_t z_obj_sz;				/* Object size */
    zcollection_fptr z_draw_func;		/* Draw function */
    void* z_child;				/* Child pointer */
};

#ifdef __cplusplus
extern "C" {
#endif
    
    /* Constructor & Destructor */
    zGenerics* zNotes_New(zNotes* obj,		/* optional argument */
			  zDevice* dev,
			  double x,
			  double y,
			  double width);
    void zNotes_Delete(zNotes* obj);

    /* Add note */
    int zNotes_Add(zNotes* obj, zDevice* dev, const char* note);

    /***********************************************/    
    /* Property methods */
    
    /* Get Note */
    inline zNote* zNotes_Get_Note(zNotes* obj, unsigned int ix);

    /* Get Height */
    inline double zNoets_Get_Height(zNotes* obj);

    /* Set and get title text */
    inline int zNotes_Set_Title(zNotes* obj, const char* title);
    inline const char* zNotes_Get_Title(zNotes* obj);

    /* Set underline flag */
    inline int zNotes_Set_Uline_Flg(zNotes* obj, unsigned int flg);
    
#ifdef __cplusplus
}
#endif
#endif /* _ZNOTES_H_ */
