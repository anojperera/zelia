/* implementation of sheet object
   Sat Oct 16 23:50:14 BST 2010 */
#include "zSheet.h"

/* token for cairo stroke in each
   function */
/* #define Z_SHEET_STROKE_FUNCTION */
    
/* private functions */
static int zsheet_draw_oborder(zSheet* obj);
static int zsheet_draw_grid(zSheet* obj);
static int zsheet_draw_top_revbox(zSheet* obj);
static int zsheet_draw_decal(zSheet* obj);

static int zsheet_draw_matbox(zSheet* obj);
static int zsheet_draw_projn(zSheet* obj);

/* add drawing header details */
static int zsheet_add_attrib_headers(zSheet* obj);

/* paints the logo into current context */
static int zsheet_paint_logo(zSheet* obj);

/* constructs wozair address from
   constants */
static char* zsheet_wozair_address();

/* add drawing attributes */
static int zsheet_add_attribs(zSheet* obj);

/*==================================================================*/
/* draw function */
static int _zsheet_draw_function(zGeneric* obj);


/* create generic sheet object */
zGeneric* zSheet_New(zSheet* obj)
{
    /* check for object, if not create it */
    if(obj == NULL)
	{
	    obj = (zSheet*) malloc(sizeof(zSheet));
	    Z_CHECK_OBJ_PTR(obj);
	    obj->z_int_flg = 1;
	}
    else
	obj->z_int_flg = 0;


    /* create generic object and check for function call */
    Z_CHECK_OBJ_PTR(zGeneric_New(&obj->z_sgeneric));

    zGeneric_Set_LineWeight(&obj->z_sgeneric,
			    zLWeight2);
    /* set properties */

    obj->z_slogo_path[0] = '\0';
    obj->z_sbrd_attrib = NULL;
    obj->z_sgrid_flg = 0;
    obj->z_ssafe_flg = 0;
    obj->z_child = NULL;
    obj->z_draw_func = NULL;
    obj->z_obj_sz = sizeof(zSheet);
    /* initialise array coordinates */
    int i = 0;
    for(i=0; i<Z_MAX_ATTRIB; i++)
	{
	    obj->z_x_attrib[i] = 0;
	    obj->z_y_attrib[i] = 0;
	}

    /* set draw function pointer of base object */
    obj->z_sgeneric.z_draw_func = _zsheet_draw_function;
    /* assign child pointer to base object */
    obj->z_sgeneric.z_child = (void*) obj;

    return &obj->z_sgeneric;

}

/* delete sheet object */
void zSheet_Delete(zSheet* obj)
{
    /* check for NULL pointer */
    Z_CHECK_OBJ_VOID(obj);

    /* destroy parent object */
    zGeneric_Delete(&obj->z_sgeneric);

    /* set border attributes pointer to NULL */
    obj->z_sbrd_attrib = NULL;
    
    /* delete object if it was internally created */
    if(obj->z_int_flg)
	free(obj);
}


/* set attributes */
inline int zSheet_Set_Attributes(zSheet* obj,
			   zBrd_Attrib* var)
{
    Z_CHECK_OBJ(obj);
    Z_CHECK_OBJ(obj);
    obj->z_sbrd_attrib = var;
    return 0;
}

/* get attributes */
inline zBrd_Attrib* zSheet_Get_Attributes(zSheet* obj)
{
    /* check for NULL pointer */
    Z_CHECK_OBJ_PTR(obj);
    return obj->z_sbrd_attrib;
}

/* set grid flag */
inline int  zSheet_Set_GridFlag(zSheet* obj, int var)
{
    /* check for NULL pointer */
    Z_CHECK_OBJ(obj);
    obj->z_sgrid_flg = var;
    return 0;
}

/* get grid flag */
inline int zSheet_Get_GridFlag(zSheet* obj)
{
    /* check for NULL pointer */
    Z_CHECK_OBJ(obj);
    return obj->z_sgrid_flg;
}

/* set logo path */
inline int zSheet_Set_LogoPath(zSheet* obj, const char* var)
{
    /* check for NULL pointer */
    Z_CHECK_OBJ(obj);
    Z_CHECK_OBJ(var);

    /* copy to buffer */
    strcpy(obj->z_slogo_path, var);

    return 0;
}

/* get logo path */
inline char* zSheet_Get_LogoPath(zSheet* obj)
{
    /* check for NULL pointer */
    Z_CHECK_OBJ_PTR(obj);
    return obj->z_slogo_path;
}

/* create border */
int zSheet_Create_Border(zSheet* obj)
{
    /* draw border */
    int dec = 0;

    /* check for NULL pointer */
    Z_CHECK_OBJ(obj);
    
    /* return if border already drawn */
    if(obj->z_ssafe_flg)
	return 0;
    
    dec += zsheet_draw_oborder(obj);
    dec += zsheet_draw_grid(obj);
    dec += zsheet_draw_top_revbox(obj);
    dec += zsheet_draw_decal(obj);
    dec += zsheet_draw_matbox(obj);
    dec += zsheet_add_attrib_headers(obj);
    dec += zsheet_add_attribs(obj);
    /* project is drawn separate as it requires center
       lines. function calls stroke internally */
    dec += zsheet_draw_projn(obj);
    dec += zsheet_paint_logo(obj);
	
    if(dec > 0)
	{
	    cairo_stroke(zGeneric_Get_Dev_Context(&obj->z_sgeneric));
	    dec = 0;
	}	

    obj->z_ssafe_flg = 1;
    return 0;
}

/* private functions */
static int zsheet_draw_oborder(zSheet* obj)
{

    /* check device context */
    if(!zGeneric_Get_Device(&obj->z_sgeneric))
	return 0;

    /* pointer to under lying device */
    zDevice* dev = zGeneric_Get_Device(&obj->z_sgeneric);

    /* draw outer border */
    const int NUM = 5;
    int i = 0;
    double X[NUM], Y[NUM];
    double X1[NUM+1], Y1[NUM+1];

    /* outer border coordinates */
    X[0] = Z_DEFAULT_BORDER_SZ;
    Y[0] = Z_DEFAULT_BORDER_SZ;

    X[1] = X[0];
    Y[1] = dev->z_page_height - Z_DEFAULT_BORDER_SZ;

    X[2] = dev->z_page_width - Z_DEFAULT_BORDER_SZ;
    Y[2] = Y[1];

    X[3] = X[2];
    Y[3] = Y[0];

    X[4] = X[0];
    Y[4] = Y[0];

    /* grid coordinates */
    X1[0] = Z_DEFAULT_BORDER_SZ + Z_DEFAULT_GRID_SZ;
    Y1[0] = Z_DEFAULT_BORDER_SZ + Z_DEFAULT_GRID_SZ;

    X1[1] = X1[0];
    Y1[1] = dev->z_page_height - Z_DEFAULT_BORDER_SZ -
	Z_DEFAULT_GRID_SZ;

    X1[2] = dev->z_page_width - Z_WOZ_MAIN_BRD_WIDTH -
	Z_DEFAULT_BORDER_SZ;
    Y1[2] = Y1[1];

    X1[3] = dev->z_page_width - Z_DEFAULT_BORDER_SZ -
	Z_DEFAULT_GRID_SZ;
    Y1[3] = dev->z_page_height - Z_WOZ_MAIN_BRD_HEIGHT -
	Z_DEFAULT_BORDER_SZ;

    X1[4] = X1[3];
    Y1[4] = Y1[0];

    X1[5] = X1[0];
    Y1[5] = Y1[0];
	
    cairo_move_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
		  ConvToPoints(&X[0]),
		  ConvToPoints(&Y[0]));
		
    for(i=1; i<NUM; i++)
	{
	    cairo_line_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
			  ConvToPoints(&X[i]),
			  ConvToPoints(&Y[i]));
	}

    
    if(obj->z_sgrid_flg > 0)
	{
	    cairo_move_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
			  ConvToPoints(&X1[0]),
			  ConvToPoints(&Y1[0]));
		
	    for(i=1; i<NUM+1; i++)
		{
		    if(i==3)
			{
			    cairo_move_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
					  ConvToPoints(&X1[i]),
					  ConvToPoints(&Y1[i]));
			}
		    else
			{
			    cairo_line_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
					  ConvToPoints(&X1[i]),
					  ConvToPoints(&Y1[i]));
			}
		}			
	}
	
