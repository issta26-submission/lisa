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
//<ID> 1461
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload using deflateSetHeader/deflatePending/inflateUndermine/gzungetc";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));

    gz_header gzhead;
    memset(&gzhead, 0, sizeof(gzhead));
    gzhead.text = 1;

    // step 2: Setup
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);

    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;

    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 15 + 16, 8, 0, version, (int)sizeof(z_stream));
    (void)rc_deflate_init;

    // step 3: Core operations
    int rc_sethdr = deflateSetHeader(&dstrm, &gzhead);
    (void)rc_sethdr;

    int rc_deflate = deflate(&dstrm, 4); // Z_FINISH == 4
    (void)rc_deflate;

    unsigned int pending_bits = 0u;
    int bits = 0;
    int rc_pending = deflatePending(&dstrm, &pending_bits, &bits);
    (void)rc_pending;
    (void)pending_bits;
    (void)bits;

    int rc_deflate_end = deflateEnd(&dstrm);
    (void)rc_deflate_end;

    uLong compSize = dstrm.total_out;

    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compSize;
    istrm.next_out = src; // reuse src as an output buffer
    istrm.avail_out = (uInt)srcLen;

    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    (void)rc_inflate_init;

    int rc_undermine = inflateUndermine(&istrm, 1);
    (void)rc_undermine;

    int rc_inflate_end = inflateEnd(&istrm);
    (void)rc_inflate_end;

    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_put = gzputc(gzf, (int)payload[0]);
    (void)rc_put;
    int rc_unget = gzungetc((int)payload[0], gzf);
    (void)rc_unget;
    int rc_gzclose = gzclose(gzf);
    (void)rc_gzclose;

    // step 4: Cleanup
    delete [] src;
    delete [] compBuf;
    (void)version;
    // API sequence test completed successfully
    return 66;
}