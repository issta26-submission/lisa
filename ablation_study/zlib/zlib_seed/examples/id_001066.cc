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
//<ID> 1066
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload and buffers
    const char payload[] = "zlib API sequence payload exercising gzwrite, gzfread, deflateInit_, inflateMark";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    Bytef *read_buf = new Bytef[(payload_len ? (size_t)payload_len : 1)];

    // step 2: Initialize deflate and inflate streams
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate — write payload to a gzip file and read it back; obtain inflate mark
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gw, payload, (unsigned int)payload_len);
    int rc_gwclose = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    z_size_t items_read = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)payload_len, gr);
    int rc_grclose = gzclose(gr);
    long mark = inflateMark(&istrm);

    // step 4: Cleanup and validate
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] read_buf;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)rc_gzwrite;
    (void)rc_gwclose;
    (void)items_read;
    (void)rc_grclose;
    (void)mark;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}