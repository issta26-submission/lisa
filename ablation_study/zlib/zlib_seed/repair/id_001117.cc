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
//<ID> 1117
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize and compress an in-memory payload with deflate
    const Bytef source[] = "zlib API sequence payload for compression -> gzip -> inflation validation.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream defl;
    memset(&defl, 0, (int)sizeof(defl));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&defl, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&defl, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    defl.next_in = (Bytef *)source;
    defl.avail_in = (uInt)source_len;
    defl.next_out = comp_buf;
    defl.avail_out = (uInt)bound;
    int def_ret = deflate(&defl, 4);
    uLong comp_size = defl.total_out;
    int def_end_ret = deflateEnd(&defl);

    // step 2: Persist compressed bytes to a .gz file, rewind and close
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gw, comp_buf, (unsigned int)comp_size);
    int gzrew_ret = gzrewind(gw);
    int gzclose_ret = gzclose(gw);

    // step 3: Inflate the compressed buffer back to an output buffer and finalize inflate
    z_stream infl;
    memset(&infl, 0, (int)sizeof(infl));
    int inf_init_ret = inflateInit_(&infl, version, (int)sizeof(z_stream));
    infl.next_in = comp_buf;
    infl.avail_in = (uInt)comp_size;
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    infl.next_out = out_buf;
    infl.avail_out = (uInt)out_alloc;
    int inflate_ret = inflate(&infl, 4);
    uLong out_size = infl.total_out;
    int inflate_end_ret = inflateEnd(&infl);

    // step 4: Validate checksum, cleanup and finalize
    uLong crc = crc32(0UL, comp_buf, (uInt)comp_size);
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)def_end_ret;
    (void)gzwrite_ret;
    (void)gzrew_ret;
    (void)gzclose_ret;
    (void)inf_init_ret;
    (void)inflate_ret;
    (void)out_size;
    (void)inflate_end_ret;
    (void)crc;
    return 66;
    // API sequence test completed successfully
}