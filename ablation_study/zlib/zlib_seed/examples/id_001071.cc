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
//<ID> 1071
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and write compressed stream via gz layer
    const char payload[] = "zlib API sequence payload exercising gzopen, gzwrite, gzread, inflateMark, gzclearerr";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gzw, (const void *)payload, (unsigned int)payload_len);
    int rc_gzclose_w = gzclose(gzw);

    // step 2: Read back decompressed data via gzread and clear any errors
    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    unsigned char *read_buf = new unsigned char[(payload_len ? payload_len : 1)];
    int rc_gzread = gzread(gzr, (voidp)read_buf, (unsigned int)payload_len);
    gzclearerr(gzr);
    int rc_gzclose_r = gzclose(gzr);

    // step 3: Initialize inflate stream, observe inflateMark, then end inflate
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    long mark = inflateMark(&istrm);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Cleanup and finalize
    delete [] read_buf;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)rc_gzclose_r;
    (void)rc_inflate_init;
    (void)mark;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}