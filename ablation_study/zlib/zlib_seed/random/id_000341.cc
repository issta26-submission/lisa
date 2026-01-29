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
//<ID> 341
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and obtain zlib version
    const char src[] = "zlib API sequence payload: write via gzwrite then read via gzread and validate with adler32";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();

    // step 2: Write compressed data to a gzip file using gzopen/gzwrite
    const char filename[] = "test_zlib_api_sequence.gz";
    gzFile wfile = gzopen(filename, "wb");
    int written = gzwrite(wfile, (voidpc)source, (unsigned int)sourceLen);
    int wclose_rc = gzclose(wfile);

    // step 3: Read back decompressed data using gzopen/gzread and compute checksums
    gzFile rfile = gzopen(filename, "rb");
    uLong bufAlloc = (uLong)(sourceLen + 64);
    Bytef *readBuf = (Bytef *)malloc((size_t)bufAlloc);
    memset(readBuf, 0, (size_t)bufAlloc);
    int readBytes = gzread(rfile, (voidp)readBuf, (unsigned int)bufAlloc);
    int rclose_rc = gzclose(rfile);
    uLong chk_src = adler32(0UL, source, (uInt)sourceLen);
    uLong chk_read = adler32(0UL, readBuf, (uInt)(readBytes > 0 ? (uLong)readBytes : 0UL));

    // step 4: Initialize and immediately end an inflate stream to exercise inflateEnd, then cleanup
    z_stream zstrm;
    memset(&zstrm, 0, sizeof(z_stream));
    int inf_init_rc = inflateInit_(&zstrm, version, (int)sizeof(z_stream));
    int inf_end_rc = inflateEnd(&zstrm);

    free(readBuf);

    (void)version;
    (void)written;
    (void)wclose_rc;
    (void)readBytes;
    (void)rclose_rc;
    (void)chk_src;
    (void)chk_read;
    (void)inf_init_rc;
    (void)inf_end_rc;

    // API sequence test completed successfully
    return 66;
}