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
//<ID> 1378
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
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf1 = new Bytef[(size_t)compBound];
    Bytef *compBuf2 = new Bytef[(size_t)compBound];
    memset(compBuf1, 0, (size_t)compBound);
    memset(compBuf2, 0, (size_t)compBound);

    // step 2: Setup (initialize deflate stream and prepare copy)
    z_stream dstrm;
    z_stream dstrm_copy;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&dstrm_copy, 0, (size_t)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf1;
    dstrm.avail_out = (uInt)compBound;
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&dstrm_copy, &dstrm);
    dstrm_copy.next_out = compBuf2;
    dstrm_copy.avail_out = (uInt)compBound;

    // step 3: Core operations (compress both original and its copy, compute checksums, write original payload to gz)
    int rc_deflate1 = deflate(&dstrm, 0);
    int rc_deflate2 = deflate(&dstrm_copy, 0);
    uLong compSize1 = dstrm.total_out;
    uLong compSize2 = dstrm_copy.total_out;
    uLong crc_src = crc32(0UL, src, (uInt)srcLen);
    uLong crc_comp1 = crc32(0UL, compBuf1, (uInt)compSize1);
    uLong crc_comp2 = crc32(0UL, compBuf2, (uInt)compSize2);
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    int rc_gzclose = gzclose(gzf);

    // step 4: Validate and Cleanup
    int rc_deflate_end1 = deflateEnd(&dstrm);
    int rc_deflate_end2 = deflateEnd(&dstrm_copy);
    delete [] src;
    delete [] compBuf1;
    delete [] compBuf2;
    (void)version;
    (void)compBound;
    (void)rc_deflate_init;
    (void)rc_deflate_copy;
    (void)rc_deflate1;
    (void)rc_deflate2;
    (void)compSize1;
    (void)compSize2;
    (void)crc_src;
    (void)crc_comp1;
    (void)crc_comp2;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_deflate_end1;
    (void)rc_deflate_end2;
    // API sequence test completed successfully
    return 66;
}