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
//<ID> 354
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload and version
    const char payload[] = "zlib API sequence payload: deflate with dictionary -> write gz -> read gz -> inflate -> adler combine";
    const Bytef *src = (const Bytef *)payload;
    uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();

    // step 2: Initialize deflate, set dictionary, compress and query pending bits
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    const Bytef dict[] = "example_dictionary_data_for_deflate";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    deflateSetDictionary(&dstrm, dict, dictLen);
    uLong bound = deflateBound(&dstrm, srcLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    dstrm.next_in = (Bytef *)src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 0);
    unsigned int pending = 0;
    int bits = 0;
    deflatePending(&dstrm, &pending, &bits);
    unsigned int compLen = (unsigned int)(bound - (uLong)dstrm.avail_out);

    // step 3: Write compressed data to a gzip file, read it back, compute adler32 and combine, then inflate
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile wfile = gzopen(fname, "wb");
    int gz_errnum = 0;
    gzwrite(wfile, (voidpc)compBuf, compLen);
    const char *gz_errmsg_w = gzerror(wfile, &gz_errnum);
    gzclose(wfile);
    gzFile rfile = gzopen(fname, "rb");
    unsigned int readBufLen = compLen + 64;
    Bytef *readBuf = (Bytef *)malloc((size_t)readBufLen);
    memset(readBuf, 0, (size_t)readBufLen);
    int readBytes = gzread(rfile, (voidp)readBuf, readBufLen);
    const char *gz_errmsg_r = gzerror(rfile, &gz_errnum);
    gzclose(rfile);
    uLong ad1 = adler32(0L, src, (uInt)srcLen);
    uLong ad2 = adler32(0L, readBuf, (uInt)readBytes);
    uLong combined = adler32_combine(ad1, ad2, (off_t)readBytes);
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = compLen;
    uLong decompAlloc = srcLen + 128;
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    inflate(&istrm, 0);
    inflateEnd(&istrm);

    // step 4: Cleanup
    deflateEnd(&dstrm);
    free(compBuf);
    free(readBuf);
    free(decompBuf);
    (void)version;
    (void)pending;
    (void)bits;
    (void)gz_errmsg_w;
    (void)gz_errmsg_r;
    (void)ad1;
    (void)ad2;
    (void)combined;

    // API sequence test completed successfully
    return 66;
}