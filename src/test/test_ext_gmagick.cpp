/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010- Facebook, Inc. (http://www.facebook.com)         |
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

#include <test/test_ext_gmagick.h>
#include <runtime/ext/ext_gmagick.h>
#include <runtime/ext/ext_options.h>
#include <unistd.h>

IMPLEMENT_SEP_EXTENSION_TEST(Gmagick);
///////////////////////////////////////////////////////////////////////////////

bool TestExtGmagick::RunTests(const std::string &which) {
  bool ret = true;

  RUN_TEST(test_GmagickPixel);
  RUN_TEST(test_Gmagick);

  return ret;
}

///////////////////////////////////////////////////////////////////////////////

bool TestExtGmagick::test_GmagickPixel() {
  return Count(true);
}

bool TestExtGmagick::test_Gmagick() {
  p_Gmagick gm(p_Gmagick(NEWOBJ(c_Gmagick))->create());
  p_GmagickPixel gmp(p_GmagickPixel(NEWOBJ(c_GmagickPixel))->create("#ffffff"));

  gm->t_readimage("images/test2.jpg");
  VS(gm->t_getimagewidth(), 800);
  VS(gm->t_getimageheight(), 600);

  gm->t_resizeimage(640, 480, 0, 1);
  VS(gm->t_getimagewidth(), 640);
  VS(gm->t_getimageheight(), 480);

  gm->t_resizeimage(320, 0, 0, 1);
  VS(gm->t_getimagewidth(), 320);
  VS(gm->t_getimageheight(), 240);

  gm->t_rotateimage(gmp, 90);
  VS(gm->t_getimagewidth(), 240);
  VS(gm->t_getimageheight(), 320);

  gm->t_cropimage(55, 65, 10, 15);
  VS(gm->t_getimagewidth(), 55);
  VS(gm->t_getimageheight(), 65);

  gm->t_borderimage(gmp, 10, 5);
  VS(gm->t_getimagewidth(), 75);
  VS(gm->t_getimageheight(), 75);

  gm->t_setimageformat("PNG");
  gm->t_writeimage("images/test2.png");
  VS(gm->t_getimageformat(), "PNG");
  unlink("images/test2.png");

  VS(gm->t_getimagetype(), q_Gmagick$$IMGTYPE_TRUECOLOR);
  VS(gm->t_getimagecolorspace(), q_Gmagick$$COLORSPACE_RGB);
  gm->t_setcompressionquality(20);
  gm->t_setimageformat("JPG");
  gm->t_writeimage("images/mala.jpg");
  VS(gm->t_getimageprofile("ICM"), "");
  VS(gm->t_getimageformat(), "JPG");
  
  String image_blob = gm->t_getimageblob();
  p_Gmagick gm2(p_Gmagick(NEWOBJ(c_Gmagick))->create());
  gm2->t_readimageblob(image_blob, "patata");
  VS(gm2->t_getimagefilename(), "patata");
  VS(gm2->t_getimagewidth(), 75);
  VS(gm2->t_getimageheight(), 75);
  unlink("images/mala.jpg");

  p_Gmagick gm3(p_Gmagick(NEWOBJ(c_Gmagick))->create());
  gm3->t_readimage("images/palomas_small.jpg");
  VS(gm3->t_getimageprofile("ICM").size(), 560);
  gm3->t_profileimage("*", (const char *)NULL);
  VS(gm3->t_getimageprofile("ICM").size(), 0);
  gm3->t_writeimage("images/palomas_noprofile.jpg");
  unlink("images/palomas_noprofile.jpg");

  return Count(true);
}
