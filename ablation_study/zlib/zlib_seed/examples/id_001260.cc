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
//<ID> 1260
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Test payload for zlib sequence";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uLong crc = crc32(0UL, src, (uInt)srcLen);
    uInt len1 = (uInt)(srcLen / 2);
    uInt len2 = (uInt)(srcLen - len1);
    uLong ad1 = adler32(0UL, src, len1);
    uLong ad2 = adler32(0UL, src + len1, len2);
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)len2);

    // step 2: Setup (compress source into buffer)
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;
    int rc_compress = compress(compBuf, &compLen_f, src, srcLen);
    uLong compLen = (uLong)compLen_f;

    // step 3: Initialize inflate and query gzip header
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    int rc_inflate_get_header = inflateGetHeader(&istrm, &head);

    // step 4: Operate (write compressed data to gz file) and Cleanup
    gzFile gzf = gzopen("zlib_api_sequence.gz", "wb");
    int rc_gzputc = gzputc(gzf, 'A');
    int rc_gzwrite = gzwrite(gzf, compBuf, (unsigned int)compLen);
    int rc_gzclose = gzclose(gzf);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] compBuf;
    (void)version;
    (void)crc;
    (void)ad_combined;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate_get_header;
    (void)rc_gzputc;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}