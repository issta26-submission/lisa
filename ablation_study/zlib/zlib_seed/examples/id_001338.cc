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
//<ID> 1338
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Step 1: Declarations
    const char payload[] = "zlib api sequence payload for inflate/deflate and crc32/header/bound";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    const char hname[] = "zlib_name";
    head.name_max = (uInt)(sizeof(hname));
    head.name = new Bytef[(size_t)head.name_max];
    memset(head.name, 0, (size_t)head.name_max);
    memcpy(head.name, hname, (size_t)(sizeof(hname) - 1));
    head.text = 0;
    head.time = 0UL;
    head.xflags = 0;
    head.os = 255;
    head.hcrc = 0;
    head.done = 0;

    // Step 2: Setup (initialize deflate with gzip header support and configure header)
    int rc_deflate_init2 = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    int rc_set_header = deflateSetHeader(&dstrm, &head);
    uLong bound = deflateBound(&dstrm, srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;

    // Step 3: Core operations (deflate, then inflate the produced data, compute crc32 checksums)
    int rc_deflate = deflate(&dstrm, 4); // Z_FINISH == 4
    uLong compLen = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    int rc_inflate_init2 = inflateInit2_(&istrm, 47, version, (int)sizeof(z_stream)); // 15 + 32 = 47 for automatic header detection
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate = inflate(&istrm, 0);
    uLong crc_original = crc32(0UL, src, (uInt)srcLen);
    uLong crc_decompressed = crc32(0UL, outBuf, (uInt)srcLen);
    int rc_inflate_end = inflateEnd(&istrm);

    // Step 4: Cleanup and finalization
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    delete [] head.name;
    (void)version;
    (void)rc_deflate_init2;
    (void)rc_set_header;
    (void)bound;
    (void)rc_deflate;
    (void)compLen;
    (void)rc_deflate_end;
    (void)rc_inflate_init2;
    (void)rc_inflate;
    (void)crc_original;
    (void)crc_decompressed;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}