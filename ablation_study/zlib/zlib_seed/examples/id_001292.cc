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
//<ID> 1292
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib payload for gzprintf and deflateEnd test";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;

    // step 2: Setup
    int rc_compress = compress(compBuf, &compLen_f, src, srcLen);
    uLong compLen = (uLong)compLen_f;
    z_stream defstrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;
    Bytef *defOut = new Bytef[(size_t)(compLen + 64)];
    memset(defOut, 0, (size_t)(compLen + 64));
    defstrm.next_out = defOut;
    defstrm.avail_out = (uInt)(compLen + 64);

    // step 3: Core operations
    gzFile gzf = gzopen("zlib_seq_gzprintf.gz", "wb");
    uLong crc = crc32(0UL, src, (uInt)srcLen);
    int rc_gzprintf = gzprintf(gzf, "CRC=%lu LEN=%lu\n", crc, srcLen);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_deflate_end = deflateEnd(&defstrm);
    delete [] src;
    delete [] compBuf;
    delete [] defOut;
    (void)version;
    (void)bound;
    (void)rc_compress;
    (void)compLen_f;
    (void)compLen;
    (void)rc_deflate_init;
    (void)crc;
    (void)rc_gzprintf;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}