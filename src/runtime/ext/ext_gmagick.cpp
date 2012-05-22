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

const int q_Gmagick___COMPOSITE_DEFAULT = OverCompositeOp;
const int q_Gmagick___COMPOSITE_UNDEFINED = UndefinedCompositeOp;
const int q_Gmagick___COMPOSITE_NO = NoCompositeOp;
const int q_Gmagick___COMPOSITE_ADD = AddCompositeOp;
const int q_Gmagick___COMPOSITE_ATOP = AtopCompositeOp;
const int q_Gmagick___COMPOSITE_BUMPMAP = BumpmapCompositeOp;
const int q_Gmagick___COMPOSITE_CLEAR = ClearCompositeOp;
const int q_Gmagick___COMPOSITE_COLORIZE = ColorizeCompositeOp;
const int q_Gmagick___COMPOSITE_COPYBLACK = CopyBlackCompositeOp;
const int q_Gmagick___COMPOSITE_COPYBLUE = CopyBlueCompositeOp;
const int q_Gmagick___COMPOSITE_COPY = CopyCompositeOp;
const int q_Gmagick___COMPOSITE_COPYCYAN = CopyCyanCompositeOp;
const int q_Gmagick___COMPOSITE_COPYGREEN = CopyGreenCompositeOp;
const int q_Gmagick___COMPOSITE_COPYMAGENTA = CopyMagentaCompositeOp;
const int q_Gmagick___COMPOSITE_COPYOPACITY = CopyOpacityCompositeOp;
const int q_Gmagick___COMPOSITE_COPYRED = CopyRedCompositeOp;
const int q_Gmagick___COMPOSITE_COPYYELLOW = CopyYellowCompositeOp;
const int q_Gmagick___COMPOSITE_DARKEN = DarkenCompositeOp;
const int q_Gmagick___COMPOSITE_DIFFERENCE = DifferenceCompositeOp;
const int q_Gmagick___COMPOSITE_DISPLACE = DisplaceCompositeOp;
const int q_Gmagick___COMPOSITE_DISSOLVE = DissolveCompositeOp;
const int q_Gmagick___COMPOSITE_HUE = HueCompositeOp;
const int q_Gmagick___COMPOSITE_IN = InCompositeOp;
const int q_Gmagick___COMPOSITE_LIGHTEN = LightenCompositeOp;
const int q_Gmagick___COMPOSITE_LUMINIZE = LuminizeCompositeOp;
const int q_Gmagick___COMPOSITE_MINUS = MinusCompositeOp;
const int q_Gmagick___COMPOSITE_MODULATE = ModulateCompositeOp;
const int q_Gmagick___COMPOSITE_MULTIPLY = MultiplyCompositeOp;
const int q_Gmagick___COMPOSITE_OUT = OutCompositeOp;
const int q_Gmagick___COMPOSITE_OVER = OverCompositeOp;
const int q_Gmagick___COMPOSITE_OVERLAY = OverlayCompositeOp;
const int q_Gmagick___COMPOSITE_PLUS = PlusCompositeOp;
const int q_Gmagick___COMPOSITE_REPLACE = ReplaceCompositeOp;
const int q_Gmagick___COMPOSITE_SATURATE = SaturateCompositeOp;
const int q_Gmagick___COMPOSITE_SCREEN = ScreenCompositeOp;
const int q_Gmagick___COMPOSITE_SUBTRACT = SubtractCompositeOp;
const int q_Gmagick___COMPOSITE_THRESHOLD = ThresholdCompositeOp;
const int q_Gmagick___COMPOSITE_XOR = XorCompositeOp;
const int q_Gmagick___MONTAGEMODE_FRAME = FrameMode;
const int q_Gmagick___MONTAGEMODE_UNFRAME = UnframeMode;
const int q_Gmagick___MONTAGEMODE_CONCATENATE = ConcatenateMode;
const int q_Gmagick___STYLE_NORMAL = NormalStyle ;
const int q_Gmagick___STYLE_ITALIC = ItalicStyle ;
const int q_Gmagick___STYLE_OBLIQUE = ObliqueStyle ;
const int q_Gmagick___STYLE_ANY = AnyStyle ;
const int q_Gmagick___FILTER_UNDEFINED = UndefinedFilter ;
const int q_Gmagick___FILTER_POINT = PointFilter ;
const int q_Gmagick___FILTER_BOX = BoxFilter ;
const int q_Gmagick___FILTER_TRIANGLE = TriangleFilter ;
const int q_Gmagick___FILTER_HERMITE = HermiteFilter ;
const int q_Gmagick___FILTER_HANNING = HanningFilter ;
const int q_Gmagick___FILTER_HAMMING = HammingFilter ;
const int q_Gmagick___FILTER_BLACKMAN = BlackmanFilter ;
const int q_Gmagick___FILTER_GAUSSIAN = GaussianFilter ;
const int q_Gmagick___FILTER_QUADRATIC = QuadraticFilter ;
const int q_Gmagick___FILTER_CUBIC = CubicFilter ;
const int q_Gmagick___FILTER_CATROM = CatromFilter ;
const int q_Gmagick___FILTER_MITCHELL = MitchellFilter ;
const int q_Gmagick___FILTER_LANCZOS = LanczosFilter ;
const int q_Gmagick___FILTER_BESSEL = BesselFilter ;
const int q_Gmagick___FILTER_SINC = SincFilter ;
const int q_Gmagick___IMGTYPE_UNDEFINED = UndefinedType ;
const int q_Gmagick___IMGTYPE_BILEVEL = BilevelType ;
const int q_Gmagick___IMGTYPE_GRAYSCALE = GrayscaleType ;
const int q_Gmagick___IMGTYPE_GRAYSCALEMATTE = GrayscaleMatteType ;
const int q_Gmagick___IMGTYPE_PALETTE =  PaletteType ;
const int q_Gmagick___IMGTYPE_PALETTEMATTE = PaletteMatteType ;
const int q_Gmagick___IMGTYPE_TRUECOLOR = TrueColorType ;
const int q_Gmagick___IMGTYPE_TRUECOLORMATTE = TrueColorMatteType ;
const int q_Gmagick___IMGTYPE_COLORSEPARATION = ColorSeparationType ;
const int q_Gmagick___IMGTYPE_COLORSEPARATIONMATTE = ColorSeparationMatteType ;
const int q_Gmagick___IMGTYPE_OPTIMIZE = OptimizeType ;
const int q_Gmagick___RESOLUTION_UNDEFINED = UndefinedResolution ;
const int q_Gmagick___RESOLUTION_PIXELSPERINCH = PixelsPerInchResolution ;
const int q_Gmagick___RESOLUTION_PIXELSPERCENTIMETER = PixelsPerCentimeterResolution;
const int q_Gmagick___COMPRESSION_UNDEFINED = UndefinedCompression;
const int q_Gmagick___COMPRESSION_NO = NoCompression;
const int q_Gmagick___COMPRESSION_BZIP = BZipCompression;
const int q_Gmagick___COMPRESSION_FAX = FaxCompression;
const int q_Gmagick___COMPRESSION_GROUP4 = Group4Compression;
const int q_Gmagick___COMPRESSION_JPEG = JPEGCompression;
const int q_Gmagick___COMPRESSION_LOSSLESSJPEG = LosslessJPEGCompression;
const int q_Gmagick___COMPRESSION_LZW = LZWCompression;
const int q_Gmagick___COMPRESSION_RLE = RLECompression;
const int q_Gmagick___COMPRESSION_ZIP = ZipCompression;
const int q_Gmagick___PAINT_POINT = PointMethod;
const int q_Gmagick___PAINT_REPLACE = ReplaceMethod;
const int q_Gmagick___PAINT_FLOODFILL = FloodfillMethod;
const int q_Gmagick___PAINT_FILLTOBORDER = FillToBorderMethod;
const int q_Gmagick___PAINT_RESET = ResetMethod;
const int q_Gmagick___GRAVITY_NORTHWEST = NorthWestGravity;
const int q_Gmagick___GRAVITY_NORTH = NorthGravity;
const int q_Gmagick___GRAVITY_NORTHEAST = NorthEastGravity;
const int q_Gmagick___GRAVITY_WEST = WestGravity;
const int q_Gmagick___GRAVITY_CENTER = CenterGravity;
const int q_Gmagick___GRAVITY_EAST = EastGravity;
const int q_Gmagick___GRAVITY_SOUTHWEST = SouthWestGravity;
const int q_Gmagick___GRAVITY_SOUTH = SouthGravity;
const int q_Gmagick___GRAVITY_SOUTHEAST = SouthEastGravity;
const int q_Gmagick___STRETCH_NORMAL = NormalStretch;
const int q_Gmagick___STRETCH_ULTRACONDENSED = UltraCondensedStretch;
const int q_Gmagick___STRETCH_CONDENSED = CondensedStretch;
const int q_Gmagick___STRETCH_SEMICONDENSED = SemiCondensedStretch;
const int q_Gmagick___STRETCH_SEMIEXPANDED = SemiExpandedStretch;
const int q_Gmagick___STRETCH_EXPANDED = ExpandedStretch;
const int q_Gmagick___STRETCH_EXTRAEXPANDED = ExtraExpandedStretch;
const int q_Gmagick___STRETCH_ULTRAEXPANDED = UltraExpandedStretch;
const int q_Gmagick___STRETCH_ANY = AnyStretch;
const int q_Gmagick___ALIGN_UNDEFINED = UndefinedAlign;
const int q_Gmagick___ALIGN_LEFT = LeftAlign;
const int q_Gmagick___ALIGN_CENTER = CenterAlign;
const int q_Gmagick___ALIGN_RIGHT = RightAlign;
const int q_Gmagick___DECORATION_NO = NoDecoration;
const int q_Gmagick___DECORATION_UNDERLINE = UnderlineDecoration;
const int q_Gmagick___DECORATION_OVERLINE = OverlineDecoration;
const int q_Gmagick___DECORATION_LINETROUGH = LineThroughDecoration;
const int q_Gmagick___NOISE_UNIFORM = UniformNoise;
const int q_Gmagick___NOISE_GAUSSIAN = GaussianNoise;
const int q_Gmagick___NOISE_MULTIPLICATIVEGAUSSIAN = MultiplicativeGaussianNoise;
const int q_Gmagick___NOISE_IMPULSE = ImpulseNoise;
const int q_Gmagick___NOISE_LAPLACIAN = LaplacianNoise ;
const int q_Gmagick___NOISE_POISSON = PoissonNoise;
const int q_Gmagick___CHANNEL_UNDEFINED = UndefinedChannel;
const int q_Gmagick___CHANNEL_RED = RedChannel;
const int q_Gmagick___CHANNEL_GRAY = GrayChannel;
const int q_Gmagick___CHANNEL_CYAN = CyanChannel;
const int q_Gmagick___CHANNEL_GREEN = GreenChannel;
const int q_Gmagick___CHANNEL_MAGENTA = MagentaChannel;
const int q_Gmagick___CHANNEL_BLUE = BlueChannel;
const int q_Gmagick___CHANNEL_YELLOW = YellowChannel;
const int q_Gmagick___CHANNEL_OPACITY = OpacityChannel;
const int q_Gmagick___CHANNEL_MATTE = MatteChannel;
const int q_Gmagick___CHANNEL_BLACK = BlackChannel;
const int q_Gmagick___CHANNEL_INDEX = IndexChannel;
const int q_Gmagick___CHANNEL_ALL = AllChannels;
const int q_Gmagick___METRIC_UNDEFINED = UndefinedMetric;
const int q_Gmagick___METRIC_MEANABSOLUTEERROR = MeanAbsoluteErrorMetric;
const int q_Gmagick___METRIC_MEANSQUAREERROR = MeanSquaredErrorMetric;
const int q_Gmagick___METRIC_PEAKABSOLUTEERROR = PeakAbsoluteErrorMetric;
const int q_Gmagick___METRIC_PEAKSIGNALTONOISERATIO = PeakSignalToNoiseRatioMetric;
const int q_Gmagick___METRIC_ROOTMEANSQUAREDERROR = RootMeanSquaredErrorMetric;
const int q_Gmagick___PIXEL_CHAR = CharPixel;
const int q_Gmagick___PIXEL_DOUBLE = DoublePixel;
const int q_Gmagick___PIXEL_FLOAT = FloatPixel;
const int q_Gmagick___PIXEL_INTEGER = IntegerPixel;
const int q_Gmagick___PIXEL_LONG = LongPixel;
const int q_Gmagick___PIXEL_SHORT = ShortPixel;
const int q_Gmagick___COLORSPACE_UNDEFINED = UndefinedColorspace;
const int q_Gmagick___COLORSPACE_RGB = RGBColorspace;
const int q_Gmagick___COLORSPACE_GRAY = GRAYColorspace;
const int q_Gmagick___COLORSPACE_TRANSPARENT = TransparentColorspace;
const int q_Gmagick___COLORSPACE_OHTA = OHTAColorspace;
const int q_Gmagick___COLORSPACE_LAB = LABColorspace;
const int q_Gmagick___COLORSPACE_XYZ = XYZColorspace;
const int q_Gmagick___COLORSPACE_YCBCR = YCbCrColorspace;
const int q_Gmagick___COLORSPACE_YCC = YCCColorspace;
const int q_Gmagick___COLORSPACE_YIQ = YIQColorspace;
const int q_Gmagick___COLORSPACE_YPBPR = YPbPrColorspace;
const int q_Gmagick___COLORSPACE_YUV = YUVColorspace;
const int q_Gmagick___COLORSPACE_CMYK = CMYKColorspace;
const int q_Gmagick___COLORSPACE_SRGB = sRGBColorspace;
const int q_Gmagick___COLORSPACE_HSL = HSLColorspace;
const int q_Gmagick___COLORSPACE_HWB = HWBColorspace;
const int q_Gmagick___VIRTUALPIXELMETHOD_UNDEFINED = UndefinedVirtualPixelMethod;
const int q_Gmagick___VIRTUALPIXELMETHOD_CONSTANT = ConstantVirtualPixelMethod;
const int q_Gmagick___VIRTUALPIXELMETHOD_EDGE = EdgeVirtualPixelMethod;
const int q_Gmagick___VIRTUALPIXELMETHOD_MIRROR = MirrorVirtualPixelMethod;
const int q_Gmagick___VIRTUALPIXELMETHOD_TILE = TileVirtualPixelMethod;
const int q_Gmagick___PREVIEW_UNDEFINED = UndefinedPreview;
const int q_Gmagick___PREVIEW_ROTATE = RotatePreview;
const int q_Gmagick___PREVIEW_SHEAR = ShearPreview;
const int q_Gmagick___PREVIEW_ROLL = RollPreview;
const int q_Gmagick___PREVIEW_HUE = HuePreview;
const int q_Gmagick___PREVIEW_SATURATION = SaturationPreview;
const int q_Gmagick___PREVIEW_BRIGHTNESS = BrightnessPreview;
const int q_Gmagick___PREVIEW_GAMMA = GammaPreview;
const int q_Gmagick___PREVIEW_SPIFF = SpiffPreview;
const int q_Gmagick___PREVIEW_DULL = DullPreview;
const int q_Gmagick___PREVIEW_GRAYSCALE = GrayscalePreview;
const int q_Gmagick___PREVIEW_QUANTIZE = QuantizePreview;
const int q_Gmagick___PREVIEW_DESPECKLE = DespecklePreview;
const int q_Gmagick___PREVIEW_REDUCENOISE = ReduceNoisePreview;
const int q_Gmagick___PREVIEW_ADDNOISE = AddNoisePreview;
const int q_Gmagick___PREVIEW_SHARPEN = SharpenPreview;
const int q_Gmagick___PREVIEW_BLUR = BlurPreview;
const int q_Gmagick___PREVIEW_THRESHOLD = ThresholdPreview;
const int q_Gmagick___PREVIEW_EDGEDETECT = EdgeDetectPreview;
const int q_Gmagick___PREVIEW_SPREAD = SpreadPreview;
const int q_Gmagick___PREVIEW_SOLARIZE = SolarizePreview;
const int q_Gmagick___PREVIEW_SHADE = ShadePreview;
const int q_Gmagick___PREVIEW_RAISE = RaisePreview;
const int q_Gmagick___PREVIEW_SEGMENT = SegmentPreview;
const int q_Gmagick___PREVIEW_SWIRL = SwirlPreview;
const int q_Gmagick___PREVIEW_IMPLODE = ImplodePreview;
const int q_Gmagick___PREVIEW_WAVE = WavePreview;
const int q_Gmagick___PREVIEW_OILPAINT = OilPaintPreview;
const int q_Gmagick___PREVIEW_CHARCOALDRAWING = CharcoalDrawingPreview;
const int q_Gmagick___PREVIEW_JPEG = JPEGPreview;
const int q_Gmagick___RENDERINGINTENT_UNDEFINED = UndefinedIntent;
const int q_Gmagick___RENDERINGINTENT_SATURATION = SaturationIntent;
const int q_Gmagick___RENDERINGINTENT_PERCEPTUAL = PerceptualIntent;
const int q_Gmagick___RENDERINGINTENT_ABSOLUTE = AbsoluteIntent;
const int q_Gmagick___RENDERINGINTENT_RELATIVE = RelativeIntent;
const int q_Gmagick___INTERLACE_UNDEFINED = UndefinedInterlace;
const int q_Gmagick___INTERLACE_NO = NoInterlace;
const int q_Gmagick___INTERLACE_LINE = LineInterlace;
const int q_Gmagick___INTERLACE_PLANE = PlaneInterlace;
const int q_Gmagick___INTERLACE_PARTITION = PartitionInterlace;
const int q_Gmagick___FILLRULE_UNDEFINED = UndefinedRule;
const int q_Gmagick___FILLRULE_EVENODD = EvenOddRule;
const int q_Gmagick___FILLRULE_NONZERO = NonZeroRule;
const int q_Gmagick___PATHUNITS_USERSPACE = UserSpace;
const int q_Gmagick___PATHUNITS_USERSPACEONUSE = UserSpaceOnUse;
const int q_Gmagick___PATHUNITS_OBJECTBOUNDINGBOX = ObjectBoundingBox;
const int q_Gmagick___LINECAP_UNDEFINED = UndefinedCap;
const int q_Gmagick___LINECAP_BUTT = ButtCap;
const int q_Gmagick___LINECAP_ROUND = RoundCap;
const int q_Gmagick___LINECAP_SQUARE = SquareCap;
const int q_Gmagick___LINEJOIN_UNDEFINED = UndefinedJoin;
const int q_Gmagick___LINEJOIN_MITER = MiterJoin;
const int q_Gmagick___LINEJOIN_ROUND = RoundJoin;
const int q_Gmagick___LINEJOIN_BEVEL = BevelJoin;
const int q_Gmagick___RESOURCETYPE_UNDEFINED = UndefinedResource;
const int q_Gmagick___RESOURCETYPE_AREA = AreaResource;
const int q_Gmagick___RESOURCETYPE_DISK = DiskResource;
const int q_Gmagick___RESOURCETYPE_FILE = FileResource;
const int q_Gmagick___RESOURCETYPE_MAP = MapResource;
const int q_Gmagick___RESOURCETYPE_MEMORY = MemoryResource;
const int q_Gmagick___DISPOSE_UNDEFINED = UndefinedDispose;
const int q_Gmagick___DISPOSE_NONE = NoneDispose;
const int q_Gmagick___DISPOSE_BACKGROUND = BackgroundDispose;
const int q_Gmagick___DISPOSE_PREVIOUS = PreviousDispose;

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

