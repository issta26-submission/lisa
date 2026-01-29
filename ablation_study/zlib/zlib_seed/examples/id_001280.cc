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
//<ID> 1280
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *buf = new Bytef[(size_t)srcLen];
    memset(buf, 0, (size_t)srcLen);
    memcpy(buf, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong crc_initial = crc32(0UL, buf, (uInt)srcLen);

    // step 2: Setup
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    gzclearerr(gzf);
    z_stream instrm;
    memset(&instrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    instrm.next_in = buf;
    instrm.avail_in = (uInt)srcLen;

    // step 3: Core operations
    int rc_gzwrite = gzwrite(gzf, (voidpc)buf, (unsigned int)srcLen);
    off64_t offset_after_write = gzoffset64(gzf);
    uLong crc_after_write = crc32(crc_initial, buf, (uInt)srcLen);

    // step 4: Cleanup and finalization
    int rc_inflate_end = inflateEnd(&instrm);
    int rc_gzclose = gzclose(gzf);
    delete [] buf;
    (void)version;
    (void)rc_inflate_init;
    (void)rc_gzwrite;
    (void)offset_after_write;
    (void)crc_initial;
    (void)crc_after_write;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}