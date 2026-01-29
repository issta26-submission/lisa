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
//<ID> 670
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and buffers
    const char src[] = "zlib API sequence test payload with preset dictionary and gzip header";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const Bytef dict[] = "preset_dictionary";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    Bytef *outBuf = (Bytef *)malloc((size_t)(sourceLen + 128));
    memset(outBuf, 0, (size_t)(sourceLen + 128));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    z_stream ibstrm;
    memset(&ibstrm, 0, sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    gz_header hdr;
    memset(&hdr, 0, sizeof(gz_header));
    Bytef *nameBuf = (Bytef *)malloc(8);
    memset(nameBuf, 0, 8);
    memcpy(nameBuf, "zname", 6);
    hdr.name = nameBuf;
    hdr.name_max = 6;

    // step 2: Initialize and configure streams (deflate with gzip header and preset dictionary; inflate-back init)
    int rc_def_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    int rc_setdict = deflateSetDictionary(&dstrm, dict, dictLen);
    int rc_sethdr = deflateSetHeader(&dstrm, &hdr);
    int rc_ib_init = inflateBackInit_(&ibstrm, 15, window, version, (int)sizeof(z_stream));
    int rc_inf_init = inflateInit2_(&istrm, 31, version, (int)sizeof(z_stream));

    // step 3: Operate — compress (deflate) then decompress (inflate)
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_deflate = deflate(&dstrm, 4);
    uLong compLenOut = dstrm.total_out;
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLenOut;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)(sourceLen + 128);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Validate and cleanup
    unsigned long codes_used = inflateCodesUsed(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    int rc_ib_end = inflateBackEnd(&ibstrm);
    free(compBuf);
    free(outBuf);
    free(window);
    free(nameBuf);
    (void)rc_def_init;
    (void)rc_setdict;
    (void)rc_sethdr;
    (void)rc_ib_init;
    (void)rc_inf_init;
    (void)rc_deflate;
    (void)compLenOut;
    (void)rc_inflate;
    (void)codes_used;
    (void)rc_def_end;
    (void)rc_inf_end;
    (void)rc_ib_end;
    // API sequence test completed successfully
    return 66;
}