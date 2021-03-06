/*
   +----------------------------------------------------------------------+
   | PHP Version 5 / Imagick	                                          |
   +----------------------------------------------------------------------+
   | Copyright (c) 2006-2009 Mikko Koppanen, Scott MacVicar               |
   | Imagemagick (c) ImageMagick Studio LLC                               |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Mikko Kopppanen <mkoppanen@php.net>                          |
   |         Scott MacVicar <scottmac@php.net>                            |
   +----------------------------------------------------------------------+
*/

#include "php_imagick.h"
#include "php_imagick_defs.h"
#include "php_imagick_macros.h"

#include "ext/standard/php_smart_str.h"

ZEND_DECLARE_MODULE_GLOBALS(imagick)

zend_class_entry *php_imagick_sc_entry;
zend_class_entry *php_imagick_exception_class_entry;
zend_class_entry *php_imagickdraw_sc_entry;
zend_class_entry *php_imagickdraw_exception_class_entry;
zend_class_entry *php_imagickpixel_sc_entry;
zend_class_entry *php_imagickpixel_exception_class_entry;
zend_class_entry *php_imagickpixeliterator_sc_entry;
zend_class_entry *php_imagickpixeliterator_exception_class_entry;

#if defined(ZTS) && defined(PHP_WIN32)
static MUTEX_T imagick_mutex;
static THREAD_T imagick_thread_id;
#endif

/* Handlers */
static zend_object_handlers imagick_object_handlers;
static zend_object_handlers imagickdraw_object_handlers;
static zend_object_handlers imagickpixel_object_handlers;
static zend_object_handlers imagickpixeliterator_object_handlers;

/* External API */
MY_IMAGICK_EXPORTS zend_class_entry *php_imagick_get_class_entry()
{
	return php_imagick_sc_entry;
}

MY_IMAGICK_EXPORTS zend_class_entry *php_imagickdraw_get_class_entry()
{
	return php_imagickdraw_sc_entry;
}

MY_IMAGICK_EXPORTS zend_class_entry *php_imagickpixel_get_class_entry()
{
	return php_imagickdraw_sc_entry;
}

/* Imagick */
#if MagickLibVersion > 0x628
	ZEND_BEGIN_ARG_INFO_EX(imagick_shadeimage_args, 0, 0, 3)
		ZEND_ARG_INFO(0, gray)
		ZEND_ARG_INFO(0, azimuth)
		ZEND_ARG_INFO(0, elevation)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagematte_args, 0, 0, 1)
		ZEND_ARG_INFO(0, enable)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_sketchimage_args, 0, 0, 3)
		ZEND_ARG_INFO(0, radius)
		ZEND_ARG_INFO(0, sigma)
		ZEND_ARG_INFO(0, angle)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_pingimageblob_args, 0, 0, 1)
		ZEND_ARG_INFO(0, imageContents)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_pingimagefile_args, 0, 0, 1)
		ZEND_ARG_INFO(0, fp)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_trimimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, fuzz)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_waveimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, amplitude)
		ZEND_ARG_INFO(0, waveLenght)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_vignetteimage_args, 0, 0, 4)
		ZEND_ARG_INFO(0, blackPoint)
		ZEND_ARG_INFO(0, whitePoint)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_adaptiveresizeimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, columns)
		ZEND_ARG_INFO(0, rows)
		ZEND_ARG_INFO(0, bestfit)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_contraststretchimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, blackPoint)
		ZEND_ARG_INFO(0, whitePoint)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_adaptiveblurimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, radius)
		ZEND_ARG_INFO(0, sigma)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_adaptivesharpenimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, radius)
		ZEND_ARG_INFO(0, sigma)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setsizeoffset_args, 0, 0, 3)
		ZEND_ARG_INFO(0, columns)
		ZEND_ARG_INFO(0, rows)
		ZEND_ARG_INFO(0, offset)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_randomthresholdimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, low)
		ZEND_ARG_INFO(0, high)
		ZEND_ARG_INFO(0, CHANNELTYPE)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_compareimagelayers_args, 0, 0, 1)
		ZEND_ARG_INFO(0, LAYER)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_roundcornersimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, xRounding)
		ZEND_ARG_INFO(0, yRounding)
		ZEND_ARG_INFO(0, strokeWidth)
		ZEND_ARG_INFO(0, displace)
		ZEND_ARG_INFO(0, sizeCorrection)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setiteratorindex_args, 0, 0, 1)
		ZEND_ARG_INFO(0, index)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_transformimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, crop)
		ZEND_ARG_INFO(0, geometry)
	ZEND_END_ARG_INFO()
#endif

#if MagickLibVersion > 0x631
	ZEND_BEGIN_ARG_INFO_EX(imagick_polaroidimage_args, 0, 0, 2)
		ZEND_ARG_OBJ_INFO(0, ImagickDraw, ImagickDraw, 0)
		ZEND_ARG_INFO(0, angle)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_getimageproperty_args, 0, 0, 1)
		ZEND_ARG_INFO(0, name)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimageproperty_args, 0, 0, 2)
		ZEND_ARG_INFO(0, name)
		ZEND_ARG_INFO(0, value)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimageinterpolatemethod_args, 0, 0, 1)
		ZEND_ARG_INFO(0, INTERPOLATE)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_linearstretchimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, blackPoint)
		ZEND_ARG_INFO(0, whitePoint)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_extentimage_args, 0, 0, 4)
		ZEND_ARG_INFO(0, width)
		ZEND_ARG_INFO(0, height)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()
#endif

#if MagickLibVersion > 0x633
	ZEND_BEGIN_ARG_INFO_EX(imagick_setimageorientation_args, 0, 0, 1)
		ZEND_ARG_INFO(0, ORIENTATION)
	ZEND_END_ARG_INFO()
#endif

#if MagickLibVersion > 0x634
	ZEND_BEGIN_ARG_INFO_EX(imagick_paintfloodfillimage_args, 0, 0, 6)
		ZEND_ARG_INFO(0, CHANNEL)
		ZEND_ARG_INFO(0, fill)
		ZEND_ARG_INFO(0, fuzz)
		ZEND_ARG_INFO(0, bordercolor)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()
#endif

#if MagickLibVersion > 0x630
	ZEND_BEGIN_ARG_INFO_EX(imagick_setimageopacity_args, 0, 0, 1)
		ZEND_ARG_INFO(0, opacity)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_orderedposterizeimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, threshold_map)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()
#endif

#if MagickLibVersion > 0x635
	ZEND_BEGIN_ARG_INFO_EX(imagick_clutimage_args, 0, 0, 1)
		ZEND_ARG_OBJ_INFO(0, Imagick, Imagick, 0)
		ZEND_ARG_INFO(0, CHANNELTYPE)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_getimageproperties_args, 0, 0, 0)
		ZEND_ARG_INFO(0, pattern)
		ZEND_ARG_INFO(0, values)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_getimageprofiles_args, 0, 0, 0)
		ZEND_ARG_INFO(0, pattern)
		ZEND_ARG_INFO(0, values)
	ZEND_END_ARG_INFO()
#endif

#if MagickLibVersion > 0x635
	ZEND_BEGIN_ARG_INFO_EX(imagick_distortimage_args, 0, 0, 3)
		ZEND_ARG_INFO(0, method)
		ZEND_ARG_INFO(0, arguments)
		ZEND_ARG_INFO(0, bestfit)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_writeimagefile_args, 0, 0, 1)
		ZEND_ARG_INFO(0, handle)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_writeimagesfile_args, 0, 0, 1)
		ZEND_ARG_INFO(0, handle)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_resetimagepage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, page)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimageclipmask_args, 0, 0, 1)
		ZEND_ARG_OBJ_INFO(0, Imagick, Imagick, 0)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_animateimages_args, 0, 0, 1)
		ZEND_ARG_INFO(0, server_name)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_recolorimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, matrix)
	ZEND_END_ARG_INFO()
#endif

#if MagickLibVersion > 0x636
	ZEND_BEGIN_ARG_INFO_EX(imagick_setfont_args, 0, 0, 1)
		ZEND_ARG_INFO(0, font)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setpointsize_args, 0, 0, 1)
		ZEND_ARG_INFO(0, pointsize)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_mergeimagelayers_args, 0, 0, 1)
		ZEND_ARG_INFO(0, LAYERMETHOD)
	ZEND_END_ARG_INFO()
#endif

#if MagickLibVersion > 0x637
	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagealphachannel_args, 0, 0, 1)
		ZEND_ARG_INFO(0, ALPHACHANNELTYPE)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_floodfillpaintimage_args, 0, 0, 6)
		ZEND_ARG_INFO(0, fill)
		ZEND_ARG_INFO(0, fuzz)
		ZEND_ARG_INFO(0, bordercolor)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
		ZEND_ARG_INFO(0, invert)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_opaquepaintimage_args, 0, 0, 4)
		ZEND_ARG_INFO(0, target)
		ZEND_ARG_INFO(0, fill)
		ZEND_ARG_INFO(0, fuzz)
		ZEND_ARG_INFO(0, invert)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_transparentpaintimage_args, 0, 0, 4)
		ZEND_ARG_INFO(0, target)
		ZEND_ARG_INFO(0, alpha)
		ZEND_ARG_INFO(0, fuzz)
		ZEND_ARG_INFO(0, invert)
	ZEND_END_ARG_INFO()
#endif

#if MagickLibVersion > 0x638
	ZEND_BEGIN_ARG_INFO_EX(imagick_liquidrescaleimage_args, 0, 0, 4)
		ZEND_ARG_INFO(0, columns)
		ZEND_ARG_INFO(0, rows)
		ZEND_ARG_INFO(0, delta_x)
		ZEND_ARG_INFO(0, rigidity)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_encipherimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, passphrase)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_decipherimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, passphrase)
	ZEND_END_ARG_INFO()
#endif

#if MagickLibVersion > 0x639
	ZEND_BEGIN_ARG_INFO_EX(imagick_setgravity_args, 0, 0, 1)
		ZEND_ARG_INFO(0, GRAVITY)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_getimagechannelrange_args, 0, 0, 1)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()
#endif

#if MagickLibVersion > 0x642
	ZEND_BEGIN_ARG_INFO_EX(imagick_getimagechanneldistortions_args, 0, 0, 1)
		ZEND_ARG_OBJ_INFO(0, Imagick, Imagick, 0)
		ZEND_ARG_INFO(0, METRICTYPE)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()
#endif

#if MagickLibVersion > 0x643
	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagegravity_args, 0, 0, 1)
		ZEND_ARG_INFO(0, GRAVITY)
	ZEND_END_ARG_INFO()
#endif

#if MagickLibVersion >= 0x645
	ZEND_BEGIN_ARG_INFO_EX(imagick_importimagepixels_args, 0, 0, 7)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
		ZEND_ARG_INFO(0, width)
		ZEND_ARG_INFO(0, height)
		ZEND_ARG_INFO(0, map)
		ZEND_ARG_INFO(0, storage)
		ZEND_ARG_INFO(0, PIXEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_deskewimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, threshold)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_segmentimage_args, 0, 0, 3)
		ZEND_ARG_INFO(0, COLORSPACE)
		ZEND_ARG_INFO(0, cluster_threshold)
		ZEND_ARG_INFO(0, smooth_threshold)
		ZEND_ARG_INFO(0, verbose)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_sparsecolorimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, SPARSE_METHOD)
		ZEND_ARG_INFO(0, arguments)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_remapimage_args, 0, 0, 2)
		ZEND_ARG_OBJ_INFO(0, Imagick, Imagick, 0)
		ZEND_ARG_INFO(0, DITHER)
	ZEND_END_ARG_INFO()
