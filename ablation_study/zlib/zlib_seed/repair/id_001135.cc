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
//<ID> 1135
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source data and compress with deflate
    const Bytef source[] = "zlib API sequence fuzz driver payload for deflate/inflate/gzwrite/gzclose test.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    const char *version = zlibVersion();
    int dinit_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)source_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)bound;
    int def_ret = deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    int def_end_ret = deflateEnd(&dstrm);

    // step 2: Inflate compressed data back to an output buffer
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int iinit_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_size;
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)out_alloc;
    int inflate_ret = inflate(&istrm, 4);
    uLong out_size = istrm.total_out;
    int inflate_end_ret = inflateEnd(&istrm);

    // step 3: Persist inflated output to a .gz file using gzwrite and close it
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gw, out_buf, (unsigned int)out_size);
    int gzclose_ret = gzclose(gw);

    // step 4: Validate checksum, cleanup and finalize
    uLong checksum = crc32(0UL, out_buf, (uInt)out_size);
    free(comp_buf);
    free(out_buf);
    (void)dinit_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)iinit_ret;
    (void)inflate_ret;
    (void)inflate_end_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)checksum;
    (void)version;
    (void)bound;
    (void)out_alloc;
    (void)source_len;
    (void)comp_size;
    (void)out_size;
    return 66;
    // API sequence test completed successfully
}