#ifdef Z_SHEET_STROKE_FUNCTION
    cairo_stroke(zGeneric_Get_Dev_Context(&obj->z_sgeneric));
#endif
	
    return i;
}

/* draws grid */
static int zsheet_draw_grid(zSheet* obj)
{
    /* check fo NULL pointer */
    if(obj == NULL)
	return 0;

    if(!obj->z_sgrid_flg)
	return 0;
    
    /* pointer to under lying device */
    zDevice* dev = zGeneric_Get_Device(&obj->z_sgeneric);
	

    /* horizontal and vertical number
       grid */
    int h_num, v_num;
    double h_pitch, v_pitch;

    double st = 0; 	/* Z_DEFAULT_BORDER_SZ + Z_DEFAULT_GRID_SZ */
    const int DEV = 2;
    
    h_num = (int) (dev->z_page_width - 2 * st) /
	Z_DEFAULT_GRID_PITCH;
    v_num = (int) (dev->z_page_height - 2 * st) /
	Z_DEFAULT_GRID_PITCH;

    h_pitch = (dev->z_page_width - 2 * st) /
	(double) h_num;
    v_pitch = (dev->z_page_height - 2 * st) /
	(double) v_num;

    double x, x1, x2, y, y1, y2, a, ch_x, ch_y;

    ch_y = 0;
    ch_x = 0;
	
    int i;
    int char_num = 65;
    
    /* array of pointers for the grid */
    char* grd_cord_v = (char*) calloc(2, sizeof(char));

    /* set font face */
    cairo_select_font_face(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
			   Z_GRD_FONT_STYLE,
			   CAIRO_FONT_SLANT_NORMAL,
			   CAIRO_FONT_WEIGHT_NORMAL);
    /* set font size */
    cairo_set_font_size(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
			Z_GRD_FONT_SZ);

    /* vertical pitch */
    for(i=0,y=st+v_pitch; i<v_num; i++,y+=v_pitch)
	{
	    /* check if exceeds border */
	    if(y > dev->z_page_height - Z_DEFAULT_BORDER_SZ)
		break;
	    grd_cord_v[0] = (char) char_num++;
	    grd_cord_v[1] = '\0';
	    

	    /* horizontal grid */
	    for(a=0; a<dev->z_page_width+1; a+=dev->z_page_width)
		{
		    x = fabs(a - Z_DEFAULT_BORDER_SZ);
		    x1 = fabs(a - (Z_DEFAULT_BORDER_SZ + Z_DEFAULT_GRID_SZ));

		    x2 = fabs(a - (Z_DEFAULT_BORDER_SZ + Z_DEFAULT_GRID_SZ_WIDE));

		    /* grid coordinates */
		    ch_x = fabs(a -  Z_DEFAULT_BORDER_SZ - 
				((a==0)?(Z_DEFAULT_GRID_SZ/3):
				 (4*Z_DEFAULT_GRID_SZ/5)));
		    
		    if(a==0)
			ch_y += ((i==0)?v_pitch/2:v_pitch);
		    

		    /* grid lines */
		    cairo_move_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
				  ConvToPoints(&x),
				  ConvToPoints(&y));
		    
		    if(a==0 || y < dev->z_page_height - Z_DEFAULT_BORDER_SZ
		       - Z_WOZ_MAIN_BRD_HEIGHT)
			{
			    if(i%DEV)
				{
				    cairo_line_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
						  ConvToPoints(&x1),
						  ConvToPoints(&y));
			
				}
			    else
				{
				    cairo_line_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
						  ConvToPoints(&x2),
						  ConvToPoints(&y));					
				}

			    /* grid reference */
			    cairo_move_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
					  ConvToPoints(&ch_x),
					  ConvToPoints(&ch_y));
			    /* display text */
			    cairo_show_text(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
					    grd_cord_v);
		    			    
			}
		}
	    
	}
    
    grd_cord_v[0] = (char) char_num++;
    grd_cord_v[1] = '\0';
    
    ch_x = Z_DEFAULT_BORDER_SZ + Z_DEFAULT_GRID_SZ / 3;
    ch_y += v_pitch;
    
    /* grid reference */
    cairo_move_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
		  ConvToPoints(&ch_x),
		  ConvToPoints(&ch_y));
    /* display text */
    cairo_show_text(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
		    grd_cord_v);
    free(grd_cord_v);
    grd_cord_v = NULL;
    /*************************************************/

    /* char buff to store int */
    grd_cord_v = (char*) malloc(sizeof(int)+1);

    
    /* horizontal pitch */
    for(i=0,x=st+h_pitch; i<h_num; i++,x+=v_pitch)
	{
	    /* check if border exceeds */
	    if(x > dev->z_page_width - Z_DEFAULT_BORDER_SZ)
		break;

	    /* copy grid number to char buffer */
	    sprintf(grd_cord_v, "%i", i+1);
	    
	    for(a=0; a<dev->z_page_height+1;
		a+=dev->z_page_height)
		{
		    y = fabs(a - Z_DEFAULT_BORDER_SZ);
		    y1 = fabs(a - (Z_DEFAULT_BORDER_SZ + Z_DEFAULT_GRID_SZ));

		    y2 = fabs(a - (Z_DEFAULT_BORDER_SZ + Z_DEFAULT_GRID_SZ_WIDE));

		    /* grid coordinates */
		    if(a==0)
			ch_x += ((i==0)?h_pitch/2:h_pitch);

		    ch_y = fabs(a - Z_DEFAULT_BORDER_SZ - 
				((a==0)?(4*Z_DEFAULT_GRID_SZ/5):
				 (Z_DEFAULT_GRID_SZ/3)));


		    /* grid lines */

		    cairo_move_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
				  ConvToPoints(&x),
				  ConvToPoints(&y));
		    if(a==0 || x < dev->z_page_width -
		       Z_DEFAULT_BORDER_SZ - Z_WOZ_MAIN_BRD_WIDTH)
			{
			    if((i%DEV) ||
			       (x < (Z_WOZ_MAIN_REVBOX_WIDTH + Z_DEFAULT_GRID_SZ
				     + Z_DEFAULT_BORDER_SZ)))
				{
				    cairo_line_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
						  ConvToPoints(&x),
						  ConvToPoints(&y1));
			
				}
			    else
				{


				    cairo_line_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
						  ConvToPoints(&x),
						  ConvToPoints(&y2));

				}
			    /* grid reference */
			    cairo_move_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
					  ConvToPoints(&ch_x),
					  ConvToPoints(&ch_y));
			    /* display text */
			    cairo_show_text(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
					    grd_cord_v);			    
			}
		}
	}


    if(grd_cord_v)
	free(grd_cord_v);
	
#ifdef Z_SHEET_STROKE_FUNCTION
    cairo_stroke(zGeneric_Get_Dev_Context(&obj->z_sgeneric));
#endif
	
    return i;
}

