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
//<ID> 643
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and one-shot compression
    const char src[] = "zlib API sequence test payload for combined usage of required APIs";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLongf compBufAlloc = (uLongf)compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufAlloc);
    memset(compBuf, 0, (size_t)compBufAlloc);
    uLongf compLen = compBufAlloc;
    int rc_compress = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize a deflate stream with deflateInit2_ and query deflateBound
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init2 = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));
    uLong bound_estimate = deflateBound(&dstrm, sourceLen);
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize an inflate stream, set a dictionary, and attempt inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf outAlloc = (uLongf)(sourceLen + 128);
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    const Bytef preset_dict[] = "example-preset-dictionary";
    uInt preset_dict_len = (uInt)(sizeof(preset_dict) - 1);
    int rc_inflate_setdict = inflateSetDictionary(&istrm, preset_dict, preset_dict_len);
    int rc_inflate = inflate(&istrm, 4);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Wrap an existing file descriptor with gzdopen and close for reading with gzclose_r
    gzFile gf = gzdopen(0, "rb");
    int rc_gzclose_r = gzclose_r(gf);

    free(compBuf);
    free(outBuf);
    (void)rc_compress;
    (void)rc_def_init2;
    (void)bound_estimate;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_inflate_setdict;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}