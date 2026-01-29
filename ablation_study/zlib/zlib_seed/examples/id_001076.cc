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
//<ID> 1076
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and write compressed file
    const char payload[] = "zlib API sequence payload for gzopen/gzread/inflateMark";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gw, (const void *)payload, (unsigned int)payload_len);
    int rc_gw_close = gzclose(gw);

    // step 2: Read compressed file via gzread and compute adler32, clear errors
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = new Bytef[(payload_len ? payload_len : 1)];
    int rc_gzread = gzread(gr, (voidp)read_buf, (unsigned int)payload_len);
    gzclearerr(gr);
    uLong adler = adler32(0UL, read_buf, (uInt)payload_len);

    // step 3: Initialize inflate stream and inspect mark (validation)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    long mark = inflateMark(&istrm);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Cleanup resources
    int rc_gr_close = gzclose(gr);
    delete [] read_buf;
    (void)rc_gzwrite;
    (void)rc_gw_close;
    (void)rc_gzread;
    (void)adler;
    (void)rc_inflate_init;
    (void)mark;
    (void)rc_inflate_end;
    (void)rc_gr_close;
    // API sequence test completed successfully
    return 66;
}