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
//<ID> 1333
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for inflateInit2_, crc32, deflateSetHeader, deflateBound";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong crc_src = crc32(0UL, src, (uInt)srcLen);
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));

    // step 2: Setup (initialize deflate stream, set header, get bound, initialize inflate stream)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    head.text = 0;
    head.name = (Bytef *)"zlib_test_name";
    head.comment = (Bytef *)"zlib_test_comment";
    head.extra = NULL;
    head.hcrc = 0;
    int rc_set_header = deflateSetHeader(&dstrm, &head);
    uLong bound = deflateBound(&dstrm, srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;
    int rc_inflate_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);

    // step 3: Operate and Validate (compress with compress2, inflate, compute crc of output)
    int rc_compress = compress2(compBuf, &compLen_f, src, srcLen, 6);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen_f;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate = inflate(&istrm, 0);
    uLong crc_out = crc32(0UL, outBuf, (uInt)srcLen);
    (void)crc_src;
    (void)crc_out;
    (void)rc_compress;
    (void)rc_inflate;

    // step 4: Cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)version;
    (void)bound;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_inflate_init2;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}