Object c_Gmagick::t_scaleimage(int64 columns, int64 rows, bool fit) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::scaleimage);
  checkNotEmpty();
  
  // Adjust width and height to work like php extension of gmagick
  long width, height;
  if (! adjust_dimensions(fit, columns, rows, &width, &height)) {
    throwException("Unable to calculate image dimensions", ImageError);
  }

  int result = MagickScaleImage(magick_wand, width, height);
  checkResult(result);

  return this;
}

Object c_Gmagick::t_setimagebackgroundcolor(CObjRef color) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::setimagebackgroundcolor);
  checkNotEmpty();
  c_GmagickPixel *gp = color.getTyped<c_GmagickPixel>();
  int result = MagickSetImageBackgroundColor(magick_wand, gp->get_PixelWand());
  checkResult(result);

  return this;
}

Object c_Gmagick::t_flattenimages() {
  INSTANCE_METHOD_INJECTION_BUILTIN(Gmagick, Gmagick::flattenimages);
  checkNotEmpty();
  MagickResetIterator(magick_wand);
  MagickWand *tmp_wand = MagickFlattenImages(magick_wand);

  if (tmp_wand == (MagickWand *)NULL) {
    throwException("Flatten images failed", ImageError);
  }

  DestroyMagickWand(magick_wand);
  magick_wand = tmp_wand;
  
  return this;
}

///////////////////////////////////////////////////////////////////////////////
}
