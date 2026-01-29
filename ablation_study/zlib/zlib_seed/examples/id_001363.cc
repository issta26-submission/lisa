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
//<ID> 1363
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload using inflateGetDictionary/adler32_z/deflateSetHeader/inflateGetHeader";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);

    // step 2: Setup (compress source, initialize streams, configure gzip header on deflate)
    int rc_compress = compress2(compBuf, &compLen_f, src, srcLen, 6);
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    head.text = 0;
    head.time = 0;
    head.xflags = 0;
    head.os = 255;
    head.extra = nullptr;
    head.extra_len = 0;
    head.extra_max = 0;
    head.name = nullptr;
    head.name_max = 0;
    head.comment = nullptr;
    head.comm_max = 0;
    head.hcrc = 0;
    head.done = 0;
    int rc_set_header = deflateSetHeader(&dstrm, &head);

    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen_f;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;

    // step 3: Core operations (compute adler, try to get dictionary and header from inflate stream)
    uLong adler_after = adler32_z(1UL, src, (z_size_t)srcLen);
    Bytef *dictBuf = new Bytef[(size_t)srcLen];
    memset(dictBuf, 0, (size_t)srcLen);
    uInt dictLenVar = (uInt)srcLen;
    int rc_inflate_getdict = inflateGetDictionary(&istrm, dictBuf, &dictLenVar);
    gz_header head2;
    memset(&head2, 0, (size_t)sizeof(gz_header));
    int rc_inflate_getheader = inflateGetHeader(&istrm, &head2);

    // step 4: Cleanup and finalization (end streams, free memory)
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    delete [] dictBuf;
    (void)version;
    (void)bound;
    (void)rc_compress;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_inflate_init;
    (void)adler_after;
    (void)rc_inflate_getdict;
    (void)rc_inflate_getheader;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}