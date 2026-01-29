#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1102
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and source preparation
    const Bytef source[] = "zlib API sequence payload for deflate -> crc32 -> inflateBackInit -> inflateBackEnd test.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream deflate_strm;
    memset(&deflate_strm, 0, (int)sizeof(deflate_strm));
    const char *version = zlibVersion();

    // step 2: Initialize deflate, compress data
    int def_init_ret = deflateInit_(&deflate_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&deflate_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    deflate_strm.next_in = (Bytef *)source;
    deflate_strm.avail_in = (uInt)source_len;
    deflate_strm.next_out = comp_buf;
    deflate_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&deflate_strm, 4); // 4 == Z_FINISH in zlib
    uLong comp_size = deflate_strm.total_out;
    int def_end_ret = deflateEnd(&deflate_strm);

    // step 3: Validate compressed payload with CRC and initialize inflate-back then teardown
    uLong crc = crc32(0UL, comp_buf, (uInt)comp_size);
    z_stream inflate_strm;
    memset(&inflate_strm, 0, (int)sizeof(inflate_strm));
    unsigned char *window_buf = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window_buf, 0, (int)(1 << 15));
    int inf_back_init_ret = inflateBackInit_(&inflate_strm, 15, window_buf, version, (int)sizeof(z_stream));
    int inf_back_end_ret = inflateBackEnd(&inflate_strm);

    // step 4: Cleanup and finalize
    free(comp_buf);
    free(window_buf);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)def_end_ret;
    (void)crc;
    (void)inf_back_init_ret;
    (void)inf_back_end_ret;
    return 66;
}
// API sequence test completed successfully