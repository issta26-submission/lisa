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
//<ID> 914
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib api sequence payload";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    uLongf comp_len = (uLongf)(bound ? bound : 1);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    Bytef *read_buf = new Bytef[(payload_len ? payload_len : 1)];
    int rc_compress = compress2(comp_buf, &comp_len, src_buf, (uLong)payload_len, 6);

    // step 2: gz file operations (open, buffer, write, rewind, read, close)
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzbuffer = gzbuffer(gf, 4096);
    int rc_gzputs = gzputs(gf, payload);
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzrewind = gzrewind(gf);
    unsigned int to_read = (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_gzread = gzread(gf, (voidp)read_buf, to_read);
    uLong crc_read = crc32(0UL, read_buf, (uInt)payload_len);
    int rc_gzclose = gzclose(gf);

    // step 3: Inflate initialization, operation and validation
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len <= (uLong)~0u ? comp_len : (uLong)~0u);
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    long mark_before = inflateMark(&istrm);
    int rc_inflate = inflate(&istrm, 0);
    long mark_after = inflateMark(&istrm);
    int rc_inflate_sync = inflateSyncPoint(&istrm);

    // step 4: Cleanup and finalization
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] comp_buf;
    delete [] uncmp_buf;
    delete [] read_buf;
    (void)rc_compress;
    (void)rc_gzbuffer;
    (void)rc_gzputs;
    (void)rc_gzflush;
    (void)rc_gzrewind;
    (void)rc_gzread;
    (void)crc_read;
    (void)rc_gzclose;
    (void)rc_inflate_init;
    (void)mark_before;
    (void)rc_inflate;
    (void)mark_after;
    (void)rc_inflate_sync;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}