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
//<ID> 86
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream defstrm;
    z_stream infstrm;
    memset(&defstrm, 0, sizeof(z_stream));
    memset(&infstrm, 0, sizeof(z_stream));
    const char src_c[] = "Example payload for zlib API sequence: compute adler32, compress, init deflate+inflateBack";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    Bytef *outBuf = (Bytef *)malloc((size_t)(sourceLen + 64));
    memset(compBuf, 0, (size_t)bound);
    memset(outBuf, 0, (size_t)(sourceLen + 64));

    // step 2: Setup (compute checksum, compress, initialize deflate and set gzip header)
    const char *version = zlibVersion();
    uLong ad = adler32_z(0UL, source, (z_size_t)sourceLen);
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    const char name_s[] = "zlib_test";
    head.name = (Bytef *)malloc((size_t)sizeof(name_s));
    memcpy(head.name, name_s, (size_t)sizeof(name_s));
    head.name_max = (uInt)(sizeof(name_s));
    head.comment = NULL;
    head.comment = NULL;
    int rc_sethead = deflateSetHeader(&defstrm, &head);

    // step 3: Core operations (initialize inflateBack with a window and associate buffers)
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_infback = inflateBackInit_(&infstrm, 15, window, version, (int)sizeof(z_stream));
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compLen;
    infstrm.next_out = outBuf;
    infstrm.avail_out = (uInt)(sourceLen + 64);
    long mark = inflateMark(&infstrm);

    // step 4: Cleanup & finalize
    deflateEnd(&defstrm);
    inflateBackEnd(&infstrm);
    free(compBuf);
    free(outBuf);
    free(window);
    free(head.name);
    (void)ad; (void)rc_comp; (void)rc_def_init; (void)rc_sethead; (void)rc_infback; (void)mark;
    // API sequence test completed successfully
    return 66;
}