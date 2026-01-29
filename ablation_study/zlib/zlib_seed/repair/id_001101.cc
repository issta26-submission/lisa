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
//<ID> 1101
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize and configure deflate stream
    const Bytef source[] = "zlib API sequence payload: initialize -> compress -> crc -> cleanup.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream deflate_strm;
    memset((void *)&deflate_strm, 0, (int)sizeof(deflate_strm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&deflate_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&deflate_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset((void *)comp_buf, 0, (int)bound);

    // step 2: Operate - compress the payload
    deflate_strm.next_in = (Bytef *)source;
    deflate_strm.avail_in = (uInt)source_len;
    deflate_strm.next_out = comp_buf;
    deflate_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&deflate_strm, 4);
    uLong comp_size = deflate_strm.total_out;

    // step 3: Validate - compute CRCs of original and compressed buffers
    uLong crc_orig = crc32(0UL, source, (uInt)source_len);
    uLong crc_comp = crc32(0UL, comp_buf, (uInt)comp_size);
    (void)crc_orig;
    (void)crc_comp;

    // step 4: Cleanup - initialize an inflateBack stream to demonstrate inflateBackEnd, then free resources
    z_stream inflate_back_strm;
    memset((void *)&inflate_back_strm, 0, (int)sizeof(inflate_back_strm));
    unsigned char *window_buf = (unsigned char *)malloc((size_t)(1 << 15));
    memset((void *)window_buf, 0, (int)(1 << 15));
    int inflback_init_ret = inflateBackInit_(&inflate_back_strm, 15, window_buf, version, (int)sizeof(z_stream));
    int inflback_end_ret = inflateBackEnd(&inflate_back_strm);
    int def_end_ret = deflateEnd(&deflate_strm);
    free((void *)comp_buf);
    free((void *)window_buf);
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)inflback_init_ret;
    (void)inflback_end_ret;
    (void)def_end_ret;
    return 66;
    // API sequence test completed successfully
}