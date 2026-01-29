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
//<ID> 1374
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for deflateCopy/gzwrite/crc32";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong crc_src = crc32(0UL, src, (uInt)srcLen);
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);

    // step 2: Setup (initialize deflate stream)
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 3: Core operations (copy deflate state, compress, write gz, compute crc)
    z_stream copystrm;
    memset(&copystrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&copystrm, &dstrm);
    int rc_deflate = deflate(&dstrm, 0);
    uLong compSize = dstrm.total_out;
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)compSize);
    int rc_gzclose = gzclose(gzf);
    uLong crc_comp = crc32(0UL, compBuf, (uInt)compSize);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_deflate_end_copy = deflateEnd(&copystrm);

    // step 4: Cleanup
    delete [] src;
    delete [] compBuf;
    (void)version;
    (void)crc_src;
    (void)crc_comp;
    (void)compBound;
    (void)rc_deflate_init;
    (void)rc_deflate_copy;
    (void)rc_deflate;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    (void)rc_deflate_end_copy;
    // API sequence test completed successfully
    return 66;
}