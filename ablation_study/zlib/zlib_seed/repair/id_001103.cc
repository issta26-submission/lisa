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
//<ID> 1103
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial data
    const Bytef source[] = "zlib API sequence payload for deflate -> crc32 -> inflateBackEnd test.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();

    // step 2: Initialize deflate stream and allocate output buffer
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    int def_init_ret = deflateInit_(&deflate_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&deflate_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);

    // step 3: Perform deflate (compress) operation
    deflate_strm.next_in = (Bytef *)source;
    deflate_strm.avail_in = (uInt)source_len;
    deflate_strm.next_out = comp_buf;
    deflate_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&deflate_strm, 4); // 4 == Z_FINISH
    uLong comp_size = deflate_strm.total_out;
    int def_end_ret = deflateEnd(&deflate_strm);

    // step 4: Validate with crc32, initialize inflate stream and call inflateBackEnd, cleanup
    uLong checksum = crc32(0UL, comp_buf, (uInt)comp_size);
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(z_stream));
    int inf_init_ret = inflateInit_(&inflate_strm, version, (int)sizeof(z_stream));
    inflate_strm.next_in = comp_buf;
    inflate_strm.avail_in = (uInt)comp_size;
    int inf_back_end_ret = inflateBackEnd(&inflate_strm);
    free(comp_buf);

    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)def_end_ret;
    (void)checksum;
    (void)inf_init_ret;
    (void)inf_back_end_ret;
    return 66;
}
// API sequence test completed successfully