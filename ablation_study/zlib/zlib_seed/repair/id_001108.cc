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
//<ID> 1108
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute initial CRC
    const Bytef source[] = "zlib API sequence fuzz driver payload for deflate and inflateBack lifecycle.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong crc_source = crc32(0UL, source, (uInt)source_len);

    // step 2: Initialize deflate stream, compress the source into a buffer
    z_stream deflate_strm;
    memset(&deflate_strm, 0, (int)sizeof(deflate_strm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&deflate_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&deflate_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    deflate_strm.next_in = (Bytef *)source;
    deflate_strm.avail_in = (uInt)source_len;
    deflate_strm.next_out = comp_buf;
    deflate_strm.avail_out = (uInt)bound;
    int deflate_ret = deflate(&deflate_strm, 4);
    uLong comp_size = deflate_strm.total_out;
    int deflate_end_ret = deflateEnd(&deflate_strm);

    // step 3: Initialize inflateBack context and immediately tear it down (lifecycle test)
    z_stream back_strm;
    memset(&back_strm, 0, (int)sizeof(back_strm));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int back_init_ret = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));
    int back_end_ret = inflateBackEnd(&back_strm);

    // step 4: Validate checksum of compressed output and cleanup
    uLong crc_comp = crc32(0UL, comp_buf, (uInt)comp_size);
    free(comp_buf);
    free(window);
    (void)crc_source;
    (void)def_init_ret;
    (void)bound;
    (void)deflate_ret;
    (void)deflate_end_ret;
    (void)back_init_ret;
    (void)back_end_ret;
    (void)crc_comp;
    return 66;
    // API sequence test completed successfully
}