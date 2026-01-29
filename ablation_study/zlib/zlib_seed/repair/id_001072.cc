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
//<ID> 1072
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Configure
    const Bytef source[] = "zlib API sequence payload for compress -> write gz -> inspect offsets.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *version = zlibVersion();
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;

    // step 2: Operate (compress and inspect pending bits)
    int def_ret = deflate(&strm, 4);
    unsigned int pending = 0;
    int bits = 0;
    int pending_ret = deflatePending(&strm, &pending, &bits);
    uLong comp_size = strm.total_out;

    // step 3: Reset and compress again to validate reset behavior
    int reset_ret = deflateReset(&strm);
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int def_ret2 = deflate(&strm, 4);
    uLong comp_size2 = strm.total_out;

    // step 4: Write compressed data to a .gz file, inspect 64-bit offset, and cleanup
    gzFile gzf = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_size2);
    off64_t off64 = gzoffset64(gzf);
    int gzclose_ret = gzclose(gzf);
    int end_ret = deflateEnd(&strm);
    free(comp_buf);

    (void)init_ret;
    (void)def_ret;
    (void)pending_ret;
    (void)pending;
    (void)bits;
    (void)comp_size;
    (void)reset_ret;
    (void)def_ret2;
    (void)comp_size2;
    (void)gzwrite_ret;
    (void)off64;
    (void)gzclose_ret;
    (void)end_ret;
    (void)version;
    (void)bound;
    (void)source_len;
    // API sequence test completed successfully
    return 66;
}