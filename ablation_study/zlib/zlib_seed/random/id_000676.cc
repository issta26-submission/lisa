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
//<ID> 676
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and buffers
    const char src[] = "zlib API sequence test payload for deflate/inflate with dictionary and headers";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    Bytef *outBuf = (Bytef *)malloc((size_t)(sourceLen + 256));
    memset(outBuf, 0, (size_t)(sourceLen + 256));
    const Bytef dictBytes[] = "example-dict";
    uInt dictLen = (uInt)(sizeof(dictBytes) - 1);

    // step 2: Initialize and configure deflate stream (gzip wrapper), set header and dictionary
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    head.name = NULL;
    head.comment = NULL;
    head.extra = NULL;
    head.hcrc = 0;
    int rc_set_header = deflateSetHeader(&dstrm, &head);
    int rc_set_dict = deflateSetDictionary(&dstrm, dictBytes, dictLen);

    // step 3: Operate — compress with deflate, initialize inflate/back streams, then inflate
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_deflate = deflate(&dstrm, 4);
    uLong compLen = dstrm.total_out;

    z_stream ibstrm;
    memset(&ibstrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_ib_init = inflateBackInit_(&ibstrm, 15, window, version, (int)sizeof(z_stream));

    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)(sourceLen + 256);
    int rc_inflate = inflate(&istrm, 0);
    unsigned long codes_used = inflateCodesUsed(&istrm);

    // step 4: Cleanup and finalization
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    int rc_ib_end = inflateBackEnd(&ibstrm);
    free(compBuf);
    free(outBuf);
    free(window);
    (void)rc_def_init;
    (void)rc_set_header;
    (void)rc_set_dict;
    (void)rc_deflate;
    (void)compLen;
    (void)rc_ib_init;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)codes_used;
    (void)rc_def_end;
    (void)rc_inf_end;
    (void)rc_ib_end;
    // API sequence test completed successfully
    return 66;
}