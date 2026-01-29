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
//<ID> 1365
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "example payload for zlib header/dictionary/adler32 sequence";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);
    uLong adler_before = adler32_z(0UL, src, (z_size_t)srcLen);

    // step 2: Setup (initialize deflate with gzip wrapper, set gzip header)
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit2_(&dstrm, 6, Z_DEFLATED, 31, 8, 0, version, (int)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, (size_t)sizeof(gz_header));
    header.text = 0;
    header.time = 0;
    header.xflags = 0;
    header.os = 255;
    int rc_set_header = deflateSetHeader(&dstrm, &header);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&dstrm, Z_FINISH);
    uLong compLen = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Operate and validate (inflate, get header and dictionary, compute adler on output)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit2_(&istrm, 31, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate = inflate(&istrm, Z_FINISH);
    gz_header out_header;
    memset(&out_header, 0, (size_t)sizeof(gz_header));
    int rc_get_header = inflateGetHeader(&istrm, &out_header);
    uInt dictBufLen = 256;
    Bytef *dictBuf = new Bytef[(size_t)dictBufLen];
    memset(dictBuf, 0, (size_t)dictBufLen);
    int rc_get_dict = inflateGetDictionary(&istrm, dictBuf, &dictBufLen);
    uLong adler_after = adler32_z(0UL, outBuf, (z_size_t)srcLen);

    // step 4: Cleanup and finalization
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    delete [] dictBuf;
    (void)version;
    (void)adler_before;
    (void)adler_after;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_get_header;
    (void)rc_get_dict;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}