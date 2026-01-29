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
//<ID> 343
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize - prepare source and get zlib version
    const char src[] = "zlib API sequence payload: gzopen -> gzwrite -> gzread -> inflateEnd";
    const Bytef *source = (const Bytef *)src;
    unsigned int sourceLen = (unsigned int)(sizeof(src) - 1);
    const char *version = zlibVersion();

    // step 2: Configure & Operate - write compressed data to a gz file
    const char *fname = "zlib_api_sequence_tmp.gz";
    gzFile wfile = gzopen(fname, "wb");
    int wrote = gzwrite(wfile, (voidpc)source, sourceLen);
    int rc_close_w = gzclose(wfile);

    // step 3: Operate & Validate - read back compressed file and compute checksum
    gzFile rfile = gzopen(fname, "rb");
    uLong readBufAlloc = (uLong)(sourceLen + 64);
    Bytef *readBuf = (Bytef *)malloc((size_t)readBufAlloc);
    memset(readBuf, 0, (size_t)readBufAlloc);
    int readBytes = gzread(rfile, (voidp)readBuf, (unsigned int)readBufAlloc);
    uLong checksum = crc32(0UL, (const Bytef *)readBuf, (uInt)(readBytes < 0 ? 0 : readBytes));

    // step 4: Cleanup - initialize a dummy inflate stream and then end it, close file and free buffers
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inf_end = inflateEnd(&istrm);
    int rc_close_r = gzclose(rfile);
    free(readBuf);

    (void)version;
    (void)wrote;
    (void)rc_close_w;
    (void)readBytes;
    (void)checksum;
    (void)rc_inf_init;
    (void)rc_inf_end;
    (void)rc_close_r;

    // API sequence test completed successfully
    return 66;
}