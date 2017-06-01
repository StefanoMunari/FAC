/**
 * @brief UNIX utility to parse files and dirs
 * @file
 * @author mirko.bez@studenti.unipd.it
 * @author stefano.munari.1@studenti.unipd.it
 */

#ifndef __UNIX_FS_H__
#define __UNIX_FS_H__
/**
 * Retrieves the root dir of the project from the given path
 * @param cur_dir - the current directory
 * @return the project's root directory
 */
char * get_root_dir(char * cur_dir);

#endif /*__UNIX_FS_H__*/

