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

#include <runtime/ext/ext_gmagick.h>
#include <wand/wand_api.h>
#include <system/lib/systemlib.h>
#include <string>
#include <stdexcept>

namespace HPHP {

/* Static Initializer to call Init and Sutdown functions only one time */
class GmagickStaticInitializer {
	public:
		GmagickStaticInitializer() {
			InitializeMagick(NULL);
		}	
		~GmagickStaticInitializer() {
			DestroyMagick();
		}
};
static GmagickStaticInitializer s_gmagick_initializer;

///////////////////////////////////////////////////////////////////////////////
// class GmagickPixel

c_GmagickPixel::c_GmagickPixel(const ObjectStaticCallbacks *cb) :ExtObjectData(cb) {
}

c_GmagickPixel::~c_GmagickPixel() {
}

void c_GmagickPixel::t___construct(CStrRef color) {
  INSTANCE_METHOD_INJECTION_BUILTIN(GmagickPixel, GmagickPixel::__construct);
  pixel_wand = NewPixelWand();
  if (!color.isNull()) {
    PixelSetColor(pixel_wand, color->data());
  }
}

Variant c_GmagickPixel::t___destruct() {
  INSTANCE_METHOD_INJECTION_BUILTIN(GmagickPixel, GmagickPixel::__destruct);
  DestroyPixelWand(pixel_wand);
  return null;
}

PixelWand * c_GmagickPixel::get_PixelWand() {
  return pixel_wand;
}

//////////////////////////////////////////////////////////////////////////////
// class Gmagick

const int q_Gmagick$$COMPOSITE_DEFAULT = OverCompositeOp;
const int q_Gmagick$$COMPOSITE_UNDEFINED = UndefinedCompositeOp;
const int q_Gmagick$$COMPOSITE_NO = NoCompositeOp;
const int q_Gmagick$$COMPOSITE_ADD = AddCompositeOp;
const int q_Gmagick$$COMPOSITE_ATOP = AtopCompositeOp;
const int q_Gmagick$$COMPOSITE_BUMPMAP = BumpmapCompositeOp;
const int q_Gmagick$$COMPOSITE_CLEAR = ClearCompositeOp;
const int q_Gmagick$$COMPOSITE_COLORIZE = ColorizeCompositeOp;
const int q_Gmagick$$COMPOSITE_COPYBLACK = CopyBlackCompositeOp;
const int q_Gmagick$$COMPOSITE_COPYBLUE = CopyBlueCompositeOp;
const int q_Gmagick$$COMPOSITE_COPY = CopyCompositeOp;
const int q_Gmagick$$COMPOSITE_COPYCYAN = CopyCyanCompositeOp;
const int q_Gmagick$$COMPOSITE_COPYGREEN = CopyGreenCompositeOp;
const int q_Gmagick$$COMPOSITE_COPYMAGENTA = CopyMagentaCompositeOp;
const int q_Gmagick$$COMPOSITE_COPYOPACITY = CopyOpacityCompositeOp;
const int q_Gmagick$$COMPOSITE_COPYRED = CopyRedCompositeOp;
const int q_Gmagick$$COMPOSITE_COPYYELLOW = CopyYellowCompositeOp;
const int q_Gmagick$$COMPOSITE_DARKEN = DarkenCompositeOp;
const int q_Gmagick$$COMPOSITE_DIFFERENCE = DifferenceCompositeOp;
const int q_Gmagick$$COMPOSITE_DISPLACE = DisplaceCompositeOp;
const int q_Gmagick$$COMPOSITE_DISSOLVE = DissolveCompositeOp;
const int q_Gmagick$$COMPOSITE_HUE = HueCompositeOp;
const int q_Gmagick$$COMPOSITE_IN = InCompositeOp;
const int q_Gmagick$$COMPOSITE_LIGHTEN = LightenCompositeOp;
const int q_Gmagick$$COMPOSITE_LUMINIZE = LuminizeCompositeOp;
const int q_Gmagick$$COMPOSITE_MINUS = MinusCompositeOp;
const int q_Gmagick$$COMPOSITE_MODULATE = ModulateCompositeOp;
const int q_Gmagick$$COMPOSITE_MULTIPLY = MultiplyCompositeOp;
const int q_Gmagick$$COMPOSITE_OUT = OutCompositeOp;
const int q_Gmagick$$COMPOSITE_OVER = OverCompositeOp;
const int q_Gmagick$$COMPOSITE_OVERLAY = OverlayCompositeOp;
const int q_Gmagick$$COMPOSITE_PLUS = PlusCompositeOp;
const int q_Gmagick$$COMPOSITE_REPLACE = ReplaceCompositeOp;
const int q_Gmagick$$COMPOSITE_SATURATE = SaturateCompositeOp;
const int q_Gmagick$$COMPOSITE_SCREEN = ScreenCompositeOp;
const int q_Gmagick$$COMPOSITE_SUBTRACT = SubtractCompositeOp;
const int q_Gmagick$$COMPOSITE_THRESHOLD = ThresholdCompositeOp;
const int q_Gmagick$$COMPOSITE_XOR = XorCompositeOp;
const int q_Gmagick$$MONTAGEMODE_FRAME = FrameMode;
const int q_Gmagick$$MONTAGEMODE_UNFRAME = UnframeMode;
const int q_Gmagick$$MONTAGEMODE_CONCATENATE = ConcatenateMode;
const int q_Gmagick$$STYLE_NORMAL = NormalStyle ;
const int q_Gmagick$$STYLE_ITALIC = ItalicStyle ;
const int q_Gmagick$$STYLE_OBLIQUE = ObliqueStyle ;
const int q_Gmagick$$STYLE_ANY = AnyStyle ;
const int q_Gmagick$$FILTER_UNDEFINED = UndefinedFilter ;
const int q_Gmagick$$FILTER_POINT = PointFilter ;
const int q_Gmagick$$FILTER_BOX = BoxFilter ;
const int q_Gmagick$$FILTER_TRIANGLE = TriangleFilter ;
const int q_Gmagick$$FILTER_HERMITE = HermiteFilter ;
const int q_Gmagick$$FILTER_HANNING = HanningFilter ;
const int q_Gmagick$$FILTER_HAMMING = HammingFilter ;
const int q_Gmagick$$FILTER_BLACKMAN = BlackmanFilter ;
const int q_Gmagick$$FILTER_GAUSSIAN = GaussianFilter ;
const int q_Gmagick$$FILTER_QUADRATIC = QuadraticFilter ;
const int q_Gmagick$$FILTER_CUBIC = CubicFilter ;
const int q_Gmagick$$FILTER_CATROM = CatromFilter ;
const int q_Gmagick$$FILTER_MITCHELL = MitchellFilter ;
const int q_Gmagick$$FILTER_LANCZOS = LanczosFilter ;
const int q_Gmagick$$FILTER_BESSEL = BesselFilter ;
const int q_Gmagick$$FILTER_SINC = SincFilter ;
const int q_Gmagick$$IMGTYPE_UNDEFINED = UndefinedType ;
const int q_Gmagick$$IMGTYPE_BILEVEL = BilevelType ;
const int q_Gmagick$$IMGTYPE_GRAYSCALE = GrayscaleType ;
const int q_Gmagick$$IMGTYPE_GRAYSCALEMATTE = GrayscaleMatteType ;
const int q_Gmagick$$IMGTYPE_PALETTE =  PaletteType ;
const int q_Gmagick$$IMGTYPE_PALETTEMATTE = PaletteMatteType ;
const int q_Gmagick$$IMGTYPE_TRUECOLOR = TrueColorType ;
const int q_Gmagick$$IMGTYPE_TRUECOLORMATTE = TrueColorMatteType ;
const int q_Gmagick$$IMGTYPE_COLORSEPARATION = ColorSeparationType ;
const int q_Gmagick$$IMGTYPE_COLORSEPARATIONMATTE = ColorSeparationMatteType ;
const int q_Gmagick$$IMGTYPE_OPTIMIZE = OptimizeType ;
const int q_Gmagick$$RESOLUTION_UNDEFINED = UndefinedResolution ;
const int q_Gmagick$$RESOLUTION_PIXELSPERINCH = PixelsPerInchResolution ;
const int q_Gmagick$$RESOLUTION_PIXELSPERCENTIMETER = PixelsPerCentimeterResolution;
const int q_Gmagick$$COMPRESSION_UNDEFINED = UndefinedCompression;
const int q_Gmagick$$COMPRESSION_NO = NoCompression;
const int q_Gmagick$$COMPRESSION_BZIP = BZipCompression;
const int q_Gmagick$$COMPRESSION_FAX = FaxCompression;
const int q_Gmagick$$COMPRESSION_GROUP4 = Group4Compression;
const int q_Gmagick$$COMPRESSION_JPEG = JPEGCompression;
const int q_Gmagick$$COMPRESSION_LOSSLESSJPEG = LosslessJPEGCompression;
const int q_Gmagick$$COMPRESSION_LZW = LZWCompression;
const int q_Gmagick$$COMPRESSION_RLE = RLECompression;
const int q_Gmagick$$COMPRESSION_ZIP = ZipCompression;
const int q_Gmagick$$PAINT_POINT = PointMethod;
const int q_Gmagick$$PAINT_REPLACE = ReplaceMethod;
const int q_Gmagick$$PAINT_FLOODFILL = FloodfillMethod;
const int q_Gmagick$$PAINT_FILLTOBORDER = FillToBorderMethod;
const int q_Gmagick$$PAINT_RESET = ResetMethod;
const int q_Gmagick$$GRAVITY_NORTHWEST = NorthWestGravity;
const int q_Gmagick$$GRAVITY_NORTH = NorthGravity;
const int q_Gmagick$$GRAVITY_NORTHEAST = NorthEastGravity;
const int q_Gmagick$$GRAVITY_WEST = WestGravity;
const int q_Gmagick$$GRAVITY_CENTER = CenterGravity;
const int q_Gmagick$$GRAVITY_EAST = EastGravity;
const int q_Gmagick$$GRAVITY_SOUTHWEST = SouthWestGravity;
const int q_Gmagick$$GRAVITY_SOUTH = SouthGravity;
const int q_Gmagick$$GRAVITY_SOUTHEAST = SouthEastGravity;
const int q_Gmagick$$STRETCH_NORMAL = NormalStretch;
const int q_Gmagick$$STRETCH_ULTRACONDENSED = UltraCondensedStretch;
const int q_Gmagick$$STRETCH_CONDENSED = CondensedStretch;
const int q_Gmagick$$STRETCH_SEMICONDENSED = SemiCondensedStretch;
const int q_Gmagick$$STRETCH_SEMIEXPANDED = SemiExpandedStretch;
const int q_Gmagick$$STRETCH_EXPANDED = ExpandedStretch;
const int q_Gmagick$$STRETCH_EXTRAEXPANDED = ExtraExpandedStretch;
const int q_Gmagick$$STRETCH_ULTRAEXPANDED = UltraExpandedStretch;
const int q_Gmagick$$STRETCH_ANY = AnyStretch;
const int q_Gmagick$$ALIGN_UNDEFINED = UndefinedAlign;
const int q_Gmagick$$ALIGN_LEFT = LeftAlign;
const int q_Gmagick$$ALIGN_CENTER = CenterAlign;
const int q_Gmagick$$ALIGN_RIGHT = RightAlign;
const int q_Gmagick$$DECORATION_NO = NoDecoration;
const int q_Gmagick$$DECORATION_UNDERLINE = UnderlineDecoration;
const int q_Gmagick$$DECORATION_OVERLINE = OverlineDecoration;
const int q_Gmagick$$DECORATION_LINETROUGH = LineThroughDecoration;
const int q_Gmagick$$NOISE_UNIFORM = UniformNoise;
const int q_Gmagick$$NOISE_GAUSSIAN = GaussianNoise;
const int q_Gmagick$$NOISE_MULTIPLICATIVEGAUSSIAN = MultiplicativeGaussianNoise;
const int q_Gmagick$$NOISE_IMPULSE = ImpulseNoise;
const int q_Gmagick$$NOISE_LAPLACIAN = LaplacianNoise ;
const int q_Gmagick$$NOISE_POISSON = PoissonNoise;
const int q_Gmagick$$CHANNEL_UNDEFINED = UndefinedChannel;
const int q_Gmagick$$CHANNEL_RED = RedChannel;
const int q_Gmagick$$CHANNEL_GRAY = GrayChannel;
const int q_Gmagick$$CHANNEL_CYAN = CyanChannel;
const int q_Gmagick$$CHANNEL_GREEN = GreenChannel;
const int q_Gmagick$$CHANNEL_MAGENTA = MagentaChannel;
const int q_Gmagick$$CHANNEL_BLUE = BlueChannel;
const int q_Gmagick$$CHANNEL_YELLOW = YellowChannel;
const int q_Gmagick$$CHANNEL_OPACITY = OpacityChannel;
const int q_Gmagick$$CHANNEL_MATTE = MatteChannel;
const int q_Gmagick$$CHANNEL_BLACK = BlackChannel;
const int q_Gmagick$$CHANNEL_INDEX = IndexChannel;
const int q_Gmagick$$CHANNEL_ALL = AllChannels;
const int q_Gmagick$$METRIC_UNDEFINED = UndefinedMetric;
const int q_Gmagick$$METRIC_MEANABSOLUTEERROR = MeanAbsoluteErrorMetric;
const int q_Gmagick$$METRIC_MEANSQUAREERROR = MeanSquaredErrorMetric;
const int q_Gmagick$$METRIC_PEAKABSOLUTEERROR = PeakAbsoluteErrorMetric;
const int q_Gmagick$$METRIC_PEAKSIGNALTONOISERATIO = PeakSignalToNoiseRatioMetric;
const int q_Gmagick$$METRIC_ROOTMEANSQUAREDERROR = RootMeanSquaredErrorMetric;
const int q_Gmagick$$PIXEL_CHAR = CharPixel;
const int q_Gmagick$$PIXEL_DOUBLE = DoublePixel;
const int q_Gmagick$$PIXEL_FLOAT = FloatPixel;
const int q_Gmagick$$PIXEL_INTEGER = IntegerPixel;
const int q_Gmagick$$PIXEL_LONG = LongPixel;
const int q_Gmagick$$PIXEL_SHORT = ShortPixel;
const int q_Gmagick$$COLORSPACE_UNDEFINED = UndefinedColorspace;
const int q_Gmagick$$COLORSPACE_RGB = RGBColorspace;
const int q_Gmagick$$COLORSPACE_GRAY = GRAYColorspace;
const int q_Gmagick$$COLORSPACE_TRANSPARENT = TransparentColorspace;
const int q_Gmagick$$COLORSPACE_OHTA = OHTAColorspace;
const int q_Gmagick$$COLORSPACE_LAB = LABColorspace;
const int q_Gmagick$$COLORSPACE_XYZ = XYZColorspace;
const int q_Gmagick$$COLORSPACE_YCBCR = YCbCrColorspace;
const int q_Gmagick$$COLORSPACE_YCC = YCCColorspace;
const int q_Gmagick$$COLORSPACE_YIQ = YIQColorspace;
const int q_Gmagick$$COLORSPACE_YPBPR = YPbPrColorspace;
const int q_Gmagick$$COLORSPACE_YUV = YUVColorspace;
const int q_Gmagick$$COLORSPACE_CMYK = CMYKColorspace;
const int q_Gmagick$$COLORSPACE_SRGB = sRGBColorspace;
const int q_Gmagick$$COLORSPACE_HSL = HSLColorspace;
const int q_Gmagick$$COLORSPACE_HWB = HWBColorspace;
const int q_Gmagick$$VIRTUALPIXELMETHOD_UNDEFINED = UndefinedVirtualPixelMethod;
const int q_Gmagick$$VIRTUALPIXELMETHOD_CONSTANT = ConstantVirtualPixelMethod;
const int q_Gmagick$$VIRTUALPIXELMETHOD_EDGE = EdgeVirtualPixelMethod;
const int q_Gmagick$$VIRTUALPIXELMETHOD_MIRROR = MirrorVirtualPixelMethod;
const int q_Gmagick$$VIRTUALPIXELMETHOD_TILE = TileVirtualPixelMethod;
const int q_Gmagick$$PREVIEW_UNDEFINED = UndefinedPreview;
const int q_Gmagick$$PREVIEW_ROTATE = RotatePreview;
const int q_Gmagick$$PREVIEW_SHEAR = ShearPreview;
const int q_Gmagick$$PREVIEW_ROLL = RollPreview;
const int q_Gmagick$$PREVIEW_HUE = HuePreview;
const int q_Gmagick$$PREVIEW_SATURATION = SaturationPreview;
const int q_Gmagick$$PREVIEW_BRIGHTNESS = BrightnessPreview;
const int q_Gmagick$$PREVIEW_GAMMA = GammaPreview;
const int q_Gmagick$$PREVIEW_SPIFF = SpiffPreview;
const int q_Gmagick$$PREVIEW_DULL = DullPreview;
const int q_Gmagick$$PREVIEW_GRAYSCALE = GrayscalePreview;
const int q_Gmagick$$PREVIEW_QUANTIZE = QuantizePreview;
const int q_Gmagick$$PREVIEW_DESPECKLE = DespecklePreview;
const int q_Gmagick$$PREVIEW_REDUCENOISE = ReduceNoisePreview;
const int q_Gmagick$$PREVIEW_ADDNOISE = AddNoisePreview;
const int q_Gmagick$$PREVIEW_SHARPEN = SharpenPreview;
const int q_Gmagick$$PREVIEW_BLUR = BlurPreview;
const int q_Gmagick$$PREVIEW_THRESHOLD = ThresholdPreview;
const int q_Gmagick$$PREVIEW_EDGEDETECT = EdgeDetectPreview;
const int q_Gmagick$$PREVIEW_SPREAD = SpreadPreview;
const int q_Gmagick$$PREVIEW_SOLARIZE = SolarizePreview;
const int q_Gmagick$$PREVIEW_SHADE = ShadePreview;
const int q_Gmagick$$PREVIEW_RAISE = RaisePreview;
const int q_Gmagick$$PREVIEW_SEGMENT = SegmentPreview;
const int q_Gmagick$$PREVIEW_SWIRL = SwirlPreview;
const int q_Gmagick$$PREVIEW_IMPLODE = ImplodePreview;
const int q_Gmagick$$PREVIEW_WAVE = WavePreview;
const int q_Gmagick$$PREVIEW_OILPAINT = OilPaintPreview;
const int q_Gmagick$$PREVIEW_CHARCOALDRAWING = CharcoalDrawingPreview;
const int q_Gmagick$$PREVIEW_JPEG = JPEGPreview;
const int q_Gmagick$$RENDERINGINTENT_UNDEFINED = UndefinedIntent;
const int q_Gmagick$$RENDERINGINTENT_SATURATION = SaturationIntent;
const int q_Gmagick$$RENDERINGINTENT_PERCEPTUAL = PerceptualIntent;
const int q_Gmagick$$RENDERINGINTENT_ABSOLUTE = AbsoluteIntent;
const int q_Gmagick$$RENDERINGINTENT_RELATIVE = RelativeIntent;
const int q_Gmagick$$INTERLACE_UNDEFINED = UndefinedInterlace;
const int q_Gmagick$$INTERLACE_NO = NoInterlace;
const int q_Gmagick$$INTERLACE_LINE = LineInterlace;
const int q_Gmagick$$INTERLACE_PLANE = PlaneInterlace;
const int q_Gmagick$$INTERLACE_PARTITION = PartitionInterlace;
const int q_Gmagick$$FILLRULE_UNDEFINED = UndefinedRule;
const int q_Gmagick$$FILLRULE_EVENODD = EvenOddRule;
const int q_Gmagick$$FILLRULE_NONZERO = NonZeroRule;
const int q_Gmagick$$PATHUNITS_USERSPACE = UserSpace;
const int q_Gmagick$$PATHUNITS_USERSPACEONUSE = UserSpaceOnUse;
const int q_Gmagick$$PATHUNITS_OBJECTBOUNDINGBOX = ObjectBoundingBox;
const int q_Gmagick$$LINECAP_UNDEFINED = UndefinedCap;
const int q_Gmagick$$LINECAP_BUTT = ButtCap;
const int q_Gmagick$$LINECAP_ROUND = RoundCap;
const int q_Gmagick$$LINECAP_SQUARE = SquareCap;
const int q_Gmagick$$LINEJOIN_UNDEFINED = UndefinedJoin;
const int q_Gmagick$$LINEJOIN_MITER = MiterJoin;
const int q_Gmagick$$LINEJOIN_ROUND = RoundJoin;
const int q_Gmagick$$LINEJOIN_BEVEL = BevelJoin;
const int q_Gmagick$$RESOURCETYPE_UNDEFINED = UndefinedResource;
const int q_Gmagick$$RESOURCETYPE_AREA = AreaResource;
const int q_Gmagick$$RESOURCETYPE_DISK = DiskResource;
const int q_Gmagick$$RESOURCETYPE_FILE = FileResource;
const int q_Gmagick$$RESOURCETYPE_MAP = MapResource;
const int q_Gmagick$$RESOURCETYPE_MEMORY = MemoryResource;
const int q_Gmagick$$DISPOSE_UNDEFINED = UndefinedDispose;
const int q_Gmagick$$DISPOSE_NONE = NoneDispose;
const int q_Gmagick$$DISPOSE_BACKGROUND = BackgroundDispose;
const int q_Gmagick$$DISPOSE_PREVIOUS = PreviousDispose;

c_Gmagick::c_Gmagick(const ObjectStaticCallbacks *cb) :ExtObjectData(cb){
  magick_wand = NewMagickWand();
}

c_Gmagick::~c_Gmagick() {
  DestroyMagickWand(magick_wand);
}

void c_Gmagick::t___construct() {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::__construct);
  
}

