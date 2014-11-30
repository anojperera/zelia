/* implementation of file handling class */
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libxml/tree.h>

#include "zVar.h"
#include "zfile.h"

static int _zfile_parse_xml(zfile* obj);
static int _zfile_callback_hook(void* obj);
/* constructor */
zfile* zfile_new(zfile* obj)
{
    if(obj == NULL)
	{
	    obj = (zfile*) malloc(sizeof(zfile));
	    obj->_init_flg = ZELIA_CONSTRUCTED;
	    ZELIA_LOG_MESSAGE("zfile object created");
	}
    else
	{
	    obj->_init_flg = ZELIA_INITIALISED;
	    ZELIA_LOG_MESSAGE("zfile object is initialised");
	}

    obj->force_flg = ZELIA_FILE_FORCE_DEFAULT;

    obj->tmp_file = NULL;
    memset((void*) obj->new_file_path, 0, Z_FILE_PATH_SZ);
    memset((void*) obj->new_file_name, 0, Z_FILE_NAME_SZ);

    obj->read_buff = NULL;
    obj->read_buff_sz = 0;

    obj->callback_hook = _zfile_callback_hook;

    /* interanl string struct pointers for easy handling */
    obj->_string.buff = obj->read_buff;
    obj->_string.buff_sz_ptr = &obj->read_buff_sz;

    obj->xml_doc = NULL;
    obj->ext_ptr = NULL;
    obj->child = NULL;

    return obj;
}

/* destructor */
void zfile_delete(zfile* obj)
{
    if(obj == NULL)
	return;

    obj->tmp_file = NULL;

    if(obj->read_buff != NULL && obj->read_buff_sz > 0)
	free(obj->read_buff);

    obj->read_buff = NULL;
    obj->read_buff_sz = 0;

    obj->callback_hook = NULL;    

    /* set struct to NULL */
    obj->_string.buff = NULL;
    obj->_string.buff_sz_ptr = NULL;

    if(obj->xml_doc)
	{
	    xmlFreeDoc(obj->xml_doc);
	    xmlCleanupParser();
	}
    
    obj->child = NULL;
    obj->ext_ptr = NULL;
    
    /* if created destroy it */
    if(obj->_init_flg == ZELIA_CONSTRUCTED)
	free(obj);

    ZELIA_LOG_MESSAGE("zfile object deleted successfully");
    return;
}

