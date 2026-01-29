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
//<ID> 215
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize / compress
    const char src_c[] = "zlib API sequence payload: compress2 -> inflateInit_ -> inflate -> deflateEnd -> gzwrite -> gzclose";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream and perform inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompAlloc = sourceLen + 256;
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inflate = inflate(&istrm, 0);

    // step 3: Initialize a deflate stream then immediately end it (cleanup)
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_end = deflateEnd(&dstrm);

    // step 4: Write decompressed data to a gzip file, close, cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    unsigned int written = gzwrite(gz, (voidpc)decompBuf, (unsigned int)istrm.total_out);
    int rc_gz_close = gzclose(gz);
    int rc_inflate_end = inflateEnd(&istrm);
    free(compBuf);
    free(decompBuf);

    (void)rc_comp2;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_def_init;
    (void)rc_def_end;
    (void)written;
    (void)rc_gz_close;
    (void)rc_inflate_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}