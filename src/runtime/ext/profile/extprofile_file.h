/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010- Facebook, Inc. (http://www.facebook.com)         |
   | Copyright (c) 1997-2010 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#ifndef __EXTPROFILE_FILE_H__
#define __EXTPROFILE_FILE_H__

// >>>>>> Generated by idl.php. Do NOT modify. <<<<<<

#include <runtime/ext/ext_file.h>

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

inline Variant x_fopen(CStrRef filename, CStrRef mode, bool use_include_path = false, CVarRef context = null) {
  FUNCTION_INJECTION_BUILTIN(fopen);
  return f_fopen(filename, mode, use_include_path, context);
}

inline Variant x_popen(CStrRef command, CStrRef mode) {
  FUNCTION_INJECTION_BUILTIN(popen);
  return f_popen(command, mode);
}

inline bool x_fclose(CObjRef handle) {
  FUNCTION_INJECTION_BUILTIN(fclose);
  return f_fclose(handle);
}

inline Variant x_pclose(CObjRef handle) {
  FUNCTION_INJECTION_BUILTIN(pclose);
  return f_pclose(handle);
}

inline Variant x_fseek(CObjRef handle, int64 offset, int64 whence = k_SEEK_SET) {
  FUNCTION_INJECTION_BUILTIN(fseek);
  return f_fseek(handle, offset, whence);
}

inline bool x_rewind(CObjRef handle) {
  FUNCTION_INJECTION_BUILTIN(rewind);
  return f_rewind(handle);
}

inline Variant x_ftell(CObjRef handle) {
  FUNCTION_INJECTION_BUILTIN(ftell);
  return f_ftell(handle);
}

inline bool x_feof(CObjRef handle) {
  FUNCTION_INJECTION_BUILTIN(feof);
  return f_feof(handle);
}

inline Variant x_fstat(CObjRef handle) {
  FUNCTION_INJECTION_BUILTIN(fstat);
  return f_fstat(handle);
}

inline Variant x_fread(CObjRef handle, int64 length) {
  FUNCTION_INJECTION_BUILTIN(fread);
  return f_fread(handle, length);
}

inline Variant x_fgetc(CObjRef handle) {
  FUNCTION_INJECTION_BUILTIN(fgetc);
  return f_fgetc(handle);
}

inline Variant x_fgets(CObjRef handle, int64 length = 0) {
  FUNCTION_INJECTION_BUILTIN(fgets);
  return f_fgets(handle, length);
}

inline Variant x_fgetss(CObjRef handle, int64 length = 0, CStrRef allowable_tags = null_string) {
  FUNCTION_INJECTION_BUILTIN(fgetss);
  return f_fgetss(handle, length, allowable_tags);
}

inline Variant x_fscanf(int _argc, CObjRef handle, CStrRef format, CArrRef _argv = null_array) {
  FUNCTION_INJECTION_BUILTIN(fscanf);
  return f_fscanf(_argc, handle, format, _argv);
}

inline Variant x_fpassthru(CObjRef handle) {
  FUNCTION_INJECTION_BUILTIN(fpassthru);
  return f_fpassthru(handle);
}

inline Variant x_fwrite(CObjRef handle, CStrRef data, int64 length = 0) {
  FUNCTION_INJECTION_BUILTIN(fwrite);
  return f_fwrite(handle, data, length);
}

inline Variant x_fputs(CObjRef handle, CStrRef data, int64 length = 0) {
  FUNCTION_INJECTION_BUILTIN(fputs);
  return f_fputs(handle, data, length);
}

inline Variant x_fprintf(int _argc, CObjRef handle, CStrRef format, CArrRef _argv = null_array) {
  FUNCTION_INJECTION_BUILTIN(fprintf);
  return f_fprintf(_argc, handle, format, _argv);
}

inline Variant x_vfprintf(CObjRef handle, CStrRef format, CArrRef args) {
  FUNCTION_INJECTION_BUILTIN(vfprintf);
  return f_vfprintf(handle, format, args);
}

inline bool x_fflush(CObjRef handle) {
  FUNCTION_INJECTION_BUILTIN(fflush);
  return f_fflush(handle);
}

inline bool x_ftruncate(CObjRef handle, int64 size) {
  FUNCTION_INJECTION_BUILTIN(ftruncate);
  return f_ftruncate(handle, size);
}

inline bool x_flock(CObjRef handle, int operation, VRefParam wouldblock = null) {
  FUNCTION_INJECTION_BUILTIN(flock);
  return f_flock(handle, operation, wouldblock);
}

inline Variant x_fputcsv(CObjRef handle, CArrRef fields, CStrRef delimiter = ",", CStrRef enclosure = "\"") {
  FUNCTION_INJECTION_BUILTIN(fputcsv);
  return f_fputcsv(handle, fields, delimiter, enclosure);
}

inline Variant x_fgetcsv(CObjRef handle, int64 length = 0, CStrRef delimiter = ",", CStrRef enclosure = "\"") {
  FUNCTION_INJECTION_BUILTIN(fgetcsv);
  return f_fgetcsv(handle, length, delimiter, enclosure);
}

inline Variant x_file_get_contents(CStrRef filename, bool use_include_path = false, CVarRef context = null, int64 offset = 0, int64 maxlen = 0) {
  FUNCTION_INJECTION_BUILTIN(file_get_contents);
  return f_file_get_contents(filename, use_include_path, context, offset, maxlen);
}

inline Variant x_file_put_contents(CStrRef filename, CVarRef data, int flags = 0, CVarRef context = null) {
  FUNCTION_INJECTION_BUILTIN(file_put_contents);
  return f_file_put_contents(filename, data, flags, context);
}

inline Variant x_file(CStrRef filename, int flags = 0, CVarRef context = null) {
  FUNCTION_INJECTION_BUILTIN(file);
  return f_file(filename, flags, context);
}

inline Variant x_readfile(CStrRef filename, bool use_include_path = false, CVarRef context = null) {
  FUNCTION_INJECTION_BUILTIN(readfile);
  return f_readfile(filename, use_include_path, context);
}

inline bool x_move_uploaded_file(CStrRef filename, CStrRef destination) {
  FUNCTION_INJECTION_BUILTIN(move_uploaded_file);
  return f_move_uploaded_file(filename, destination);
}

inline Variant x_parse_ini_file(CStrRef filename, bool process_sections = false, int scanner_mode = k_INI_SCANNER_NORMAL) {
  FUNCTION_INJECTION_BUILTIN(parse_ini_file);
  return f_parse_ini_file(filename, process_sections, scanner_mode);
}

inline Variant x_parse_ini_string(CStrRef ini, bool process_sections = false, int scanner_mode = k_INI_SCANNER_NORMAL) {
  FUNCTION_INJECTION_BUILTIN(parse_ini_string);
  return f_parse_ini_string(ini, process_sections, scanner_mode);
}

inline Variant x_parse_hdf_file(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(parse_hdf_file);
  return f_parse_hdf_file(filename);
}

inline Variant x_parse_hdf_string(CStrRef input) {
  FUNCTION_INJECTION_BUILTIN(parse_hdf_string);
  return f_parse_hdf_string(input);
}

inline bool x_write_hdf_file(CArrRef data, CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(write_hdf_file);
  return f_write_hdf_file(data, filename);
}

inline String x_write_hdf_string(CArrRef data) {
  FUNCTION_INJECTION_BUILTIN(write_hdf_string);
  return f_write_hdf_string(data);
}

inline Variant x_md5_file(CStrRef filename, bool raw_output = false) {
  FUNCTION_INJECTION_BUILTIN(md5_file);
  return f_md5_file(filename, raw_output);
}

inline Variant x_sha1_file(CStrRef filename, bool raw_output = false) {
  FUNCTION_INJECTION_BUILTIN(sha1_file);
  return f_sha1_file(filename, raw_output);
}

inline bool x_chmod(CStrRef filename, int64 mode) {
  FUNCTION_INJECTION_BUILTIN(chmod);
  return f_chmod(filename, mode);
}

inline bool x_chown(CStrRef filename, CVarRef user) {
  FUNCTION_INJECTION_BUILTIN(chown);
  return f_chown(filename, user);
}

inline bool x_lchown(CStrRef filename, CVarRef user) {
  FUNCTION_INJECTION_BUILTIN(lchown);
  return f_lchown(filename, user);
}

inline bool x_chgrp(CStrRef filename, CVarRef group) {
  FUNCTION_INJECTION_BUILTIN(chgrp);
  return f_chgrp(filename, group);
}

inline bool x_lchgrp(CStrRef filename, CVarRef group) {
  FUNCTION_INJECTION_BUILTIN(lchgrp);
  return f_lchgrp(filename, group);
}

inline bool x_touch(CStrRef filename, int64 mtime = 0, int64 atime = 0) {
  FUNCTION_INJECTION_BUILTIN(touch);
  return f_touch(filename, mtime, atime);
}

inline bool x_copy(CStrRef source, CStrRef dest, CVarRef context = null) {
  FUNCTION_INJECTION_BUILTIN(copy);
  return f_copy(source, dest, context);
}

inline bool x_rename(CStrRef oldname, CStrRef newname, CVarRef context = null) {
  FUNCTION_INJECTION_BUILTIN(rename);
  return f_rename(oldname, newname, context);
}

inline int x_umask(CVarRef mask = null_variant) {
  FUNCTION_INJECTION_BUILTIN(umask);
  return f_umask(mask);
}

inline bool x_unlink(CStrRef filename, CVarRef context = null) {
  FUNCTION_INJECTION_BUILTIN(unlink);
  return f_unlink(filename, context);
}

inline bool x_link(CStrRef target, CStrRef link) {
  FUNCTION_INJECTION_BUILTIN(link);
  return f_link(target, link);
}

inline bool x_symlink(CStrRef target, CStrRef link) {
  FUNCTION_INJECTION_BUILTIN(symlink);
  return f_symlink(target, link);
}

inline String x_basename(CStrRef path, CStrRef suffix = null_string) {
  FUNCTION_INJECTION_BUILTIN(basename);
  return f_basename(path, suffix);
}

inline bool x_fnmatch(CStrRef pattern, CStrRef filename, int flags = 0) {
  FUNCTION_INJECTION_BUILTIN(fnmatch);
  return f_fnmatch(pattern, filename, flags);
}

inline Variant x_glob(CStrRef pattern, int flags = 0) {
  FUNCTION_INJECTION_BUILTIN(glob);
  return f_glob(pattern, flags);
}

inline Variant x_tempnam(CStrRef dir, CStrRef prefix) {
  FUNCTION_INJECTION_BUILTIN(tempnam);
  return f_tempnam(dir, prefix);
}

inline Variant x_tmpfile() {
  FUNCTION_INJECTION_BUILTIN(tmpfile);
  return f_tmpfile();
}

inline Variant x_fileperms(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(fileperms);
  return f_fileperms(filename);
}

inline Variant x_fileinode(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(fileinode);
  return f_fileinode(filename);
}

inline Variant x_filesize(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(filesize);
  return f_filesize(filename);
}

inline Variant x_fileowner(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(fileowner);
  return f_fileowner(filename);
}

inline Variant x_filegroup(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(filegroup);
  return f_filegroup(filename);
}

inline Variant x_fileatime(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(fileatime);
  return f_fileatime(filename);
}

inline Variant x_filemtime(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(filemtime);
  return f_filemtime(filename);
}

inline Variant x_filectime(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(filectime);
  return f_filectime(filename);
}

inline Variant x_filetype(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(filetype);
  return f_filetype(filename);
}

inline Variant x_linkinfo(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(linkinfo);
  return f_linkinfo(filename);
}

inline bool x_is_writable(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(is_writable);
  return f_is_writable(filename);
}

inline bool x_is_writeable(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(is_writeable);
  return f_is_writeable(filename);
}

inline bool x_is_readable(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(is_readable);
  return f_is_readable(filename);
}

inline bool x_is_executable(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(is_executable);
  return f_is_executable(filename);
}

inline bool x_is_file(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(is_file);
  return f_is_file(filename);
}

inline bool x_is_dir(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(is_dir);
  return f_is_dir(filename);
}

inline bool x_is_link(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(is_link);
  return f_is_link(filename);
}

inline bool x_is_uploaded_file(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(is_uploaded_file);
  return f_is_uploaded_file(filename);
}

inline bool x_file_exists(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(file_exists);
  return f_file_exists(filename);
}

inline Variant x_stat(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(stat);
  return f_stat(filename);
}

inline Variant x_lstat(CStrRef filename) {
  FUNCTION_INJECTION_BUILTIN(lstat);
  return f_lstat(filename);
}

inline void x_clearstatcache(bool clear_realpath_cache = false, CVarRef filename = null) {
  FUNCTION_INJECTION_BUILTIN(clearstatcache);
  f_clearstatcache(clear_realpath_cache, filename);
}

inline Variant x_readlink(CStrRef path) {
  FUNCTION_INJECTION_BUILTIN(readlink);
  return f_readlink(path);
}

inline Variant x_realpath(CStrRef path) {
  FUNCTION_INJECTION_BUILTIN(realpath);
  return f_realpath(path);
}

inline Variant x_pathinfo(CStrRef path, int opt = 15) {
  FUNCTION_INJECTION_BUILTIN(pathinfo);
  return f_pathinfo(path, opt);
}

inline Variant x_disk_free_space(CStrRef directory) {
  FUNCTION_INJECTION_BUILTIN(disk_free_space);
  return f_disk_free_space(directory);
}

inline Variant x_diskfreespace(CStrRef directory) {
  FUNCTION_INJECTION_BUILTIN(diskfreespace);
  return f_diskfreespace(directory);
}

inline Variant x_disk_total_space(CStrRef directory) {
  FUNCTION_INJECTION_BUILTIN(disk_total_space);
  return f_disk_total_space(directory);
}

inline bool x_mkdir(CStrRef pathname, int64 mode = 0777, bool recursive = false, CVarRef context = null) {
  FUNCTION_INJECTION_BUILTIN(mkdir);
  return f_mkdir(pathname, mode, recursive, context);
}

inline bool x_rmdir(CStrRef dirname, CVarRef context = null) {
  FUNCTION_INJECTION_BUILTIN(rmdir);
  return f_rmdir(dirname, context);
}

inline String x_dirname(CStrRef path) {
  FUNCTION_INJECTION_BUILTIN(dirname);
  return f_dirname(path);
}

inline Variant x_getcwd() {
  FUNCTION_INJECTION_BUILTIN(getcwd);
  return f_getcwd();
}

inline bool x_chdir(CStrRef directory) {
  FUNCTION_INJECTION_BUILTIN(chdir);
  return f_chdir(directory);
}

inline bool x_chroot(CStrRef directory) {
  FUNCTION_INJECTION_BUILTIN(chroot);
  return f_chroot(directory);
}

inline Variant x_dir(CStrRef directory) {
  FUNCTION_INJECTION_BUILTIN(dir);
  return f_dir(directory);
}

inline Variant x_opendir(CStrRef path, CVarRef context = null) {
  FUNCTION_INJECTION_BUILTIN(opendir);
  return f_opendir(path, context);
}

inline Variant x_readdir(CObjRef dir_handle) {
  FUNCTION_INJECTION_BUILTIN(readdir);
  return f_readdir(dir_handle);
}

inline void x_rewinddir(CObjRef dir_handle) {
  FUNCTION_INJECTION_BUILTIN(rewinddir);
  f_rewinddir(dir_handle);
}

inline Variant x_scandir(CStrRef directory, bool descending = false, CVarRef context = null) {
  FUNCTION_INJECTION_BUILTIN(scandir);
  return f_scandir(directory, descending, context);
}

inline void x_closedir(CObjRef dir_handle) {
  FUNCTION_INJECTION_BUILTIN(closedir);
  f_closedir(dir_handle);
}


///////////////////////////////////////////////////////////////////////////////
}

#endif // __EXTPROFILE_FILE_H__
