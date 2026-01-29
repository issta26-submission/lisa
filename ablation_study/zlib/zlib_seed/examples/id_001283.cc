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
//<ID> 1283
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib sequence payload for crc and gzip operations";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    gzFile gzf = gzopen64("zlib_api_sequence_out.gz", "wb");

    // step 2: Initialize (prepare inflate stream)
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Operate and validate (write data to gzip, compute crc, inspect offset and clear errors)
    istrm.next_in = src;
    istrm.avail_in = (uInt)srcLen;
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    uLong crc_val = crc32(0UL, src, (uInt)srcLen);
    off64_t out_pos = gzoffset64(gzf);
    gzclearerr(gzf);

    // step 4: Cleanup and finalization
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    (void)version;
    (void)rc_inflate_init;
    (void)rc_gzwrite;
    (void)crc_val;
    (void)out_pos;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}