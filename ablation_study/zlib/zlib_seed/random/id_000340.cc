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
//<ID> 340
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data, buffers and version
    const char src[] = "zlib API sequence payload: write with gzwrite, read back with gzread, init/inflateEnd lifecycle";
    const Bytef *source = (const Bytef *)src;
    unsigned int source_len = (unsigned int)(sizeof(src) - 1);
    const char *version = zlibVersion();
    Bytef *readBuf = (Bytef *)malloc((size_t)source_len + 64);
    memset(readBuf, 0, (size_t)source_len + 64);
    const char *fname = "test_zlib_api_sequence_tmp.gz";

    // step 2: Initialize inflate stream (setup)
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Operate - write compressed data with gzwrite, then read it back with gzread
    gzFile wfile = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(wfile, (voidpc)source, source_len);
    int rc_gzclose_w = gzclose(wfile);
    gzFile rfile = gzopen(fname, "rb");
    int rc_gzread = gzread(rfile, (voidp)readBuf, source_len + 64);
    int rc_gzclose_r = gzclose(rfile);

    // step 4: Validate and cleanup - compute checksums and end inflate stream
    uLong ad_src = adler32(0UL, source, (uInt)source_len);
    uLong ad_read = adler32(0UL, readBuf, (uInt)((rc_gzread >= 0) ? rc_gzread : 0));
    int rc_inf_end = inflateEnd(&istrm);
    free(readBuf);

    (void)version;
    (void)rc_inf_init;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)rc_gzclose_r;
    (void)ad_src;
    (void)ad_read;
    (void)rc_inf_end;

    // API sequence test completed successfully
    return 66;
}