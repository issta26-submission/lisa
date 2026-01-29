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
//<ID> 212
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize source and compress
    const char src_c[] = "zlib API sequence payload: compress2 -> inflateInit_ -> inflate -> deflateInit_/deflateEnd -> gzwrite/gzclose";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream, set inputs/outputs, and inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompBufLen = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompBufLen);
    memset(decompBuf, 0, (size_t)decompBufLen);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompBufLen;
    int rc_inflate = inflate(&istrm, Z_FINISH);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 3: Initialize a deflate stream then tear it down with deflateEnd
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 4: Write decompressed data to a gzip file, close it, and cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, (voidpc)decompBuf, (unsigned int)istrm.total_out);
    off_t gz_pos = gztell(gz);
    int rc_gz_close = gzclose(gz);

    free(compBuf);
    free(decompBuf);
    (void)rc_comp;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_deflate_init;
    (void)rc_deflate_end;
    (void)rc_gz_write;
    (void)gz_pos;
    (void)rc_gz_close;

    // API sequence test completed successfully
    return 66;
}