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
//<ID> 1286
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib_api_sequence_payload";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize inflate stream and open gzip file)
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = src;
    istrm.avail_in = (uInt)srcLen;
    gzFile gzf = gzopen64("zlib_api_sequence_out.gz", "wb");

    // step 3: Core operations (compute checksums, write to gzip, inspect offset and clear errors)
    uLong crc1 = crc32(0UL, src, (uInt)srcLen);
    uLong crc2 = crc32(crc1, src, (uInt)srcLen);
    int written = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    off64_t pos_after_write = gzoffset64(gzf);
    gzclearerr(gzf);

    // step 4: Cleanup and finalization
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    (void)version;
    (void)rc_inflate_init;
    (void)crc1;
    (void)crc2;
    (void)written;
    (void)pos_after_write;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}