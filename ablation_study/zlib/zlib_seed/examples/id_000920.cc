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
//<ID> 920
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and gzFile write/read
    const char payload[] = "zlib sequence payload exercising compress/inflate, gzwrite/gzflush and crc32_combine";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    unsigned int to_write = (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, to_write);
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzrewind = gzrewind(gf);
    Bytef *read_buf = new Bytef[(payload_len ? payload_len : 1)];
    unsigned int to_read = (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_gzread = gzread(gf, (voidp)read_buf, to_read);
    uLong crc1 = crc32(0UL, read_buf, (uInt)payload_len);
    uLong crc2 = crc32(0UL, src_buf, (uInt)payload_len);
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)payload_len);
    uLong compile_flags = zlibCompileFlags();

    // step 2: Compress the payload
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLong comp_len = bound;
    int rc_compress = compress2(comp_buf, &comp_len, src_buf, (uLong)payload_len, 6);

    // step 3: Initialize inflate, configure and operate
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len <= (uLong)~0u ? comp_len : (uLong)~0u);
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Cleanup
    int rc_gzclose = gzclose(gf);
    delete [] comp_buf;
    delete [] read_buf;
    delete [] uncmp_buf;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzrewind;
    (void)rc_gzread;
    (void)crc1;
    (void)crc2;
    (void)combined_crc;
    (void)compile_flags;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}