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
//<ID> 1463
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for deflatePending/inflateUndermine/deflateSetHeader/gzungetc";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);

    // step 2: Setup
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(head));
    deflateSetHeader(&dstrm, &head);

    // step 3: Core operations / Validate
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_deflate = deflate(&dstrm, Z_FINISH);
    unsigned int pending = 0;
    int bits = 0;
    int rc_pending = deflatePending(&dstrm, &pending, &bits);
    int rc_deflate_end = deflateEnd(&dstrm);

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate_undermine = inflateUndermine(&istrm, 1);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Cleanup
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_put = gzputc(gzf, (int)src[0]);
    int rc_unget = gzungetc((int)src[0], gzf);
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] compBuf;
    (void)version;
    (void)rc_deflate;
    (void)pending;
    (void)bits;
    (void)rc_pending;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate_undermine;
    (void)rc_inflate_end;
    (void)rc_put;
    (void)rc_unget;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}