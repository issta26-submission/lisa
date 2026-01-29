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
//<ID> 918
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib test payload for gzopen/gzbuffer and inflateMark/inflateSyncPoint";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    Bytef *out_buf = new Bytef[(payload_len ? payload_len : 1)];

    // step 2: Configure storage and write a gzip file (demonstrate gzopen/gzbuffer)
    int rc_compress = compress2(comp_buf, &comp_len, src_buf, (uLong)payload_len, 6);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzbuffer = gzbuffer(gf, 65536U);
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, (unsigned int)payload_len);
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzclose = gzclose(gf);

    // step 3: Initialize inflate stream, provide compressed data, operate and validate with mark/sync
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len <= (uLong)~0u ? comp_len : (uLong)~0u);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate = inflate(&istrm, 0);
    long mark = inflateMark(&istrm);
    int rc_syncpoint = inflateSyncPoint(&istrm);

    // step 4: Cleanup and finalization
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] comp_buf;
    delete [] out_buf;
    (void)rc_compress;
    (void)rc_gzbuffer;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)mark;
    (void)rc_syncpoint;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}