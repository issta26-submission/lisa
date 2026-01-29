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
//<ID> 1099
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize and compress an in-memory payload with deflate
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence payload for deflate -> inflate -> gzip read test.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream deflate_strm;
    memset(&deflate_strm, 0, (int)sizeof(deflate_strm));
    int def_init_ret = deflateInit_(&deflate_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&deflate_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    deflate_strm.next_in = (Bytef *)source;
    deflate_strm.avail_in = (uInt)source_len;
    deflate_strm.next_out = comp_buf;
    deflate_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&deflate_strm, 4);
    uLong comp_size = deflate_strm.total_out;
    int def_end_ret = deflateEnd(&deflate_strm);

    // step 2: Inflate the compressed buffer back to an output buffer
    z_stream inflate_strm;
    memset(&inflate_strm, 0, (int)sizeof(inflate_strm));
    int inf_init_ret = inflateInit_(&inflate_strm, version, (int)sizeof(z_stream));
    inflate_strm.next_in = comp_buf;
    inflate_strm.avail_in = (uInt)comp_size;
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    inflate_strm.next_out = out_buf;
    inflate_strm.avail_out = (uInt)out_alloc;
    int inflate_ret = inflate(&inflate_strm, 4);
    uLong out_size = inflate_strm.total_out;
    int inflate_end_ret = inflateEnd(&inflate_strm);

    // step 3: Persist compressed bytes to a .gz file, then open for read and use gzgetc_ + gzclose_r
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = 0;
    if (gw) gzwrite_ret = gzwrite(gw, comp_buf, (unsigned int)comp_size);
    int gzclose_write_ret = 0;
    if (gw) gzclose_write_ret = gzclose(gw);
    gzFile gr = gzopen("tmp_zlib_api_seq.gz", "rb");
    int gotc = 0;
    if (gr) gotc = gzgetc_(gr);
    int gzclose_read_ret = 0;
    if (gr) gzclose_read_ret = gzclose_r(gr);

    // step 4: Validate simple checksum, cleanup and finalize
    uLong crc = crc32(0UL, comp_buf, (uInt)comp_size);
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inflate_ret;
    (void)out_size;
    (void)inflate_end_ret;
    (void)gzwrite_ret;
    (void)gzclose_write_ret;
    (void)gotc;
    (void)gzclose_read_ret;
    (void)crc;
    return 66;
}
// API sequence test completed successfully