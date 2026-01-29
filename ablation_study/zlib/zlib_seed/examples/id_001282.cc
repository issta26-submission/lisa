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
//<ID> 1282
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib fuzz payload";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *buf = new Bytef[(size_t)srcLen];
    memset(buf, 0, (size_t)srcLen);
    memcpy(buf, payload, (size_t)srcLen);
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize inflate stream and open gzip file)
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = buf;
    strm.avail_in = (uInt)srcLen;
    gzFile gzf = gzopen("zlib_api_seq_out.gz", "wb");

    // step 3: Core operations (write data to gzip, inspect offset, clear errors, compute CRCs)
    int rc_gzwrite = gzwrite(gzf, (voidpc)buf, (unsigned int)srcLen);
    off64_t offset_after_write = gzoffset64(gzf);
    gzclearerr(gzf);
    uLong crc1 = crc32(0UL, buf, (uInt)srcLen);
    uLong crc2 = crc32(crc1, buf, (uInt)srcLen);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_inflate_end = inflateEnd(&strm);
    delete [] buf;
    (void)version;
    (void)rc_inflate_init;
    (void)rc_gzwrite;
    (void)offset_after_write;
    (void)crc1;
    (void)crc2;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}