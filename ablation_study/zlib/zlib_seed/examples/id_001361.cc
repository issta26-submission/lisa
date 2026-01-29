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
//<ID> 1361
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "example payload for zlib header/dictionary/adler32_z sequence";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong adler_before = adler32_z(1UL, src, (z_size_t)srcLen);

    // prepare header name buffer
    const char name_literal[] = "test-name";
    Bytef *name_buf = new Bytef[(size_t)(sizeof(name_literal))];
    memset(name_buf, 0, (size_t)sizeof(name_literal));
    memcpy(name_buf, name_literal, (size_t)(sizeof(name_literal)));

    // step 2: Setup (initialize deflate with gzip header and set header)
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 15 + 16, 8, 0, version, (int)sizeof(z_stream));
    gz_header deflate_head;
    memset(&deflate_head, 0, (size_t)sizeof(gz_header));
    deflate_head.name = name_buf;
    deflate_head.name_max = (uInt)(sizeof(name_literal));
    deflate_head.comment = NULL;
    deflate_head.extra = NULL;
    int rc_set_header = deflateSetHeader(&dstrm, &deflate_head);

    // step 3: Core operations (initialize inflate, attempt to retrieve header and dictionary, update adler)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    gz_header inflate_head;
    memset(&inflate_head, 0, (size_t)sizeof(gz_header));
    int rc_get_header = inflateGetHeader(&istrm, &inflate_head);
    // allocate dictionary buffer and request its length
    uInt dict_len = 128;
    Bytef *dict_buf = new Bytef[(size_t)dict_len];
    memset(dict_buf, 0, (size_t)dict_len);
    int rc_get_dict = inflateGetDictionary(&istrm, dict_buf, &dict_len);
    // combine checksum with same source to show dataflow
    uLong adler_after = adler32_z(adler_before, src, (z_size_t)srcLen);

    // step 4: Cleanup and finalization
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] name_buf;
    delete [] dict_buf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_inflate_init;
    (void)rc_get_header;
    (void)rc_get_dict;
    (void)adler_before;
    (void)adler_after;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}