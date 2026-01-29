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
//<ID> 1362
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for deflateSetHeader/inflateGetHeader/inflateGetDictionary/adler32_z";
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
    uLong adler_before = adler32_z(1UL, src, (z_size_t)srcLen);

    // step 2: Setup (deflate with gzip header)
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    gz_header head_out;
    memset(&head_out, 0, (size_t)sizeof(gz_header));
    const char name_bytes[] = "test_name";
    const uInt name_len = (uInt)(sizeof(name_bytes) - 1);
    head_out.name = new Bytef[(size_t)name_len];
    memset(head_out.name, 0, (size_t)name_len);
    memcpy(head_out.name, name_bytes, (size_t)name_len);
    head_out.name_max = name_len;
    int rc_set_header = deflateSetHeader(&dstrm, &head_out);
    int rc_deflate_finish = deflate(&dstrm, 4); // Z_FINISH == 4
    uLong compLen = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Operate (inflate, register header retrieval, retrieve dictionary)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit2_(&istrm, 31, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    gz_header head_in;
    memset(&head_in, 0, (size_t)sizeof(gz_header));
    int rc_register_header = inflateGetHeader(&istrm, &head_in);
    int rc_inflate = inflate(&istrm, 0);
    uInt dictBufSize = (uInt)64;
    Bytef *dictBuf = new Bytef[(size_t)dictBufSize];
    memset(dictBuf, 0, (size_t)dictBufSize);
    int rc_inflate_get_dict = inflateGetDictionary(&istrm, dictBuf, &dictBufSize);
    uLong adler_after = adler32_z(1UL, outBuf, (z_size_t)srcLen);

    // step 4: Cleanup and finalization
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    delete [] head_out.name;
    delete [] dictBuf;
    (void)version;
    (void)adler_before;
    (void)adler_after;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_deflate_finish;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_register_header;
    (void)rc_inflate;
    (void)rc_inflate_get_dict;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}