Variant c_Gmagick::t___destruct() {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::__destruct);
  return null;
}

void c_Gmagick::t_destroy() {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::destroy);
  DestroyMagickWand(magick_wand);
  magick_wand = NewMagickWand();
}

/* Check the integer and raise an exception if the result is equal to MagickFalse */
void c_Gmagick::throwException(const char * magick_message, int magick_severity) {
  Object e(SystemLib::AllocGmagickExceptionObject(magick_message, magick_severity));
  throw e;
}

/* Check the integer and raise an exception if the result is not MagickTrue */
void c_Gmagick::checkResult(int result) {
  if (MagickTrue == result) 
    return;
  ExceptionType magick_severity;
  char * magick_message = MagickGetException(magick_wand, &magick_severity);
  throwException(magick_message, magick_severity);
}

/* Check the magick_wand is not empty. If it's empty and raise an exception */
void c_Gmagick::checkNotEmpty() {
  if (MagickGetNumberImages(magick_wand) == 0) {
    throwException("Can not process empty Gmagick object", ImageError);
  }
}

/* Check the magick_wand is not empty. If it's empty and raise an exception */
void c_Gmagick::checkHasFormat() {
  char *buffer = MagickGetImageFormat(magick_wand);
  if (!buffer || *buffer == '\0') {
    MagickRelinquishMemory(buffer);
    throwException("Image has no format", ImageError);
  } else {
    MagickRelinquishMemory(buffer);
  }
}