#endif

#if MagickLibVersion > 0x646
	ZEND_BEGIN_ARG_INFO_EX(imagick_exportimagepixels_args, 0, 0, 6)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
		ZEND_ARG_INFO(0, width)
		ZEND_ARG_INFO(0, height)
		ZEND_ARG_INFO(0, map)
		ZEND_ARG_INFO(0, STORAGE)
	ZEND_END_ARG_INFO()
#endif

#if MagickLibVersion > 0x648
	ZEND_BEGIN_ARG_INFO_EX(imagick_getimagechannelkurtosis_args, 0, 0, 0)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_functionimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, FUNCTION)
		ZEND_ARG_INFO(0, arguments)
	ZEND_END_ARG_INFO()
#endif

#if MagickLibVersion > 0x652
	ZEND_BEGIN_ARG_INFO_EX(imagick_haldclutimage_args, 0, 0, 1)
		ZEND_ARG_OBJ_INFO(0, Imagick, Imagick, 0)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()
	
	ZEND_BEGIN_ARG_INFO_EX(imagick_transformimagecolorspace_args, 0, 0, 1)
		ZEND_ARG_INFO(0, COLORSPACE)
	ZEND_END_ARG_INFO()	
#endif

#if MagickLibVersion > 0x656
	ZEND_BEGIN_ARG_INFO_EX(imagick_setimageartifact_args, 0, 0, 2)
		ZEND_ARG_INFO(0, artifact)
		ZEND_ARG_INFO(0, value)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_getimageartifact_args, 0, 0, 1)
		ZEND_ARG_INFO(0, artifact)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_deleteimageartifact_args, 0, 0, 1)
		ZEND_ARG_INFO(0, artifact)
	ZEND_END_ARG_INFO()
	
	ZEND_BEGIN_ARG_INFO_EX(imagick_setcolorspace_args, 0, 0, 1)
		ZEND_ARG_INFO(0, COLORSPACE)
	ZEND_END_ARG_INFO()
