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
//<ID> 1373
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload
    const char payload[] = "zlib api sequence payload for deflateCopy/gzwrite/crc32/zlibVersion";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong crc_before = crc32(0UL, src, (uInt)srcLen);

    // step 2: Initialize and configure deflate stream, then copy it
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    z_stream dstrm_copy;
    memset(&dstrm_copy, 0, (size_t)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&dstrm_copy, &dstrm);

    // step 3: Operate — write original data to gzip file and compute CRC
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    int rc_gzclose = gzclose(gzf);
    uLong crc_after = crc32(crc_before, src, (uInt)srcLen);

    // step 4: Cleanup and finalization
    int rc_deflate_end_copy = deflateEnd(&dstrm_copy);
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] src;
    (void)version;
    (void)crc_before;
    (void)crc_after;
    (void)rc_deflate_init;
    (void)rc_deflate_copy;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_deflate_end_copy;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}