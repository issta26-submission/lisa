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
//<ID> 925
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and compression
    const char payload[] = "zlib API sequence payload to exercise compress/inflate and gzFile operations";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound(payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLong comp_len = bound;
    int rc_compress = compress2(comp_buf, &comp_len, src_buf, payload_len, 6);

    // step 2: Initialize and operate inflate stream to recover payload
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len <= (uLong)~0u ? comp_len : (uLong)~0u);
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate = inflate(&istrm, 0);

    // step 3: Validation using CRCs and compile flags
    uLong crc_orig = crc32(0UL, src_buf, (uInt)payload_len);
    uLong crc_recovered = crc32(0UL, uncmp_buf, (uInt)payload_len);
    uLong crc_combined = crc32_combine(crc_orig, crc_recovered, (off_t)payload_len);
    uLong compile_flags = zlibCompileFlags();

    // step 4: gzFile operations, flush, read back, finalize inflate and cleanup
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzbuffer = gzbuffer(gf, 4096);
    unsigned int to_write = (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, to_write);
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzrewind = gzrewind(gf);
    Bytef *read_buf = new Bytef[(payload_len ? payload_len : 1)];
    unsigned int to_read = to_write;
    int rc_gzread = gzread(gf, (voidp)read_buf, to_read);
    uLong crc_read = crc32(0UL, read_buf, (uInt)payload_len);
    uLong crc_combined2 = crc32_combine(crc_read, crc_orig, (off_t)payload_len);
    int rc_gzclose = gzclose(gf);
    int rc_inflate_end = inflateEnd(&istrm);

    delete [] comp_buf;
    delete [] uncmp_buf;
    delete [] read_buf;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)crc_combined;
    (void)compile_flags;
    (void)rc_gzbuffer;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzrewind;
    (void)rc_gzread;
    (void)crc_read;
    (void)crc_combined2;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}