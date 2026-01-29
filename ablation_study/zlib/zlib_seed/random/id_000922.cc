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
//<ID> 922
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and compression (Initialize → Configure → Operate)
    const char payload[] = "zlib api sequence payload for compress/inflate and gzflush/crc combine tests";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLong comp_len = bound;
    int rc_compress = compress2(comp_buf, &comp_len, src_buf, (uLong)payload_len, 6);

    // step 2: Write compressed data into a gzFile and flush (Operate)
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    unsigned int to_write = (unsigned int)(comp_len <= (uLong)~0u ? comp_len : (uLong)~0u);
    int rc_gzwrite = gzwrite(gf, (voidpc)comp_buf, to_write);
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzclose = gzclose(gf);

    // step 3: Compute CRCs, combine them, query compile flags, and prepare inflate (Validate → Initialize)
    uLong crc1 = crc32(0UL, src_buf, (uInt)payload_len);
    uLong crc2 = crc32(0UL, comp_buf, (uInt)to_write);
    uLong crc_combined = crc32_combine(crc1, crc2, (off_t)payload_len);
    uLong compile_flags = zlibCompileFlags();
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len <= (uLong)~0u ? comp_len : (uLong)~0u);
    Bytef *out_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate = inflate(&istrm, 0);
    uLong out_size = istrm.total_out;
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Cleanup and finish (Cleanup)
    delete [] comp_buf;
    delete [] out_buf;
    (void)rc_compress;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose;
    (void)crc1;
    (void)crc2;
    (void)crc_combined;
    (void)compile_flags;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)out_size;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}