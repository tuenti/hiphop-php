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

#include <runtime/ext/ext_fileinfo.h>
#include <magic.h>

#include <sys/types.h>
#include <regex.h>
#include <stdio.h>

namespace HPHP {
IMPLEMENT_DEFAULT_EXTENSION(fileinfo);
///////////////////////////////////////////////////////////////////////////////
 
const int64 k_FILEINFO_NONE = MAGIC_NONE;
const int64 k_FILEINFO_SYMLINK = MAGIC_SYMLINK;
const int64 k_FILEINFO_MIME = MAGIC_MIME;
const int64 k_FILEINFO_MIME_TYPE = MAGIC_MIME_TYPE;
const int64 k_FILEINFO_MIME_ENCODING = MAGIC_MIME_ENCODING;
const int64 k_FILEINFO_DEVICES = MAGIC_DEVICES;
const int64 k_FILEINFO_CONTINUE = MAGIC_CONTINUE;
const int64 k_FILEINFO_PRESERVE_ATIME = MAGIC_PRESERVE_ATIME;
const int64 k_FILEINFO_RAW = MAGIC_RAW;

class FileInfoResource : public SweepableResourceData {
public:
  DECLARE_OBJECT_ALLOCATION(FileInfoResource)

  static StaticString s_class_name;
  // overriding ResourceData
  CStrRef o_getClassNameHook() const { return s_class_name; }

  FileInfoResource(int _options) {
    options = _options;
    cookie = NULL;
  }

  void close() {
    if (cookie) {
      magic_close(cookie);
      cookie = NULL;
    }
  }

  ~FileInfoResource() {
    close();
  }

 bool setflags(int _options) {
    options = _options;
    if (magic_setflags(cookie, options) == -1) {
      raise_warning("Error calling magic_setflags: %s", magic_error(cookie));
      return false;
    }

    return true;
  }

  bool load(CStrRef magic_file) {
    cookie = magic_open(options);

    if (cookie == NULL) {
      raise_warning("Error calling magic_open: %s", magic_error(cookie));
      return false;
    }

    const char *file = NULL;
    if (!magic_file.empty()) 
      file = magic_file.data();
    else
      file = "/opt/hiphop/lib/magic.mgc";

    if (magic_load(cookie, file) == -1) {
      raise_warning("Error calling magic_load: %s", magic_error(cookie));
      return false;
    }

    return true;
  }

  const char * buffer (CStrRef buffer, int custom_options) {
    const char *ret;

    if (false && options != k_FILEINFO_NONE) {
      setflags(custom_options);
      ret = magic_buffer(cookie, buffer.data(), buffer.size());
      setflags(options);
    } else {
      ret = magic_buffer(cookie, buffer.data(), buffer.size());
    }

    if (ret == NULL) {
      raise_warning("Error calling magic_buffer: %d: %s", magic_errno(cookie), magic_error(cookie));
    }

    return ret;
  }

  const char * file (CStrRef file_name, int custom_options) {
    const char *ret;

    if (custom_options != k_FILEINFO_NONE) {
      setflags(custom_options);
      ret = magic_file(cookie, file_name.data());
      setflags(options);
    } else {
      ret = magic_file(cookie, file_name.data());
    }

   if (ret == NULL) {
      raise_warning("Error calling magic_file: %s", magic_error(cookie));
   }
  
    return ret;
  }

private:
  int options;
  magic_t cookie;
};

IMPLEMENT_OBJECT_ALLOCATION_NO_DEFAULT_SWEEP(FileInfoResource)

StaticString FileInfoResource::s_class_name("file_info");

void FileInfoResource::sweep() {
  close();
}
 
#define CHECK_FILEINFO_RESOURCE(f_info)                                  \
  FileInfoResource *f;                                                   \
  if (!f_info.isNull()) f = f_info.getTyped<FileInfoResource>(true, true); \
  else {                                                                 \
     raise_warning("expects parameter 1 to be file_info resource");      \
     return false;                                                       \
  }                                                                      \

Variant f_finfo_open(int64 options /* = k_FILEINFO_NONE */, CStrRef magic_file /* = null_string */) {
  FileInfoResource *file_info = NEWOBJ(FileInfoResource)(options);
  
  const char * fname = magic_file.empty() ? NULL : magic_file.c_str();
  if (fname && strlen(basename(fname)) < 1) {
    raise_warning("expects parameter 2 to be a valid path, string given");
    return false;
  }

  if (!file_info->load(magic_file)) {
    return false;
  }

  return file_info;
}

Variant f_finfo_close(CObjRef finfo) {
  CHECK_FILEINFO_RESOURCE(finfo);
  FileInfoResource *file_info = finfo.getTyped<FileInfoResource>();
  file_info->close();

  return true;
}

Variant f_finfo_buffer(CObjRef finfo, CStrRef buffer /* = null_string */, int64 options /* = k_FILEINFO_NONE */, CStrRef context /* = null_string */) {
  if (!context.empty()) {
    throw NotSupportedException(__func__, "context parameter is not supported");
  }
  
  if (buffer.empty()) {
    return String("empty", AttachLiteral);
  }

  CHECK_FILEINFO_RESOURCE(finfo);
  FileInfoResource *file_info = finfo.getTyped<FileInfoResource>();

  const char *ret = file_info->buffer(buffer, options);

  if (ret) return String(ret, CopyString);
  else     return false;
}

Variant f_finfo_file(CObjRef finfo, CStrRef file_name /* = null_string */, int64 options /* = k_FILEINFO_NONE */, CStrRef context /* = null_string */) {
  if (!context.empty()) {
    throw NotSupportedException(__func__, "context parameter is not supported");
  }

  const char * fname = file_name.empty() ? "" : file_name.c_str();
  if (strlen(basename(fname)) < 1) {
    raise_warning("finfo_file: Empty filename or path");
    return false;
  }

  CHECK_FILEINFO_RESOURCE(finfo);
  FileInfoResource *file_info = finfo.getTyped<FileInfoResource>();

  const char *ret = file_info->file(file_name, options);

  if (ret) return String(ret, CopyString);
  else     return false;
}

Variant f_finfo_set_flags(CObjRef finfo, int64 options /* = k_FILEINFO_NONE */) {
  CHECK_FILEINFO_RESOURCE(finfo);
  FileInfoResource *file_info = finfo.getTyped<FileInfoResource>();

  return file_info->setflags(options);
}


///////////////////////////////////////////////////////////////////////////////
}
