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
//<ID> 929
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and input preparation
    const char payload[] = "zlib API sequence payload to exercise compress/inflate and gzflush usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    uLongf comp_len_f = (uLongf)bound;

    // step 2: Compress the payload and record compile flags
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);
    uLong comp_size = (uLong)comp_len_f;
    uLong compile_flags = zlibCompileFlags();

    // step 3: Initialize inflate, configure streams, perform inflate and CRC combine
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size <= (uLong)~0u ? comp_size : (uLong)~0u);
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate = inflate(&istrm, 0);

    uLong crc_payload = crc32(0UL, src_buf, (uInt)payload_len);
    uLong crc_comp = crc32(0UL, comp_buf, (uInt)comp_size);
    uLong crc_combined = crc32_combine(crc_payload, crc_comp, (off_t)payload_len);

    // step 4: Use gzFile to write original payload and flush, then cleanup inflate and buffers
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u));
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzclose = gzclose(gf);
    int rc_inflate_end = inflateEnd(&istrm);

    delete [] comp_buf;
    delete [] uncmp_buf;

    (void)rc_compress;
    (void)compile_flags;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)crc_payload;
    (void)crc_comp;
    (void)crc_combined;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}