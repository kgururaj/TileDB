/**
 * @file storage_fs.h
 *
 * @section LICENSE
 *
 * The MIT License
 *
 * @copyright Copyright (c) 2018-2019 Omics Data Automation, Inc.
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
 * Storage FS Base Implementation
 *
 */

#include "storage_fs.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h>

/* ****************************** */
/*        GLOBAL VARIABLES        */
/* ****************************** */

std::string tiledb_fs_errmsg = "";

StorageFS::~StorageFS() {
  // Default
}

int StorageFS::close_file(const std::string& filename) {
  return TILEDB_FS_OK;
}

bool StorageFS::locking_support() {
  return false;
}

void StorageFS::set_disable_file_locking(const bool val) {
  disable_file_locking_ = val;
}

bool StorageFS::disable_file_locking() {
  if (is_disable_file_locking_set) {
    return disable_file_locking_;
  }

  is_disable_file_locking_set = true;

  if(disable_file_locking_)
    return true;

  auto env_var = getenv("TILEDB_DISABLE_FILE_LOCKING");
  if(env_var && (strcasecmp(env_var, "true") == 0 || strcmp(env_var, "1") == 0)) {
    disable_file_locking_ = true;
    return true;
  }
  disable_file_locking_ = false;
  return false;
}
