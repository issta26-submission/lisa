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
//<ID> 358
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare data
    const char *version = zlibVersion();
    const char src[] = "zlib API sequence payload: dictionary-aware deflate, pending bits, gz write, adler combine";
    const Bytef *input = (const Bytef *)src;
    uInt inputLen = (uInt)(sizeof(src) - 1);
    uInt compBufSize = inputLen + 128;
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufSize);
    memset(compBuf, 0, (size_t)compBufSize);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));

    // step 2: Initialize deflate stream and set dictionary
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_set_dict = deflateSetDictionary(&dstrm, input, (uInt)inputLen);
    dstrm.next_in = (Bytef *)input;
    dstrm.avail_in = (uInt)inputLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBufSize;

    // step 3: Perform deflate, query pending bits, write via gz, and compute combined adler32
    int rc_def = deflate(&dstrm, 0);
    unsigned int pending = 0;
    int bits = 0;
    int rc_pending = deflatePending(&dstrm, &pending, &bits);
    uInt produced = (uInt)dstrm.total_out;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gz, (voidpc)compBuf, produced);
    const char *gz_err_msg = gzerror(gz, &rc_gzwrite);
    uInt firstLen = (uInt)(inputLen / 3);
    uInt secondLen = (uInt)(inputLen - firstLen);
    uLong ad1 = adler32(0UL, input, firstLen);
    uLong ad2 = adler32(0UL, input + firstLen, secondLen);
    uLong combined = adler32_combine(ad1, ad2, (off_t)secondLen);

    // step 4: Cleanup and finalize
    int rc_def_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gz);
    free(compBuf);

    (void)version;
    (void)rc_def_init;
    (void)rc_set_dict;
    (void)rc_def;
    (void)rc_pending;
    (void)pending;
    (void)bits;
    (void)produced;
    (void)rc_gzwrite;
    (void)gz_err_msg;
    (void)firstLen;
    (void)secondLen;
    (void)ad1;
    (void)ad2;
    (void)combined;
    (void)rc_def_end;
    (void)rc_gzclose;

    // API sequence test completed successfully
    return 66;
}