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
//<ID> 1284
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API fuzz sequence payload";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *buf = new Bytef[(size_t)srcLen];
    memset(buf, 0, (size_t)srcLen);
    memcpy(buf, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Initialize inflate stream and compute checksum
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = buf;
    istrm.avail_in = (uInt)srcLen;
    uLong crc1 = crc32(0UL, buf, (uInt)srcLen);

    // step 3: Write data to a gzip file, inspect offset, and clear errors
    gzFile gzf = gzopen64("zlib_api_seq_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)buf, (unsigned int)srcLen);
    off64_t pos_after_write = gzoffset64(gzf);
    gzclearerr(gzf);

    // step 4: Further checksum, cleanup and finalization
    uLong crc2 = crc32(crc1, buf, (uInt)srcLen);
    int rc_gzclose = gzclose(gzf);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] buf;
    (void)version;
    (void)rc_inflate_init;
    (void)crc1;
    (void)rc_gzwrite;
    (void)pos_after_write;
    (void)crc2;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}