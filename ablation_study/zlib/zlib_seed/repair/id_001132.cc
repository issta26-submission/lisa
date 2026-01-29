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
//<ID> 1132
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source and deflate stream
    const Bytef source[] = "zlib API sequence fuzz driver payload for deflate/inflate/gzwrite/gzclose test.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);

    // step 2: Compress with deflate and finalize
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)source_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)bound;
    int def_ret = deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    int def_end_ret = deflateEnd(&dstrm);

    // step 3: Inflate compressed data back, finalize inflate
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)out_alloc;
    int inf_ret = inflate(&istrm, 4);
    uLong out_size = istrm.total_out;
    int inf_end_ret = inflateEnd(&istrm);

    // step 4: Persist original source to .gz using gzwrite, close, validate and cleanup
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gw, source, (unsigned int)source_len);
    int gzclose_ret = gzclose(gw);
    uLong checksum = crc32(0UL, out_buf, (uInt)out_size);
    free(comp_buf);
    free(out_buf);
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)checksum;
    (void)version;
    (void)bound;
    (void)source_len;
    (void)out_size;
    return 66;
    // API sequence test completed successfully
}