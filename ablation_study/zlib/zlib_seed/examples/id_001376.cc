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
//<ID> 1376
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
    uLong crc_orig = crc32(0UL, src, (uInt)srcLen);
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);

    // step 2: Setup (initialize and run deflate)
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_deflate = deflate(&dstrm, 0);
    uLong compSize = dstrm.total_out;

    // step 3: Configure/Operate (copy deflate state, compute checksum, write original via gz)
    z_stream cstrm;
    memset(&cstrm, 0, (size_t)sizeof(z_stream));
    int rc_copy = deflateCopy(&cstrm, &dstrm);
    uLong crc_comp = crc32(0UL, compBuf, (uInt)compSize);
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup and finalization
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_copy_end = deflateEnd(&cstrm);
    delete [] src;
    delete [] compBuf;
    (void)version;
    (void)crc_orig;
    (void)crc_comp;
    (void)compSize;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_copy;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    (void)rc_copy_end;
    // API sequence test completed successfully
    return 66;
}