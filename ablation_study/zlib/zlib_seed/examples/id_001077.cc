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
//<ID> 1077
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization (prepare payload and buffers)
    const char payload[] = "zlib API sequence payload for gzopen/gzread, inflateMark, gzclearerr";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    Bytef *read_buf = new Bytef[(payload_len ? payload_len : 1)];
    Bytef *out_buf = new Bytef[(payload_len ? payload_len : 1)];

    // step 2: Write payload to a gz file (produce compressed data via gz layer)
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gw, (const void *)payload, (unsigned int)payload_len);
    int rc_gzclose_w = gzclose(gw);

    // step 3: Read back via gzread and prepare an inflate stream to inspect mark
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    int rc_gzread = gzread(gr, (voidp)read_buf, (unsigned int)payload_len);
    gzclearerr(gr);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)(rc_gzread > 0 ? rc_gzread : 0);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)payload_len;
    long mark = inflateMark(&istrm);

    // step 4: Cleanup inflate stream, close reader, free buffers and finalize
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose_r = gzclose(gr);
    delete [] read_buf;
    delete [] out_buf;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)rc_inflate_init;
    (void)mark;
    (void)rc_inflate_end;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}