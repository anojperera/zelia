/* Header file for blade class. Defines interfaces for drawing
 * and manipulating blades */

#ifndef _ZBLADE_H_
#define _ZBLADE_H_

#include "zBase.h"
#include "zVar.h"

/* Forward declaration of structr */
typedef struct _zBlade zBlade;

struct _zBlade
{
    zBase z_parent;				/* parent object */
    unsigned int z_int_flg;			/* internal flag */
    zBladeType z_blade_type;			/* blade type */
    zBladeISO_Type z_iso_type;			/* isolating blade type */
    zgeneric_fptr z_draw_func;			/* draw function */
    void* z_child;				/* child pointer */
};


#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zGeneric* zBlade_New(zBlade* obj);
    void zBlade_Delete(zBlade* obj);

    /* Draw method */
    int zBlade_Draw(zBlade* obj);

    /* Set and get blade type */
    inline int zBlade_Set_Blade_Type(zBlade* obj, zBladeType ztype);
    inline zBladeType zBlade_Get_Blade_Type(zBlade* obj);

    /* Set and get isolating blade type */
    /* Default is set to AA */
    inline int zBlade_Set_Blade_ISO_Type(zBlade* obj, zBladeISO_Type ztype);
    inline zBladeISO_Type zBlade_Get_Blade_ISO_Type(zBlade* obj);

#ifdef __cplusplus
}
#endif

#endif /* _ZBLADE_H_ */
