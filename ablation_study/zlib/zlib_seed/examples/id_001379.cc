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
//<ID> 1379
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for deflateCopy/gzwrite/crc32/zlibVersion";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong crc_before = crc32(0UL, src, (uInt)srcLen);
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);

    // step 2: Setup and initialize deflate stream and create a copy
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    z_stream dstrm_copy;
    memset(&dstrm_copy, 0, (size_t)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&dstrm_copy, &dstrm);

    // step 3: Operate - perform deflate, inflate, write original data to gz, and validate checksums
    int rc_deflate = deflate(&dstrm, 0);
    uLong compSize = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_deflate_copy_end = deflateEnd(&dstrm_copy);

    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compSize;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&istrm, 0);
    uLong outSize = istrm.total_out;
    int rc_inflate_end = inflateEnd(&istrm);

    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    int rc_gzclose = gzclose(gzf);

    uLong crc_after = crc32(crc_before, outBuf, (uInt)outSize);

    // step 4: Cleanup
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate_copy;
    (void)rc_deflate;
    (void)compSize;
    (void)rc_deflate_end;
    (void)rc_deflate_copy_end;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)crc_before;
    (void)crc_after;
    // API sequence test completed successfully
    return 66;
}