/* draw top revision box */
static int zsheet_draw_top_revbox(zSheet* obj)
{
    /* check for NULL pointer */
    if(obj == NULL)
	return 0;

    /* pointer to under lying device */
    /* zDevice* dev = zGeneric_Get_Device(&obj->z_sgeneric); */
	
    double x, y;
    if(obj->z_sgrid_flg > 0)
	{
	    x = Z_DEFAULT_GRID_SZ;
	    y = Z_DEFAULT_GRID_SZ;
	}
    else
	{
	    x = 0;
	    y = 0;
	}

    const int NUM = 7;
    double X[NUM], Y[NUM];
    int i = 0;

    X[0] = x + Z_DEFAULT_BORDER_SZ;
    Y[0] = y + Z_DEFAULT_BORDER_SZ +
	Z_WOZ_MAIN_REVBOX_HEIGHT;

    X[1] = X[0] + Z_WOZ_MAIN_REVBOX_WIDTH;
    Y[1] = Y[0];

    X[2] = X[1];
    Y[2] = y + Z_DEFAULT_BORDER_SZ;

    /* drawing number box */
    X[3] = X[0] + Z_WOZ_MAIN_DRWGBOX_WIDTH;
    Y[3] = Y[0];

    X[4] = X[3];
    Y[4] = Y[2];

    /* sheet box */
    X[5] = X[0] + Z_WOZ_MAIN_SHTBOX_WIDTH;
    Y[5] = Y[0];

    X[6] = X[5];
    Y[6] = Y[2];

    cairo_move_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
		  ConvToPoints(&X[0]),
		  ConvToPoints(&Y[0]));
    for(i=1; i<NUM; i++)
	{
	    if(i==3 || i==5)
		{
		    cairo_move_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
				  ConvToPoints(&X[i]),
				  ConvToPoints(&Y[i]));
		}
	    else
		{
		    cairo_line_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
				  ConvToPoints(&X[i]),
				  ConvToPoints(&Y[i]));
		}
	}

#ifdef Z_SHEET_STROKE_FUNCTION	
    cairo_stroke(zGeneric_Get_Dev_Context(&obj->z_sgeneric));
#endif
	
    return i;
}


/* draw decal */
static int zsheet_draw_decal(zSheet* obj)
{
    /* check for NULL pointer */
    if(obj == NULL)
	return 0;

    /* pointer to under lying device */
    zDevice* dev = zGeneric_Get_Device(&obj->z_sgeneric);
	
    /* check device context */
    if(!dev)
	return 0;

    const int NUM = 36;

    double X[NUM], Y[NUM];
    int i=0;			/* counter */

    /* outer decal box coordinates */
    X[0] = dev->z_page_width - Z_WOZ_MAIN_BRD_WIDTH
	- Z_DEFAULT_BORDER_SZ;
    Y[0] = dev->z_page_height - Z_DEFAULT_BORDER_SZ;

    X[1] = X[0];
    Y[1] = dev->z_page_height - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_BRD_HEIGHT;

    X[2] = dev->z_page_width - Z_DEFAULT_BORDER_SZ;
    Y[2] = Y[1];

    /* description box coordinates */
    X[3] = X[0];
    Y[3] = Y[1] + Z_WOZ_MAIN_DES_HEIGHT;

    X[4] = X[2];
    Y[4] = Y[3];

    /* client box coordinates */
    X[5] = X[0];
    Y[5] = Y[3] + Z_WOZ_MAIN_CLT_HEIGHT;

    X[6] = X[2];
    Y[6] = Y[5];

    /* project box coordinates */
    X[7] = X[0];
    Y[7] = Y[5] + Z_WOZ_MAIN_PROJ_HEIGHT;

    X[8] = X[2];
    Y[8] = Y[7];

    /* job  and po box */
    X[9] = X[0];
    Y[9] = Y[7] + Z_WOZ_MAIN_JOB_HEIGHT;

    X[10] = X[2];
    Y[10] = Y[9];

    /* clients vertical po divider */
    X[11] = dev->z_page_width - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_MAIN_CLIENT_LEFT;
    Y[11] = Y[7];

    X[12] = X[11];
    Y[12] = Y[9];

    /* worziar address vertical divider */
    X[13] = dev->z_page_width - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_PROJ_LEFT;
    Y[13] = Y[9];

    X[14] = X[13];
    Y[14] = dev->z_page_height - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_BRD_BOTTOM;

    /* weight and page num border */
    X[15] = X[0];
    Y[15] = Y[14];

    X[16] = X[2];
    Y[16] = Y[15];

    /* projection box h divider */
    X[17] = X[13];
    Y[17] = Y[13] + 2 * Z_WOZ_MAIN_GEN_HEIGHT;

    X[18] = X[2];
    Y[18] = Y[17];

    /* scale box h divider */
    X[19] = X[17];
    Y[19] = Y[17] + 2 * Z_WOZ_MAIN_GEN_HEIGHT;

    X[20] = X[2];
    Y[20] = Y[19];

    /* general column vertical divider */
    X[21] = dev->z_page_width - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_GEN_LEFT;
    Y[21] = Y[5];

    X[22] = X[21];
    Y[22] = Y[19];

    /* general column horizontal dividers */
    X[23] = X[21];
    Y[23] = Y[13] + Z_WOZ_MAIN_GEN_HEIGHT;

    X[24] = X[2];
    Y[24] = Y[23];

    /******************/
    X[25] = X[23];
    Y[25] = Y[17] + Z_WOZ_MAIN_GEN_HEIGHT;

    X[26] = X[2];
    Y[26] = Y[25];

    /* revision box */
    X[27] = dev->z_page_width - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_REV_LEFT;
    Y[27] = Y[19];

    X[28] = X[27];
    Y[28] = Y[14];

    /* sheet number box */
    X[29] = dev->z_page_width - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_SHT_LEFT;
    Y[29] = Y[19];

    X[30] = X[29];
    Y[30] = Y[14];

    /* sheet size box */
    X[31] = X[19];
    Y[31] = Y[19] + Z_WOZ_MAIN_GEN_HEIGHT;

    X[32] = X[31] + 2 * Z_WOZ_MAIN_GEN_HEIGHT;
    Y[32] = Y[31];

    X[33] = X[32];
    Y[33] = Y[14];

    /* page number vertical divider */
    X[34] = dev->z_page_width - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_SHTMIN_LEFT;
    Y[34] = Y[14];

    X[35] = X[34];
    Y[35] = Y[0];
    
    for(i=0; i<NUM; i++)
	{
	    switch(i)
		{
		case 0:
		case 3:
		case 5:
		case 7:
		case 9:
		case 11:
		case 13:
		case 15:
		case 17:
		case 19:
		case 21:
		case 23:
		case 25:
		case 27:
		case 29:
		case 31:
		case 34:
		    cairo_move_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
				  ConvToPoints(&X[i]),
				  ConvToPoints(&Y[i]));
		    break;
		default:
		    cairo_line_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
				  ConvToPoints(&X[i]),
				  ConvToPoints(&Y[i]));
		    break;
		}
	}

#ifdef Z_SHEET_STROKE_FUNCTION
    cairo_stroke(zGeneric_Get_Dev_Context(&obj->z_sgeneric));
#endif
	
    return i;
}

