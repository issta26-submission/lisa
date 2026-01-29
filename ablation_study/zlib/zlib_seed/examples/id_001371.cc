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
//<ID> 1371
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload to exercise zlibVersion/deflateCopy/gzwrite/crc32";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong initial_crc = crc32(0UL, src, (uInt)srcLen);
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf1 = new Bytef[(size_t)compBound];
    memset(compBuf1, 0, (size_t)compBound);
    Bytef *compBuf2 = new Bytef[(size_t)compBound];
    memset(compBuf2, 0, (size_t)compBound);

    // step 2: Setup (initialize deflate stream and make a copy)
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf1;
    dstrm.avail_out = (uInt)compBound;
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    z_stream cstrm;
    memset(&cstrm, 0, (size_t)sizeof(z_stream));
    int rc_copy = deflateCopy(&cstrm, &dstrm);
    cstrm.next_out = compBuf2;
    cstrm.avail_out = (uInt)compBound;

    // step 3: Core operations (deflate both streams, write original to gzip, compute crc of compressed data)
    int rc_def1 = deflate(&dstrm, 0);
    uLong compSize1 = dstrm.total_out;
    int rc_def2 = deflate(&cstrm, 0);
    uLong compSize2 = cstrm.total_out;
    int rc_defend1 = deflateEnd(&dstrm);
    int rc_defend2 = deflateEnd(&cstrm);

    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    int rc_gzclose = gzclose(gzf);

    uLong crc_comp1 = 0UL;
    if (compSize1 > 0UL) {
        crc_comp1 = crc32(0UL, compBuf1, (uInt)compSize1);
    } else {
        crc_comp1 = crc32(0UL, compBuf2, (uInt)compSize2);
    }

    // step 4: Cleanup
    delete [] src;
    delete [] compBuf1;
    delete [] compBuf2;
    (void)version;
    (void)initial_crc;
    (void)compBound;
    (void)rc_deflate_init;
    (void)rc_copy;
    (void)rc_def1;
    (void)rc_def2;
    (void)rc_defend1;
    (void)rc_defend2;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)crc_comp1;
    // API sequence test completed successfully
    return 66;
}