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
//<ID> 1142
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "zlib API sequence fuzz driver payload for deflateBound/gzbuffer/inflateResetKeep/inflateEnd usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    const char *version = zlibVersion();

    // step 2: Deflate initialization, determine bound, compress source
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)(bound + 16));
    memset(comp_buf, 0, (size_t)(bound + 16));
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound + 16);
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Persist original source into a .gz file with buffer tuning
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzbuffer_ret = gzbuffer(gw, 8192);
    int gzwrite_ret = gzwrite(gw, source, (unsigned int)source_len);
    int gzclose_write_ret = gzclose(gw);

    // step 4: Inflate the compressed buffer back, use inflateResetKeep then finalize and cleanup
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_size;
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)out_alloc;
    int inf_ret = inflate(&inf_strm, 4);
    uLong out_size = inf_strm.total_out;
    int inf_reset_keep_ret = inflateResetKeep(&inf_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    uLong checksum = crc32(0UL, out_buf, (uInt)out_size);

    free(comp_buf);
    free(out_buf);

    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)bound;
    (void)version;
    (void)gzbuffer_ret;
    (void)gzwrite_ret;
    (void)gzclose_write_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_reset_keep_ret;
    (void)inf_end_ret;
    (void)checksum;
    (void)source_len;
    (void)out_size;
    return 66;
    // API sequence test completed successfully
}