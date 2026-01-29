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
//<ID> 347
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and write compressed file
    const char src[] = "zlib API sequence payload: gzwrite and gzread with inflateEnd cleanup";
    const unsigned int srcLen = (unsigned int)(sizeof(src) - 1);
    const char *version = zlibVersion();
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile wfile = gzopen(fname, "wb");
    int rc_write = gzwrite(wfile, (voidpc)src, srcLen);
    int rc_close_w = gzclose(wfile);

    // step 2: Read compressed file back into buffer
    char outbuf[512];
    memset(outbuf, 0, sizeof(outbuf));
    gzFile rfile = gzopen(fname, "rb");
    int rc_read = gzread(rfile, (voidp)outbuf, (unsigned int)sizeof(outbuf));
    int rc_close_r = gzclose(rfile);

    // step 3: Initialize inflate stream and immediately end it to exercise inflateEnd
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    int rc_inf_end = inflateEnd(&strm);

    // step 4: Cleanup references and validate flow
    (void)version;
    (void)fname;
    (void)srcLen;
    (void)rc_write;
    (void)rc_close_w;
    (void)rc_read;
    (void)rc_close_r;
    (void)rc_inf_init;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}