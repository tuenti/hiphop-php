/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010 Facebook, Inc. (http://www.facebook.com)          |
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

#ifndef __GENERATED_CLS_SPLOBJECTSTORAGE_H__
#define __GENERATED_CLS_SPLOBJECTSTORAGE_H__

#include <cls/iterator.h>
#include <cls/countable.h>

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

/* SRC: classes/splobjectstorage.php line 3 */
class c_splobjectstorage : virtual public c_iterator, virtual public c_countable {
  BEGIN_CLASS_MAP(splobjectstorage)
    PARENT_CLASS(traversable)
    PARENT_CLASS(iterator)
    PARENT_CLASS(countable)
  END_CLASS_MAP(splobjectstorage)
  DECLARE_CLASS(splobjectstorage, SplObjectStorage, ObjectData)
  DECLARE_INVOKES_FROM_EVAL
  void init();
  public: Variant m_storage;
  public: int64 m_index;
  public: void t_rewind();
  public: bool t_valid();
  public: int64 t_key();
  public: Variant t_current();
  public: void t_next();
  public: int t_count();
  public: bool t_contains(CVarRef v_obj);
  public: void t_attach(CVarRef v_obj);
  public: void t_detach(CVarRef v_obj);
};

///////////////////////////////////////////////////////////////////////////////
}

#endif // __GENERATED_CLS_SPLOBJECTSTORAGE_H__
