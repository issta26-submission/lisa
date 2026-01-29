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
//<ID> 1375
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for gzwrite/zlibVersion/deflateCopy/crc32";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong crc_before = crc32(0UL, (const Bytef *)src, (uInt)srcLen);

    // step 2: Setup (initialize source deflate stream and allocate buffers)
    z_stream srcStrm;
    memset(&srcStrm, 0, (size_t)sizeof(z_stream));
    srcStrm.next_in = src;
    srcStrm.avail_in = (uInt)srcLen;
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)(compBound ? compBound : 1)];
    memset(compBuf, 0, (size_t)(compBound ? compBound : 1));
    srcStrm.next_out = compBuf;
    srcStrm.avail_out = (uInt)(compBound ? compBound : 1);
    int rc_deflate_init = deflateInit_(&srcStrm, 6, version, (int)sizeof(z_stream));

    // step 3: Core operations (deflate, copy stream, compute crc, write gz)
    int rc_deflate = deflate(&srcStrm, 0);
    uLong compSize = srcStrm.total_out;
    z_stream destStrm;
    memset(&destStrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&destStrm, &srcStrm);
    uLong crc_after = crc32(crc_before, compBuf, (uInt)(compSize ? compSize : 0));
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)(compSize ? compSize : 0));
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup and finalization
    int rc_deflate_end_src = deflateEnd(&srcStrm);
    int rc_deflate_end_dest = deflateEnd(&destStrm);
    delete [] src;
    delete [] compBuf;
    (void)version;
    (void)crc_before;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)compSize;
    (void)rc_deflate_copy;
    (void)crc_after;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_deflate_end_src;
    (void)rc_deflate_end_dest;
    // API sequence test completed successfully
    return 66;
}