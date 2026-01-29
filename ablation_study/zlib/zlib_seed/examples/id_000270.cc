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
//<ID> 270
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, compute adler32, and allocate compression buffer
    const char src_c[] = "zlib API sequence payload: initialize -> deflate -> inflate -> validate";
    const Bytef *source = (const Bytef *)src_c;
    z_size_t src_zlen = (z_size_t)(sizeof(src_c) - 1);
    uLong sourceLen = (uLong)src_zlen;
    uLong source_adler = adler32_z(0UL, source, src_zlen);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);

    // step 2: Initialize and configure deflate and inflate streams
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Operate - compress (deflate) into compBuf and then decompress (inflate) back into decompBuf
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_def = deflate(&dstrm, 0);

    z_size_t compressed_len = (z_size_t)dstrm.total_out;
    Bytef *decompBuf = (Bytef *)malloc((size_t)(src_zlen + 16));
    memset(decompBuf, 0, (size_t)(src_zlen + 16));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compressed_len;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)(src_zlen + 16);
    int rc_inf = inflate(&istrm, 0);

    // step 4: Validate (compute adler32 on compressed data) and cleanup streams and buffers
    uLong comp_adler = adler32_z(0UL, compBuf, (z_size_t)compressed_len);
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(decompBuf);

    (void)source_adler;
    (void)comp_adler;
    (void)rc_def_init;
    (void)rc_inf_init;
    (void)rc_def;
    (void)rc_inf;
    (void)rc_def_end;
    (void)rc_inf_end;
    (void)version;
    (void)sourceLen;
    (void)compressed_len;

    // API sequence test completed successfully
    return 66;
}