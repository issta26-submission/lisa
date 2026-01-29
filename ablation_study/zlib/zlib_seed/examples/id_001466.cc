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
//<ID> 1466
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
    z_stream deflateStrm;
    z_stream inflateStrm;
    memset(&deflateStrm, 0, sizeof(deflateStrm));
    memset(&inflateStrm, 0, sizeof(inflateStrm));
    gz_header header;
    memset(&header, 0, sizeof(header));
    Bytef *nameBuf = new Bytef[16];
    memset(nameBuf, 0, 16);
    const char gname[] = "testname";
    memcpy(nameBuf, gname, (size_t)(sizeof(gname) - 1));
    header.name = nameBuf;
    header.name_max = 16;
    header.text = 1;

    // step 2: Setup
    uLong compBound = compressBound(srcLen);
    Bytef *outBuf = new Bytef[(size_t)compBound];
    memset(outBuf, 0, (size_t)compBound);
    int rc_deflate_init = deflateInit2_(&deflateStrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    deflateStrm.next_in = src;
    deflateStrm.avail_in = (uInt)srcLen;
    deflateStrm.next_out = outBuf;
    deflateStrm.avail_out = (uInt)compBound;
    int rc_deflate_set_header = deflateSetHeader(&deflateStrm, &header);

    // step 3: Core operations
    int rc_deflate_call = deflate(&deflateStrm, 0);
    unsigned int pending = 0;
    int bits = 0;
    int rc_deflate_pending = deflatePending(&deflateStrm, &pending, &bits);
    int rc_deflate_end = deflateEnd(&deflateStrm);
    int rc_inflate_init = inflateInit2_(&inflateStrm, 31, version, (int)sizeof(z_stream));
    int rc_inflate_undermine = inflateUndermine(&inflateStrm, 1);
    gzFile gzw = gzopen("test_zlib_api_sequence_temp.gz", "wb");
    int rc_gzput = gzputc(gzw, (int)src[0]);
    int rc_gzclose_w = gzclose_w(gzw);
    gzFile gzr = gzopen("test_zlib_api_sequence_temp.gz", "rb");
    int rc_gzungetc = gzungetc((int)'B', gzr);
    int rc_gzgetc = gzgetc(gzr);
    int rc_gzclose_r = gzclose(gzr);
    int rc_inflate_end = inflateEnd(&inflateStrm);

    // step 4: Cleanup
    delete [] src;
    delete [] outBuf;
    delete [] nameBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate_set_header;
    (void)rc_deflate_call;
    (void)pending;
    (void)bits;
    (void)rc_deflate_pending;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate_undermine;
    (void)rc_gzput;
    (void)rc_gzclose_w;
    (void)rc_gzungetc;
    (void)rc_gzgetc;
    (void)rc_gzclose_r;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}