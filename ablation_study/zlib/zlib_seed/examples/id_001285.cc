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
//<ID> 1285
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api fuzz payload";
    uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream instrm;
    memset(&instrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup
    int rc_inflate_init = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    instrm.next_in = src;
    instrm.avail_in = (uInt)srcLen;
    gzFile gzf = gzopen("zlib_api_seq_out.gz", "wb");

    // step 3: Core operations
    uLong crc1 = crc32(0UL, src, (uInt)srcLen);
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    off64_t pos64 = gzoffset64(gzf);
    gzclearerr(gzf);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_inflate_end = inflateEnd(&instrm);
    delete [] src;
    (void)version;
    (void)rc_inflate_init;
    (void)crc1;
    (void)rc_gzwrite;
    (void)pos64;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}