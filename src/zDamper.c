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
    obj->z_bld_flg = 0;					/* blade flag set to false */
    zDFrame_Set_Side_Flg(&obj->z_b_frm.z_parent, 1);    
    /* Set transom and mullion collection */
    obj->z_frm_type = 0;    
    obj->z_mls = NULL;
    obj->z_trs = NULL;
    obj->z_oflange = Z_DAMPER_FLANGE;
    obj->z_dflange = Z_DAMPER_FLANGE;
    obj->z_num_ml = 0;
    obj->z_num_tr = 0;
    obj->z_num_blades = 1;				/* number of blades */
    obj->z_bld_type = zBlade_ISO;
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
    zBlades_Delete(&obj->z_blds);

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
    double _bld_pitch;
    unsigned int _num_h_mod = 1, _num_v_mod = 1;
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
    zBase_Set_Thickness(Z_BASE(_lf), obj->z_parent.z_thk);
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
    zBase_Set_Thickness(Z_BASE(_rf), obj->z_parent.z_thk);

    zGeneric_Set_Device(_rf, _dev);
    zGeneric_Set_Default_Dev_Context(_rf);

    /****************************************************/
    /* Top frame */
    _tf = &obj->z_t_frm.z_parent.z_parent.z_sgeneric;
    zBase_Set_Base_Coords(Z_BASE(_tf),
    			  obj->z_parent.z_x + obj->z_dflange,
    			  obj->z_parent.z_y);
    zBase_Set_Width_and_Height(Z_BASE(_tf),
    			       obj->z_parent.z_width + (obj->z_frm_type > 0? 0.0 : obj->z_oflange),
    			       obj->z_oflange);
    zBase_Set_Thickness(Z_BASE(_tf), obj->z_parent.z_thk);
    zGeneric_Set_Device(_tf, _dev);
    zGeneric_Set_Default_Dev_Context(_tf);

    /****************************************************/
    /* Bottom frame */
    _bf = &obj->z_b_frm.z_parent.z_parent.z_sgeneric;
    zBase_Set_Base_Coords(Z_BASE(_bf),
    			  obj->z_parent.z_x + obj->z_dflange,
    			  obj->z_parent.z_y + _height - obj->z_oflange);
    zBase_Set_Width_and_Height(Z_BASE(_bf),
    			       obj->z_parent.z_width + (obj->z_frm_type > 0? 0.0 : obj->z_oflange),
    			       obj->z_oflange);
    zBase_Set_Thickness(Z_BASE(_bf), obj->z_parent.z_thk);
    zGeneric_Set_Device(_bf, _dev);
    zGeneric_Set_Default_Dev_Context(_bf);
    /****************************************************/
    /* Draw mullion and transom */
    /* calculate modular width and height */
    _ml_width = 0.0;
    _tr_width = 0.0;
    _mod_height = obj->z_parent.z_height;
    _mod_width = obj->z_parent.z_width;

    if(obj->z_mls)
    	{
    	    _ml_width = obj->z_mls[0].z_parent.z_parent.z_width > 0.0? obj->z_mls[0].z_parent.z_parent.z_width :
    			 Z_MULLION_WIDTH;
    	    _mod_width = (obj->z_parent.z_width - _ml_width * (double) obj->z_num_ml) / (obj->z_num_ml + 1);
    	    for(a = 0; a < obj->z_num_ml; a++)
    		{
    		    _ml = &obj->z_mls[a].z_parent.z_parent.z_sgeneric;
    		    zBase_Set_Base_Coords(Z_BASE(_ml),
    					  obj->z_parent.z_x + obj->z_dflange + (double) (a + 1) * _mod_width +
    					  (double) a * _ml_width,
    					  obj->z_parent.z_y + obj->z_oflange);
    		    zBase_Set_Width_and_Height(Z_BASE(_ml),
    					       _ml_width,
    					       obj->z_parent.z_height);
    		    zGeneric_Set_Device(_ml, _dev);
    		    zGeneric_Set_Default_Dev_Context(_ml);
    		    zGeneric_Draw(_ml);
    		}
    	}

    if(obj->z_trs)
    	{
    	    _tr_width = obj->z_trs[0].z_parent.z_parent.z_width > 0.0? obj->z_trs[0].z_parent.z_parent.z_width :
    		Z_TRANSOM_WIDTH;
    	    _mod_height = (obj->z_parent.z_height - _tr_width * (double) obj->z_num_tr) / (obj->z_num_tr + 1);
    	    for(a = 0; a < obj->z_num_tr; a++)
    		{
    		    _tr = &obj->z_trs[a].z_parent.z_parent.z_sgeneric;
    		    zGeneric_Set_Device(_tr, _dev);
    		    zGeneric_Set_Default_Dev_Context(_tr);
    		    for(j = 0; j < obj->z_num_ml; j++)
    			{

    			    zBase_Set_Base_Coords(Z_BASE(_tr),
    						  obj->z_parent.z_x + obj->z_dflange + (double) j * (_mod_width + _ml_width),
    						  obj->z_parent.z_y + obj->z_oflange + (double) (a + 1) * _mod_height);
    			    zBase_Set_Width_and_Height(Z_BASE(_tr),
    						       _mod_width,
    						       _tr_width);
    			    zGeneric_Draw(_tr);
    			}
    		}
    	}
    
    /* Call draw methods of frame components */
    zGeneric_Draw(_lf);
    zGeneric_Draw(_rf);
    zGeneric_Draw(_tf);
    zGeneric_Draw(_bf);
    
    /* Create blades and shafts collection */
    _num_h_mod += obj->z_num_ml;				/* assign number of horizontal and vertical modules */
    _num_v_mod += obj->z_num_tr;
    _bld_pitch = _mod_height / (double) obj->z_num_blades;
    for(j=0; j<_num_v_mod; j++)    
	{
	    for(a=0; a<_num_h_mod; a++)
		{
		    if(obj->z_bld_flg==0 && a==0)
			{
			    zBlades_New(&obj->z_blds,
					_dev,
					obj->z_parent.z_x + obj->z_dflange,
					obj->z_parent.z_y + obj->z_oflange,
					_mod_width,
					_mod_height,
					obj->z_num_blades,
					obj->z_bld_type);

			    zShafts_New(&obj->z_shfts,
					_dev,
					obj->z_parent.z_x + obj->z_dflange - Z_SHAFT_EXTRA,
					obj->z_parent.z_y + obj->z_oflange + _bld_pitch / 2,
					obj->z_parent.z_width + Z_SHAFT_EXTRA * 2,
					Z_SHAFT_DIA,
					obj->z_num_blades,
					_bld_pitch);
			    
			    obj->z_bld_flg = 1;
			}
		    else
			{
			    zBlades_Set_Base_Coordinates(&obj->z_blds,
							 obj->z_parent.z_x + obj->z_dflange + (double) a  * (_mod_width + _ml_width),
							 obj->z_parent.z_y + obj->z_oflange + (double) j * (_mod_height + _tr_width));
			}

		    zGenerics_Draw(&obj->z_blds.z_parent);
		}
	    if(obj->z_bld_flg)
		{
		    zShafts_Set_Coordinates(&obj->z_shfts,
					    obj->z_parent.z_x + obj->z_dflange - Z_SHAFT_EXTRA,
					    obj->z_parent.z_y + obj->z_oflange + _bld_pitch / 2 + 
					    (double) j * (_mod_height + _tr_width));
		}
	    zGenerics_Draw(&obj->z_shfts.z_parent);
	}
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

/* Set frame type */
inline int zDamper_Set_Frame_Type(zDamper* obj, unsigned int ztype)
{
    Z_CHECK_OBJ(obj);
    obj->z_frm_type = ztype;
    return 0;
}

/* Get frame type */
inline unsigned int zDamper_Get_Frame_Type(zDamper* obj)
{
    if(obj == NULL)
	return 0;
    return obj->z_frm_type;
}

/* Set number of blades */
inline int zDamper_Set_Num_Blades(zDamper* obj, unsigned int num_blades)
{
    Z_CHECK_OBJ(obj);
    obj->z_num_blades = num_blades;
    return 0;
}

/* Set blade type */
inline int zDamper_Set_Blade_Type(zDamper* obj, zBladeType ztype)
{
    Z_CHECK_OBJ(obj);
    obj->z_bld_type = ztype;
    return 0;
}

/* Get blades collection */
inline zBlades* zDamper_Get_Blades(zDamper* obj)
{
    Z_CHECK_OBJ_PTR(obj);
    return &obj->z_blds;
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
