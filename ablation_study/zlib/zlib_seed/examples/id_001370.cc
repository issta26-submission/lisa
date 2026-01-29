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
//<ID> 1370
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

    // step 2: Initialize and configure deflate source stream and prepare destination stream
    z_stream srcStrm;
    memset(&srcStrm, 0, (size_t)sizeof(z_stream));
    srcStrm.next_in = src;
    srcStrm.avail_in = (uInt)srcLen;
    int rc_deflate_init = deflateInit_(&srcStrm, 6, version, (int)sizeof(z_stream));

    z_stream dstStrm;
    memset(&dstStrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&dstStrm, &srcStrm);

    // step 3: Operate (compute checksum and write original data to a gzip file)
    uLong checksum = crc32(0UL, src, (uInt)srcLen);
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup
    int rc_end_dst = deflateEnd(&dstStrm);
    int rc_end_src = deflateEnd(&srcStrm);
    delete [] src;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate_copy;
    (void)checksum;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_end_dst;
    (void)rc_end_src;
    // API sequence test completed successfully
    return 66;
}