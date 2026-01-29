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
//<ID> 1269
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Zlib sequence payload";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, (size_t)sizeof(gz_header));

    // step 2: Setup (initialize inflate stream and prepare gzip output)
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = src;
    istrm.avail_in = (uInt)srcLen;
    gzFile gzf = gzopen("zlib_api_seq_out.gz", "wb");

    // step 3: Core operations (inspect gzip header, write a byte to gz, compute checksums and combine)
    int rc_inflate_get_header = inflateGetHeader(&istrm, &header);
    int rc_gzputc = gzputc(gzf, (int)src[0]);
    uInt len32 = (uInt)srcLen;
    uLong crc1 = crc32(0UL, src, len32);
    uLong crc2 = crc32(crc1, src, len32);
    uLong combined_adler = adler32_combine(crc1, crc2, (off_t)srcLen);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    (void)version;
    (void)rc_inflate_init;
    (void)rc_inflate_get_header;
    (void)rc_gzputc;
    (void)crc1;
    (void)crc2;
    (void)combined_adler;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}