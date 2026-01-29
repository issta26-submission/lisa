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
//<ID> 1464
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for deflateSetHeader/deflatePending/inflateUndermine/gzungetc";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    // step 2: Setup (compress a source buffer and prepare deflate stream + gzip header)
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    uLongf compDestLen = (uLongf)compBound;
    int rc_compress = compress(compBuf, &compDestLen, src, srcLen);

    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    int rc_deflate_init = deflateInit2_(&defstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));

    gz_header head;
    memset(&head, 0, sizeof(head));
    head.text = 1;
    head.time = 0;
    head.xflags = 0;
    head.os = 255;
    head.name = (Bytef *)"test_name";
    head.comment = (Bytef *)"test_comment";
    head.hcrc = 0;
    head.done = 0;
    int rc_set_header = deflateSetHeader(&defstrm, &head);

    // step 3: Core operations (query pending bits on deflate, then teardown; then initialize inflate and call undermine)
    unsigned int pending = 0;
    int bits = 0;
    int rc_pending = deflatePending(&defstrm, &pending, &bits);
    int rc_deflate_end = deflateEnd(&defstrm);

    z_stream instrm;
    memset(&instrm, 0, sizeof(instrm));
    int rc_inflate_init = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    int rc_inflate_undermine = inflateUndermine(&instrm, 1);
    int rc_inflate_end = inflateEnd(&instrm);

    // step 4: Operate (write gz file, push back a byte) and Cleanup
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, src, (unsigned int)srcLen);
    int rc_gzungetc = gzungetc((int)src[0], gzf);
    int rc_gzclose = gzclose(gzf);

    delete [] src;
    delete [] compBuf;
    (void)version;
    (void)rc_compress;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)pending;
    (void)bits;
    (void)rc_pending;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate_undermine;
    (void)rc_inflate_end;
    (void)rc_gzwrite;
    (void)rc_gzungetc;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}