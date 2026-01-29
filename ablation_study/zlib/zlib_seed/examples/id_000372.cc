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
//<ID> 372
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and buffer setup
    const char src[] = "zlib API sequence payload: deflate -> uncompress2 -> write gz -> gztell -> gzclose";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    uLong compBound = deflateBound(&strm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);

    // step 2: Compress (deflate) the source into compBuf
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)sourceLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)compBound;
    int rc_deflate = deflate(&strm, 0);
    uLong compressed_size = (uLong)strm.total_out;
    int rc_def_end = deflateEnd(&strm);

    // step 3: Decompress using uncompress2 to validate and then write compressed data to gzip file
    uLongf destLen = (uLongf)sourceLen;
    Bytef *destBuf = (Bytef *)malloc((size_t)destLen);
    memset(destBuf, 0, (size_t)destLen);
    uLong srcLenForUncompress = compressed_size;
    int rc_uncomp = uncompress2(destBuf, &destLen, compBuf, &srcLenForUncompress);
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf, compBuf, (unsigned int)compressed_size);
    off_t pos = gztell(gzf);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup and finalization
    free(compBuf);
    free(destBuf);
    (void)version;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_uncomp;
    (void)rc_gzwrite;
    (void)pos;
    (void)rc_gzclose;

    // API sequence test completed successfully
    return 66;
}