/* draw material and quantity box */
static int zsheet_draw_matbox(zSheet* obj)
{
    /* check for NULL pointer */
    if(obj == NULL)
	return 0;

    /* pointer to under lying device */
    zDevice* dev = zGeneric_Get_Device(&obj->z_sgeneric);

    const int NUM = 5;
    int i = 0;
    
    double X[NUM], Y[NUM];
    double x, y;
	
    if(obj->z_sgrid_flg > 0)
	{
	    x = Z_DEFAULT_GRID_SZ;
	    y = Z_DEFAULT_GRID_SZ;
	}
    else
	{
	    x = 0;
	    y = 0;
	}
	
    /* check for grid */
    if(zDevice_Get_PageSize(dev) == zSheetA4_Portrait)
	{

	    X[0] = dev->z_page_width - Z_DEFAULT_BORDER_SZ -
		x;

	    Y[0] = Z_DEFAULT_BORDER_SZ + y +
		2 * Z_BORDER_MAT_BOX_HEIGHT;

	    X[1] = X[0] - Z_BORDER_MAT_BOX_WIDTH;
	    Y[1] = Y[0];

	    X[2] = X[1];
	    Y[2] = Z_DEFAULT_BORDER_SZ + y;

	    X[3] = X[0];
	    Y[3] = Y[0] - Z_BORDER_MAT_BOX_HEIGHT;

	    X[4] = X[1];
	    Y[4] = Y[3];

	}
    else
	{

	    X[0] = Z_DEFAULT_BORDER_SZ + x;
	    Y[0] = dev->z_page_height -
		Z_DEFAULT_BORDER_SZ - y -
		2 * Z_BORDER_MAT_BOX_HEIGHT;

	    X[1] = X[0] + Z_BORDER_MAT_BOX_WIDTH;
	    Y[1] = Y[0];

	    X[2] = X[1];
	    Y[2] = dev->z_page_height -
		Z_DEFAULT_BORDER_SZ - y;

	    X[3] = X[0];
	    Y[3] = Y[0] + Z_BORDER_MAT_BOX_HEIGHT;

	    X[4] = X[1];
	    Y[4] = Y[3];
			
	}

	
    for(i=0; i<NUM; i++)
	{
	    if(i==0 || i==3)
		{
		    cairo_move_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
				  ConvToPoints(&X[i]),
				  ConvToPoints(&Y[i]));
		}
	    else
		{
		    cairo_line_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
				  ConvToPoints(&X[i]),
				  ConvToPoints(&Y[i]));
		}
	}

#ifdef Z_SHEET_STROKE_FUNCTION	
    cairo_stroke(zGeneric_Get_Dev_Context(&obj->z_sgeneric));
#endif
	
    return i;
}

