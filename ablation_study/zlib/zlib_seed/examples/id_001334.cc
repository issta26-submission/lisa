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
//<ID> 1334
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for inflate/deflate and crc32/header/bound";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    const char nameStr[] = "fuzzed-name";
    Bytef *nameBuf = new Bytef[(size_t)sizeof(nameStr)];
    memset(nameBuf, 0, (size_t)sizeof(nameStr));
    memcpy(nameBuf, nameStr, (size_t)sizeof(nameStr));
    head.name = nameBuf;
    head.name_max = (uInt)(sizeof(nameStr) - 1);

    // step 2: Setup (initialize deflate, set header, compute bound, allocate buffers)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_deflate_set_header = deflateSetHeader(&dstrm, &head);
    uLong bound = deflateBound(&dstrm, srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;

    // step 3: Core operations (compress, initialize inflater, inflate, compute CRCs)
    int rc_deflate = deflate(&dstrm, 4); /* 4 == Z_FINISH */
    uLong compLen = dstrm.total_out;
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);
    int rc_inflate_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate = inflate(&istrm, 0);
    uLong crc_orig = crc32(0UL, src, (uInt)srcLen);
    uLong crc_decomp = crc32(0UL, outBuf, (uInt)srcLen);

    // step 4: Validate and Cleanup
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] nameBuf;
    delete [] compBuf;
    delete [] outBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate_set_header;
    (void)bound;
    (void)rc_deflate;
    (void)compLen;
    (void)rc_inflate_init2;
    (void)rc_inflate;
    (void)crc_orig;
    (void)crc_decomp;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}