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
//<ID> 926
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare gzFile I/O buffers
    const char payload[] = "zlib example payload for gz write/read, crc combine and inflate lifecycle";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    Bytef *read_buf = new Bytef[(payload_len ? payload_len : 1)];
    Bytef *uncomp_buf = new Bytef[(payload_len ? payload_len : 1)];
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzbuffer = gzbuffer(gf, 4096);

    // step 2: Write payload to gzFile, flush, rewind and read back decompressed bytes
    unsigned int to_write = (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, to_write);
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzrewind = gzrewind(gf);
    unsigned int to_read = (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_gzread = gzread(gf, (voidp)read_buf, to_read);

    // step 3: Compute CRCs and combine, initialize inflate stream and perform lightweight inspect
    uLong crc1 = crc32(0UL, src_buf, (uInt)payload_len);
    uLong crc2 = crc32(0UL, read_buf, (uInt)payload_len);
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)payload_len);
    uLong compile_flags = zlibCompileFlags();
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    istrm.next_out = uncomp_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    long mark = inflateMark(&istrm);
    int syncp = inflateSyncPoint(&istrm);

    // step 4: Cleanup inflate stream, gzFile and buffers
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gf);
    delete [] read_buf;
    delete [] uncomp_buf;
    (void)rc_gzbuffer;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzrewind;
    (void)rc_gzread;
    (void)crc1;
    (void)crc2;
    (void)combined_crc;
    (void)compile_flags;
    (void)rc_inflate_init;
    (void)mark;
    (void)syncp;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}