bool c_Gmagick::adjust_dimensions(bool bestfit, long desired_width, long desired_height, 
                                  long *new_width, long *new_height) {
  long orig_width, orig_height;

  orig_width  = MagickGetImageWidth(magick_wand); 
  orig_height = MagickGetImageHeight(magick_wand);

  if ((orig_width == desired_width) && (orig_height == desired_height)) { 
    *new_width  = desired_width; 
    *new_height = desired_height;
    return true;
  }

  if (bestfit) { 
    double ratio_x, ratio_y; 

    if (desired_width <= 0 || desired_height <= 0) {
      return false;
    }

    ratio_x = (double)desired_width / (double)orig_width; 
    ratio_y = (double)desired_height / (double)orig_height; 

    if (ratio_x < ratio_y) { 
      *new_width  = desired_width; 
      *new_height = ratio_x * (double)orig_height; 
    } else { 
      *new_height = desired_height; 
      *new_width  = ratio_y * (double)orig_width; 
    } 

    *new_width  = (*new_width < 1)  ? 1 : *new_width; 
    *new_height = (*new_height < 1) ? 1 : *new_height; 
  } else { 
    double ratio; 

    if (desired_width <= 0 && desired_height <= 0) { 
      return false;
    }

    if (desired_width <= 0 || desired_height <= 0) {		 
      if (desired_width <= 0) { 
        ratio = (double)orig_height / (double)desired_height; 
        *new_width  = orig_width / ratio;
        *new_height = desired_height;
      } else { 
        ratio = (double)orig_width / (double)desired_width; 
        *new_height = orig_height / ratio;
        *new_width  = desired_width;
      }
    } else {
      *new_width  = desired_width; 
      *new_height = desired_height;
    }
  }
  return true;
}

