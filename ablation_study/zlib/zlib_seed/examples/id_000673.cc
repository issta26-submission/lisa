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
//<ID> 673
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compressed buffer
    const char src[] = "zlib API sequence test payload for dictionary/header/inflateBack usage";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_compress = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize and configure a deflate stream (dictionary + header)
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    Bytef dictData[] = "example_dictionary_data";
    uInt dictLen = (uInt)(sizeof(dictData) - 1);
    int rc_set_dict = deflateSetDictionary(&dstrm, dictData, dictLen);
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    head.text = 0;
    head.time = 0;
    head.xflags = 0;
    head.os = 255;
    int rc_set_header = deflateSetHeader(&dstrm, &head);

    // step 3: Initialize inflateBack stream and regular inflate stream, then inflate compressed buffer
    z_stream ibstrm;
    memset(&ibstrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_ib_init = inflateBackInit_(&ibstrm, 15, window, version, (int)sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong outAlloc = sourceLen + 256;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 0);
    unsigned long codes_used = inflateCodesUsed(&istrm);

    // step 4: Cleanup and finalization
    int rc_inf_end = inflateEnd(&istrm);
    int rc_ib_end = inflateBackEnd(&ibstrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(outBuf);
    free(window);
    (void)rc_compress;
    (void)rc_def_init;
    (void)rc_set_dict;
    (void)rc_set_header;
    (void)rc_ib_init;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)codes_used;
    (void)rc_inf_end;
    (void)rc_ib_end;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}