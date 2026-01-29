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
//<ID> 1421
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for deflateInit_/deflateCopy/deflateGetDictionary/gzopen/gzwrite";
    uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    z_stream dstrm_copy;
    memset(&dstrm_copy, 0, (size_t)sizeof(z_stream));

    // step 2: Setup - initialize deflate and prepare output buffer
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong defBoundVal = deflateBound(&dstrm, srcLen);
    Bytef *defBuf = new Bytef[(size_t)defBoundVal];
    memset(defBuf, 0, (size_t)defBoundVal);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = defBuf;
    dstrm.avail_out = (uInt)defBoundVal;

    // step 3: Core operations - compress, copy state, extract dictionary
    int rc_deflate = deflate(&dstrm, 0);
    uLong compLen = dstrm.total_out;
    int rc_deflate_copy = deflateCopy(&dstrm_copy, &dstrm);
    uInt dictBufSize = (uInt)64;
    Bytef *dictBuf = new Bytef[(size_t)dictBufSize];
    memset(dictBuf, 0, (size_t)dictBufSize);
    int rc_deflate_get_dict = deflateGetDictionary(&dstrm_copy, dictBuf, &dictBufSize);

    // step 4: Finalize and cleanup - end deflate streams, write original data to a gzip file, release memory
    int rc_deflate_end_copy = deflateEnd(&dstrm_copy);
    int rc_deflate_end = deflateEnd(&dstrm);
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    int rc_gzclose_w = gzclose_w(gzf);

    delete [] src;
    delete [] defBuf;
    delete [] dictBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)defBoundVal;
    (void)rc_deflate;
    (void)compLen;
    (void)rc_deflate_copy;
    (void)dictBufSize;
    (void)rc_deflate_get_dict;
    (void)rc_deflate_end_copy;
    (void)rc_deflate_end;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    // API sequence test completed successfully
    return 66;
}