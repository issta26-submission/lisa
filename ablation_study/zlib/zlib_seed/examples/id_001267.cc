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
//<ID> 1267
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Zlib sample payload";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, (size_t)sizeof(gz_header));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 2: Setup (compute checksums and prepare secondary buffer)
    uLong crc_val = crc32(0UL, src, (uInt)srcLen);
    const char tail[] = "tail";
    const uLong tailLen = (uLong)(sizeof(tail) - 1);
    Bytef *tailBuf = new Bytef[(size_t)tailLen];
    memset(tailBuf, 0, (size_t)tailLen);
    memcpy(tailBuf, tail, (size_t)tailLen);
    uLong ad1 = adler32(0UL, src, (uInt)srcLen);
    uLong ad2 = adler32(0UL, tailBuf, (uInt)tailLen);
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)tailLen);

    // step 3: Core operations (gzip write and header inspection)
    gzFile gzf = gzopen("zlib_sequence_test.gz", "wb");
    int rc_gzput = gzputc(gzf, (int)'Z');
    int rc_inflate_gethdr = inflateGetHeader(&istrm, &header);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] tailBuf;
    (void)version;
    (void)rc_inflate_init;
    (void)crc_val;
    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    (void)rc_gzput;
    (void)rc_inflate_gethdr;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}