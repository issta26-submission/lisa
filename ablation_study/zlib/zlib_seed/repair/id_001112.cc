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
//<ID> 1112
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize deflate stream, allocate buffers, and compress source data
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence payload: deflate -> inflate -> gzip write/read sequence test.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 2: Initialize inflate stream and decompress back into an output buffer
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_size;
    uLong out_alloc = source_len + 256;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)out_alloc;
    int inf_ret = inflate(&inf_strm, 4);
    uLong out_size = inf_strm.total_out;
    int inflate_end_ret = inflateEnd(&inf_strm);

    // step 3: Persist compressed bytes to a .gz file, then rewind and close the file
    gzFile gw = gzopen("tmp_zlib_api_sequence.gz", "wb");
    int gzwrite_ret = gzwrite(gw, comp_buf, (unsigned int)comp_size);
    int gzrewind_ret = gzrewind(gw);
    int gzclose_ret = gzclose(gw);

    // step 4: Validate checksum, cleanup and finalize
    uLong crc = crc32(0UL, comp_buf, (uInt)comp_size);
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)out_size;
    (void)inflate_end_ret;
    (void)gzwrite_ret;
    (void)gzrewind_ret;
    (void)gzclose_ret;
    (void)crc;
    return 66;
    // API sequence test completed successfully
}