String c_Gmagick::t_getcopyright() {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::getcopyright);
  return MagickGetCopyright();
}

Object c_Gmagick::t_readimage(CStrRef filename) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::readimage);
  int result = MagickReadImage(magick_wand, filename.data());
  checkResult(result);

  return this;
}

Object c_Gmagick::t_readimageblob(CStrRef blob, CStrRef filename) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::readimageblob);
  if (blob->size() == 0) {
    throwException("Zero size image string passed", BlobError);
  }

  int result = MagickReadImageBlob(magick_wand, (const unsigned char *)blob->data(), blob->size());
  checkResult(result);

  const char * c_filename;
  if (filename.isNull()) {
    c_filename = "";
  } else {
    c_filename = filename.data();
  }

  result = MagickSetImageFilename(magick_wand, c_filename);
  checkResult(result);

  return this;
}

// Returns the current image sequence as a string
String c_Gmagick::t_getimageblob() {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::getimageblob);
  unsigned char * image_contents;
  size_t image_size;

  checkNotEmpty();
  checkHasFormat();

  MagickResetIterator(magick_wand);
  image_contents = MagickWriteImageBlob(magick_wand, &image_size);
  if (!image_contents) {
    return null;
  }    

  String ret((const char *)image_contents, image_size, CopyString);
  MagickRelinquishMemory(image_contents);
  return ret;
}

