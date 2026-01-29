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
//<ID> 220
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & compress source data
    const char src_c[] = "zlib API sequence payload: compress -> uncompress -> inflate -> gzrewind";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Uncompress into a simple buffer and compute checksums for validation
    uLongf decompLen_uncomp = (uLongf)(sourceLen + 16);
    Bytef *decomp_uncomp = (Bytef *)malloc((size_t)decompLen_uncomp);
    memset(decomp_uncomp, 0, (size_t)decompLen_uncomp);
    int rc_uncomp = uncompress(decomp_uncomp, &decompLen_uncomp, compBuf, (uLong)compLen);
    uLong orig_adler = adler32(0UL, source, (uInt)sourceLen);
    uLong uncmp_adler = adler32(0UL, decomp_uncomp, (uInt)decompLen_uncomp);

    // step 3: Initialize inflate stream, inflate compressed data into another buffer, write to gz and rewind
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompLen_infl = (uLongf)(sourceLen + 64);
    Bytef *decomp_infl = (Bytef *)malloc((size_t)decompLen_infl);
    memset(decomp_infl, 0, (size_t)decompLen_infl);
    istrm.next_out = decomp_infl;
    istrm.avail_out = (uInt)decompLen_infl;
    int rc_inflate = inflate(&istrm, 0);
    uLong inflated_bytes = istrm.total_out;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, (voidpc)decomp_infl, (unsigned int)inflated_bytes);
    int rc_gz_rewind = gzrewind(gz);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup & finalize
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(decomp_uncomp);
    free(decomp_infl);

    (void)rc_comp;
    (void)rc_uncomp;
    (void)orig_adler;
    (void)uncmp_adler;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)inflated_bytes;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_close;
    (void)rc_inf_end;
    (void)bound;
    (void)sourceLen;
    // API sequence test completed successfully
    return 66;
}