/* add drawing header details */
static int zsheet_add_attrib_headers(zSheet* obj)
{
    /* check for NULL pointer */
    if(obj == NULL)
	return 0;

    /* pointer to under lying device */
    zDevice* dev = zGeneric_Get_Device(&obj->z_sgeneric);
	
    int attrib_cnt = 0;			/* attrib counter */
    
    PangoFontDescription* z_desp_g;	/* font description generic */
    PangoFontDescription* z_desp_s;	/* font description special */
    PangoFontDescription* z_desp_c;	/* font description customer */
    PangoFontDescription* z_desp_w;	/* wozair title font */
    PangoFontDescription* z_desp_a;	/* wozair address font */
    PangoFontDescription* z_desp_m;	/* material and qty */
	
    PangoLayout* z_pango_layout;	/* main pango layout */

    /************************************************************/
    /* create new description */
    z_desp_g = pango_font_description_new();
    z_desp_s = pango_font_description_new();
    z_desp_c = pango_font_description_new();
    z_desp_w = pango_font_description_new();
    z_desp_a = pango_font_description_new();
    z_desp_m = pango_font_description_new();

    /* set font family to default */
    pango_font_description_set_family(z_desp_g,
				      Z_GRD_FONT_STYLE);
    pango_font_description_set_family(z_desp_s,
				      Z_GRD_FONT_STYLE);
    pango_font_description_set_family(z_desp_c,
				      Z_GRD_FONT_STYLE);
    pango_font_description_set_family(z_desp_w,
				      Z_GRD_FONT_STYLE);
    pango_font_description_set_family(z_desp_a,
				      Z_GRD_FONT_STYLE);
    pango_font_description_set_family(z_desp_m,
				      Z_GRD_FONT_STYLE);
    
    /* set style normal */
    pango_font_description_set_style(z_desp_g,
				     PANGO_STYLE_NORMAL);
    pango_font_description_set_style(z_desp_s,
				     PANGO_STYLE_NORMAL);
    pango_font_description_set_style(z_desp_c,
				     PANGO_STYLE_NORMAL);
    pango_font_description_set_style(z_desp_w,
				     PANGO_STYLE_NORMAL);
    pango_font_description_set_style(z_desp_a,
				     PANGO_STYLE_NORMAL);
    pango_font_description_set_style(z_desp_m,
				     PANGO_STYLE_NORMAL);
	
    /* set font size */
    pango_font_description_set_size(z_desp_g,
				    PANGO_SCALE*Z_GRD_GENERIC_SZ);
    pango_font_description_set_size(z_desp_s,
				    PANGO_SCALE*Z_GRD_SPECIAL_SZ);
    pango_font_description_set_size(z_desp_c,
				    PANGO_SCALE*Z_GRD_CUSTOMER_SZ);
    pango_font_description_set_size(z_desp_w,
				    PANGO_SCALE*Z_GRD_WOZ_SZ);
    pango_font_description_set_size(z_desp_a,
				    PANGO_SCALE*Z_GRD_WOZ_ADRS_SZ);
    pango_font_description_set_size(z_desp_m,
				    PANGO_SCALE*Z_GRD_MAT_SZ);

    /* set font weight for special font */
    pango_font_description_set_weight(z_desp_g,
				      PANGO_WEIGHT_LIGHT);
    pango_font_description_set_weight(z_desp_s,
				      PANGO_WEIGHT_LIGHT);
    pango_font_description_set_weight(z_desp_c,
				      PANGO_WEIGHT_LIGHT);
    pango_font_description_set_weight(z_desp_w,
				      PANGO_WEIGHT_NORMAL);
    pango_font_description_set_weight(z_desp_a,
				      PANGO_WEIGHT_NORMAL);
    pango_font_description_set_weight(z_desp_m,
				      PANGO_WEIGHT_SEMIBOLD);    
    /************************************************************/

    /* create pango layout for dev context */
    z_pango_layout =
	pango_cairo_create_layout(zGeneric_Get_Dev_Context(&obj->z_sgeneric));

    /* check for NULL pointer */
    if(!z_pango_layout)
	{
	    pango_font_description_free(z_desp_g);
	    pango_font_description_free(z_desp_s);
	    pango_font_description_free(z_desp_c);
	    pango_font_description_free(z_desp_w);
	    pango_font_description_free(z_desp_a);
	    pango_font_description_free(z_desp_m);
	    printf("%s\n", Z_ERROR_PANGO_LAYOUT);
	    return 0;
	}

    const int NUM = 26;
    int i = 0;			/* counter */
    double X[NUM], Y[NUM];

    /* char buffer to hold attribute headers */
    char** buff = (char**) calloc(NUM, sizeof(char*));

    zcCopy(&buff[0], Z_BORDER_TXT_DRG_NUMBER);
    zcCopy(&buff[1], Z_BORDER_TXT_SHT_NUMBER);
    zcCopy(&buff[2], Z_BORDER_TXT_REV_NUMBER);
    zcCopy(&buff[3], Z_BORDER_TXT_DIM_SPEC);
    zcCopy(&buff[4], Z_BORDER_TXT_SCL_SPEC);
    zcCopy(&buff[5], Z_BORDER_TXT_FINISH_SPEC);
    zcCopy(&buff[6], Z_BORDER_TXT_DRG_NUMBER);
    zcCopy(&buff[7], Z_BORDER_TXT_SHT_NUMBER);
    zcCopy(&buff[8], Z_BORDER_TXT_REV_NUMBER);
    zcCopy(&buff[9], Z_BORDER_TXT_TITLE_DES);
    zcCopy(&buff[10], Z_BORDER_TXT_SCALE);
    zcCopy(&buff[11], Z_BORDER_TXT_DRAWN_DES);
    zcCopy(&buff[12], Z_BORDER_TXT_DATE_DES);
    zcCopy(&buff[13], Z_BORDER_TXT_CHKD_DES);
    zcCopy(&buff[14], Z_BORDER_TXT_DATE_DES);
    zcCopy(&buff[15], Z_BORDER_TXT_APPV_DES);
    zcCopy(&buff[16], Z_BORDER_TXT_DATE_DES);
    zcCopy(&buff[17], Z_BORDER_TXT_PROJECT_DES);
    zcCopy(&buff[18], Z_BORDER_TXT_JOB_DES);
    zcCopy(&buff[19], Z_BORDER_TXT_CLIENTORDER_DES);
    zcCopy(&buff[20], Z_BORDER_TXT_CLIENT_DES);

    char* ch = zDevice_Get_PageSizeStr(dev);
    if(ch != NULL)
	zcCopy(&buff[21], ch);
    else
	zcCopy(&buff[21], "N/A");

    zcCopy(&buff[22], Z_BORDER_TXT_WOZ_DES);	

    /* get address */
    ch = zsheet_wozair_address();
    printf("%s\n", ch);
    if(ch != NULL)
    	zcCopy(&buff[23], ch);
    else
    	zcCopy(&buff[23], "N/A");

    zcCopy(&buff[24], Z_BORDER_TXT_MAT_DES);
    zcCopy(&buff[25], Z_BORDER_TXT_QTY_DES);
    
    /***********************************************/
    double x, y, adj;
	
    if(obj->z_sgrid_flg > 0)
	{
	    x = Z_DEFAULT_GRID_SZ;
	    y = Z_DEFAULT_GRID_SZ;
	}
    else
	{
	    x = 0;
	    y = 0;
	}    

    adj = Z_WOZ_MAIN_DES_HEIGHT + Z_WOZ_MAIN_CLT_HEIGHT
	+ Z_WOZ_MAIN_PROJ_HEIGHT + Z_WOZ_MAIN_JOB_HEIGHT
	+ Z_WOZ_MAIN_PROJN_HEIGHT + 2 *
	Z_WOZ_MAIN_GEN_HEIGHT;
	
    /* coordinates */
    /* drawing number top */
    X[0] = Z_DEFAULT_BORDER_SZ + x + Z_BORDER_TXT_LEFT_ADJ;
    Y[0] = Z_DEFAULT_BORDER_SZ + y + Z_BORDER_TXT_TOP_ADJ;
    
    /* sheet number top */
    X[1] = X[0] + Z_WOZ_MAIN_DRWGBOX_WIDTH;
    Y[1] = Y[0];

    /* revision number top */
    X[2] = X[0] + Z_WOZ_MAIN_SHTBOX_WIDTH;
    Y[2] = Y[0];

    /* drawing notes, scale debur and dimensions */
    if(zDevice_Get_PageSize(dev) == zSheetA4_Portrait)
	{
	    X[3] = Z_DEFAULT_BORDER_SZ + x +
		Z_BORDER_TXT_LEFT_ADJ2;

	    Y[3] = dev->z_page_height - Z_DEFAULT_BORDER_SZ
		- x - Z_WOZ_MAIN_BRD_HEIGHT +
		Z_WOZ_MAIN_DES_HEIGHT +
		Z_WOZ_MAIN_CLT_HEIGHT +
		Z_WOZ_MAIN_PROJ_HEIGHT;

	    X[4] = X[3];
	    Y[4] = dev->z_page_height - Z_DEFAULT_BORDER_SZ
		- x - Z_BORDER_ATTRIB_SCALE_DIST_A4_PORT;

	    X[5] = X[3];
	    Y[5] = dev->z_page_height - Z_DEFAULT_BORDER_SZ
		- x - Z_BORDER_ATTRIB_BUR_DIST_A4_PORT;
	    
	    /* material and qty */
	    X[24] = dev->z_page_width - Z_DEFAULT_BORDER_SZ
		- x - Z_BORDER_MAT_BOX_WIDTH +
		Z_BORDER_TXT_LEFT_ADJ;
	    Y[24] = Z_DEFAULT_BORDER_SZ + y + Z_BORDER_TXT_TOP_ADJ1;

	    X[25] = X[24];
	    Y[25] = Y[24] + Z_BORDER_MAT_BOX_HEIGHT;

	}
    else
	{
	    X[3] = X[0] + Z_BORDER_ATTRIB_MM_DIST;
	    Y[3] = Y[0];

	    X[4] = X[0] + Z_BORDER_ATTRIB_SCALE_DIST;
	    Y[4] = Y[0];

	    X[5] = X[0] + Z_BORDER_ATTRIB_BUR_DIST;
	    Y[5] = Y[0];

	    /* material and qty */
	    X[24] = X[0];
	    Y[24] = dev->z_page_height - Z_DEFAULT_BORDER_SZ
		- y - 2 * Z_BORDER_MAT_BOX_HEIGHT +
		Z_BORDER_TXT_CLEAR;

	    X[25] = X[24];
	    Y[25] = Y[24] + Z_BORDER_MAT_BOX_HEIGHT;
	    /* - Z_BORDER_TXT_TOP_ADJ1; */
	}

    /* drawing number decal */
    X[6] = dev->z_page_width - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_PROJ_LEFT +
	Z_BORDER_TXT_LEFT_ADJ;
    Y[6] = dev->z_page_height - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_BRD_HEIGHT +
	adj + Z_BORDER_TXT_TOP_ADJ;

    /* sheet number */
    X[7] = dev->z_page_width - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_SHT_LEFT + 
	Z_BORDER_TXT_LEFT_ADJ;
    Y[7] = Y[6];

    /* revision number */
    X[8] = dev->z_page_width - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_REV_LEFT +
	Z_BORDER_TXT_LEFT_ADJ;
    Y[8] = Y[6];

    /* title description */
    X[9] = dev->z_page_width - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_BRD_WIDTH +
	Z_BORDER_TXT_LEFT_ADJ;
    Y[9] = dev->z_page_height - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_BRD_HEIGHT + Z_BORDER_TXT_TOP_ADJ;

    /* scale */
    X[10] = dev->z_page_width - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_PROJ_LEFT + Z_BORDER_TXT_LEFT_ADJ;
    Y[10] = dev->z_page_height - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_BRD_HEIGHT + adj - 2 *
	Z_WOZ_MAIN_GEN_HEIGHT + Z_BORDER_TXT_TOP_ADJ;

    /* drawn */
    X[11] = dev->z_page_width - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_GEN_LEFT + Z_BORDER_TXT_LEFT_ADJ;
    Y[11] = dev->z_page_height - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_BRD_HEIGHT + Z_WOZ_MAIN_DES_HEIGHT
	+ Z_WOZ_MAIN_CLT_HEIGHT + Z_BORDER_TXT_TOP_ADJ4;

    /* drawn date */
    X[12] = X[11];
    Y[12] = Y[11] + Z_WOZ_MAIN_CLT_HEIGHT - Z_BORDER_TXT_TOP_ADJ4;

    /* checked */
    X[13] = X[11];
    Y[13] = Y[12] + Z_WOZ_MAIN_GEN_HEIGHT;

    /* checked date */
    X[14] = X[11];
    Y[14] = Y[13] + Z_WOZ_MAIN_GEN_HEIGHT;

    /* approved */
    X[15] = X[11];
    Y[15] = Y[14] + Z_WOZ_MAIN_GEN_HEIGHT;


    /* approved date */
    X[16] = X[11];
    Y[16] = Y[15] + Z_WOZ_MAIN_GEN_HEIGHT;


    /* project description */
    X[17] = dev->z_page_width - Z_DEFAULT_BORDER_SZ -
	Z_WOZ_MAIN_BRD_WIDTH + Z_BORDER_TXT_LEFT_ADJ;
    Y[17] = dev->z_page_height - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_BRD_HEIGHT +
	Z_WOZ_MAIN_DES_HEIGHT + Z_WOZ_MAIN_CLT_HEIGHT
	+ Z_BORDER_TXT_TOP_ADJ - Z_BORDER_TXT_TOP_ADJ4;

    /* job number */
    X[18] = X[17];
    Y[18] = Y[17] + Z_WOZ_MAIN_PROJ_HEIGHT +
	Z_BORDER_TXT_TOP_ADJ4;

    /* client's p.o */
    X[19] = dev->z_page_width - Z_DEFAULT_BORDER_SZ -
	Z_WOZ_MAIN_MAIN_CLIENT_LEFT +
	Z_BORDER_TXT_LEFT_ADJ;
    Y[19] = Y[18];

    /* client name */
    X[20] = X[17];
    Y[20] = dev->z_page_height - Z_DEFAULT_BORDER_SZ -
	Z_WOZ_MAIN_BRD_HEIGHT + Z_WOZ_MAIN_DES_HEIGHT
	+ Z_BORDER_TXT_TOP_ADJ - Z_BORDER_TXT_TOP_ADJ3;

    /* page size string */
    X[21] = dev->z_page_width - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_PROJ_LEFT + Z_BORDER_TXT_LEFT_ADJ;
    Y[21] = dev->z_page_height - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_BRD_BOTTOM - Z_WOZ_MAIN_GEN_HEIGHT
	+ Z_BORDER_TXT_TOP_ADJ3;

    /* wozair */
    X[22] = X[20];
    Y[22] = dev->z_page_height - Z_DEFAULT_BORDER_SZ
	- x - Z_WOZ_MAIN_BRD_HEIGHT +
	Z_WOZ_MAIN_DES_HEIGHT +
	Z_WOZ_MAIN_CLT_HEIGHT + Z_WOZ_MAIN_PROJ_HEIGHT
	+ Z_WOZ_MAIN_JOB_HEIGHT + Z_BORDER_TXT_TOP_ADJ1
	+ Z_BORDER_TXT_TOP_ADJ4;

    /* address */
    X[23] = X[22];
    Y[23] = Y[22] + Z_BORDER_TXT_TOP_ADJ2 +
	Z_BORDER_TXT_TOP_ADJ3;

    for(i=0; i<NUM; i++)
	{
	    if(zDevice_Get_PageSize(dev) == zSheetA4_Portrait
	       && i>2 && i<6)
		{

		    cairo_save(zGeneric_Get_Dev_Context(&obj->z_sgeneric));

		    cairo_translate(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
				    ConvToPoints(&X[i]),
				    ConvToPoints(&Y[i]));
		    cairo_rotate(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
				 -90 * G_PI / 180);

		    cairo_translate(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
				    -1 * ConvToPoints(&X[i]),
				    -1 * ConvToPoints(&Y[i]));
		}
	    
	    if(i>23)
		{
		    pango_layout_set_font_description(z_pango_layout,
						      z_desp_m);
		}
	    else if(i>22 && i<24)
		{
		    pango_layout_set_font_description(z_pango_layout,
						      z_desp_a);
		}
	    else if(i>21 && i<23)
		{
		    pango_layout_set_font_description(z_pango_layout,
						      z_desp_w);
		}
	    else if(i>19 && i<21)
		{
		    pango_layout_set_font_description(z_pango_layout,
						      z_desp_c);
		}			
	    else if((i>10 && i<20) || (i>20 && i<22))
		{
		    pango_layout_set_font_description(z_pango_layout,
						      z_desp_s);
		}
	    else
		{
		    pango_layout_set_font_description(z_pango_layout,
						      z_desp_g);
		}
					
	    pango_layout_set_text(z_pango_layout, buff[i], -1);
	    cairo_move_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
			  ConvToPoints(&X[i]), ConvToPoints(&Y[i]));
    
	    pango_cairo_update_layout(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
				      z_pango_layout);
	    pango_cairo_show_layout(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
				    z_pango_layout);

	    if(zDevice_Get_PageSize(dev) == zSheetA4_Portrait
	       && i>2 && i<6)
		{
		    cairo_restore(zGeneric_Get_Dev_Context(&obj->z_sgeneric));
		}

	    /* set drawing attribute coordinates */
	    switch(i)
		{
		case 3:
		case 4:
		case 5:
		    break;
		case 9:
		    obj->z_x_attrib[attrib_cnt] = X[i];
		    obj->z_y_attrib[attrib_cnt] = Y[i] +
			Z_BORDER_TXT_TOP_ADJ1;
		    attrib_cnt++;
		    break;
		case 17:
		case 20:
		    obj->z_x_attrib[attrib_cnt] = X[i] +
			Z_BORDER_TXT_LEFT_ADJ4;
		    obj->z_y_attrib[attrib_cnt] = Y[i];
		    attrib_cnt++;					
		    break;
		case 21:
		case 22:
		case 23:
		    break;
		default:
		    obj->z_x_attrib[attrib_cnt] = X[i];
		    obj->z_y_attrib[attrib_cnt] = Y[i];
		    attrib_cnt++;
		    break;
		}
	}

    /* cairo_restore(obj->z_sgeneric->z_gcairo_dev); */
	
    /* free description */
    pango_font_description_free(z_desp_g);
    pango_font_description_free(z_desp_s);
    pango_font_description_free(z_desp_c);
    pango_font_description_free(z_desp_w);
    pango_font_description_free(z_desp_a);
    pango_font_description_free(z_desp_m);
	
    g_object_unref(z_pango_layout);
    for(i = 0; i < NUM; i++)
	{
	    if(buff[i])
		free(buff[i]);
	    buff[i] = NULL;
	}
    if(buff)
	free(buff);
    
    return 1;
}

