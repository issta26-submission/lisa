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
//<ID> 371
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial one-shot compression
    const char src[] = "zlib API sequence payload: compress2 -> uncompress2 -> deflateInit_ -> gzwrite -> gztell -> gzclose";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound1 = compressBound(sourceLen);
    uLongf comp1Len = (uLongf)bound1;
    Bytef *comp1 = (Bytef *)malloc((size_t)bound1);
    memset(comp1, 0, (size_t)bound1);
    int rc_comp2 = compress2(comp1, &comp1Len, source, sourceLen, 6);

    // step 2: Uncompress the compressed blob back into a buffer using uncompress2
    uLong compressedLenUL = (uLong)comp1Len;
    uLongf uncomprBufLen = (uLongf)sourceLen;
    Bytef *uncomprBuf = (Bytef *)malloc((size_t)uncomprBufLen);
    memset(uncomprBuf, 0, (size_t)uncomprBufLen);
    int rc_uncomp2 = uncompress2(uncomprBuf, &uncomprBufLen, comp1, &compressedLenUL);

    // step 3: Initialize deflate stream and recompress the uncompressed data
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = uncomprBuf;
    strm.avail_in = (uInt)uncomprBufLen;
    uLong bound2 = deflateBound(&strm, (uLong)uncomprBufLen);
    Bytef *comp2 = (Bytef *)malloc((size_t)bound2);
    memset(comp2, 0, (size_t)bound2);
    strm.next_out = comp2;
    strm.avail_out = (uInt)bound2;
    int rc_deflate = deflate(&strm, 0);
    uLong comp2Out = strm.total_out;
    int rc_def_end = deflateEnd(&strm);

    // step 4: Write compressed result to a gzip file, query tell, close and cleanup
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf, comp2, (unsigned int)comp2Out);
    off_t gz_pos = gztell(gzf);
    int rc_gzclose = gzclose(gzf);
    free(comp1);
    free(uncomprBuf);
    free(comp2);

    (void)version;
    (void)bound1;
    (void)rc_comp2;
    (void)rc_uncomp2;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)gz_pos;
    (void)rc_gzclose;

    // API sequence test completed successfully
    return 66;
}