String c_Gmagick::t_getimagefilename() {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::getimagefilename);
  checkNotEmpty();

  char *filename = MagickGetImageFilename(magick_wand);

  if (!filename) {
    return null;
  }

  return String(filename, strlen(filename), AttachString);
}

int64 c_Gmagick::t_getimagetype() {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::getimagetype);
  checkNotEmpty();

  return MagickGetImageType(magick_wand);
}
 
String c_Gmagick::t_getimageformat() {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::getimageformat);
  checkNotEmpty();

  char * format = MagickGetImageFormat(magick_wand);

  if (!format) {
    return null;
  }

  return String(format, strlen(format), AttachString);
}

int64 c_Gmagick::t_getimagecolorspace() {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::getimagecolorspace);
  checkNotEmpty();

  return MagickGetImageColorspace(magick_wand);
}
 
String c_Gmagick::t_getimageprofile(CStrRef profile) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::getimageprofile);
  checkNotEmpty();

  size_t profile_size;
  unsigned char *ret_profile = MagickGetImageProfile(magick_wand, profile.data(), &profile_size);

  if (!ret_profile) {
    return null;
  }

  String ret((char *)ret_profile, profile_size, CopyString);
  MagickRelinquishMemory(ret_profile);
  return ret;
}

// Adds or removes a ICC, IPTC, or generic profile from an image.  
// If the profile is NULL, it is removed from the image otherwise added.  
// Use a name of '*' and a profile of NULL to remove all profiles from the image.
Object c_Gmagick::t_profileimage(CStrRef name, CStrRef profile) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::profileimage);
  checkNotEmpty();

  int result;
  if (profile.isNull()) {
       result = MagickProfileImage(magick_wand, name, NULL, 0);
  } else {
       result = MagickProfileImage(magick_wand, name, (const unsigned char *)profile->data(), profile.size());
  }

  checkResult(result);

  return this;
}