#endif

	ZEND_BEGIN_ARG_INFO_EX(imagick_zero_args, 0, 0, 0)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_construct_args, 0, 0, 0)
		ZEND_ARG_INFO(0, files)
	ZEND_END_ARG_INFO()
	
	ZEND_BEGIN_ARG_INFO_EX(imagick_getpixelregioniterator_args, 0, 0, 5)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
		ZEND_ARG_INFO(0, columns)
		ZEND_ARG_INFO(0, rows)
		ZEND_ARG_INFO(0, modify)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_readimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, filename)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_readimages_args, 0, 0, 1)
		ZEND_ARG_INFO(0, filenames)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_readimageblob_args, 0, 0, 1)
		ZEND_ARG_INFO(0, imageContents)
		ZEND_ARG_INFO(0, filename)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimageformat_args, 0, 0, 1)
		ZEND_ARG_INFO(0, imageFormat)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_scaleimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, width)
		ZEND_ARG_INFO(0, height)
		ZEND_ARG_INFO(0, bestfit)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_writeimage_args, 0, 0, 0)
		ZEND_ARG_INFO(0, filename)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_writeimages_args, 0, 0, 2)
		ZEND_ARG_INFO(0, filename)
		ZEND_ARG_INFO(0, adjoin)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_blurimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, radius)
		ZEND_ARG_INFO(0, sigma)
		ZEND_ARG_INFO(0, CHANNELTYPE)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_thumbnailimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, width)
		ZEND_ARG_INFO(0, height)
		ZEND_ARG_INFO(0, bestfit)
		ZEND_ARG_INFO(0, fill)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_cropthumbnailimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, width)
		ZEND_ARG_INFO(0, height)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagefilename_args, 0, 0, 1)
		ZEND_ARG_INFO(0, filename)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimageindex_args, 0, 0, 1)
		ZEND_ARG_INFO(0, index)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_commentimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, comment)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_cropimage_args, 0, 0, 4)
		ZEND_ARG_INFO(0, width)
		ZEND_ARG_INFO(0, height)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_labelimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, label)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_drawimage_args, 0, 0, 1)
		ZEND_ARG_OBJ_INFO(0, ImagickDraw, ImagickDraw, 0)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagecompressionquality_args, 0, 0, 1)
		ZEND_ARG_INFO(0, quality)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_annotateimage_args, 0, 0, 5)
		ZEND_ARG_OBJ_INFO(0, ImagickDraw, ImagickDraw, 0)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
		ZEND_ARG_INFO(0, angle)
		ZEND_ARG_INFO(0, text)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_compositeimage_args, 0, 0, 4)
		ZEND_ARG_OBJ_INFO(0, Imagick, Imagick, 0)
		ZEND_ARG_INFO(0, COMPOSITE)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
		ZEND_ARG_INFO(0, CHANNELTYPE)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_modulateimage_args, 0, 0, 3)
		ZEND_ARG_INFO(0, brightness)
		ZEND_ARG_INFO(0, saturation)
		ZEND_ARG_INFO(0, hue)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_montageimage_args, 0, 0, 5)
		ZEND_ARG_OBJ_INFO(0, ImagickDraw, ImagickDraw, 0)
		ZEND_ARG_INFO(0, tileGeometry)
		ZEND_ARG_INFO(0, thumbnailGeometry)
		ZEND_ARG_INFO(0, MONTAGEMODE)
		ZEND_ARG_INFO(0, frame)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_identifyimage_args, 0, 0, 0)
		ZEND_ARG_INFO(0, appendRawOutput)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_thresholdimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, threshold)
		ZEND_ARG_INFO(0, CHANNELTYPE)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_adaptivethresholdimage_args, 0, 0, 3)
		ZEND_ARG_INFO(0, width)
		ZEND_ARG_INFO(0, height)
		ZEND_ARG_INFO(0, offset)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_blackthresholdimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, color)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_whitethresholdimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, color)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_appendimages_args, 0, 0, 1)
		ZEND_ARG_INFO(0, stack)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_charcoalimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, radius)
		ZEND_ARG_INFO(0, sigma)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_normalizeimage_args, 0, 0, 0)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_oilpaintimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, radius)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_posterizeimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, levels)
		ZEND_ARG_INFO(0, dither)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_radialblurimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, angle)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_raiseimage_args, 0, 0, 5)
		ZEND_ARG_INFO(0, width)
		ZEND_ARG_INFO(0, height)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
		ZEND_ARG_INFO(0, raise)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_resampleimage_args, 0, 0, 4)
		ZEND_ARG_INFO(0, xResolution)
		ZEND_ARG_INFO(0, yResolution)
		ZEND_ARG_INFO(0, FILTER)
		ZEND_ARG_INFO(0, blur)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_resizeimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
		ZEND_ARG_INFO(0, filter)
		ZEND_ARG_INFO(0, blur)
		ZEND_ARG_INFO(0, bestfit)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_rollimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_rotateimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, color)
		ZEND_ARG_INFO(0, degrees)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_sampleimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, columns)
		ZEND_ARG_INFO(0, rows)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_solarizeimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, threshold)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_shadowimage_args, 0, 0, 4)
		ZEND_ARG_INFO(0, opacity)
		ZEND_ARG_INFO(0, sigma)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimageattribute_args, 0, 0, 2)
		ZEND_ARG_INFO(0, key)
		ZEND_ARG_INFO(0, value)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_getimageattribute_args, 0, 0, 1)
		ZEND_ARG_INFO(0, key)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagebackgroundcolor_args, 0, 0, 1)
		ZEND_ARG_INFO(0, color)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagecompose_args, 0, 0, 1)
		ZEND_ARG_INFO(0, COMPOSITE)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagecompression_args, 0, 0, 1)
		ZEND_ARG_INFO(0, COMPRESSION)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagedelay_args, 0, 0, 1)
		ZEND_ARG_INFO(0, delay)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagedepth_args, 0, 0, 1)
		ZEND_ARG_INFO(0, depth)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagegamma_args, 0, 0, 1)
		ZEND_ARG_INFO(0, gamma)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimageiterations_args, 0, 0, 1)
		ZEND_ARG_INFO(0, iterations)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagemattecolor_args, 0, 0, 1)
		ZEND_ARG_INFO(0, color)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagepage_args, 0, 0, 4)
		ZEND_ARG_INFO(0, width)
		ZEND_ARG_INFO(0, height)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimageprogressmonitor_args, 0, 0, 1)
		ZEND_ARG_INFO(0, filename)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimageresolution_args, 0, 0, 2)
		ZEND_ARG_INFO(0, xResolution)
		ZEND_ARG_INFO(0, yResolution)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagescene_args, 0, 0, 1)
		ZEND_ARG_INFO(0, scene)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagetickspersecond_args, 0, 0, 1)
		ZEND_ARG_INFO(0, ticksPerSecond)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagetype_args, 0, 0, 1)
		ZEND_ARG_INFO(0, IMGTYPE)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimageunits_args, 0, 0, 1)
		ZEND_ARG_INFO(0, RESOLUTION)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_sharpenimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, radius)
		ZEND_ARG_INFO(0, sigma)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_shaveimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, columns)
		ZEND_ARG_INFO(0, rows)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_shearimage_args, 0, 0, 3)
		ZEND_ARG_INFO(0, color)
		ZEND_ARG_INFO(0, xShear)
		ZEND_ARG_INFO(0, yShear)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_spliceimage_args, 0, 0, 4)
		ZEND_ARG_INFO(0, width)
		ZEND_ARG_INFO(0, height)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_pingimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, filename)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_readimagefile_args, 0, 0, 1)
		ZEND_ARG_INFO(0, fp)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_displayimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, serverName)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_displayimages_args, 0, 0, 1)
		ZEND_ARG_INFO(0, serverName)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_spreadimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, radius)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_swirlimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, degrees)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_queryformats_args, 0, 0, 1)
		ZEND_ARG_INFO(0, pattern)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_queryfonts_args, 0, 0, 1)
		ZEND_ARG_INFO(0, pattern)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_queryfontmetrics_args, 0, 0, 2)
		ZEND_ARG_OBJ_INFO(0, ImagickDraw, ImagickDraw, 0)
		ZEND_ARG_INFO(0, text)
		ZEND_ARG_INFO(0, multiline)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_steganoimage_args, 0, 0, 2)
		ZEND_ARG_OBJ_INFO(0, Imagick, Imagick, 0)
		ZEND_ARG_INFO(0, offset)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_addnoiseimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, NOISE)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_motionblurimage_args, 0, 0, 3)
		ZEND_ARG_INFO(0, radius)
		ZEND_ARG_INFO(0, sigma)
		ZEND_ARG_INFO(0, angle)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_morphimages_args, 0, 0, 1)
		ZEND_ARG_INFO(0, frames)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_affinetransformimage_args, 0, 0, 1)
		ZEND_ARG_OBJ_INFO(0, ImagickDraw, ImagickDraw, 0)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_borderimage_args, 0, 0, 3)
		ZEND_ARG_INFO(0, color)
		ZEND_ARG_INFO(0, width)
		ZEND_ARG_INFO(0, height)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_chopimage_args, 0, 0, 4)
		ZEND_ARG_INFO(0, width)
		ZEND_ARG_INFO(0, height)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_clippathimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, pathname)
		ZEND_ARG_INFO(0, inside)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_colorfloodfillimage_args, 0, 0, 5)
		ZEND_ARG_INFO(0, fill_color)
		ZEND_ARG_INFO(0, fuzz)
		ZEND_ARG_INFO(0, border_color)
		ZEND_ARG_INFO(0, y)
		ZEND_ARG_INFO(0, x)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_colorizeimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, colorize_color)
		ZEND_ARG_INFO(0, opacity)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_compareimagechannels_args, 0, 0, 3)
		ZEND_ARG_OBJ_INFO(0, Imagick, Imagick, 0)
		ZEND_ARG_INFO(0, CHANNEL)
		ZEND_ARG_INFO(0, METRIC)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_compareimages_args, 0, 0, 2)
		ZEND_ARG_OBJ_INFO(0, Imagick, Imagick, 0)
		ZEND_ARG_INFO(0, METRIC)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_contrastimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, sharpen)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_convolveimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, kernel)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_cyclecolormapimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, displace)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_edgeimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, radius)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_embossimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, radius)
		ZEND_ARG_INFO(0, sigma)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_evaluateimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, EVALUATE)
		ZEND_ARG_INFO(0, constant)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_frameimage_args, 0, 0, 5)
		ZEND_ARG_INFO(0, color)
		ZEND_ARG_INFO(0, width)
		ZEND_ARG_INFO(0, height)
		ZEND_ARG_INFO(0, innerBevel)
		ZEND_ARG_INFO(0, outerBevel)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_fximage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, expression)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_gammaimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, gamma)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_gaussianblurimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, radius)
		ZEND_ARG_INFO(0, sigma)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_getimagechanneldepth_args, 0, 0, 1)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_getimagechanneldistortion_args, 0, 0, 3)
		ZEND_ARG_OBJ_INFO(0, Imagick, Imagick, 0)
		ZEND_ARG_INFO(0, CHANNEL)
		ZEND_ARG_INFO(0, METRIC)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_getimagechannelextrema_args, 0, 0, 1)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_getimagechannelmean_args, 0, 0, 1)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_getimagecolormapcolor_args, 0, 0, 1)
		ZEND_ARG_INFO(0, index)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_getimagedistortion_args, 0, 0, 2)
		ZEND_ARG_OBJ_INFO(0, Imagick, Imagick, 0)
		ZEND_ARG_INFO(0, METRIC)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_getimagepixelcolor_args, 0, 0, 2)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_getimageprofile_args, 0, 0, 1)
		ZEND_ARG_INFO(0, name)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_getimagetotalinkdensity_args, 0, 0, 1)
		ZEND_ARG_INFO(0, radius)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_getimageregion_args, 0, 0, 4)
		ZEND_ARG_INFO(0, width)
		ZEND_ARG_INFO(0, height)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_levelimage_args, 0, 0, 3)
		ZEND_ARG_INFO(0, blackPoint)
		ZEND_ARG_INFO(0, gamma)
		ZEND_ARG_INFO(0, whitePoint)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_mapimage_args, 0, 0, 2)
		ZEND_ARG_OBJ_INFO(0, Imagick, Imagick, 0)
		ZEND_ARG_INFO(0, dither)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_mattefloodfillimage_args, 0, 0, 5)
		ZEND_ARG_INFO(0, alpha)
		ZEND_ARG_INFO(0, fuzz)
		ZEND_ARG_INFO(0, color)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_medianfilterimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, radius)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_negateimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, gray)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_paintopaqueimage_args, 0, 0, 3)
		ZEND_ARG_INFO(0, target_color)
		ZEND_ARG_INFO(0, fill_color)
		ZEND_ARG_INFO(0, fuzz)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_painttransparentimage_args, 0, 0, 3)
		ZEND_ARG_INFO(0, target_color)
		ZEND_ARG_INFO(0, alpha)
		ZEND_ARG_INFO(0, fuzz)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_previewimages_args, 0, 0, 1)
		ZEND_ARG_INFO(0, PREVIEW)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_profileimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, name)
		ZEND_ARG_INFO(0, profile)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_quantizeimage_args, 0, 0, 5)
		ZEND_ARG_INFO(0, numColors)
		ZEND_ARG_INFO(0, COLORSPACE)
		ZEND_ARG_INFO(0, treeDepth)
		ZEND_ARG_INFO(0, dither)
		ZEND_ARG_INFO(0, measureError)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_quantizeimages_args, 0, 0, 5)
		ZEND_ARG_INFO(0, numColors)
		ZEND_ARG_INFO(0, COLORSPACE)
		ZEND_ARG_INFO(0, treeDepth)
		ZEND_ARG_INFO(0, dither)
		ZEND_ARG_INFO(0, measureError)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_reducenoiseimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, radius)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_removeimageprofile_args, 0, 0, 1)
		ZEND_ARG_INFO(0, name)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_separateimagechannel_args, 0, 0, 1)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_sepiatoneimage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, threshold)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagebias_args, 0, 0, 1)
		ZEND_ARG_INFO(0, bias)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimageblueprimary_args, 0, 0, 2)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagebordercolor_args, 0, 0, 1)
		ZEND_ARG_INFO(0, color)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagechanneldepth_args, 0, 0, 2)
		ZEND_ARG_INFO(0, CHANNEL)
		ZEND_ARG_INFO(0, depth)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagecolormapcolor_args, 0, 0, 2)
		ZEND_ARG_INFO(0, index)
		ZEND_ARG_INFO(0, color)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagecolorspace_args, 0, 0, 1)
		ZEND_ARG_INFO(0, COLORSPACE)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagedispose_args, 0, 0, 1)
		ZEND_ARG_INFO(0, DISPOSETYPE)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimageextent_args, 0, 0, 2)
		ZEND_ARG_INFO(0, columns)
		ZEND_ARG_INFO(0, rows)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagegreenprimary_args, 0, 0, 2)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimageinterlacescheme_args, 0, 0, 1)
		ZEND_ARG_INFO(0, INTERLACE)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimageprofile_args, 0, 0, 2)
		ZEND_ARG_INFO(0, name)
		ZEND_ARG_INFO(0, profile)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimageredprimary_args, 0, 0, 2)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagerenderingintent_args, 0, 0, 1)
		ZEND_ARG_INFO(0, RENDERINGINTENT)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagevirtualpixelmethod_args, 0, 0, 1)
		ZEND_ARG_INFO(0, VIRTUALPIXELMETHOD)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimagewhitepoint_args, 0, 0, 2)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_sigmoidalcontrastimage_args, 0, 0, 3)
		ZEND_ARG_INFO(0, sharpen)
		ZEND_ARG_INFO(0, alpha)
		ZEND_ARG_INFO(0, beta)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_stereoimage_args, 0, 0, 1)
		ZEND_ARG_OBJ_INFO(0, Imagick, Imagick, 0)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_textureimage_args, 0, 0, 1)
		ZEND_ARG_OBJ_INFO(0, Imagick, Imagick, 0)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_tintimage_args, 0, 0, 2)
		ZEND_ARG_INFO(0, tint_color)
		ZEND_ARG_INFO(0, opacity)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_unsharpmaskimage_args, 0, 0, 4)
		ZEND_ARG_INFO(0, radius)
		ZEND_ARG_INFO(0, sigma)
		ZEND_ARG_INFO(0, amount)
		ZEND_ARG_INFO(0, threshold)
		ZEND_ARG_INFO(0, CHANNEL)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_addimage_args, 0, 0, 1)
		ZEND_ARG_OBJ_INFO(0, Imagick, Imagick, 0)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setimage_args, 0, 0, 1)
		ZEND_ARG_OBJ_INFO(0, Imagick, Imagick, 0)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_newimage_args, 0, 0, 3)
		ZEND_ARG_INFO(0, columns)
		ZEND_ARG_INFO(0, rows)
		ZEND_ARG_INFO(0, background_color)
		ZEND_ARG_INFO(0, format)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_newpseudoimage_args, 0, 0, 3)
		ZEND_ARG_INFO(0, columns)
		ZEND_ARG_INFO(0, rows)
		ZEND_ARG_INFO(0, pseudoString)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_getoption_args, 0, 0, 1)
		ZEND_ARG_INFO(0, key)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_getresource_args, 0, 0, 1)
		ZEND_ARG_INFO(0, resource_type)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_getresourcelimit_args, 0, 0, 1)
		ZEND_ARG_INFO(0, resource_type)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setbackgroundcolor_args, 0, 0, 1)
		ZEND_ARG_INFO(0, color)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setcompression_args, 0, 0, 1)
		ZEND_ARG_INFO(0, compression)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setcompressionquality_args, 0, 0, 1)
		ZEND_ARG_INFO(0, compressionquality)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setfilename_args, 0, 0, 1)
		ZEND_ARG_INFO(0, filename)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setformat_args, 0, 0, 1)
		ZEND_ARG_INFO(0, format)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setinterlacescheme_args, 0, 0, 1)
		ZEND_ARG_INFO(0, INTERLACE)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setoption_args, 0, 0, 2)
		ZEND_ARG_INFO(0, key)
		ZEND_ARG_INFO(0, value)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setpage_args, 0, 0, 4)
		ZEND_ARG_INFO(0, width)
		ZEND_ARG_INFO(0, height)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setresourcelimit_args, 0, 0, 2)
		ZEND_ARG_INFO(0, RESOURCETYPE)
		ZEND_ARG_INFO(0, limit)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setresolution_args, 0, 0, 2)
		ZEND_ARG_INFO(0, xResolution)
		ZEND_ARG_INFO(0, yResolution)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setsamplingfactors_args, 0, 0, 1)
		ZEND_ARG_INFO(0, factors)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_setsize_args, 0, 0, 2)
		ZEND_ARG_INFO(0, columns)
		ZEND_ARG_INFO(0, rows)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagick_settype_args, 0, 0, 1)
		ZEND_ARG_INFO(0, IMGTYPE)
	ZEND_END_ARG_INFO()


/* ImagickDraw */
#if MagickLibVersion > 0x649
	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_settextkerning_args, 0, 0, 1)
		ZEND_ARG_INFO(0, kerning)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_settextinterwordspacing_args, 0, 0, 1)
		ZEND_ARG_INFO(0, spacing)
	ZEND_END_ARG_INFO()
