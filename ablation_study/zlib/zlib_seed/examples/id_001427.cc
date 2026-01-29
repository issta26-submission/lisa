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
//<ID> 1427
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for deflate/deflateCopy/deflateGetDictionary/gzopen/gzwrite";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    // step 2: Setup - initialize deflate stream and allocate buffers
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong defBound = deflateBound(&dstrm, srcLen);
    Bytef *defBuf = new Bytef[(size_t)defBound];
    memset(defBuf, 0, (size_t)defBound);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = defBuf;
    dstrm.avail_out = (uInt)defBound;

    // step 3: Core operations - deflate, copy stream, retrieve dictionary, and write original data to gzip file
    int rc_deflate = deflate(&dstrm, 0);
    uLong compLen = dstrm.total_out;
    z_stream copystrm;
    memset(&copystrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&copystrm, &dstrm);
    uInt dictBufSize = 256;
    Bytef *dictBuf = new Bytef[(size_t)dictBufSize];
    memset(dictBuf, 0, (size_t)dictBufSize);
    int rc_getdict = deflateGetDictionary(&copystrm, dictBuf, &dictBufSize);
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    int rc_gzclose_w = gzclose_w(gzf);

    // step 4: Cleanup - end deflate streams and free buffers
    int rc_deflate_end_src = deflateEnd(&dstrm);
    int rc_deflate_end_copy = deflateEnd(&copystrm);
    delete [] src;
    delete [] defBuf;
    delete [] dictBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)defBound;
    (void)rc_deflate;
    (void)compLen;
    (void)rc_deflate_copy;
    (void)rc_getdict;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_deflate_end_src;
    (void)rc_deflate_end_copy;
    // API sequence test completed successfully
    return 66;
}