/* draw projection */
static int zsheet_draw_projn(zSheet* obj)
{
    /* check for NULL pointer */
    if(obj == NULL)
	return 0;

    /* pointer to under lying device */
    zDevice* dev = zGeneric_Get_Device(&obj->z_sgeneric);
	
    const int NUM = 10;
    int i = 0;
    double X[NUM], Y[NUM];
    double rad1, rad2;

    double dash_on = Z_BORDER_PROJN_DASH_ON;
    double dash_off = Z_BORDER_PROJN_DASH_OFF;

    const int NUM_DASHES = 3;
    double dashes[] = {ConvToPoints(&dash_on),
		       ConvToPoints(&dash_off),
		       ConvToPoints(&dash_on)};
	
    rad1 = Z_BORDER_PROJN_DIA_MAJ / 2;
    rad2 = Z_BORDER_PROJN_DIA_MIN / 2;
	
    X[0] = dev->z_page_width - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_PROJ_LEFT + Z_BORDER_PROJN_DIA_MAJ;

    Y[0] = dev->z_page_height - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_BRD_HEIGHT + Z_WOZ_MAIN_DES_HEIGHT
	+ Z_WOZ_MAIN_CLT_HEIGHT + Z_WOZ_MAIN_PROJ_HEIGHT
	+ Z_WOZ_MAIN_JOB_HEIGHT + Z_WOZ_MAIN_GEN_HEIGHT;

    X[1] = X[0] + Z_BORDER_PROJN_DIA_MAJ +
	Z_BORDER_PROJN_DIA_MIN / 2;
    Y[1] = Y[0] - Z_BORDER_PROJN_DIA_MIN / 2;

    X[2] = X[1];
    Y[2] = Y[1] + Z_BORDER_PROJN_DIA_MIN;

    X[3] = X[2] + Z_BORDER_PROJN_CYL_DEPTH;
    Y[3] = Y[0] + Z_BORDER_PROJN_DIA_MAJ / 2;

    X[4] = X[3];
    Y[4] = Y[0] - Z_BORDER_PROJN_DIA_MAJ / 2;

    X[5] = X[1];
    Y[5] = Y[1];

    X[6] = X[0];
    Y[6] = dev->z_page_height - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_BRD_HEIGHT + Z_WOZ_MAIN_DES_HEIGHT
	+ Z_WOZ_MAIN_CLT_HEIGHT + Z_WOZ_MAIN_PROJ_HEIGHT
	+ Z_WOZ_MAIN_JOB_HEIGHT + Z_BORDER_PROJN_CLINE_VOFF;

    X[7] = X[0];
    Y[7] = Y[6] + 2 * Z_WOZ_MAIN_GEN_HEIGHT -
	2 * Z_BORDER_PROJN_CLINE_VOFF;

    X[8] = dev->z_page_width - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_PROJ_LEFT + Z_BORDER_PROJN_CLINE_VOFF;
    Y[8] = Y[0];

    X[9] = X[8] + 2 * Z_BORDER_PROJN_CLINE_VOFF +
	2 * Z_BORDER_PROJN_DIA_MAJ + Z_BORDER_PROJN_DIA_MIN / 2
	+ Z_BORDER_PROJN_CYL_DEPTH;
    Y[9] = Y[8];

    /* save cairo context */
    cairo_save(zGeneric_Get_Dev_Context(&obj->z_sgeneric));
	
    for(i=0; i<NUM; i++)
	{
	    switch(i)
		{

		case 0:
		    cairo_new_sub_path(zGeneric_Get_Dev_Context(&obj->z_sgeneric));
					
		    cairo_arc(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
			      ConvToPoints(&X[i]),
			      ConvToPoints(&Y[i]),
			      ConvToPoints(&rad1),
			      0,
			      M_PI*180);

		    cairo_new_sub_path(zGeneric_Get_Dev_Context(&obj->z_sgeneric));
					
		    cairo_arc(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
			      ConvToPoints(&X[i]),
			      ConvToPoints(&Y[i]),
			      ConvToPoints(&rad2),
			      0,
			      M_PI*180);
					
		    break;
		case 1:
		case 6:
		case 8:
										
		    if(i==6)
			{
			    cairo_set_dash(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
					   dashes, NUM_DASHES, 0);
			}
					
		    cairo_move_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
				  ConvToPoints(&X[i]),
				  ConvToPoints(&Y[i]));
		    break;
		case 5:
		    cairo_close_path(zGeneric_Get_Dev_Context(&obj->z_sgeneric));
		    cairo_stroke(zGeneric_Get_Dev_Context(&obj->z_sgeneric));
		    break;
		default:
		    cairo_line_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
				  ConvToPoints(&X[i]),
				  ConvToPoints(&Y[i]));
		    break;
					
		}
	}

    cairo_stroke(zGeneric_Get_Dev_Context(&obj->z_sgeneric));
    /* disable dashes */
    cairo_set_dash(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
		   dashes,
		   0,
		   0);

    cairo_restore(zGeneric_Get_Dev_Context(&obj->z_sgeneric));
    return i;
}

