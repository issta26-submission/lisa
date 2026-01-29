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
//<ID> 1372
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for deflateCopy/crc32/gzwrite/zlibVersion";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong crc_before = crc32(0UL, src, (uInt)srcLen);

    // step 2: Setup and initialize deflate stream
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_deflate1 = deflate(&dstrm, 0);

    // step 3: Operate and validate (copy deflate state, continue, checksum, write gzip)
    z_stream dstrm_copy;
    memset(&dstrm_copy, 0, (size_t)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&dstrm_copy, &dstrm);
    int rc_deflate2 = deflate(&dstrm_copy, 0);
    uLong finalCompSize = dstrm_copy.total_out;
    uLong crc_comp = crc32(0UL, compBuf, (uInt)finalCompSize);
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup
    int rc_deflate_end1 = deflateEnd(&dstrm);
    int rc_deflate_end2 = deflateEnd(&dstrm_copy);
    delete [] src;
    delete [] compBuf;
    (void)version;
    (void)crc_before;
    (void)crc_comp;
    (void)rc_deflate_init;
    (void)rc_deflate1;
    (void)rc_deflate_copy;
    (void)rc_deflate2;
    (void)finalCompSize;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_deflate_end1;
    (void)rc_deflate_end2;
    // API sequence test completed successfully
    return 66;
}