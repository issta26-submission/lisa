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
//<ID> 1367
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for header/dictionary/adler32 usage";
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

    // step 2: Setup (initialize deflate with gzip wrapper and set gzip header)
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    const uInt name_sz = 16;
    const uInt comm_sz = 24;
    head.name = new Bytef[(size_t)name_sz];
    head.comment = new Bytef[(size_t)comm_sz];
    memset(head.name, 0, (size_t)name_sz);
    memset(head.comment, 0, (size_t)comm_sz);
    memcpy(head.name, "test_name", 9);
    memcpy(head.comment, "test_comment", 12);
    head.name_max = name_sz;
    head.comm_max = comm_sz;
    head.text = 1;
    int rc_set_header = deflateSetHeader(&dstrm, &head);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;

    // step 3: Operate (deflate, initialize inflate, inflate, retrieve header and dictionary, compute adler)
    int rc_deflate = deflate(&dstrm, 0);
    uLong compressedLen = dstrm.total_out;
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit2_(&istrm, 31, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compressedLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate = inflate(&istrm, 0);
    int rc_get_header = inflateGetHeader(&istrm, &head);
    uInt dictLen = 32;
    Bytef *dictBuf = new Bytef[(size_t)dictLen];
    memset(dictBuf, 0, (size_t)dictLen);
    int rc_get_dict = inflateGetDictionary(&istrm, dictBuf, &dictLen);
    uLong adler_after = adler32_z(0UL, outBuf, (z_size_t)srcLen);

    // step 4: Cleanup and finalization
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    delete [] dictBuf;
    delete [] head.name;
    delete [] head.comment;
    (void)version;
    (void)bound;
    (void)adler_before;
    (void)adler_after;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_deflate;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_get_header;
    (void)rc_get_dict;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}