/* add logo */
static int zsheet_paint_logo(zSheet* obj)
{
    /* check for NULL pointer */
    if(obj == NULL)
	return 0;

    /* pointer to under lying device */
    zDevice* dev = zGeneric_Get_Device(&obj->z_sgeneric);

    /* check for logo path */
    if(obj->z_slogo_path == NULL)
	return 0;

    /* create logo context */
    cairo_surface_t* logo_surf = NULL;
    double x, y;
    x = dev->z_page_width - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_PROJ_LEFT - Z_WOZ_MAIN_LOGO_WIDTH;
    y = dev->z_page_height - Z_DEFAULT_BORDER_SZ
	- Z_WOZ_MAIN_LOGO_HEIGHT;
	
    logo_surf =
	cairo_image_surface_create_from_png(obj->z_slogo_path);

    /* check for NULL pointer */
    if(!logo_surf)
	{
	    printf("%s\n", Z_ERROR_LOGO_FAILED);
	    return 0;
	}

    /* set surface */
    cairo_set_source_surface(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
    			     logo_surf,
    			     ConvToPoints(&x),
    			     ConvToPoints(&y));

    cairo_paint(zGeneric_Get_Dev_Context(&obj->z_sgeneric));

    cairo_surface_destroy(logo_surf);


    return 1;

}
	
/* constructs wozair address from
   constants */
static char* zsheet_wozair_address()
{
    const int NUM = 9;
    char* buff;

    /* array to hold address */
    char** arr_buff = (char**)
	calloc(NUM, sizeof(char*));

    int i = strlen(Z_BORDER_TXT_WOZ_COPY) + 1;

    char* ch1 = (char*) malloc(sizeof(char) * i);

    strcpy(ch1, Z_BORDER_TXT_WOZ_COPY);
    
    zcCopy(&arr_buff[0], Z_BORDER_TXT_WOZ_ADRS1);
    zcCopy(&arr_buff[1], Z_BORDER_TXT_WOZ_ADRS2);
    zcCopy(&arr_buff[2], Z_BORDER_TXT_WOZ_ADRS3);
    zcCopy(&arr_buff[3], Z_BORDER_TXT_WOZ_ADRS4);
    zcCopy(&arr_buff[4], Z_BORDER_TXT_WOZ_ADRS5);
    zcCopy(&arr_buff[5], Z_BORDER_TXT_WOZ_ADRS6);
    zcCopy(&arr_buff[6], Z_BORDER_TXT_WOZ_ADRS7);
    zcCopy(&arr_buff[7], Z_BORDER_TXT_WOZ_EML);
    zcCopy(&arr_buff[8], ch1);
    /* obtain total length of buffer */
    int len = zBuffLen(NUM, arr_buff);

    if(!len)
	{
	    free(arr_buff);
	    return NULL;
	}

    buff = (char*) malloc(sizeof(char) * (len+1));

    /* copy array to buffer */
    zCopyToBuff(&buff, arr_buff, NUM);
    if(ch1)
	free(ch1);
    ch1 = NULL;

    return buff;
    
}

