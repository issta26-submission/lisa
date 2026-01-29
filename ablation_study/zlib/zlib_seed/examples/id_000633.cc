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
//<ID> 633
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into an in-memory buffer
    const char src[] = "zlib API sequence test payload";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    uLongf compLen = (uLongf)compBound;
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    int rc_compress2 = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Persist compressed bytes to a gzFile (write) and close
    gzFile gf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf_w, compBuf, (unsigned int)compLen);
    int rc_gzclose_w = gzclose(gf_w);

    // step 3: Re-open gzFile for reading and read compressed bytes back into a buffer
    gzFile gf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    z_size_t readAlloc = (z_size_t)compBound;
    Bytef *readBuf = (Bytef *)malloc((size_t)readAlloc);
    memset(readBuf, 0, (size_t)readAlloc);
    z_size_t readItems = gzfread(readBuf, (z_size_t)1, readAlloc, gf_r);
    int gzerrnum = 0;
    const char * gz_err_str = gzerror(gf_r, &gzerrnum);

    // step 4: Initialize an inflate stream, perform one-shot uncompress2 using the read data, then cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char * version = zlibVersion();
    int rc_inflate_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    uLong sourceLenForUncompress = (uLong)readItems;
    uLongf outAlloc = (uLongf)(sourceLen + 64);
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    uLongf destLen = outAlloc;
    int rc_uncompress2 = uncompress2(outBuf, &destLen, readBuf, &sourceLenForUncompress);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose_r = gzclose(gf_r);
    free(compBuf);
    free(readBuf);
    free(outBuf);
    (void)rc_compress2;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)readItems;
    (void)gz_err_str;
    (void)gzerrnum;
    (void)rc_inflate_init2;
    (void)rc_uncompress2;
    (void)rc_inflate_end;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}