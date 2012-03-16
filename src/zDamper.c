/* Implementation of damper class */
/* Thu Mar 15 10:22:45 GMTST 2012 */

#include "zDamper.h"

/* Virtual function */
static int _zdamper_draw(zGeneric* obj);

/* Constructor */
zGeneric* zDamper_New(zDamper* obj)
{
    /* call constructor helper */
    Z_CONSTRUCTOR_HELPER(obj, zDamper);

    /* create parent object */
    if(!zBase_New(&obj->z_parent))
	{
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}

    /* set properties */
    zDSideFrm_New(&obj->z_lh_frm);
    zDSideFrm_New(&obj->z_rh_frm);
    zDFrame_Set_Side_Flg(&obj->z_rh_frm.z_parent, 1);
    zDTBFrm_New(&obj->z_t_frm);
    zDTBFrm_New(&obj->z_b_frm);
    zDFrame_Set_Side_Flg(&obj->z_b_frm.z_parent, 1);    
    /* Set transom and mullion collection */
    obj->z_mls = NULL;
    obj->z_trs = NULL;
    obj->z_oflange = Z_DAMPER_FLANGE;
    obj->z_dflange = Z_DAMPER_FLANGE;
    obj->z_num_ml = 0;
    obj->z_num_tr = 0;
    
    obj->z_draw_func = NULL;
    obj->z_child = NULL;

    /* set child and function poitners of parent
     * object */
    obj->z_parent.z_draw_func = _zdamper_draw;
    obj->z_parent.z_child = (void*) obj;

    /* return parent object */
    return &obj->z_parent.z_sgeneric;
}


/* Destructor */
void zDamper_Delete(zDamper* obj)
{
    int i;
    /* check object */
    Z_CHECK_OBJ_VOID(obj);

    zBase_Delete(&obj->z_parent);

    /* Delete objects */
    zDSideFrm_Delete(&obj->z_lh_frm);
    zDSideFrm_Delete(&obj->z_rh_frm);
    zDTBFrm_Delete(&obj->z_t_frm);
    zDTBFrm_Delete(&obj->z_b_frm);

    if(obj->z_num_ml > 0 && obj->z_mls)
	{
	    for(i = 0; i < obj->z_num_ml; i++)
		zDMLFrm_Delete(&obj->z_mls[i]);
	    free(obj->z_mls);

	}

    if(obj->z_num_tr > 0 && obj->z_trs)
	{
	    for(i = 0; i < obj->z_num_tr; i++)
		zDTRFrm_Delete(&obj->z_trs[i]);
	    free(obj->z_trs);
	}

    obj->z_mls = NULL;
    obj->z_trs = NULL;
    obj->z_draw_func = NULL;
    obj->z_child = NULL;

    if(obj->z_int_flg)
	free(obj);
}

