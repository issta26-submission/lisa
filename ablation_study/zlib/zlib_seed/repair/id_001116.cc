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
//<ID> 1116
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize and compress an in-memory payload with deflate
    const Bytef source[] = "zlib API sequence payload for gzopen/gzrewind/gzclose and inflateEnd test.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong comp_bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;

    // step 2: Persist compressed bytes to a .gz file, then open for read and rewind/close
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gw, comp_buf, (unsigned int)comp_size);
    int gzclose_write_ret = gzclose(gw);
    gzFile gr = gzopen("tmp_zlib_api_seq.gz", "rb");
    int gzrewind_ret = gzrewind(gr);
    int gzclose_read_ret = gzclose(gr);

    // step 3: Inflate the compressed buffer back to an output buffer and call inflateEnd
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_size;
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)out_alloc;
    int inflate_ret = inflate(&inf_strm, 4);
    uLong out_size = inf_strm.total_out;
    int inflate_end_ret = inflateEnd(&inf_strm);

    // step 4: Cleanup and finalize
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)def_init_ret;
    (void)comp_bound;
    (void)def_ret;
    (void)comp_size;
    (void)gzwrite_ret;
    (void)gzclose_write_ret;
    (void)gzrewind_ret;
    (void)gzclose_read_ret;
    (void)inf_init_ret;
    (void)inflate_ret;
    (void)out_size;
    (void)inflate_end_ret;
    (void)def_end_ret;
    return 66;
    // API sequence test completed successfully
}