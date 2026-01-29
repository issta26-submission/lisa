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
//<ID> 919
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and write/read via gzFile
    const char payload[] = "zlib example payload to exercise gzopen, gzbuffer, gzwrite and gzread";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzbuffer = gzbuffer(gf, 4096);
    Bytef *read_buf = new Bytef[(payload_len ? payload_len : 1)];
    unsigned int to_write = (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, to_write);
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzrewind = gzrewind(gf);
    unsigned int to_read = (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_gzread = gzread(gf, (voidp)read_buf, to_read);
    uLong crc_read = crc32(0UL, read_buf, (uInt)payload_len);

    // step 2: Initialize inflate stream
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Configure inflate stream with data and query mark/sync point
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    long inflate_mark = inflateMark(&istrm);
    int inflate_sync = inflateSyncPoint(&istrm);

    // step 4: Cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gf);
    delete [] read_buf;
    (void)rc_gzbuffer;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzrewind;
    (void)rc_gzread;
    (void)crc_read;
    (void)rc_inflate_init;
    (void)inflate_mark;
    (void)inflate_sync;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}