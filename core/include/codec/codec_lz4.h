/**
 * @file codec_lz4.h
 *
 * @section LICENSE
 *
 * The MIT License
 *
 * @copyright Copyright (c) 2018 Omics Data Automation, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * @section DESCRIPTION
 *
 * CodecLZ4 derived from Codec for LZ4 support
 *
 */

#ifndef __CODEC_LZ4_H__
#define  __CODEC_LZ4_H__

#include "codec.h"

// Function Pointers for LZ4
#if !defined(LZ4_EXTERN_DECL)
#  define LZ4_EXTERN_DECL
#endif

LZ4_EXTERN_DECL int(*LZ4_compressBound)(int);
LZ4_EXTERN_DECL size_t(*LZ4_compress_default)(const char *, char *, int, int);
LZ4_EXTERN_DECL size_t(*LZ4_decompress_safe)(const char *, char *, int, int);

class CodecLZ4 : public Codec {
 public:
  
  CodecLZ4(int compression_level):Codec(compression_level) {
    static bool loaded = false;
    static std::mutex loading;
    
    if (!loaded) {
      loading.lock();
      
      if (!loaded) {
        dl_handle_ = get_dlopen_handle("lz4");
        if (dl_handle_ != NULL) {
	  BIND_SYMBOL(dl_handle_, LZ4_compressBound, "LZ4_compressBound", (int(*)(int)));
	  BIND_SYMBOL(dl_handle_, LZ4_compress_default, "LZ4_compress_default", (size_t(*)(const char *, char *, int, int)));
	  BIND_SYMBOL(dl_handle_, LZ4_decompress_safe, "LZ4_decompress_safe", (size_t(*)(const char *, char *, int, int)));
	  loaded = true;
	}
      }

      loading.unlock();

      if (dl_handle_ == NULL || !loaded) {
	if (dl_handle_ == NULL) {
	  char *error = dlerror();
	  if (error) {
	    std::cerr << dlerror() << std::endl << std::flush;
	  }
	}
	throw std::system_error(ECANCELED, std::generic_category(), "LZ4 library not found. Install LZ4 and setup library paths.");
      }
    }
  }
  
  int compress_tile(unsigned char* tile, size_t tile_size, void** tile_compressed, size_t& tile_compressed_size);

  int decompress_tile(unsigned char* tile_compressed,  size_t tile_compressed_size, unsigned char* tile, size_t tile_size);
  
};

#endif /*__CODEC_LZ4_H__*/
