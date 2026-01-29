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
//<ID> 917
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib example payload for gzopen/gzbuffer and inflate sequence";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    int rc_compress = compress2(comp_buf, &comp_len, src_buf, (uLong)payload_len, 6);

    // step 2: Write payload to a gzip file and configure buffering
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzbuffer = gzbuffer(gf, 8192);
    int rc_gzputs = gzputs(gf, payload);
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzclose = gzclose(gf);

    // step 3: Initialize inflate stream, inspect mark and sync point, then inflate compressed data
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len <= (uLong)~0u ? comp_len : (uLong)~0u);
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    long inflate_mark = inflateMark(&istrm);
    int rc_inflate_syncp = inflateSyncPoint(&istrm);
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Cleanup and finalization
    delete [] comp_buf;
    delete [] uncmp_buf;
    (void)rc_compress;
    (void)rc_gzbuffer;
    (void)rc_gzputs;
    (void)rc_gzflush;
    (void)rc_gzclose;
    (void)rc_inflate_init;
    (void)inflate_mark;
    (void)rc_inflate_syncp;
    (void)rc_inflate;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}