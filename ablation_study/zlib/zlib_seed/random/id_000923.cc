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
//<ID> 923
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations, write payload to a gzFile and flush
    const char payload[] = "zlib api sequence payload to exercise compress/inflate, gzflush, flags and crc combine";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzbuffer = gzbuffer(gf, 8192);
    unsigned int to_write = (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, to_write);
    int rc_gzflush = gzflush(gf, 0);

    // step 2: Compress the payload into a compressed buffer
    uLong bound = compressBound(payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len, src_buf, payload_len, 6);

    // step 3: Initialize inflate stream, configure with compressed data and inflate, then end inflate
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len <= (uLong)~0u ? comp_len : (uLong)~0u);
    Bytef *out_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Validation and cleanup — query compile flags, compute and combine CRCs, close gzFile and free memory
    uLong flags = zlibCompileFlags();
    uLong half = (uLong)(payload_len / 2);
    uLong crc1 = crc32(0UL, (const Bytef *)payload, (uInt)half);
    uLong crc2 = crc32(0UL, (const Bytef *)(payload + half), (uInt)(payload_len - half));
    uLong crc_combined = crc32_combine(crc1, crc2, (off_t)(payload_len - half));
    int rc_gzclose = gzclose(gf);
    delete [] comp_buf;
    delete [] out_buf;
    (void)rc_gzbuffer;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)flags;
    (void)crc1;
    (void)crc2;
    (void)crc_combined;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}