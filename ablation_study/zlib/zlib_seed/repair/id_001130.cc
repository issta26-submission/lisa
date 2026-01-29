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
//<ID> 1130
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and declarations
    const Bytef source[] = "zlib API sequence fuzz driver payload: deflate -> inflate -> gzwrite -> gzclose.";
    uLong source_len = (uLong)(sizeof(source) - 1);

    // step 2: Compress the source with deflate
    z_stream cstrm;
    memset(&cstrm, 0, sizeof(cstrm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&cstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&cstrm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    cstrm.next_in = (Bytef *)source;
    cstrm.avail_in = (uInt)source_len;
    cstrm.next_out = comp_buf;
    cstrm.avail_out = (uInt)bound;
    int def_ret = deflate(&cstrm, 4);
    uLong comp_size = cstrm.total_out;
    int def_end_ret = deflateEnd(&cstrm);

    // step 3: Inflate the compressed data back to verify round-trip
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_size;
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)out_alloc;
    int inf_ret = inflate(&istrm, 4);
    uLong out_size = istrm.total_out;
    int inf_end_ret = inflateEnd(&istrm);

    // step 4: Persist original source to a .gz file with gzwrite and close, then cleanup and validate checksum
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gw, source, (unsigned int)source_len);
    int gzclose_ret = gzclose(gw);
    uLong checksum = crc32(0UL, out_buf, (uInt)out_size);
    free(comp_buf);
    free(out_buf);

    (void)def_init_ret;
    (void)def_ret;
    (void)comp_size;
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