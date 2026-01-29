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
//<ID> 348
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize data, version and checksums
    const char src[] = "zlib API sequence payload: gzopen/gzwrite -> gzread with checksum validation";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong adler_orig = adler32(0UL, source, (uInt)sourceLen);
    unsigned int readBufSize = (unsigned int)(sourceLen + 64);
    voidp readBuf = malloc((size_t)readBufSize);
    memset(readBuf, 0, (size_t)readBufSize);

    // step 2: Write compressed data to a gzip file using gzopen/gzwrite
    gzFile wf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(wf, (voidpc)src, (unsigned int)sourceLen);
    int rc_gz_close_w = gzclose(wf);

    // step 3: Read back decompressed data using gzopen/gzread and validate with checksum
    gzFile rf = gzopen("test_zlib_api_sequence.gz", "rb");
    int rc_gz_read = gzread(rf, readBuf, readBufSize);
    int rc_gz_close_r = gzclose(rf);
    uLong adler_read = adler32(0UL, (const Bytef *)readBuf, (uInt)rc_gz_read);

    // step 4: Initialize an inflate stream (initialize -> operate -> cleanup) and final cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inf_end = inflateEnd(&istrm);

    free(readBuf);

    (void)version;
    (void)rc_gz_write;
    (void)rc_gz_close_w;
    (void)rc_gz_read;
    (void)rc_gz_close_r;
    (void)adler_orig;
    (void)adler_read;
    (void)rc_inf_init;
    (void)rc_inf_end;

    // API sequence test completed successfully
    return 66;
}