/* add drawing attributes */
static int zsheet_add_attribs(zSheet* obj)
{
    /* check for NULL pointer */
    if(obj == NULL)
	return 0;

    /* pointer to under lying device */
    /* zDevice* dev = zGeneric_Get_Device(&obj->z_sgeneric); */

    /* check for attribute null pointer */
    /* if(obj->z_sbrd_attrib == NULL || */
    /*    *obj->z_sbrd_attrib == NULL) */
    /* 	{ */
    /* 	    return 0; */
    /* 	} */

    /* obtain pointer to attribute struct */
    zBrd_Attrib* tmp_attrib = obj->z_sbrd_attrib;

    PangoFontDescription* z_desp_g;	/* font description generic */
    PangoFontDescription* z_desp_s;	/* font description special */
    PangoFontDescription* z_desp_t;	/* font description title */

    PangoLayout* z_pango_layout;	/* main pango layout */
    
    /* create new description */
    z_desp_g = pango_font_description_new();
    z_desp_s = pango_font_description_new();
    z_desp_t = pango_font_description_new();

    /* set font family to default */
    pango_font_description_set_family(z_desp_g,
				      Z_GRD_FONT_STYLE);
    pango_font_description_set_family(z_desp_s,
				      Z_GRD_FONT_STYLE);
    pango_font_description_set_family(z_desp_t,
				      Z_GRD_FONT_STYLE);
	
    /* set style normal */
    pango_font_description_set_style(z_desp_g,
				     PANGO_STYLE_NORMAL);
    pango_font_description_set_style(z_desp_s,
				     PANGO_STYLE_NORMAL);
    pango_font_description_set_style(z_desp_t,
				     PANGO_STYLE_NORMAL);

    /* set font size */
    pango_font_description_set_size(z_desp_g,
				    PANGO_SCALE*Z_GRD_FONT_SZ);
    pango_font_description_set_size(z_desp_s,
				    PANGO_SCALE*Z_GRD_SPECIAL2_SZ);
    pango_font_description_set_size(z_desp_t,
				    PANGO_SCALE*Z_GRD_TITLE_SZ);

    /* set font weight for special font */
    pango_font_description_set_weight(z_desp_g,
				      PANGO_WEIGHT_LIGHT);
    pango_font_description_set_weight(z_desp_s,
				      PANGO_WEIGHT_LIGHT);
    pango_font_description_set_weight(z_desp_t,
				      PANGO_WEIGHT_LIGHT);

    /* create pango layout for dev context */
    z_pango_layout =
	pango_cairo_create_layout(zGeneric_Get_Dev_Context(&obj->z_sgeneric));

    /* check for NULL pointer */
    if(!z_pango_layout)
	{
	    pango_font_description_free(z_desp_g);
	    pango_font_description_free(z_desp_s);
	    pango_font_description_free(z_desp_t);
	    printf("%s\n", Z_ERROR_PANGO_LAYOUT);
	    return 0;
	}

    const int ATTRIB_ARR = 17;
    /* temporary buffer to assign
       attributes */
    char** buff = (char**) calloc(ATTRIB_ARR,
				  sizeof(char*));

    int i = Z_MAX_ATTRIB;
    int MATCH_ATTRIB[Z_MAX_ATTRIB];
    
    /* drawing number */
    if(tmp_attrib->z_dwg_num != NULL)
	buff[0] = tmp_attrib->z_dwg_num;
    else
	buff[0] = NULL;
    
    MATCH_ATTRIB[0] = 0;
    MATCH_ATTRIB[3] = 0;
    /**************************************/
    /* sheet number */
    if(tmp_attrib->z_sht_num != NULL)
	buff[1] = tmp_attrib->z_sht_num;
    else
	buff[1] = NULL;
    
    MATCH_ATTRIB[1] = 1;
    MATCH_ATTRIB[4] = 1;
    /**************************************/
    /* revision number */
    if(tmp_attrib->z_rev_num != NULL)
	buff[2] = tmp_attrib->z_rev_num;
    else
	buff[2] = NULL;

    MATCH_ATTRIB[2] = 2;
    MATCH_ATTRIB[5] = 2;
    /**************************************/
    /* scale */
    if(tmp_attrib->z_scale != NULL)
	buff[3] = tmp_attrib->z_scale;
    else
	buff[3] = NULL;
    
    MATCH_ATTRIB[7] = 3;
    /**************************************/
    /* drawing title */
    if(tmp_attrib->z_title != NULL)
	buff[4] = tmp_attrib->z_title;
    else
	buff[4] = NULL;

    MATCH_ATTRIB[6] = 4;
    /**************************************/
    /* client */
    if(tmp_attrib->z_client != NULL)
	buff[5] = tmp_attrib->z_client;
    else
	buff[5] = NULL;

    MATCH_ATTRIB[17] = 5;
    /**************************************/
    /* site */
    if(tmp_attrib->z_site != NULL)

	buff[6] = tmp_attrib->z_site;
    else
	buff[6] = NULL;

    MATCH_ATTRIB[14] = 6;
    /**************************************/
    /* job number */
    if(tmp_attrib->z_jobnum != NULL)
	buff[7] = tmp_attrib->z_jobnum;
    else
	buff[7] = NULL;

    MATCH_ATTRIB[15] = 7;
    /**************************************/
    /* order number */
    if(tmp_attrib->z_ordnum != NULL)
	buff[8] = tmp_attrib->z_ordnum;
    else
	buff[8] = NULL;

    MATCH_ATTRIB[16] = 8;
    /**************************************/
    /* drawn */
    if(tmp_attrib->z_drawn != NULL)
	buff[9] = tmp_attrib->z_drawn;
    else
	buff[9] = NULL;

    MATCH_ATTRIB[8] = 9;
    /**************************************/
  
    /* drawn date */
    if(tmp_attrib->z_drawn_date != NULL)
	buff[10] = tmp_attrib->z_drawn_date;
    else
	buff[10] = NULL;

    MATCH_ATTRIB[9] = 10;
    /**************************************/
    /* checked by */
    if(tmp_attrib->z_chked != NULL)
	buff[11] = tmp_attrib->z_chked;
    else
	buff[11] = NULL;

    MATCH_ATTRIB[10] = 11;
    /**************************************/
    /* checked date */
    if(tmp_attrib->z_chk_date != NULL)
	buff[12] = tmp_attrib->z_chk_date;
    else
	buff[12] = NULL;

    MATCH_ATTRIB[11] = 12;
    /**************************************/
    /* approved by */
    if(tmp_attrib->z_app != NULL)
	buff[13] = tmp_attrib->z_app;
    else
	buff[13] = NULL;

    MATCH_ATTRIB[12] = 13;
    /**************************************/
    /* approved date */
    if(tmp_attrib->z_app_date != NULL)
	buff[14] = tmp_attrib->z_app_date;
    else
	buff[14] = NULL;

    MATCH_ATTRIB[13] = 14;
    /**************************************/
    /* materials */
    if(tmp_attrib->z_material != NULL)
	buff[15] = tmp_attrib->z_material;
    else
	buff[15] = NULL;

    MATCH_ATTRIB[18] = 15;
    /**************************************/
    /* quantity */
    if(tmp_attrib->z_qty != NULL)
	buff[16] = tmp_attrib->z_qty;
    else
	buff[16] = NULL;

    MATCH_ATTRIB[19] = 16;
    /**************************************/
    for(i=0; i<Z_MAX_ATTRIB; i++)
	{

	    if(buff[MATCH_ATTRIB[i]] != NULL)
		{

		    pango_layout_set_text(z_pango_layout,
					  buff[MATCH_ATTRIB[i]],
					  -1);

		    switch(i)
			{
			case 6:
			    /* title */
			    pango_layout_set_font_description(z_pango_layout,
							      z_desp_t);
			    break;
			default:
			    pango_layout_set_font_description(z_pango_layout,
							      z_desp_g);
			    break;
			}


		    cairo_move_to(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
				  ConvToPoints(&obj->z_x_attrib[i]),
				  ConvToPoints(&obj->z_y_attrib[i]));

		    pango_cairo_update_layout(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
					      z_pango_layout);

		    pango_cairo_show_layout(zGeneric_Get_Dev_Context(&obj->z_sgeneric),
					    z_pango_layout);
		}
	    
	}

    pango_font_description_free(z_desp_g);
    pango_font_description_free(z_desp_s);
    pango_font_description_free(z_desp_t);

    g_object_unref(z_pango_layout);

    if(buff)
	free(buff);

    return i;
}

/*=====================================================================*/
/* Virtual function implementation */
static int _zsheet_draw_function(zGeneric* obj)
{
    return zSheet_Create_Border(Z_SHEET(obj));
}
