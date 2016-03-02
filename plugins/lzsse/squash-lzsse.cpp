/* Copyright (c) 2015-2016 The Squash Authors
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *   Evan Nemerson <evan@nemerson.com>
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <squash/squash.h>

#include "lzsse/lzsse2/lzsse2.h"
#include "lzsse/lzsse4/lzsse4.h"
#include "lzsse/lzsse8/lzsse8.h"

enum SquashLZSSEOptIndex {
  SQUASH_LZSSE_OPT_LEVEL = 0,
};

static SquashOptionInfo squash_lzsse_options[] = {
  { "level", SQUASH_OPTION_TYPE_RANGE_INT, },
  { NULL, SQUASH_OPTION_TYPE_NONE, }
};

extern "C" SQUASH_PLUGIN_EXPORT
SquashStatus              squash_plugin_init_codec    (SquashCodec* codec, SquashCodecImpl* impl);
extern "C" SQUASH_PLUGIN_EXPORT
SquashStatus              squash_plugin_init_plugin   (SquashPlugin* plugin);

static size_t
squash_lzsse_get_max_compressed_size (SquashCodec* codec, size_t uncompressed_size) {
  for (size_t extra = 0 ; extra < (sizeof(size_t) * 8) ; extra += 7) {
    if (uncompressed_size < (((size_t) 1) << extra)) {
      return (extra / 7) + uncompressed_size;
    }
  }
  return (sizeof(size_t) * 8) + uncompressed_size;
}

static SquashStatus
squash_lzsse2_compress_buffer (SquashCodec* codec,
                              size_t* compressed_size,
                              uint8_t compressed[SQUASH_ARRAY_PARAM(*compressed_size)],
                              size_t uncompressed_size,
                              const uint8_t uncompressed[SQUASH_ARRAY_PARAM(uncompressed_size)],
                              SquashOptions* options) {
  const int level = squash_options_get_int_at (options, codec, SQUASH_LZSSE_OPT_LEVEL);
  LZSSE2_OptimalParseState* state = LZSSE2_MakeOptimalParseState (uncompressed_size);

  *compressed_size = LZSSE2_CompressOptimalParse(state, uncompressed, uncompressed_size, compressed, *compressed_size, level);

  LZSSE2_FreeOptimalParseState (state);
  return (*compressed_size != 0) ? SQUASH_OK : SQUASH_FAILED;
}

static SquashStatus
squash_lzsse2_decompress_buffer (SquashCodec* codec,
                                size_t* decompressed_size,
                                uint8_t decompressed[SQUASH_ARRAY_PARAM(*decompressed_size)],
                                size_t compressed_size,
                                const uint8_t compressed[SQUASH_ARRAY_PARAM(compressed_size)],
                                SquashOptions* options) {
  const size_t bytes_decompressed = LZSSE2_Decompress (compressed, compressed_size, decompressed, *decompressed_size);
  if (bytes_decompressed == *decompressed_size)
    return SQUASH_OK;
  else
    return SQUASH_FAILED;
}

static SquashStatus
squash_lzsse4_compress_buffer (SquashCodec* codec,
                              size_t* compressed_size,
                              uint8_t compressed[SQUASH_ARRAY_PARAM(*compressed_size)],
                              size_t uncompressed_size,
                              const uint8_t uncompressed[SQUASH_ARRAY_PARAM(uncompressed_size)],
                              SquashOptions* options) {
  LZSSE4_FastParseState* state = LZSSE4_MakeFastParseState ();

  *compressed_size = LZSSE4_CompressFast(state, uncompressed, uncompressed_size, compressed, *compressed_size);

  LZSSE4_FreeFastParseState (state);
  return (*compressed_size != 0) ? SQUASH_OK : SQUASH_FAILED;
}

static SquashStatus
squash_lzsse4_decompress_buffer (SquashCodec* codec,
                                size_t* decompressed_size,
                                uint8_t decompressed[SQUASH_ARRAY_PARAM(*decompressed_size)],
                                size_t compressed_size,
                                const uint8_t compressed[SQUASH_ARRAY_PARAM(compressed_size)],
                                SquashOptions* options) {
  const size_t bytes_decompressed = LZSSE4_Decompress (compressed, compressed_size, decompressed, *decompressed_size);
  if (bytes_decompressed == *decompressed_size)
    return SQUASH_OK;
  else
    return SQUASH_FAILED;
}

static SquashStatus
squash_lzsse8_compress_buffer (SquashCodec* codec,
                              size_t* compressed_size,
                              uint8_t compressed[SQUASH_ARRAY_PARAM(*compressed_size)],
                              size_t uncompressed_size,
                              const uint8_t uncompressed[SQUASH_ARRAY_PARAM(uncompressed_size)],
                              SquashOptions* options) {
  const int level = squash_options_get_int_at (options, codec, SQUASH_LZSSE_OPT_LEVEL);
  LZSSE8_OptimalParseState* state = LZSSE8_MakeOptimalParseState (uncompressed_size);

  *compressed_size = LZSSE8_CompressOptimalParse(state, uncompressed, uncompressed_size, compressed, *compressed_size, level);

  LZSSE8_FreeOptimalParseState (state);
  return (*compressed_size != 0) ? SQUASH_OK : SQUASH_FAILED;
}

static SquashStatus
squash_lzsse8_decompress_buffer (SquashCodec* codec,
                                size_t* decompressed_size,
                                uint8_t decompressed[SQUASH_ARRAY_PARAM(*decompressed_size)],
                                size_t compressed_size,
                                const uint8_t compressed[SQUASH_ARRAY_PARAM(compressed_size)],
                                SquashOptions* options) {
  const size_t bytes_decompressed = LZSSE8_Decompress (compressed, compressed_size, decompressed, *decompressed_size);
  if (bytes_decompressed == *decompressed_size)
    return SQUASH_OK;
  else
    return SQUASH_FAILED;
}

extern "C" SquashStatus
squash_plugin_init_plugin (SquashPlugin* plugin) {
  const SquashOptionInfoRangeInt level_range = { 1, 17, 0, false };

  squash_lzsse_options[SQUASH_LZSSE_OPT_LEVEL].default_value.int_value = 7;
  squash_lzsse_options[SQUASH_LZSSE_OPT_LEVEL].info.range_int = level_range;

  return SQUASH_OK;
}

extern "C" SquashStatus
squash_plugin_init_codec (SquashCodec* codec, SquashCodecImpl* impl) {
  const char* name = squash_codec_get_name (codec);

  if (strcmp ("lzsse2", name) == 0) {
    impl->decompress_buffer = squash_lzsse2_decompress_buffer;
    impl->compress_buffer = squash_lzsse2_compress_buffer;
    impl->options = squash_lzsse_options;
  } else if (strcmp ("lzsse4", name) == 0) {
    impl->decompress_buffer = squash_lzsse4_decompress_buffer;
    impl->compress_buffer = squash_lzsse4_compress_buffer;
  } else if (strcmp ("lzsse8", name) == 0) {
    impl->decompress_buffer = squash_lzsse8_decompress_buffer;
    impl->compress_buffer = squash_lzsse8_compress_buffer;
    impl->options = squash_lzsse_options;
  } else {
    return squash_error (SQUASH_UNABLE_TO_LOAD);
  }

  impl->info = SQUASH_CODEC_INFO_WRAP_SIZE;
  impl->get_max_compressed_size = squash_lzsse_get_max_compressed_size;

  return SQUASH_OK;
}