#endif

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_zero_args, 0, 0, 0)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setfillcolor_args, 0, 0, 1)
		ZEND_ARG_INFO(0, color)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setfillalpha_args, 0, 0, 1)
		ZEND_ARG_INFO(0, alpha)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setstrokecolor_args, 0, 0, 1)
		ZEND_ARG_INFO(0, color)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setstrokealpha_args, 0, 0, 1)
		ZEND_ARG_INFO(0, alpha)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setstrokewidth_args, 0, 0, 1)
		ZEND_ARG_INFO(0, width)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_circle_args, 0, 0, 4)
		ZEND_ARG_INFO(0, ox)
		ZEND_ARG_INFO(0, oy)
		ZEND_ARG_INFO(0, px)
		ZEND_ARG_INFO(0, py)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_annotation_args, 0, 0, 3)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
		ZEND_ARG_INFO(0, text)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_settextantialias_args, 0, 0, 1)
		ZEND_ARG_INFO(0, antialias)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_settextencoding_args, 0, 0, 1)
		ZEND_ARG_INFO(0, encoding)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setfont_args, 0, 0, 1)
		ZEND_ARG_INFO(0, font)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setfontfamily_args, 0, 0, 1)
		ZEND_ARG_INFO(0, fontfamily)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setfontsize_args, 0, 0, 1)
		ZEND_ARG_INFO(0, pointsize)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setfontstyle_args, 0, 0, 1)
		ZEND_ARG_INFO(0, STYLE)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setfontweight_args, 0, 0, 1)
		ZEND_ARG_INFO(0, weight)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_rectangle_args, 0, 0, 4)
		ZEND_ARG_INFO(0, x1)
		ZEND_ARG_INFO(0, y1)
		ZEND_ARG_INFO(0, x2)
		ZEND_ARG_INFO(0, y2)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_roundrectangle_args, 0, 0, 6)
		ZEND_ARG_INFO(0, x1)
		ZEND_ARG_INFO(0, y1)
		ZEND_ARG_INFO(0, x2)
		ZEND_ARG_INFO(0, y2)
		ZEND_ARG_INFO(0, rx)
		ZEND_ARG_INFO(0, ry)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_ellipse_args, 0, 0, 6)
		ZEND_ARG_INFO(0, ox)
		ZEND_ARG_INFO(0, oy)
		ZEND_ARG_INFO(0, px)
		ZEND_ARG_INFO(0, py)
		ZEND_ARG_INFO(0, start)
		ZEND_ARG_INFO(0, end)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_skewx_args, 0, 0, 1)
		ZEND_ARG_INFO(0, degrees)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_skewy_args, 0, 0, 1)
		ZEND_ARG_INFO(0, degrees)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_translate_args, 0, 0, 2)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_line_args, 0, 0, 4)
		ZEND_ARG_INFO(0, sx)
		ZEND_ARG_INFO(0, sy)
		ZEND_ARG_INFO(0, ex)
		ZEND_ARG_INFO(0, ey)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_arc_args, 0, 0, 6)
		ZEND_ARG_INFO(0, sx)
		ZEND_ARG_INFO(0, sy)
		ZEND_ARG_INFO(0, ex)
		ZEND_ARG_INFO(0, ey)
		ZEND_ARG_INFO(0, sd)
		ZEND_ARG_INFO(0, ed)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_matte_args, 0, 0, 3)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
		ZEND_ARG_INFO(0, METHOD)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_polygon_args, 0, 0, 1)
		ZEND_ARG_INFO(0, coordinates)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_point_args, 0, 0, 2)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setfontstretch_args, 0, 0, 1)
		ZEND_ARG_INFO(0, STRETCH)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setstrokeantialias_args, 0, 0, 1)
		ZEND_ARG_INFO(0, antialias)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setfontalignment_args, 0, 0, 1)
		ZEND_ARG_INFO(0, ALIGN)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_settextdecoration_args, 0, 0, 1)
		ZEND_ARG_INFO(0, DECORATION)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_settextundercolor_args, 0, 0, 1)
		ZEND_ARG_INFO(0, color)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setviewbox_args, 0, 0, 4)
		ZEND_ARG_INFO(0, sx)
		ZEND_ARG_INFO(0, sy)
		ZEND_ARG_INFO(0, ex)
		ZEND_ARG_INFO(0, ey)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_affine_args, 0, 0, 1)
		ZEND_ARG_INFO(0, affineMatrix)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_bezier_args, 0, 0, 1)
		ZEND_ARG_INFO(0, coordinateArray)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_composite_args, 0, 0, 6)
		ZEND_ARG_INFO(0, COMPOSE)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
		ZEND_ARG_INFO(0, width)
		ZEND_ARG_INFO(0, height)
		ZEND_ARG_OBJ_INFO(0, Imagick, Imagick, 0)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_color_args, 0, 0, 3)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
		ZEND_ARG_INFO(0, PAINTMETHOD)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_comment_args, 0, 0, 1)
		ZEND_ARG_INFO(0, comment)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_pathcurvetoabsolute_args, 0, 0, 6)
		ZEND_ARG_INFO(0, x1)
		ZEND_ARG_INFO(0, y1)
		ZEND_ARG_INFO(0, x2)
		ZEND_ARG_INFO(0, y2)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_pathcurvetorelative_args, 0, 0, 6)
		ZEND_ARG_INFO(0, x1)
		ZEND_ARG_INFO(0, y1)
		ZEND_ARG_INFO(0, x2)
		ZEND_ARG_INFO(0, y2)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_pathcurvetoquadraticbezierabsolute_args, 0, 0, 4)
		ZEND_ARG_INFO(0, x1)
		ZEND_ARG_INFO(0, y1)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_pathcurvetoquadraticbezierrelative_args, 0, 0, 4)
		ZEND_ARG_INFO(0, x1)
		ZEND_ARG_INFO(0, y1)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_pathcurvetoquadraticbeziersmoothabsolute_args, 0, 0, 2)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_pathcurvetoquadraticbeziersmoothrelative_args, 0, 0, 2)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_pathcurvetosmoothabsolute_args, 0, 0, 4)
		ZEND_ARG_INFO(0, x1)
		ZEND_ARG_INFO(0, y1)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_pathcurvetosmoothrelative_args, 0, 0, 4)
		ZEND_ARG_INFO(0, x1)
		ZEND_ARG_INFO(0, y1)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_pathellipticarcabsolute_args, 0, 0, 7)
		ZEND_ARG_INFO(0, rx)
		ZEND_ARG_INFO(0, ry)
		ZEND_ARG_INFO(0, xAxisRotation)
		ZEND_ARG_INFO(0, largeArc)
		ZEND_ARG_INFO(0, sweep)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_pathellipticarcrelative_args, 0, 0, 7)
		ZEND_ARG_INFO(0, rx)
		ZEND_ARG_INFO(0, ry)
		ZEND_ARG_INFO(0, xAxisRotation)
		ZEND_ARG_INFO(0, largeArc)
		ZEND_ARG_INFO(0, sweep)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_pathlinetoabsolute_args, 0, 0, 2)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_pathlinetorelative_args, 0, 0, 2)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_pathlinetohorizontalabsolute_args, 0, 0, 1)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_pathlinetohorizontalrelative_args, 0, 0, 1)
		ZEND_ARG_INFO(0, x)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_pathlinetoverticalabsolute_args, 0, 0, 1)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_pathlinetoverticalrelative_args, 0, 0, 1)
		ZEND_ARG_INFO(0, x)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_pathmovetoabsolute_args, 0, 0, 2)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_pathmovetorelative_args, 0, 0, 2)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_polyline_args, 0, 0, 1)
		ZEND_ARG_INFO(0, coordinateArray)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_pushclippath_args, 0, 0, 1)
		ZEND_ARG_INFO(0, clipMask)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_pushpattern_args, 0, 0, 5)
		ZEND_ARG_INFO(0, pattern_id)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
		ZEND_ARG_INFO(0, width)
		ZEND_ARG_INFO(0, height)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_rotate_args, 0, 0, 1)
		ZEND_ARG_INFO(0, degrees)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_scale_args, 0, 0, 2)
		ZEND_ARG_INFO(0, x)
		ZEND_ARG_INFO(0, y)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setclippath_args, 0, 0, 1)
		ZEND_ARG_INFO(0, clipMask)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setcliprule_args, 0, 0, 1)
		ZEND_ARG_INFO(0, FILLRULE)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setclipunits_args, 0, 0, 1)
		ZEND_ARG_INFO(0, PATHUNITS)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setfillopacity_args, 0, 0, 1)
		ZEND_ARG_INFO(0, fillOpacity)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setfillpatternurl_args, 0, 0, 1)
		ZEND_ARG_INFO(0, url)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setfillrule_args, 0, 0, 1)
		ZEND_ARG_INFO(0, FILLRULE)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setgravity_args, 0, 0, 1)
		ZEND_ARG_INFO(0, GRAVITY)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setstrokepatternurl_args, 0, 0, 1)
		ZEND_ARG_INFO(0, url)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setstrokedashoffset_args, 0, 0, 1)
		ZEND_ARG_INFO(0, offset)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setstrokelinecap_args, 0, 0, 1)
		ZEND_ARG_INFO(0, LINECAP)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setstrokelinejoin_args, 0, 0, 1)
		ZEND_ARG_INFO(0, LINEJOIN)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setstrokemiterlimit_args, 0, 0, 1)
		ZEND_ARG_INFO(0, miterLimit)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setstrokeopacity_args, 0, 0, 1)
		ZEND_ARG_INFO(0, strokeOpacity)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setvectorgraphics_args, 0, 0, 1)
		ZEND_ARG_INFO(0, xml)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickdraw_setstrokedasharray_args, 0, 0, 1)
		ZEND_ARG_INFO(0, dashArray)
	ZEND_END_ARG_INFO()

/* ImagickPixel */
#if MagickLibVersion > 0x628
	ZEND_BEGIN_ARG_INFO_EX(imagickpixel_sethsl_args, 0, 0, 3)
		ZEND_ARG_INFO(0, hue)
		ZEND_ARG_INFO(0, saturation)
		ZEND_ARG_INFO(0, luminosity)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickpixel_setcolorvaluequantum_args, 0, 0, 1)
		ZEND_ARG_INFO(0, color_value)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickpixel_setindex_args, 0, 0, 1)
		ZEND_ARG_INFO(0, index)
	ZEND_END_ARG_INFO()
#endif

	ZEND_BEGIN_ARG_INFO_EX(imagickpixel_zero_args, 0, 0, 0)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickpixel_construct_args, 0, 0, 0)
		ZEND_ARG_INFO(0, color)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickpixel_setcolor_args, 0, 0, 1)
		ZEND_ARG_INFO(0, color)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickpixel_issimilar_args, 0, 0, 1)
		ZEND_ARG_INFO(0, color)
		ZEND_ARG_INFO(0, fuzz)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickpixel_setcolorvalue_args, 0, 0, 2)
		ZEND_ARG_INFO(0, color)
		ZEND_ARG_INFO(0, value)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickpixel_getcolorvalue_args, 0, 0, 1)
		ZEND_ARG_INFO(0, color)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickpixel_setcolorcount_args, 0, 0, 1)
		ZEND_ARG_INFO(0, colorCount)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickpixel_getcolor_args, 0, 0, 0)
		ZEND_ARG_INFO(0, normalized)
	ZEND_END_ARG_INFO()

/* ImagickPixelIterator */

	ZEND_BEGIN_ARG_INFO_EX(imagickpixeliterator_zero_args, 0, 0, 0)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickpixeliterator_construct_args, 0, 0, 1)
		ZEND_ARG_OBJ_INFO(0, Imagick, Imagick, 0)
	ZEND_END_ARG_INFO()

	ZEND_BEGIN_ARG_INFO_EX(imagickpixeliterator_setiteratorrow_args, 0, 0, 1)
		ZEND_ARG_INFO(0, row)
	ZEND_END_ARG_INFO()