/* create a file from a template */
int zfile_create_file_from_template(zfile* obj, const char* tmp_file, const char* file_path)
{
    int r_fd = 0, w_fd = 0;								/* read and write file descriptors */
    struct stat tmp_stat;								/* struct for holding the template file stat */
    mode_t prev_mode, current_mode;							/* previous file mode */
    
    if(obj == NULL || tmp_file == NULL || file_path == NULL)
	return ZELIA_NULL;

    /* set the template file path */
    obj->tmp_file = tmp_file;

    ZELIA_LOG_MESSAGE("zfile object checking if file exists");
    /*
     * Copy the file path to internal if the file does not exists.
     * If it exists, we check for force flag and copy to internal.
     */
    if(access(file_path, F_OK) || (!access(file_path, W_OK) && obj->force_flg == ZELIA_FORCE_OVERWRITE))
	{
	    /* file doesn't exist, therefore we copy to internal */
	    strncpy(obj->new_file_path, file_path, Z_FILE_PATH_SZ-1);
	    obj->new_file_path[Z_FILE_PATH_SZ-1] = '\0';
	    ZELIA_LOG_MESSAGE("zfile file path copied to internal successfully");
	}
    else
	{
	    ZELIA_LOG_MESSAGE("zfile file path copy error, bailing out");
	    return ZELIA_FILE_COPY_ERROR;
	}

    /* check to see if the temporary file exist */
    ZELIA_LOG_MESSAGE("zfile, checking if template file exists");
    if(access(obj->tmp_file, F_OK))
	{
	    ZELIA_LOG_MESSAGE("zfile template does not exist, bailing out");
	    return ZELIA_FILE_COPY_ERROR;	    
	}

    /* reading file and copy to new location */
    ZELIA_LOG_MESSAGE("zfile opening template");

    r_fd = open(obj->tmp_file, O_RDONLY);
    if(r_fd == -1)
	{
	    ZELIA_LOG_MESSAGE("zfile errors occured while reading the template");

	    return ZELIA_FILE_COPY_ERROR;	    
	}

    /* set the umask mode for the file creation */
    current_mode = S_IXOTH | S_IWOTH | S_IWGRP | S_IXGRP | S_IXUSR;
    prev_mode = umask(current_mode);
    w_fd = open(obj->new_file_path, O_RDWR | O_CREAT);
    
    /* once open reset the file mask */    
    umask(prev_mode);
    
    if(w_fd == -1)
	{
	    /* write file descriptor failed, so we bail */
	    ZELIA_LOG_MESSAGE("zfile write file descriptor failed");
	    ZELIA_LOG_MESSAGE("zfile closing template file");
	    
	    /* close template file */
	    close(r_fd);

	    return ZELIA_FILE_COPY_ERROR;
	}

    fchmod(w_fd, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    
    /* check stat for file size */
    if(fstat(r_fd, &tmp_stat))
	{
	    ZELIA_LOG_MESSAGE("zfile unable to get tempalate file size");
	    ZELIA_LOG_MESSAGE("zfile unable closing template file and write file");
	    
	    close(r_fd);
	    close(w_fd);

	    /* delete the new file */
	    unlink(obj->new_file_path);
	    ZELIA_LOG_MESSAGE("zfile created file deleted due to errors");
	    return ZELIA_FILE_COPY_ERROR;
	}


    /* read contents of the the tempalate file */
    /* create buffer */
    obj->read_buff_sz = tmp_stat.st_size + 1;    
    obj->read_buff = (char*) malloc((size_t) obj->read_buff_sz);

    if(!read(r_fd, (void*) obj->read_buff, tmp_stat.st_size))
	{
	    ZELIA_LOG_MESSAGE("zfile template file read error, bailing out");

	    close(r_fd);
	    close(w_fd);

	    unlink(obj->new_file_path);
	    ZELIA_LOG_MESSAGE("zfile created file deleted due to errors");
	    
	    return ZELIA_FILE_COPY_ERROR;
	}

    /* write to disc */
    if(!write(w_fd, obj->read_buff, tmp_stat.st_size))
	{
	    ZELIA_LOG_MESSAGE("zelia copying failed");

	    close(r_fd);
	    close(w_fd);

	    unlink(obj->new_file_path);
	    ZELIA_LOG_MESSAGE("zfile created file deleted due to errors");
	    
	    return ZELIA_FILE_COPY_ERROR;	    
	}

    fsync(w_fd);

    /* close all file descriptors and exit */
    close(r_fd);
    close(w_fd);

    /* call to parse the xml document */
    _zfile_parse_xml(obj);    

    return ZELIA_OK;
}

int zfile_update_file(zfile* obj)
{
    if(obj == NULL)
	return ZELIA_NULL;

    if(obj->xml_doc == NULL)
	return ZELIA_NULL;

    
    /* check if the file exists */
    if(access(obj->new_file_path, F_OK))
	{
	    ZELIA_LOG_MESSAGE("file does not exist");
	    return ZELIA_NULL;
	}

    if(xmlSaveFile(obj->new_file_path, obj->xml_doc) != -1)
	{
	    ZELIA_LOG_MESSAGE("file written successfully");
	    /* update cache here */
	}

    return ZELIA_OK;
}

int zfile_update_cache(zfile* obj)
{
    struct stat _stat;
    int r_fd = 0;
    char* read_buff = NULL, *old_buff = NULL;
    ssize_t read_buff_sz = 0;
    
    if(obj == NULL)
	return ZELIA_NULL;

    /* check if file paths are set */
    if(obj->new_file_path[0] == 0)
	{
	    ZELIA_LOG_MESSAGE("zfile cache update error, file path was not set");
	    return ZELIA_CACHE_ERROR;
	}

    /* check if file exists */
    if(access(obj->new_file_path, F_OK))
	{
	    ZELIA_LOG_MESSAGE("zfile file not found");
	    return ZELIA_CACHE_ERROR;
	}

    /* open file and read the contents */
    r_fd = open(obj->new_file_path, O_RDONLY);
    if(r_fd == -1)
	{
	    ZELIA_LOG_MESSAGE("zfile unable to open the file");
	    return ZELIA_CACHE_ERROR;
	}

    /* get stat */
    if(fstat(r_fd, &_stat))
	{
	    ZELIA_LOG_MESSAGE("zfile unable to get the stats for the file");
	    ZELIA_LOG_MESSAGE("zfile closing file descriptor");
	    return ZELIA_CACHE_ERROR;
	}

    read_buff_sz = _stat.st_size+1;
    read_buff = (char*) malloc(read_buff_sz);

    if(!read(r_fd, read_buff, _stat.st_size))
	{
	    ZELIA_LOG_MESSAGE("unable to read the file, bailing out");
	    ZELIA_LOG_MESSAGE("zfile closing file descriptor");

	    return ZELIA_CACHE_ERROR;
	}

    /* destroy the old buffer and use the new one */
    old_buff = obj->read_buff;

    obj->read_buff = read_buff;
    obj->read_buff_sz = (size_t) read_buff_sz;

    /* remove old buffer */
    free(old_buff);

    return ZELIA_OK;
}

/* parse the xml document */
static int _zfile_parse_xml(zfile* obj)
{

    if(obj->read_buff == NULL || obj->read_buff_sz <= 0)
	return ZELIA_NULL;
    
    if(!obj->xml_doc)
	xmlInitParser();
    else
	{
	    ZELIA_LOG_MESSAGE("zfile xml_doc already created, freeing before using");
	    xmlFreeDoc(obj->xml_doc);
	}

    ZELIA_LOG_MESSAGE("parsing memory");
    obj->xml_doc = xmlParseMemory(obj->read_buff, obj->read_buff_sz);
    if(obj->xml_doc)
	ZELIA_LOG_MESSAGE("xml parsed successfully");
    else
	ZELIA_LOG_MESSAGE("xml parsing failed, document pointer set to NULL");
    
    return ZELIA_OK;
}

/* callback hook implementation */
static int _zfile_callback_hook(void* obj)
{
    zfile* _obj;
    
    if(obj == NULL)
	return ZELIA_NULL;

    /* cast object */
    _obj = (zfile*) obj;
    return zfile_update_file(_obj);
}