Object c_Gmagick::t_setcompressionquality(int64 quality) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::setcompressionquality);
  checkNotEmpty();

  int result = MagickSetCompressionQuality(magick_wand, quality);

  checkResult(result);

  return this;
}

int64 c_Gmagick::t_getimagewidth() {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::getimagewidth);
  checkNotEmpty();

  return MagickGetImageWidth(magick_wand);
}

int64 c_Gmagick::t_getimageheight() {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::getimageheight);
  checkNotEmpty();

  return MagickGetImageHeight(magick_wand);
}

Object c_Gmagick::t_resizeimage(int64 columns, int64 rows, int64 filter_type, double blur, bool fit) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::resizeimage);
  checkNotEmpty();
  
  // Adjust width and height to work like php extension of gmagick
  long width, height;
  if (! adjust_dimensions(fit, columns, rows, &width, &height)) {
    throwException("Unable to calculate image dimensions", ImageError);
  }

  int result = MagickResizeImage(magick_wand, width, height, static_cast<FilterTypes>(filter_type), blur);
  checkResult(result);

  return this;
}

Object c_Gmagick::t_rotateimage(CObjRef background, double degrees) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::rotateimage);
  checkNotEmpty();
  
  c_GmagickPixel *gp = background.getTyped<c_GmagickPixel>();
  int result = MagickRotateImage(magick_wand, gp->get_PixelWand(), degrees);
  checkResult(result);

  return this;
}