/* Draw method */
int zDamper_Draw(zDamper* obj)
{
    zGeneric* _zg;
    zGeneric* _lf, *_rf, *_tf, *_bf;		/* frames */
    zGeneric* _ml, *_tr;			/* mullion and transoms */
    zDevice* _dev;
    double _height;
    double _mod_width, _mod_height, _ml_width, _tr_width;
    int a, j;
    
    /* Check object */
    Z_CHECK_OBJ(obj);

    /* check drive and non drive flanges */
    if(obj->z_oflange <= 0.0 || obj->z_dflange <= 0.0)
	return 1;

    /* Get device object */
    _zg = &obj->z_parent.z_sgeneric;
    _dev = zGeneric_Get_Device(_zg);
    Z_CHECK_OBJ(_dev);
    
    /****************************************************/
    /* Drive side frame dimensions */
    _height = obj->z_parent.z_height + 2 * obj->z_oflange;
    _lf = &obj->z_lh_frm.z_parent.z_parent.z_sgeneric;
    zBase_Set_Base_Coords(Z_BASE(_lf), obj->z_parent.z_x, obj->z_parent.z_y);
    zBase_Set_Width_and_Height(Z_BASE(_lf),
    			       obj->z_dflange,
    			       _height);
    zGeneric_Set_Device(_lf, _dev);
    zGeneric_Set_Default_Dev_Context(_lf);
    zDFrame_Set_Return_Lip_Flg(Z_DFRAME(_lf), 1);

    /****************************************************/
    /* Non Drive side frame dimensions */
    _rf = &obj->z_rh_frm.z_parent.z_parent.z_sgeneric;
    zBase_Set_Base_Coords(Z_BASE(_rf),
			  obj->z_parent.z_x + obj->z_dflange + obj->z_parent.z_width,
			  obj->z_parent.z_y +
			  (obj->z_frm_type > 0? 0 : 1) * obj->z_oflange);
    zBase_Set_Width_and_Height(Z_BASE(_rf),
			       obj->z_oflange,
			       obj->z_frm_type > 0? _height : obj->z_parent.z_height);

    zGeneric_Set_Device(_rf, _dev);
    zGeneric_Set_Default_Dev_Context(_rf);

    /****************************************************/
    /* Top frame */
    _tf = &obj->z_t_frm.z_parent.z_parent.z_sgeneric;
    zBase_Set_Base_Coords(Z_BASE(_tf),
    			  obj->z_parent.z_x + obj->z_dflange,
    			  obj->z_parent.z_y);
    zBase_Set_Width_and_Height(Z_BASE(_tf),
    			       obj->z_parent.z_width + obj->z_frm_type > 0? 0.0 : obj->z_oflange,
    			       obj->z_oflange);
    zGeneric_Set_Device(_tf, _dev);
    zGeneric_Set_Default_Dev_Context(_tf);

    /****************************************************/
    /* Bottom frame */
    _bf = &obj->z_b_frm.z_parent.z_parent.z_sgeneric;
    zBase_Set_Base_Coords(Z_BASE(_bf),
    			  obj->z_parent.z_x + obj->z_dflange,
    			  _height - obj->z_oflange);
    zBase_Set_Width_and_Height(Z_BASE(_bf),
    			       obj->z_parent.z_width + obj->z_frm_type > 0? 0.0 : obj->z_oflange,
    			       obj->z_oflange);
    zGeneric_Set_Device(_bf, _dev);
    zGeneric_Set_Default_Dev_Context(_bf);
    /****************************************************/
    /* Draw mullion and transom */
    /* calculate modular width and height */
    /* _ml_width = 0.0; */
    /* _tr_width = 0.0; */
    /* _mod_height = obj->z_parent.z_height; */
    /* _mod_width = obj->z_parent.z_width; */
    
    /* if(obj->z_mls) */
    /* 	{ */
    /* 	    _ml_width = obj->z_mls[0].z_parent.z_parent.z_width > 0.0? obj->z_mls[0].z_parent.z_parent.z_width : */
    /* 			 Z_MULLION_WIDTH; */
    /* 	    _mod_width = (obj->z_parent.z_width - _ml_width * (double) obj->z_num_ml) / (obj->z_num_ml + 1); */
    /* 	    for(a = 0; a < obj->z_num_ml; a++) */
    /* 		{ */
    /* 		    _ml = &obj->z_mls[a].z_parent.z_parent.z_sgeneric; */
    /* 		    zBase_Set_Base_Coords(Z_BASE(_ml), */
    /* 					  obj->z_parent.z_x + obj->z_dflange + (double) (a + 1) * _mod_width + */
    /* 					  (double) a * _ml_width, */
    /* 					  obj->z_parent.z_y + obj->z_oflange); */
    /* 		    zBase_Set_Width_and_Height(Z_BASE(_ml), */
    /* 					       _ml_width, */
    /* 					       obj->z_parent.z_height); */
    /* 		    zGeneric_Set_Device(_ml, _dev); */
    /* 		    zGeneric_Set_Default_Dev_Context(_ml); */
    /* 		    zGeneric_Draw(_ml); */
    /* 		} */
    /* 	} */

    /* if(obj->z_trs) */
    /* 	{ */
    /* 	    _tr_width = obj->z_trs[0].z_parent.z_parent.z_width > 0.0? obj->z_trs[0].z_parent.z_parent.z_width : */
    /* 		Z_TRANSOM_WIDTH; */
    /* 	    _mod_height = (obj->z_parent.z_height - _tr_width * (double) obj->z_num_tr) / (obj->z_num_tr + 1); */
    /* 	    for(a = 0; a < obj->z_num_tr; a++) */
    /* 		{ */
    /* 		    _tr = &obj->z_trs[a].z_parent.z_parent.z_sgeneric; */
    /* 		    zGeneric_Set_Device(_tr, _dev); */
    /* 		    zGeneric_Set_Default_Dev_Context(_tr);		     */
    /* 		    for(j = 0; j < obj->z_num_ml; j++) */
    /* 			{ */

    /* 			    zBase_Set_Base_Coords(Z_BASE(_tr), */
    /* 						  obj->z_parent.z_x + obj->z_dflange + (double) j * (_mod_width + _ml_width), */
    /* 						  obj->z_parent.z_y + obj->z_oflange + (double) (a + 1) * _mod_height); */
    /* 			    zBase_Set_Width_and_Height(Z_BASE(_tr), */
    /* 						       _mod_width, */
    /* 						       _tr_width); */
    /* 			    zGeneric_Draw(_tr); */
    /* 			} */
    /* 		} */
    /* 	} */

    /* Call draw methods of frame components */
    zGeneric_Draw(_lf);
    zGeneric_Draw(_rf);
    zGeneric_Draw(_tf);
    zGeneric_Draw(_bf);

    _zg = NULL;
    _lf = NULL;
    _rf = NULL;
    _tf = NULL;
    _bf = NULL;
    _dev = NULL;
    return 0;
}

