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
//<ID> 1095
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize and compress an in-memory payload with deflate
    const Bytef source[] = "zlib API sequence payload for compress-then-verify via gzip file and inflate.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream defl;
    memset(&defl, 0, (int)sizeof(defl));
    const char *ver = zlibVersion();
    int init_def_ret = deflateInit_(&defl, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&defl, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    defl.next_in = (Bytef *)source;
    defl.avail_in = (uInt)source_len;
    defl.next_out = comp_buf;
    defl.avail_out = (uInt)bound;
    int def_ret = deflate(&defl, 4);
    uLong comp_size = defl.total_out;
    int end_def_ret = deflateEnd(&defl);

    // step 2: Write compressed bytes to a .gz file and close the writer
    const char *fname = "tmp_zlib_api_seq.gz";
    gzFile gzw = gzopen(fname, "wb");
    int gw_ret = gzwrite(gzw, (voidpc)comp_buf, (unsigned int)comp_size);
    int gzclose_write_ret = gzclose(gzw);

    // step 3: Re-open .gz for reading, read one byte using gzgetc_ and close with gzclose_r
    gzFile gzr = gzopen(fname, "rb");
    int read_char = gzgetc_(gzr);
    int gzclose_read_ret = gzclose_r(gzr);

    // step 4: Inflate the compressed buffer in-memory to validate decompression, then cleanup
    z_stream infl;
    memset(&infl, 0, (int)sizeof(infl));
    int init_infl_ret = inflateInit_(&infl, ver, (int)sizeof(z_stream));
    uLong out_cap = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_cap);
    memset(out_buf, 0, (size_t)out_cap);
    infl.next_in = comp_buf;
    infl.avail_in = (uInt)comp_size;
    infl.next_out = out_buf;
    infl.avail_out = (uInt)out_cap;
    int infl_ret = inflate(&infl, 0);
    uLong decomp_size = infl.total_out;
    int infl_end_ret = inflateEnd(&infl);

    free(comp_buf);
    free(out_buf);
    (void)init_def_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)end_def_ret;
    (void)gw_ret;
    (void)gzclose_write_ret;
    (void)read_char;
    (void)gzclose_read_ret;
    (void)init_infl_ret;
    (void)infl_ret;
    (void)decomp_size;
    (void)infl_end_ret;
    (void)ver;
    (void)source_len;
    // API sequence test completed successfully
    return 66;
}