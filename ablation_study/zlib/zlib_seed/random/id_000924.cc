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
//<ID> 924
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and gzFile write/read
    const char payload[] = "zlib fuzz driver payload to exercise gz* APIs, CRC combine and inflate lifecycle";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzbuffer = gzbuffer(gf, 4096);
    unsigned int to_write = (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, to_write);
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzrewind = gzrewind(gf);
    Bytef *read_buf = new Bytef[(payload_len ? payload_len : 1)];
    unsigned int to_read = (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_gzread = gzread(gf, (voidp)read_buf, to_read);

    // step 2: CRC computation on two halves and combine
    uInt first_len_u = (uInt)(payload_len / 2);
    uInt second_len_u = (uInt)(payload_len - (uLong)first_len_u);
    uLong crc1 = crc32(0UL, read_buf, first_len_u);
    uLong crc2 = crc32(0UL, read_buf + first_len_u, second_len_u);
    off_t second_len_off = (off_t)second_len_u;
    uLong crc_combined = crc32_combine(crc1, crc2, second_len_off);

    // step 3: Inflate stream lifecycle and compile flags query
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    Bytef *out_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    uLong compile_flags = zlibCompileFlags();

    // step 4: Cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gf);
    delete [] read_buf;
    delete [] out_buf;
    (void)rc_gzbuffer;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzrewind;
    (void)rc_gzread;
    (void)crc1;
    (void)crc2;
    (void)crc_combined;
    (void)rc_inflate_init;
    (void)compile_flags;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}