/************************************************************************************************************/
/* Property methods */
/* Get left hand side frame */
inline zDSideFrm* zDamper_Get_LH_Frame(zDamper* obj)
{
    Z_CHECK_OBJ_PTR(obj);
    return &obj->z_lh_frm;
}

/* Get right hand side frame */
inline zDSideFrm* zDamper_Get_RH_Frame(zDamper* obj)
{
    Z_CHECK_OBJ_PTR(obj);
    return &obj->z_rh_frm;
}

/* Get top frame */
inline zDTBFrm* zDamper_Get_T_Frame(zDamper* obj)
{
    Z_CHECK_OBJ_PTR(obj);
    return &obj->z_t_frm;
}

/* Get bottom frame */
inline zDTBFrm* zDamper_Get_B_Frame(zDamper* obj)
{
    Z_CHECK_OBJ_PTR(obj);
    return &obj->z_b_frm;
}

/* Set drive flange */
inline int zDamper_Set_Drive_Flagne(zDamper* obj, double flange)
{
    Z_CHECK_OBJ(obj);
    obj->z_dflange = flange;
    return 0;
}


/* Get drive side flange */
inline double zDamper_Get_Drive_Flange(zDamper* obj)
{
    Z_CHECK_OBJ_DOUBLE(obj);
    return obj->z_dflange;
}

/* Set non drive flange */
inline int zDamper_Set_NonDrive_Flange(zDamper* obj, double flange)
{
    Z_CHECK_OBJ(obj);
    obj->z_oflange = flange;
    return 0;
}

/* Get non drive flange */
inline double zDamper_Get_NonDrive_Flange(zDamper* obj)
{
    Z_CHECK_OBJ_DOUBLE(obj);
    return obj->z_oflange;
}

/* Set number of mullions */
inline int zDamper_Set_Num_Mullions(zDamper* obj, unsigned int num)
{
    int i;
    Z_CHECK_OBJ(obj);
    if(num <= 0)
	return 1;
    
    obj->z_num_ml = num;

    /* Check if mullions were created, then remove */
    if(obj->z_mls)
	free(obj->z_mls);

    obj->z_mls = (zDMLFrm*) calloc(num, sizeof(zDMLFrm));
    for(i = 0; i < num; i++)
	zDMLFrm_New(&obj->z_mls[i]);

    return 0;
}

/* Set number of transoms */
inline int zDamper_Set_Num_Transoms(zDamper* obj, unsigned int num)
{
    int i;
    Z_CHECK_OBJ(obj);

    if(num <= 0)
	return 1;

    obj->z_num_tr = num;

    /* Check if transoms were created, then remove */
    if(obj->z_trs)
	free(obj->z_trs);

    obj->z_trs	 = (zDTRFrm*) calloc(num, sizeof(zDTRFrm));
    for(i = 0; i < num; i++)
	zDTRFrm_New(&obj->z_trs[i]);
    
    return 0;
}

/* Get mullion pointed by index */
inline zDMLFrm* zDamper_Get_Mullion(zDamper* obj, unsigned int ix)
{
    Z_CHECK_OBJ_PTR(obj);
    if(ix > obj->z_num_ml)
	return NULL;

    return &obj->z_mls[ix];
}

/* Get transom pointed by index */
inline zDTRFrm* zDamper_Get_Transom(zDamper* obj, unsigned int ix)
{
    Z_CHECK_OBJ_PTR(obj);
    if(ix > obj->z_num_tr)
	return NULL;

    return &obj->z_trs[ix];
}

/*******************************************************************/
/* Private functions */

/* Virtual draw functions */
static int _zdamper_draw(zGeneric* obj)
{
    zDamper* _dmp;
    int rt_val;

    Z_CHECK_OBJ(obj);
    _dmp = Z_DAMPER(obj);
    rt_val = zDamper_Draw(Z_DAMPER(obj));
    if(_dmp->z_draw_func)
	return _dmp->z_draw_func(obj);
    else
	return rt_val;
}