static zend_function_entry php_imagick_functions[] =
{
	{ NULL, NULL, NULL }
};

static zend_function_entry php_imagickdraw_class_methods[] =
{
#if MagickLibVersion > 0x628
	PHP_ME(imagickdraw, resetvectorgraphics, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
#endif
#if MagickLibVersion > 0x649
	PHP_ME(imagickdraw, gettextkerning, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, settextkerning, imagickdraw_settextkerning_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, gettextinterwordspacing, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, settextinterwordspacing, imagickdraw_settextinterwordspacing_args, ZEND_ACC_PUBLIC)
#endif
	PHP_ME(imagickdraw, __construct, imagickdraw_zero_args, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(imagickdraw, setfillcolor, imagickdraw_setfillcolor_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setfillalpha, imagickdraw_setfillalpha_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setstrokecolor, imagickdraw_setstrokecolor_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setstrokealpha, imagickdraw_setstrokealpha_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setstrokewidth, imagickdraw_setstrokewidth_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, clear, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, circle, imagickdraw_circle_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, annotation, imagickdraw_annotation_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, settextantialias, imagickdraw_settextantialias_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, settextencoding, imagickdraw_settextencoding_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setfont, imagickdraw_setfont_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setfontfamily, imagickdraw_setfontfamily_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setfontsize, imagickdraw_setfontsize_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setfontstyle, imagickdraw_setfontstyle_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setfontweight, imagickdraw_setfontweight_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getfont, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getfontfamily, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getfontsize, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getfontstyle, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getfontweight, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, destroy, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, rectangle, imagickdraw_rectangle_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, roundrectangle, imagickdraw_roundrectangle_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, ellipse, imagickdraw_ellipse_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, skewx, imagickdraw_skewx_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, skewy, imagickdraw_skewy_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, translate, imagickdraw_translate_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, line, imagickdraw_line_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, arc, imagickdraw_arc_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, matte, imagickdraw_matte_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, polygon, imagickdraw_polygon_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, point, imagickdraw_point_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, gettextdecoration, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, gettextencoding, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getfontstretch, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setfontstretch, imagickdraw_setfontstretch_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setstrokeantialias, imagickdraw_setstrokeantialias_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, settextalignment, imagickdraw_setfontalignment_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, settextdecoration, imagickdraw_settextdecoration_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, settextundercolor, imagickdraw_settextundercolor_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setviewbox, imagickdraw_setviewbox_args, ZEND_ACC_PUBLIC)
  	PHP_ME(imagickdraw, clone, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, affine, imagickdraw_affine_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, bezier, imagickdraw_bezier_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, composite, imagickdraw_composite_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, color, imagickdraw_color_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, comment, imagickdraw_comment_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getclippath, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getcliprule, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getclipunits, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getfillcolor, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getfillopacity, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getfillrule, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getgravity, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getstrokeantialias, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getstrokecolor, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getstrokedasharray, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getstrokedashoffset, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getstrokelinecap, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getstrokelinejoin, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getstrokemiterlimit, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getstrokeopacity, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getstrokewidth, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, gettextalignment, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, gettextantialias, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, getvectorgraphics, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, gettextundercolor, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathclose, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathcurvetoabsolute, imagickdraw_pathcurvetoabsolute_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathcurvetorelative, imagickdraw_pathcurvetorelative_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathcurvetoquadraticbezierabsolute, imagickdraw_pathcurvetoquadraticbezierabsolute_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathcurvetoquadraticbezierrelative, imagickdraw_pathcurvetoquadraticbezierrelative_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathcurvetoquadraticbeziersmoothabsolute, imagickdraw_pathcurvetoquadraticbeziersmoothabsolute_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathcurvetoquadraticbeziersmoothrelative, imagickdraw_pathcurvetoquadraticbeziersmoothrelative_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathcurvetosmoothabsolute, imagickdraw_pathcurvetosmoothabsolute_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathcurvetosmoothrelative, imagickdraw_pathcurvetosmoothrelative_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathellipticarcabsolute, imagickdraw_pathellipticarcabsolute_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathellipticarcrelative, imagickdraw_pathellipticarcrelative_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathfinish, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathlinetoabsolute, imagickdraw_pathlinetoabsolute_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathlinetorelative, imagickdraw_pathlinetorelative_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathlinetohorizontalabsolute, imagickdraw_pathlinetohorizontalabsolute_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathlinetohorizontalrelative, imagickdraw_pathlinetohorizontalrelative_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathlinetoverticalabsolute, imagickdraw_pathlinetoverticalabsolute_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathlinetoverticalrelative, imagickdraw_pathlinetoverticalrelative_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathmovetoabsolute, imagickdraw_pathmovetoabsolute_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathmovetorelative, imagickdraw_pathmovetorelative_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pathstart, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, polyline, imagickdraw_polyline_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, popclippath, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, popdefs, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, poppattern, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pushclippath, imagickdraw_pushclippath_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pushdefs, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pushpattern, imagickdraw_pushpattern_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, render, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, rotate, imagickdraw_rotate_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, scale, imagickdraw_scale_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setclippath, imagickdraw_setclippath_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setcliprule, imagickdraw_setcliprule_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setclipunits, imagickdraw_setclipunits_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setfillopacity, imagickdraw_setfillopacity_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setfillpatternurl, imagickdraw_setfillpatternurl_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setfillrule, imagickdraw_setfillrule_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setgravity, imagickdraw_setgravity_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setstrokepatternurl, imagickdraw_setstrokepatternurl_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setstrokedashoffset, imagickdraw_setstrokedashoffset_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setstrokelinecap, imagickdraw_setstrokelinecap_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setstrokelinejoin, imagickdraw_setstrokelinejoin_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setstrokemiterlimit, imagickdraw_setstrokemiterlimit_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setstrokeopacity, imagickdraw_setstrokeopacity_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setvectorgraphics, imagickdraw_setvectorgraphics_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, pop, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, push, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickdraw, setstrokedasharray, imagickdraw_setstrokedasharray_args, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};

static zend_function_entry php_imagickpixeliterator_class_methods[] =
{
	PHP_ME(imagickpixeliterator, __construct, imagickpixeliterator_construct_args, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(imagickpixeliterator, newpixeliterator, imagickpixeliterator_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixeliterator, newpixelregioniterator, imagickpixeliterator_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixeliterator, getiteratorrow, imagickpixeliterator_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixeliterator, setiteratorrow, imagickpixeliterator_setiteratorrow_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixeliterator, setiteratorfirstrow, imagickpixeliterator_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixeliterator, setiteratorlastrow, imagickpixeliterator_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixeliterator, getpreviousiteratorrow, imagickpixeliterator_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixeliterator, getcurrentiteratorrow, imagickpixeliterator_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixeliterator, getnextiteratorrow, imagickpixeliterator_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixeliterator, resetiterator, imagickpixeliterator_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixeliterator, synciterator, imagickpixeliterator_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixeliterator, destroy, imagickpixeliterator_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixeliterator, clear, imagickpixeliterator_zero_args, ZEND_ACC_PUBLIC)

	/* Iterator interface */
	PHP_MALIAS(imagickpixeliterator, key, getiteratorrow, imagickpixeliterator_zero_args, ZEND_ACC_PUBLIC)
	PHP_MALIAS(imagickpixeliterator, next, getnextiteratorrow, imagickpixeliterator_zero_args, ZEND_ACC_PUBLIC)
	PHP_MALIAS(imagickpixeliterator, rewind, resetiterator, imagickpixeliterator_zero_args, ZEND_ACC_PUBLIC)
	PHP_MALIAS(imagickpixeliterator, current, getcurrentiteratorrow, imagickpixeliterator_zero_args, ZEND_ACC_PUBLIC)

	PHP_ME(imagickpixeliterator, valid, imagickpixeliterator_zero_args, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};


static zend_function_entry php_imagickpixel_class_methods[] =
{
#if MagickLibVersion > 0x628
	PHP_ME(imagickpixel, gethsl, imagickpixel_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixel, sethsl, imagickpixel_sethsl_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixel, getcolorvaluequantum, imagickpixel_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixel, setcolorvaluequantum, imagickpixel_setcolorvaluequantum_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixel, getindex, imagickpixel_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixel, setindex, imagickpixel_setindex_args, ZEND_ACC_PUBLIC)
#endif
	PHP_ME(imagickpixel, __construct, imagickpixel_construct_args, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(imagickpixel, setcolor, imagickpixel_setcolor_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixel, setcolorvalue, imagickpixel_setcolorvalue_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixel, getcolorvalue, imagickpixel_getcolorvalue_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixel, clear, imagickpixel_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixel, destroy, imagickpixel_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixel, issimilar, imagickpixel_issimilar_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixel, getcolor, imagickpixel_getcolor_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixel, getcolorasstring, imagickpixel_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixel, getcolorcount, imagickpixel_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixel, setcolorcount, imagickpixel_setcolorcount_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagickpixel, clone, imagickpixel_zero_args, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};

static zend_function_entry php_imagick_class_methods[] =
{
#if MagickLibVersion > 0x628
	PHP_ME(imagick, optimizeimagelayers, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, compareimagelayers, imagick_compareimagelayers_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, pingimageblob, imagick_pingimageblob_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, pingimagefile, imagick_pingimagefile_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, transposeimage, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, transverseimage, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, trimimage, imagick_trimimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, waveimage, imagick_waveimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, vignetteimage, imagick_vignetteimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, uniqueimagecolors, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagematte, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagematte, imagick_setimagematte_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, adaptiveresizeimage, imagick_adaptiveresizeimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, sketchimage, imagick_sketchimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, shadeimage, imagick_shadeimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getsizeoffset, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setsizeoffset, imagick_setsizeoffset_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, adaptiveblurimage, imagick_adaptiveblurimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, contraststretchimage, imagick_contraststretchimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, adaptivesharpenimage, imagick_adaptivesharpenimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, randomthresholdimage, imagick_randomthresholdimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, roundcornersimage, imagick_roundcornersimage_args, ZEND_ACC_PUBLIC)
	/* This alias is due to BWC */
	PHP_MALIAS(imagick, roundcorners, roundcornersimage, imagick_roundcornersimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setiteratorindex, imagick_setiteratorindex_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getiteratorindex, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, transformimage, imagick_transformimage_args, ZEND_ACC_PUBLIC)
#endif
#if MagickLibVersion > 0x630
	PHP_ME(imagick, setimageopacity, imagick_setimageopacity_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, orderedposterizeimage, imagick_orderedposterizeimage_args, ZEND_ACC_PUBLIC)
#endif
#if MagickLibVersion > 0x631
	PHP_ME(imagick, polaroidimage, imagick_polaroidimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimageproperty, imagick_getimageproperty_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimageproperty, imagick_setimageproperty_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimageinterpolatemethod, imagick_setimageinterpolatemethod_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimageinterpolatemethod, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, linearstretchimage, imagick_linearstretchimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagelength, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, extentimage, imagick_extentimage_args, ZEND_ACC_PUBLIC)
#endif
#if MagickLibVersion > 0x633
	PHP_ME(imagick, getimageorientation, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimageorientation, imagick_setimageorientation_args, ZEND_ACC_PUBLIC)
#endif
#if MagickLibVersion > 0x634
	PHP_ME(imagick, paintfloodfillimage, imagick_paintfloodfillimage_args, ZEND_ACC_PUBLIC)
#endif
#if MagickLibVersion > 0x635
	PHP_ME(imagick, clutimage, imagick_clutimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimageproperties, imagick_getimageproperties_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimageprofiles, imagick_getimageprofiles_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, distortimage, imagick_distortimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, writeimagefile, imagick_writeimagefile_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, writeimagesfile, imagick_writeimagesfile_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, resetimagepage, imagick_resetimagepage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimageclipmask, imagick_setimageclipmask_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimageclipmask, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, animateimages, imagick_animateimages_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, recolorimage, imagick_recolorimage_args, ZEND_ACC_PUBLIC)
#endif
#if MagickLibVersion > 0x636
	PHP_ME(imagick, setfont, imagick_setfont_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getfont, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setpointsize, imagick_setpointsize_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getpointsize, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, mergeimagelayers, imagick_mergeimagelayers_args, ZEND_ACC_PUBLIC)
#endif
#if MagickLibVersion > 0x637
	PHP_ME(imagick, setimagealphachannel, imagick_setimagealphachannel_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, floodfillpaintimage, imagick_floodfillpaintimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, opaquepaintimage, imagick_opaquepaintimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, transparentpaintimage, imagick_transparentpaintimage_args, ZEND_ACC_PUBLIC)
#endif
#if MagickLibVersion > 0x638
	PHP_ME(imagick, liquidrescaleimage, imagick_liquidrescaleimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, encipherimage, imagick_encipherimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, decipherimage, imagick_decipherimage_args, ZEND_ACC_PUBLIC)
#endif
#if MagickLibVersion > 0x639
	PHP_ME(imagick, setgravity, imagick_setgravity_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getgravity, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagechannelrange, imagick_getimagechannelrange_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagealphachannel, imagick_zero_args, ZEND_ACC_PUBLIC)
#endif
#if MagickLibVersion > 0x642
	PHP_ME(imagick, getimagechanneldistortions, imagick_getimagechanneldistortions_args, ZEND_ACC_PUBLIC)
#endif
#if MagickLibVersion > 0x643
	PHP_ME(imagick, setimagegravity, imagick_setimagegravity_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagegravity, imagick_zero_args, ZEND_ACC_PUBLIC)
#endif
#if MagickLibVersion > 0x645
	PHP_ME(imagick, importimagepixels, imagick_importimagepixels_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, deskewimage, imagick_deskewimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, segmentimage, imagick_segmentimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, sparsecolorimage, imagick_sparsecolorimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, remapimage, imagick_remapimage_args, ZEND_ACC_PUBLIC)
#endif
#if MagickLibVersion > 0x646
	PHP_ME(imagick, exportimagepixels, imagick_exportimagepixels_args, ZEND_ACC_PUBLIC)
#endif
#if MagickLibVersion > 0x648
	PHP_ME(imagick, getimagechannelkurtosis, imagick_getimagechannelkurtosis_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, functionimage, imagick_functionimage_args, ZEND_ACC_PUBLIC)
#endif
#if MagickLibVersion > 0x651
	PHP_ME(imagick, transformimagecolorspace, imagick_transformimagecolorspace_args, ZEND_ACC_PUBLIC)
#endif
#if MagickLibVersion > 0x652
	PHP_ME(imagick, haldclutimage, imagick_haldclutimage_args, ZEND_ACC_PUBLIC)
#endif
#if MagickLibVersion > 0x656
	PHP_ME(imagick, getimageartifact, imagick_getimageartifact_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimageartifact, imagick_setimageartifact_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, deleteimageartifact, imagick_deleteimageartifact_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getcolorspace, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setcolorspace, imagick_setcolorspace_args, ZEND_ACC_PUBLIC)
#endif
	PHP_ME(imagick, __construct, imagick_construct_args, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(imagick, __tostring, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getpixeliterator, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getpixelregioniterator, imagick_getpixelregioniterator_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, readimage, imagick_readimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, readimages, imagick_readimages_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, readimageblob, imagick_readimageblob_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimageformat, imagick_setimageformat_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, scaleimage, imagick_scaleimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, writeimage, imagick_writeimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, writeimages, imagick_writeimages_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, blurimage, imagick_blurimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, thumbnailimage, imagick_thumbnailimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, cropthumbnailimage, imagick_cropthumbnailimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagefilename, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagefilename, imagick_setimagefilename_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimageformat, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagemimetype, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, removeimage, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, destroy, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, clear, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, clone, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagesize, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimageblob, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagesblob, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setfirstiterator, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setlastiterator, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, resetiterator, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, previousimage, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, nextimage, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, haspreviousimage, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, hasnextimage, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimageindex, imagick_setimageindex_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimageindex, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, commentimage, imagick_commentimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, cropimage, imagick_cropimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, labelimage, imagick_labelimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagegeometry, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, drawimage, imagick_drawimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagecompressionquality, imagick_setimagecompressionquality_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagecompressionquality, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, annotateimage, imagick_annotateimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, compositeimage, imagick_compositeimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, modulateimage, imagick_modulateimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagecolors, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, montageimage, imagick_montageimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, identifyimage, imagick_identifyimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, thresholdimage, imagick_thresholdimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, adaptivethresholdimage, imagick_adaptivethresholdimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, blackthresholdimage, imagick_blackthresholdimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, whitethresholdimage, imagick_whitethresholdimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, appendimages, imagick_appendimages_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, charcoalimage, imagick_charcoalimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, normalizeimage, imagick_normalizeimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, oilpaintimage, imagick_oilpaintimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, posterizeimage, imagick_posterizeimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, radialblurimage, imagick_radialblurimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, raiseimage, imagick_raiseimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, resampleimage, imagick_resampleimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, resizeimage, imagick_resizeimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, rollimage, imagick_rollimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, rotateimage, imagick_rotateimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, sampleimage, imagick_sampleimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, solarizeimage, imagick_solarizeimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, shadowimage, imagick_shadowimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimageattribute, imagick_setimageattribute_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagebackgroundcolor, imagick_setimagebackgroundcolor_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagecompose, imagick_setimagecompose_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagecompression, imagick_setimagecompression_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagedelay, imagick_setimagedelay_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagedepth, imagick_setimagedepth_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagegamma, imagick_setimagegamma_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimageiterations, imagick_setimageiterations_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagemattecolor, imagick_setimagemattecolor_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagepage, imagick_setimagepage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimageprogressmonitor, imagick_setimageprogressmonitor_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimageresolution, imagick_setimageresolution_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagescene, imagick_setimagescene_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagetickspersecond, imagick_setimagetickspersecond_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagetype, imagick_setimagetype_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimageunits, imagick_setimageunits_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, sharpenimage, imagick_sharpenimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, shaveimage, imagick_shaveimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, shearimage, imagick_shearimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, spliceimage, imagick_spliceimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, pingimage, imagick_pingimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, readimagefile, imagick_readimagefile_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, displayimage, imagick_displayimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, displayimages, imagick_displayimages_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, spreadimage, imagick_spreadimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, swirlimage, imagick_swirlimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, stripimage, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, queryformats, imagick_queryformats_args, ZEND_ACC_ALLOW_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(imagick, queryfonts, imagick_queryfonts_args, ZEND_ACC_ALLOW_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(imagick, queryfontmetrics, imagick_queryfontmetrics_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, steganoimage, imagick_steganoimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, addnoiseimage, imagick_addnoiseimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, motionblurimage, imagick_motionblurimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, mosaicimages, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, morphimages, imagick_morphimages_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, minifyimage, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, affinetransformimage, imagick_affinetransformimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, averageimages, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, borderimage, imagick_borderimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, chopimage, imagick_chopimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, clipimage, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, clippathimage, imagick_clippathimage_args, ZEND_ACC_PUBLIC)
	/* clippathimage has been deprecated. Create alias here and use the newer API function if present */
	PHP_MALIAS(imagick, clipimagepath, clippathimage, imagick_clippathimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, coalesceimages, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, colorfloodfillimage, imagick_colorfloodfillimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, colorizeimage, imagick_colorizeimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, compareimagechannels, imagick_compareimagechannels_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, compareimages, imagick_compareimages_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, contrastimage, imagick_contrastimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, combineimages, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, convolveimage, imagick_convolveimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, cyclecolormapimage, imagick_cyclecolormapimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, deconstructimages, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, despeckleimage, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, edgeimage, imagick_edgeimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, embossimage, imagick_embossimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, enhanceimage, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, equalizeimage, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, evaluateimage, imagick_evaluateimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, flattenimages, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, flipimage, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, flopimage, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, frameimage, imagick_frameimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, fximage, imagick_fximage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, gammaimage, imagick_gammaimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, gaussianblurimage, imagick_gaussianblurimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimageattribute, imagick_getimageattribute_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagebackgroundcolor, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimageblueprimary, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagebordercolor, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagechanneldepth, imagick_getimagechanneldepth_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagechanneldistortion, imagick_getimagechanneldistortion_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagechannelextrema, imagick_getimagechannelextrema_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagechannelmean, imagick_getimagechannelmean_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagechannelstatistics, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagecolormapcolor, imagick_getimagecolormapcolor_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagecolorspace, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagecompose, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagedelay, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagedepth, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagedistortion, imagick_getimagedistortion_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimageextrema, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagedispose, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagegamma, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagegreenprimary, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimageheight, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagehistogram, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimageinterlacescheme, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimageiterations, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagemattecolor, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagepage, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagepixelcolor, imagick_getimagepixelcolor_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimageprofile, imagick_getimageprofile_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimageredprimary, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagerenderingintent, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimageresolution, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagescene, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagesignature, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagetickspersecond, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagetype, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimageunits, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagevirtualpixelmethod, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagewhitepoint, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagewidth, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getnumberimages, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimagetotalinkdensity, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimageregion, imagick_getimageregion_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, implodeimage, imagick_getimagetotalinkdensity_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, levelimage, imagick_levelimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, magnifyimage, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, mapimage, imagick_mapimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, mattefloodfillimage, imagick_mattefloodfillimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, medianfilterimage, imagick_medianfilterimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, negateimage, imagick_negateimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, paintopaqueimage, imagick_paintopaqueimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, painttransparentimage, imagick_painttransparentimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, previewimages, imagick_previewimages_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, profileimage, imagick_profileimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, quantizeimage, imagick_quantizeimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, quantizeimages, imagick_quantizeimages_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, reducenoiseimage, imagick_reducenoiseimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, removeimageprofile, imagick_removeimageprofile_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, separateimagechannel, imagick_separateimagechannel_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, sepiatoneimage, imagick_sepiatoneimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagebias, imagick_setimagebias_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimageblueprimary, imagick_setimageblueprimary_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagebordercolor, imagick_setimagebordercolor_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagechanneldepth, imagick_setimagechanneldepth_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagecolormapcolor, imagick_setimagecolormapcolor_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagecolorspace, imagick_setimagecolorspace_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagedispose, imagick_setimagedispose_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimageextent, imagick_setimageextent_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagegreenprimary, imagick_setimagegreenprimary_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimageinterlacescheme, imagick_setimageinterlacescheme_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimageprofile, imagick_setimageprofile_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimageredprimary, imagick_setimageredprimary_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagerenderingintent, imagick_setimagerenderingintent_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagevirtualpixelmethod, imagick_setimagevirtualpixelmethod_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimagewhitepoint, imagick_setimagewhitepoint_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, sigmoidalcontrastimage, imagick_sigmoidalcontrastimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, stereoimage, imagick_stereoimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, textureimage, imagick_textureimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, tintimage, imagick_tintimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, unsharpmaskimage, imagick_unsharpmaskimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getimage, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, addimage, imagick_addimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setimage, imagick_setimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, newimage, imagick_newimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, newpseudoimage, imagick_newpseudoimage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getcompression, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getcompressionquality, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getcopyright, imagick_zero_args, ZEND_ACC_ALLOW_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getfilename, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getformat, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, gethomeurl, imagick_zero_args, ZEND_ACC_ALLOW_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getinterlacescheme, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getoption, imagick_getoption_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getpackagename, imagick_zero_args, ZEND_ACC_ALLOW_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getpage, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getquantumdepth, imagick_zero_args, ZEND_ACC_ALLOW_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getquantumrange, imagick_zero_args, ZEND_ACC_ALLOW_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getreleasedate, imagick_zero_args, ZEND_ACC_ALLOW_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getresource, imagick_getresource_args, ZEND_ACC_ALLOW_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getresourcelimit, imagick_getresourcelimit_args, ZEND_ACC_ALLOW_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getsamplingfactors, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getsize, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, getversion, imagick_zero_args, ZEND_ACC_ALLOW_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setbackgroundcolor, imagick_setbackgroundcolor_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setcompression, imagick_setcompression_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setcompressionquality, imagick_setcompressionquality_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setfilename, imagick_setfilename_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setformat, imagick_setformat_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setinterlacescheme, imagick_setinterlacescheme_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setoption, imagick_setoption_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setpage, imagick_setpage_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setresourcelimit, imagick_setresourcelimit_args, ZEND_ACC_ALLOW_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setresolution, imagick_setresolution_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setsamplingfactors, imagick_setsamplingfactors_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, setsize, imagick_setsize_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, settype, imagick_settype_args, ZEND_ACC_PUBLIC)
#if MagickLibVersion > 0x628
	PHP_MALIAS(imagick, key, getiteratorindex, imagick_zero_args, ZEND_ACC_PUBLIC)
#else
	PHP_MALIAS(imagick, key, getimageindex, imagick_zero_args, ZEND_ACC_PUBLIC)
#endif
	PHP_MALIAS(imagick, next, nextimage, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_MALIAS(imagick, rewind, setfirstiterator, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, valid, imagick_zero_args, ZEND_ACC_PUBLIC)
	PHP_ME(imagick, current, imagick_zero_args, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};

static void php_imagick_object_free_storage(void *object TSRMLS_DC)
{
	php_imagick_object *intern = (php_imagick_object *)object;

	if (!intern) {
		return;
	}

	if (intern->progress_monitor_name) {
		efree(intern->progress_monitor_name);
	}

	if (intern->magick_wand != (MagickWand *)NULL) {
		intern->magick_wand = DestroyMagickWand(intern->magick_wand);
	}

	zend_object_std_dtor(&intern->zo TSRMLS_CC);
	efree(intern);
}


static void php_imagickdraw_object_free_storage(void *object TSRMLS_DC)
{
	php_imagickdraw_object *intern = (php_imagickdraw_object *)object;

	if (!intern) {
		return;
	}

	if (intern->drawing_wand != (DrawingWand *)NULL) {
		intern->drawing_wand = DestroyDrawingWand(intern->drawing_wand);
	}

	zend_object_std_dtor(&intern->zo TSRMLS_CC);
	efree(intern);
}

static void php_imagickpixeliterator_object_free_storage(void *object TSRMLS_DC)
{
	php_imagickpixeliterator_object *intern = (php_imagickpixeliterator_object *)object;

	if (!intern) {
		return;
	}

	if (intern->pixel_iterator != (PixelIterator *)NULL) {
		intern->pixel_iterator = DestroyPixelIterator(intern->pixel_iterator);
	}

	zend_object_std_dtor(&intern->zo TSRMLS_CC);
	efree(intern);

}

static void php_imagickpixel_object_free_storage(void *object TSRMLS_DC)
{
	php_imagickpixel_object *intern = (php_imagickpixel_object *)object;

	if (!intern) {
		return;
	}
	if (intern->initialized_via_iterator < 1) {
		if(intern->pixel_wand != (PixelWand *)NULL) {
			intern->pixel_wand = DestroyPixelWand(intern->pixel_wand);
		}
	}

	zend_object_std_dtor(&intern->zo TSRMLS_CC);
	efree(intern);
}


static zend_object_value php_imagick_object_new_ex(zend_class_entry *class_type, php_imagick_object **ptr, zend_bool init_wand TSRMLS_DC)
{
	zval *tmp;
	zend_object_value retval;
	php_imagick_object *intern;

	/* Allocate memory for it */
	intern = (php_imagick_object *) emalloc(sizeof(php_imagick_object));
	memset(&intern->zo, 0, sizeof(zend_object));

#if defined(ZTS) && defined(PHP_WIN32)
	/* If its our thread then we already have the lock so no need to lock again */
	if (imagick_thread_id != tsrm_thread_id()) {
		tsrm_mutex_lock(imagick_mutex);
		imagick_thread_id = tsrm_thread_id();
	}
#endif

	if (ptr) {
		*ptr = intern;
	}

	/* Set the magickwand */
	if (init_wand) {
		intern->magick_wand = NewMagickWand();
	} else {
		intern->magick_wand = NULL;
	}
	intern->next_out_of_bound = 0;
	intern->progress_monitor_name = NULL;

	/* ALLOC_HASHTABLE(intern->zo.properties); */

	zend_object_std_init(&intern->zo, class_type TSRMLS_CC);
	zend_hash_copy(intern->zo.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref,(void *) &tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(intern, NULL, (zend_objects_free_object_storage_t) php_imagick_object_free_storage, NULL TSRMLS_CC);
	retval.handlers = (zend_object_handlers *) &imagick_object_handlers;
	return retval;
}

static zend_object_value php_imagick_object_new(zend_class_entry *class_type TSRMLS_DC)
{
	return php_imagick_object_new_ex(class_type, NULL, 1 TSRMLS_CC);
}

static zend_object_value php_imagickdraw_object_new_ex(zend_class_entry *class_type, php_imagickdraw_object **ptr, zend_bool init_wand TSRMLS_DC)
{
	zval *tmp;
	zend_object_value retval;
	php_imagickdraw_object *intern;

	/* Allocate memory for it */
	intern = (php_imagickdraw_object *) emalloc(sizeof(php_imagickdraw_object));
	memset(&intern->zo, 0, sizeof(zend_object));

	if (ptr) {
		*ptr = intern;
	}

	/* Set the DrawingWand */
	if (init_wand) {
		intern->drawing_wand = NewDrawingWand();
	} else {
		intern->drawing_wand = NULL;
	}
	/* ALLOC_HASHTABLE(intern->zo.properties); */

	zend_object_std_init(&intern->zo, class_type TSRMLS_CC);
	zend_hash_copy(intern->zo.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref,(void *) &tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(intern, NULL, (zend_objects_free_object_storage_t) php_imagickdraw_object_free_storage, NULL TSRMLS_CC);
	retval.handlers = (zend_object_handlers *) &imagickdraw_object_handlers;
	return retval;
}

static zend_object_value php_imagickdraw_object_new(zend_class_entry *class_type TSRMLS_DC)
{
	return php_imagickdraw_object_new_ex(class_type, NULL, 1 TSRMLS_CC);
}

static zend_object_value php_imagickpixeliterator_object_new(zend_class_entry *class_type TSRMLS_DC)
{
	zval *tmp;
	zend_object_value retval;
	php_imagickpixeliterator_object *intern;

	/* Allocate memory for it */
	intern = (php_imagickpixeliterator_object *) emalloc(sizeof(php_imagickpixeliterator_object));
	memset(&intern->zo, 0, sizeof(zend_object));

	/* We cant initialize yet */
	intern->pixel_iterator = NULL;
	intern->instanciated_correctly = 0;

#if MagickLibVersion <= 0x628
	intern->rows = 0;
	intern->iterator_position = 0;
#endif

	/* ALLOC_HASHTABLE(intern->zo.properties); */

	zend_object_std_init(&intern->zo, class_type TSRMLS_CC);
	zend_hash_copy(intern->zo.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref,(void *) &tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(intern, NULL, (zend_objects_free_object_storage_t) php_imagickpixeliterator_object_free_storage, NULL TSRMLS_CC);
	retval.handlers = (zend_object_handlers *) &imagickpixeliterator_object_handlers;
	return retval;
}

static zend_object_value php_imagickpixel_object_new_ex(zend_class_entry *class_type, php_imagickpixel_object **ptr TSRMLS_DC)
{
	zval *tmp;
	zend_object_value retval;
	php_imagickpixel_object *intern;

	/* Allocate memory for it */
	intern = (php_imagickpixel_object *) emalloc(sizeof(php_imagickpixel_object));
	memset(&intern->zo, 0, sizeof(zend_object));

	if (ptr) {
		*ptr = intern;
	}

	/* Set the pixeliterator */
	intern->pixel_wand = NULL;
	intern->initialized_via_iterator = 0;

	/* ALLOC_HASHTABLE(intern->zo.properties); */

	zend_object_std_init(&intern->zo, class_type TSRMLS_CC);
	zend_hash_copy(intern->zo.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref,(void *) &tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(intern, NULL, (zend_objects_free_object_storage_t) php_imagickpixel_object_free_storage, NULL TSRMLS_CC);
	retval.handlers = (zend_object_handlers *) &imagickpixel_object_handlers;
	return retval;
}

static zend_object_value php_imagickpixel_object_new(zend_class_entry *class_type TSRMLS_DC)
{
	return php_imagickpixel_object_new_ex(class_type, NULL TSRMLS_CC);
}

PHP_INI_BEGIN()
	STD_PHP_INI_ENTRY("imagick.locale_fix", "0", PHP_INI_ALL, OnUpdateBool, locale_fix, zend_imagick_globals, imagick_globals)
	STD_PHP_INI_ENTRY("imagick.progress_monitor", "0", PHP_INI_SYSTEM, OnUpdateBool, progress_monitor, zend_imagick_globals, imagick_globals)
PHP_INI_END()

static void php_imagick_init_globals(zend_imagick_globals *imagick_globals)
{
	imagick_globals->locale_fix = 0;
	imagick_globals->progress_monitor = 0;
}

static zval *php_imagick_read_property(zval *object, zval *member, int type TSRMLS_DC) 
{
	php_imagick_object *intern;
	zval *retval;
	MAKE_STD_ZVAL(retval);
	
	intern = (php_imagick_object *)zend_object_store_get_object(object TSRMLS_CC);
	
	if (Z_TYPE_P(member) != IS_STRING || MagickGetNumberImages(intern->magick_wand) == 0) {
		ZVAL_NULL(retval);
	} else {	
		char *property   = Z_STRVAL_P(member);
		int property_len = Z_STRLEN_P(member);

		if ((property_len == 5) && (memcmp(property, "width", 5) == 0)) {
			ZVAL_LONG(retval, MagickGetImageWidth(intern->magick_wand));
		} else if ((property_len == 6) && (memcmp(property, "height", 6) == 0)) {
			ZVAL_LONG(retval, MagickGetImageHeight(intern->magick_wand));
		} else if ((property_len == 6) && (memcmp(property, "format", 6) == 0)) {
			char *format = MagickGetImageFormat(intern->magick_wand);
			if (format) {
				ZVAL_STRING(retval, format, 1);
				IMAGICK_FREE_MEMORY(char *, format);
			} else {
				ZVAL_STRING(retval, "", 1);
			}
		} else {
			ZVAL_NULL(retval);
		}
	}
#ifdef Z_SET_REFCOUNT_P
	Z_SET_REFCOUNT_P(retval, 0);
#else
	retval->refcount = 0;
#endif
	return retval;
}

static zend_object_value php_imagick_clone_imagick_object(zval *this_ptr TSRMLS_DC)
{
	MagickWand *wand_copy = NULL;
	php_imagick_object *new_obj = NULL;
	php_imagick_object *old_obj = (php_imagick_object *) zend_object_store_get_object(this_ptr TSRMLS_CC);
	zend_object_value new_ov = php_imagick_object_new_ex(old_obj->zo.ce, &new_obj, 0 TSRMLS_CC);

	zend_objects_clone_members(&new_obj->zo, new_ov, &old_obj->zo, Z_OBJ_HANDLE_P(this_ptr) TSRMLS_CC);

	wand_copy = CloneMagickWand(old_obj->magick_wand);
	IMAGICK_REPLACE_MAGICKWAND(new_obj, wand_copy);
	new_obj->next_out_of_bound = old_obj->next_out_of_bound;

	if (old_obj->progress_monitor_name) {
		new_obj->progress_monitor_name = estrdup(old_obj->progress_monitor_name);
	}

	return new_ov;
}

static zend_object_value php_imagick_clone_imagickdraw_object(zval *this_ptr TSRMLS_DC)
{
	DrawingWand *wand_copy = NULL;
	php_imagickdraw_object *new_obj = NULL;
	php_imagickdraw_object *old_obj = (php_imagickdraw_object *) zend_object_store_get_object(this_ptr TSRMLS_CC);
	zend_object_value new_ov = php_imagickdraw_object_new_ex(old_obj->zo.ce, &new_obj, 0 TSRMLS_CC);

	zend_objects_clone_members(&new_obj->zo, new_ov, &old_obj->zo, Z_OBJ_HANDLE_P(this_ptr) TSRMLS_CC);
	wand_copy = CloneDrawingWand(old_obj->drawing_wand);

	IMAGICKDRAW_REPLACE_DRAWINGWAND(new_obj, wand_copy);
	return new_ov;
}

static zend_object_value php_imagick_clone_imagickpixel_object(zval *this_ptr TSRMLS_DC)
{
	PixelWand *wand_copy = NULL;
	php_imagickpixel_object *new_obj = NULL;
	php_imagickpixel_object *old_obj = (php_imagickpixel_object *) zend_object_store_get_object(this_ptr TSRMLS_CC);
	zend_object_value new_ov = php_imagickpixel_object_new_ex(old_obj->zo.ce, &new_obj TSRMLS_CC);

	zend_objects_clone_members(&new_obj->zo, new_ov, &old_obj->zo, Z_OBJ_HANDLE_P(this_ptr) TSRMLS_CC);

	IMAGICK_CLONE_PIXELWAND(old_obj->pixel_wand, wand_copy);
	IMAGICKPIXEL_REPLACE_PIXELWAND(new_obj, wand_copy);

	new_obj->initialized_via_iterator = 0;
	return new_ov;
}

PHP_MINIT_FUNCTION(imagick)
{
	zend_class_entry ce;

	/* Initialize globals */
	ZEND_INIT_MODULE_GLOBALS(imagick, php_imagick_init_globals, NULL);

	/*
		Allocate some memory
	*/
	memcpy(&imagick_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	memcpy(&imagickdraw_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	memcpy(&imagickpixeliterator_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	memcpy(&imagickpixel_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	/* Setup magickwand env */
	MagickWandGenesis();

	/*
		Initialize exceptions (Imagick exception)
	*/
	INIT_CLASS_ENTRY(ce, PHP_IMAGICK_EXCEPTION_SC_NAME, NULL);
	php_imagick_exception_class_entry = zend_register_internal_class_ex(&ce, zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC);
	php_imagick_exception_class_entry->ce_flags |= ZEND_ACC_FINAL;

	/*
	Initialize exceptions (ImagickDraw exception)
	*/
	INIT_CLASS_ENTRY(ce, PHP_IMAGICKDRAW_EXCEPTION_SC_NAME, NULL);
	php_imagickdraw_exception_class_entry = zend_register_internal_class_ex(&ce, zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC);
	php_imagickdraw_exception_class_entry->ce_flags |= ZEND_ACC_FINAL;

	/*
	Initialize exceptions (ImagickPixelIterator exception)
	*/
	INIT_CLASS_ENTRY(ce, PHP_IMAGICKPIXELITERATOR_EXCEPTION_SC_NAME, NULL);
	php_imagickpixeliterator_exception_class_entry = zend_register_internal_class_ex(&ce, zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC);
	php_imagickpixeliterator_exception_class_entry->ce_flags |= ZEND_ACC_FINAL;

	/*
	Initialize exceptions (ImagickPixel exception)
	*/
	INIT_CLASS_ENTRY(ce, PHP_IMAGICKPIXEL_EXCEPTION_SC_NAME, NULL);
	php_imagickpixel_exception_class_entry = zend_register_internal_class_ex(&ce, zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC);
	php_imagickpixel_exception_class_entry->ce_flags |= ZEND_ACC_FINAL;

	/*
		Initialize the class (Imagick)
	*/
	INIT_CLASS_ENTRY(ce, PHP_IMAGICK_SC_NAME, php_imagick_class_methods);
	ce.create_object = php_imagick_object_new;
	imagick_object_handlers.clone_obj = php_imagick_clone_imagick_object;
	imagick_object_handlers.read_property = php_imagick_read_property;
	php_imagick_sc_entry = zend_register_internal_class(&ce TSRMLS_CC);
	zend_class_implements(php_imagick_sc_entry TSRMLS_CC, 1, zend_ce_iterator);

	/*
		Initialize the class (ImagickDraw)
	*/
	INIT_CLASS_ENTRY(ce, PHP_IMAGICKDRAW_SC_NAME, php_imagickdraw_class_methods);
	ce.create_object = php_imagickdraw_object_new;
	imagickdraw_object_handlers.clone_obj = php_imagick_clone_imagickdraw_object;
	php_imagickdraw_sc_entry = zend_register_internal_class(&ce TSRMLS_CC);

	/*
		Initialize the class (ImagickPixelIterator)
	*/
	INIT_CLASS_ENTRY(ce, PHP_IMAGICKPIXELITERATOR_SC_NAME, php_imagickpixeliterator_class_methods);
	ce.create_object = php_imagickpixeliterator_object_new;
	imagickpixeliterator_object_handlers.clone_obj = NULL;
	php_imagickpixeliterator_sc_entry = zend_register_internal_class(&ce TSRMLS_CC);
	zend_class_implements(php_imagickpixeliterator_sc_entry TSRMLS_CC, 1, zend_ce_iterator);

	/*
		Initialize the class (ImagickPixel)
	*/
	INIT_CLASS_ENTRY(ce, PHP_IMAGICKPIXEL_SC_NAME, php_imagickpixel_class_methods);
	ce.create_object = php_imagickpixel_object_new;
	imagickpixel_object_handlers.clone_obj = php_imagick_clone_imagickpixel_object;
	php_imagickpixel_sc_entry = zend_register_internal_class(&ce TSRMLS_CC);

	initialize_imagick_constants();

#if defined(ZTS) && defined(PHP_WIN32)
	imagick_mutex = tsrm_mutex_alloc();
#endif

	REGISTER_INI_ENTRIES();
	return SUCCESS;
}


PHP_MINFO_FUNCTION(imagick)
{
	smart_str formats = {0};
	
	char **supported_formats, buffer[52];
	unsigned long version_number, num_formats = 0, i;

	supported_formats = MagickQueryFormats("*", &num_formats);
	snprintf(buffer, 52, "%ld", num_formats);

	php_info_print_table_start();
	php_info_print_table_header(2, "imagick module", "enabled");
	php_info_print_table_row(2, "imagick module version", PHP_IMAGICK_VERSION);
	php_info_print_table_row(2, "imagick classes", "Imagick, ImagickDraw, ImagickPixel, ImagickPixelIterator");
	php_info_print_table_row(2, "ImageMagick version", MagickGetVersion(&version_number));
	php_info_print_table_row(2, "ImageMagick copyright", MagickGetCopyright());
	php_info_print_table_row(2, "ImageMagick release date", MagickGetReleaseDate());
	php_info_print_table_row(2, "ImageMagick number of supported formats: ", buffer);

	if (supported_formats) {
		for (i = 0; i < num_formats; i++) {
			smart_str_appends(&formats, supported_formats[i]);
			if (i != (num_formats - 1)) {
 				smart_str_appends(&formats, ", ");
			}
			IMAGICK_FREE_MEMORY(char *, supported_formats[i]);
		}
		smart_str_0(&formats);
		php_info_print_table_row(2, "ImageMagick supported formats", formats.c);
		smart_str_free(&formats);
		IMAGICK_FREE_MEMORY(char **, supported_formats);
	}

	php_info_print_table_end();
	DISPLAY_INI_ENTRIES();
}


PHP_MSHUTDOWN_FUNCTION(imagick)
{
	/* Destroy the magick wand env */
	MagickWandTerminus();
#if defined(ZTS) && defined(PHP_WIN32)
	tsrm_mutex_free(imagick_mutex);
#endif
	UNREGISTER_INI_ENTRIES();
	return(SUCCESS);
}

#if defined(ZTS) && defined(PHP_WIN32)
PHP_RSHUTDOWN_FUNCTION(imagick)
{
	/* We have the lock so lets release it */
	if (imagick_thread_id == tsrm_thread_id()) {
		imagick_thread_id = (THREAD_T)NULL;
		tsrm_mutex_unlock(imagick_mutex);
	}

	return SUCCESS;
}
#endif

zend_module_entry imagick_module_entry =
{
#if ZEND_MODULE_API_NO >= 20010901
        STANDARD_MODULE_HEADER,
#endif
        PHP_IMAGICK_EXTNAME,
        php_imagick_functions,                  /* Functions */
        PHP_MINIT(imagick),                     /* MINIT */
        PHP_MSHUTDOWN(imagick),         /* MSHUTDOWN */
        NULL,                                               /* RINIT */
#if defined(ZTS) && defined(PHP_WIN32)
        PHP_RSHUTDOWN(imagick),
#else
        NULL,                                               /* RSHUTDOWN */
#endif
        PHP_MINFO(imagick),                     /* MINFO */
        PHP_IMAGICK_VERSION,
        STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_IMAGICK
ZEND_GET_MODULE(imagick)
#endif
