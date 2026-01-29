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
//<ID> 1366
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for header/dictionary/adler32_z testing";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong adler_before = adler32_z(1UL, src, (z_size_t)srcLen);

    // step 2: Initialize and configure deflate with a gzip header
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    gz_header gzhead;
    memset(&gzhead, 0, (size_t)sizeof(gz_header));
    Bytef *name_buf = new Bytef[16];
    memset(name_buf, 0, 16);
    const char name_text[] = "test_name";
    memcpy(name_buf, name_text, (size_t)(sizeof(name_text) - 1));
    Bytef *comm_buf = new Bytef[32];
    memset(comm_buf, 0, 32);
    const char comm_text[] = "test_comment";
    memcpy(comm_buf, comm_text, (size_t)(sizeof(comm_text) - 1));
    gzhead.name = name_buf;
    gzhead.name_max = 16;
    gzhead.comment = comm_buf;
    gzhead.comm_max = 32;
    gzhead.time = adler_before;
    gzhead.text = 1;
    int rc_set_header = deflateSetHeader(&dstrm, &gzhead);

    // step 3: Operate - compress the source into a buffer
    uLong bound = deflateBound(&dstrm, srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;
    int rc_deflate_finish = deflate(&dstrm, 4); // Z_FINISH
    uLong compUsed = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 4: Initialize inflate, retrieve header and dictionary, validate, cleanup
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compUsed;
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    gz_header gzhead_in;
    memset(&gzhead_in, 0, (size_t)sizeof(gz_header));
    Bytef *in_name = new Bytef[16];
    memset(in_name, 0, 16);
    Bytef *in_comm = new Bytef[32];
    memset(in_comm, 0, 32);
    gzhead_in.name = in_name;
    gzhead_in.name_max = 16;
    gzhead_in.comment = in_comm;
    gzhead_in.comm_max = 32;
    int rc_get_header = inflateGetHeader(&istrm, &gzhead_in);
    uInt dictLen = (uInt)((srcLen < 8UL) ? srcLen : 8UL);
    Bytef *dictBuf = new Bytef[(size_t)dictLen];
    memset(dictBuf, 0, (size_t)dictLen);
    memcpy(dictBuf, src, (size_t)dictLen);
    int rc_inflate_set_dict = inflateSetDictionary(&istrm, dictBuf, dictLen);
    Bytef *dictOut = new Bytef[(size_t)dictLen];
    memset(dictOut, 0, (size_t)dictLen);
    uInt dictOutLen = dictLen;
    int rc_inflate_get_dict = inflateGetDictionary(&istrm, dictOut, &dictOutLen);
    uLong adler_after = adler32_z(0UL, dictOut, (z_size_t)dictOutLen);
    int rc_inflate_end = inflateEnd(&istrm);

    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    delete [] name_buf;
    delete [] comm_buf;
    delete [] in_name;
    delete [] in_comm;
    delete [] dictBuf;
    delete [] dictOut;
    (void)version;
    (void)adler_before;
    (void)adler_after;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_deflate_finish;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_get_header;
    (void)rc_inflate_set_dict;
    (void)rc_inflate_get_dict;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}