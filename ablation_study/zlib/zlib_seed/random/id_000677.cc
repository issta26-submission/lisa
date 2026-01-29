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
//<ID> 677
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data and buffers
    const char src[] = "zlib API sequence test payload for dictionary, deflate, inflateBack and codes usage";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    Bytef *outBuf = (Bytef *)malloc((size_t)(sourceLen + 128));
    memset(outBuf, 0, (size_t)(sourceLen + 128));

    // step 2: Initialize and configure deflate stream with header and dictionary
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header gzhead;
    memset(&gzhead, 0, sizeof(gz_header));
    gzhead.text = 0;
    gzhead.time = (uLong)0;
    int rc_set_header = deflateSetHeader(&dstrm, &gzhead);
    const Bytef dict[] = "common_dictionary_for_tests";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    int rc_set_dict = deflateSetDictionary(&dstrm, dict, dictLen);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_deflate = deflate(&dstrm, 0);
    compLen = (uLongf)dstrm.total_out;

    // step 3: Initialize inflateBack and inflate streams, perform inflate, then query codes used
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
    istrm.avail_out = (uInt)(sourceLen + 128);
    int rc_inflate = inflate(&istrm, 0);
    unsigned long codesUsed = inflateCodesUsed(&istrm);
    (void)codesUsed;

    // step 4: Cleanup resources and end streams
    int rc_inf_end = inflateEnd(&istrm);
    int rc_ib_end = inflateBackEnd(&ibstrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(outBuf);
    free(window);
    (void)rc_def_init;
    (void)rc_set_header;
    (void)rc_set_dict;
    (void)rc_deflate;
    (void)rc_ib_init;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_ib_end;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}