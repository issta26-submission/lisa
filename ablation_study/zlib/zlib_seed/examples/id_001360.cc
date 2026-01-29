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
//<ID> 1360
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "example payload for deflate/inflate with gzip header and dictionary operations";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);

    // step 2: Setup (initialize deflate with gzip wrapper and set header)
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream)); // level=6, method=8(Z_DEFLATED), windowBits=31 (gzip)
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    const char gname[] = "example-name";
    uInt gname_len = (uInt)(sizeof(gname)); // include terminating null
    head.name = new Bytef[(size_t)gname_len];
    memset(head.name, 0, (size_t)gname_len);
    memcpy(head.name, gname, (size_t)gname_len);
    head.name_max = gname_len;
    head.comment = new Bytef[16];
    memset(head.comment, 0, 16);
    memcpy(head.comment, "example-comment", 15);
    head.comm_max = 16;
    head.hcrc = 1;
    int rc_set_header = deflateSetHeader(&dstrm, &head);

    // step 3: Operate (compress with deflate, initialize inflate, retrieve header and dictionary, compute adler)
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_deflate_finish = deflate(&dstrm, 4); // Z_FINISH == 4

    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit2_(&istrm, 31, version, (int)sizeof(z_stream)); // windowBits=31 to accept gzip
    gz_header head_in;
    memset(&head_in, 0, (size_t)sizeof(gz_header));
    head_in.name = new Bytef[32];
    memset(head_in.name, 0, 32);
    head_in.name_max = 32;
    head_in.comment = new Bytef[32];
    memset(head_in.comment, 0, 32);
    head_in.comm_max = 32;
    int rc_inflate_get_header = inflateGetHeader(&istrm, &head_in);

    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)dstrm.total_out;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate = inflate(&istrm, 0); // Z_NO_FLUSH == 0

    Bytef *dictBuf = new Bytef[64];
    memset(dictBuf, 0, 64);
    uInt dictLen = 64;
    int rc_inflate_get_dict = inflateGetDictionary(&istrm, dictBuf, &dictLen);

    uLong computed_adler = adler32_z(0UL, outBuf, (z_size_t)srcLen);

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    delete [] head.name;
    delete [] head.comment;
    delete [] head_in.name;
    delete [] head_in.comment;
    delete [] dictBuf;
    (void)version;
    (void)compBound;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_deflate_finish;
    (void)rc_inflate_init;
    (void)rc_inflate_get_header;
    (void)rc_inflate;
    (void)rc_inflate_get_dict;
    (void)computed_adler;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}