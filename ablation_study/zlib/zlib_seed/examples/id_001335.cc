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
//<ID> 1335
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for inflateInit2_, crc32, deflateSetHeader and deflateBound";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize deflate stream, set gz header, compute bound and compress)
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    Bytef *head_name = new Bytef[6];
    memset(head_name, 0, 6);
    memcpy(head_name, "fname", 5);
    head.name = head_name;
    head.name_max = 6u;
    head.time = 0UL;
    int rc_set_header = deflateSetHeader(&dstrm, &head);
    uLong bound = deflateBound(&dstrm, srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;
    int rc_compress = compress2(compBuf, &compLen_f, src, srcLen, 6);

    // step 3: Initialize inflate (using inflateInit2_), inflate compressed data, compute crc32
    int rc_inflate_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen_f;
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate = inflate(&istrm, 0);
    uLong out_crc = crc32(0UL, outBuf, (uInt)srcLen);
    uLong src_crc = crc32(0UL, src, (uInt)srcLen);

    // step 4: Cleanup and finalization
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    delete [] head_name;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)bound;
    (void)compLen_f;
    (void)rc_compress;
    (void)rc_inflate_init2;
    (void)rc_inflate;
    (void)out_crc;
    (void)src_crc;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}