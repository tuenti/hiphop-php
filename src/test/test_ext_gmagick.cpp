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

#define IMG_DIR "test/images"

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

  gm->t_readimage(IMG_DIR "/test2.jpg");
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
  gm->t_writeimage(IMG_DIR "/test2.png");
  VS(gm->t_getimageformat(), "PNG");
  unlink(IMG_DIR "/test2.png");

  VS(gm->t_getimagetype(), q_Gmagick___IMGTYPE_TRUECOLOR);
  VS(gm->t_getimagecolorspace(), q_Gmagick___COLORSPACE_RGB);
  gm->t_setcompressionquality(20);
  gm->t_setimageformat("JPG");
  gm->t_writeimage(IMG_DIR "/mala.jpg");
  VS(gm->t_getimageprofile("ICM"), "");
  VS(gm->t_getimageformat(), "JPG");
  
  String image_blob = gm->t_getimageblob();
  p_Gmagick gm2(p_Gmagick(NEWOBJ(c_Gmagick))->create());
  gm2->t_readimageblob(image_blob, "patata");
  VS(gm2->t_getimagefilename(), "patata");
  VS(gm2->t_getimagewidth(), 75);
  VS(gm2->t_getimageheight(), 75);
  unlink(IMG_DIR "/mala.jpg");

  p_Gmagick gm3(p_Gmagick(NEWOBJ(c_Gmagick))->create());
  gm3->t_readimage(IMG_DIR "/palomas_small.jpg");
  VS(gm3->t_getimageprofile("ICM").size(), 560);
  gm3->t_profileimage("*", (const char *)NULL);
  VS(gm3->t_getimageprofile("ICM").size(), 0);
  gm3->t_writeimage(IMG_DIR "/palomas_noprofile.jpg");
  unlink(IMG_DIR "/palomas_noprofile.jpg");

  p_Gmagick gm4(p_Gmagick(NEWOBJ(c_Gmagick))->create());
  gm4->t_readimage(IMG_DIR "/test2.jpg");
  gm4->t_borderimage(gmp, 10, 20);
  VS(gm4->t_getimagewidth(), 820);
  VS(gm4->t_getimageheight(), 640);
  gm4->t_scaleimage(100, 200);
  VS(gm4->t_getimagewidth(), 100);
  VS(gm4->t_getimageheight(), 200);
  gm4->t_setimagebackgroundcolor(gmp);
  gm4->t_flattenimages();

  p_Gmagick gm5(p_Gmagick(NEWOBJ(c_Gmagick))->create());
  gm5->t_readimage(IMG_DIR "/test2.jpg");
  p_Gmagick gm6 = gm5->t_getimage();
  gm6->t_borderimage(gmp, 10, 20);
  VS(gm5->t_getimagewidth(), 800);
  VS(gm5->t_getimageheight(), 600);
  VS(gm6->t_getimagewidth(), 820);
  VS(gm6->t_getimageheight(), 640);

  p_Gmagick gm7(p_Gmagick(NEWOBJ(c_Gmagick))->create());
  gm7->t_readimage(IMG_DIR "/test2.jpg");
  gm7->t_newimage(100, 100, "red");
  VS(gm7->t_getimagewidth(), 100);
  VS(gm7->t_getimageheight(), 100);
  gm7->t_removeimage();
  VS(gm7->t_getimagewidth(), 800);
  VS(gm7->t_getimageheight(), 600);
  gm7->t_compositeimage(gm5, q_Gmagick___COMPOSITE_COPY, 0, 0);

  return Count(true);
}
