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
//<ID> 921
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and buffers
    const char payload[] = "zlib payload for combined gzFile, compress2, inflate, crc32_combine and flags usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong comp_bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(comp_bound ? comp_bound : 1)];
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    Bytef *read_buf = new Bytef[(payload_len ? payload_len : 1)];

    // step 2: Write payload to gzFile, flush and read it back
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    unsigned int write_len = (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, write_len);
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzrewind = gzrewind(gf);
    unsigned int read_len = (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_gzread = gzread(gf, (voidp)read_buf, read_len);

    // step 3: Compress the read data, then inflate it using z_stream; compute CRCs and combine with zlib flags
    uLongf comp_len = (uLongf)comp_bound;
    int rc_compress = compress2(comp_buf, &comp_len, read_buf, (uLong)payload_len, 6);
    uLong compressed_size = (uLong)comp_len;
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(compressed_size <= (uLong)~0u ? compressed_size : (uLong)~0u);
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);

    uLong crc_orig = crc32(0UL, src_buf, (uInt)payload_len);
    uLong crc_read = crc32(0UL, read_buf, (uInt)payload_len);
    uLong crc_combined = crc32_combine(crc_orig, crc_read, (off_t)payload_len);
    uLong compile_flags = zlibCompileFlags();
    uLong final_crc = crc32_combine(crc_combined, (uLong)(compile_flags & 0xffffffffUL), (off_t)0);

    // step 4: Cleanup
    int rc_gzclose = gzclose(gf);
    delete [] comp_buf;
    delete [] uncmp_buf;
    delete [] read_buf;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzrewind;
    (void)rc_gzread;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)crc_orig;
    (void)crc_read;
    (void)crc_combined;
    (void)compile_flags;
    (void)final_crc;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}