Object c_Gmagick::t_setimageformat(CStrRef format) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::setimageformat);
  checkNotEmpty();
  
  int result = MagickSetImageFormat(magick_wand, format->data());
  checkResult(result);

  return this;
}

Object c_Gmagick::t_cropimage(int64 width, int64 height, int64 x, int64 y) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::cropimage);
  checkNotEmpty();
  
  int result = MagickCropImage(magick_wand, width, height, x, y);
  checkResult(result);

  return this;
}

Object c_Gmagick::t_writeimage(CStrRef filename, bool all_frames) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::writeimage);
  checkNotEmpty();

  const char *c_filename;

  if (filename.isNull()) {
    c_filename = MagickGetImageFilename(magick_wand);
    if (!c_filename) {
  	  throwException("No image filename specified", FileOpenFatalError);
    }
  } else {
    c_filename = filename->data();
  }

  int filename_len = strlen(c_filename);

  if (!filename_len) {
    throwException("Unable to write the image. Empty filename string provided", FileOpenFatalError);
  }

  int result;
  if (all_frames) {
    result = MagickWriteImage(magick_wand, c_filename);
  } else {
    result = MagickWriteImages(magick_wand, c_filename, MagickTrue);
  }

  checkResult(result);

  return this;
}

Object c_Gmagick::t_borderimage(CObjRef bordercolor, int64 width, int64 height) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::borderimage);
  checkNotEmpty();

  c_GmagickPixel *gp = bordercolor.getTyped<c_GmagickPixel>();
  int result = MagickBorderImage(magick_wand, gp->get_PixelWand(), width, height);
  checkResult(result);

  return this;
}

///////////////////////////////////////////////////////////////////////////////
}
