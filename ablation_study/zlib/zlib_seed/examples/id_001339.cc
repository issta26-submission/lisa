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
//<ID> 1339
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for deflate/inflate and crc32 with header";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong crc_before = crc32(0UL, src, (uInt)srcLen);

    // step 2: Setup and configuration (deflate stream, gzip header, estimate bound)
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    const char name[] = "zlib_test_name";
    Bytef *name_buf = new Bytef[(size_t)(sizeof(name))];
    memset(name_buf, 0, (size_t)(sizeof(name)));
    memcpy(name_buf, name, (size_t)(sizeof(name) - 1));
    head.name = name_buf;
    head.name_max = (uInt)(sizeof(name));
    head.comment = NULL;
    head.comm_max = 0; // corrected field name (comm_max)
    int rc_set_header = deflateSetHeader(&dstrm, &head);
    uLong est_bound = deflateBound(&dstrm, srcLen);
    Bytef *compBuf = new Bytef[(size_t)est_bound];
    memset(compBuf, 0, (size_t)est_bound);

    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)est_bound;
    int rc_deflate_finish = deflate(&dstrm, 4); // Z_FINISH (numeric)
    uLong compLen = est_bound - (uLong)dstrm.avail_out;

    // step 3: Operate and validate (inflate the produced gzip stream and compute crc)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit2_(&istrm, 31, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate = inflate(&istrm, 0);
    uLong crc_after = crc32(0UL, outBuf, (uInt)srcLen);

    // step 4: Cleanup and finalization
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    delete [] name_buf;
    (void)version;
    (void)crc_before;
    (void)crc_after;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)est_bound;
    (void)rc_deflate_finish;
    (void)compLen;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}