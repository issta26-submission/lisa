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
//<ID> 1368
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload demonstrating deflateSetHeader/inflateGetHeader/inflateGetDictionary/adler32_z";
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
    uLong adler_before = adler32_z(1UL, src, (z_size_t)srcLen);

    // step 2: Setup (deflate with gzip header configured)
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    head.text = 1;
    Bytef *name_buf = new Bytef[32];
    memset(name_buf, 0, 32);
    memcpy(name_buf, "example-name", 12);
    head.name = name_buf;
    head.name_max = 32;
    int rc_set_header = deflateSetHeader(&dstrm, &head);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;

    // step 3: Core operations (compress and then prepare inflate with header capture)
    int rc_deflate = deflate(&dstrm, 4); // 4 == Z_FINISH
    uLong compLen = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit2_(&istrm, 31, version, (int)sizeof(z_stream));
    gz_header head_in;
    memset(&head_in, 0, (size_t)sizeof(gz_header));
    int rc_inflate_gethead = inflateGetHeader(&istrm, &head_in);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate = inflate(&istrm, 4); // 4 == Z_FINISH

    // step 4: Validate and Cleanup
    Bytef *dictBuf = new Bytef[64];
    memset(dictBuf, 0, 64);
    uInt dictLen = (uInt)64;
    int rc_getdict = inflateGetDictionary(&istrm, dictBuf, &dictLen);
    uLong adler_after = adler32_z(1UL, outBuf, (z_size_t)srcLen);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    delete [] name_buf;
    delete [] dictBuf;
    (void)version;
    (void)bound;
    (void)adler_before;
    (void)adler_after;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate_gethead;
    (void)rc_inflate;
    (void)rc_getdict;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}