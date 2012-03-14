/* Implementation of zDFrame class */
/* Sun Mar 11 13:45:41 GMT 2012 */

#include "zDFrame.h"
#include "zVar.h"

/* Virtual functions */
static int _zddrame_draw(zGeneric* obj);

/* Constructor */
zGeneric* zDFrame_New(zDFrame* obj)
{
    /* constructor helper */
    Z_CONSTRUCTOR_HELPER(obj, zDFrame);

    /* create parent object */
    if(!zBase_New(&obj->z_parent))
	{
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}

    /* Set properties */
    obj->z_rtlip_flg = 0;
    obj->z_fillet_flg = 1;		/* Fillet as required */
    obj->z_side_flg = 0;		/* Side flag */
    obj->z_draw_func = NULL;
    obj->z_child = NULL;

    /* Set function pointers and child pointer of parent
     * object */
    obj->z_parent.z_draw_func = _zddrame_draw;
    obj->z_parent.z_child = (void*) obj;

    /* Return parent object */
    return &obj->z_parent.z_sgeneric;
}

/* Destructor */
void zDFrame_Delete(zDFrame* obj)
{
    /* check object */ 
    Z_CHECK_OBJ_VOID(obj);

    /* Delete parent object */
    zBase_Delete(&obj->z_parent);

    obj->z_draw_func = NULL;
    obj->z_child = NULL;

    /* If internally created, free memory */
    if(obj->z_int_flg)
	free(obj);
}

/**********************************************************************/
/* Property methods */

/* Set return lip flag */
inline int zDFrame_Set_Return_Lip_Flg(zDFrame* obj, unsigned int flg)
{
    Z_CHECK_OBJ(obj);
    obj->z_rtlip_flg = flg;
    return 0;
}

/* Get return lip flag */
inline unsigned int zDFrame_Get_Return_Lip_Flg(zDFrame* obj)
{
    if(obj == NULL)
	return 0;

    return obj->z_rtlip_flg;
}

/* Set frame fillet flag */
inline int zDFrame_Set_Fillet_Flg(zDFrame* obj, unsigned int flg)
{
    Z_CHECK_OBJ(obj);
    obj->z_fillet_flg = flg;
    return 0;
}

/* Get frame fillet flag */
inline unsigned int zDFrame_Get_Fillet_Flg(zDFrame* obj)
{
    if(obj == NULL)
	return 0;
    return obj->z_fillet_flg;
}

/* Set frame return lip width */
inline int zDFrame_Set_Return_Lip_Width(zDFrame* obj, double width)
{
    /* Check object */
    Z_CHECK_OBJ(obj);
    obj->z_rtlip_width = width;
    return 0;
}

/* Get return lip width */
inline double zDFrame_Get_Return_Lip_Width(zDFrame* obj)
{
    Z_CHECK_OBJ_DOUBLE(obj);
    return obj->z_rtlip_width;
}

/* Set side flag */
inline int zDFrame_Set_Side_Flg(zDFrame* obj, unsigned int flg)
{
    Z_CHECK_OBJ(obj);
    obj->z_side_flg = flg;
    return 0;
}

/* Get side flag */
inline unsigned int zDFrame_Get_Side_Flg(zDFrame* obj)
{
    if(obj == NULL)
	return 0;
    return obj->z_side_flg;
}
/****************************************************************************/
/* Private draw functions */

/* Virtual draw functions */
static int _zddrame_draw(zGeneric* obj)
{
    zDFrame* _dfrm;
    
    /* Check object */
    Z_CHECK_OBJ(obj);

    _dfrm = Z_DFRAME(obj);
    /* Check if function poitner was
     * assigned. */
    if(_dfrm->z_draw_func)
	return _dfrm->z_draw_func(obj);
    else
	return 1;
}
