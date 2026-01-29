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
//<ID> 1533
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload for inflate and gz I/O";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream defstrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    uLong compBound = deflateBound(&defstrm, srcLen);
    size_t compBufSize = (size_t)compBound + 64;
    Bytef *compBuf = new Bytef[compBufSize];
    memset(compBuf, 0, compBufSize);
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;
    defstrm.next_out = compBuf;
    defstrm.avail_out = (uInt)compBufSize;
    int rc_deflate_finish = deflate(&defstrm, Z_FINISH);
    uLong compressedLen = (uLong)((size_t)compBufSize - (size_t)defstrm.avail_out);
    int rc_deflate_end = deflateEnd(&defstrm);

    // step 2: Configure (write original payload to a gz file, then read it back)
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf_w, src, (unsigned int)srcLen);
    int rc_gzclose_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen(fname, "rb");
    Bytef *readBuf = new Bytef[(size_t)srcLen];
    memset(readBuf, 0, (size_t)srcLen);
    int bytes_read = gzread(gzf_r, readBuf, (unsigned int)srcLen);

    // step 3: Operate (inflate the compressed buffer produced by deflate)
    z_stream infstrm;
    memset(&infstrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    Bytef *decompBuf = new Bytef[(size_t)srcLen + 64];
    memset(decompBuf, 0, (size_t)srcLen + 64);
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compressedLen;
    infstrm.next_out = decompBuf;
    infstrm.avail_out = (uInt)(srcLen + 64);
    int rc_inflate = inflate(&infstrm, Z_NO_FLUSH);
    uLong decompProduced = infstrm.total_out;
    int rc_inflate_end = inflateEnd(&infstrm);

    // step 4: Validate and Cleanup
    uLong flags = zlibCompileFlags();
    int cmp_read = memcmp(src, readBuf, (size_t)srcLen);
    int cmp_decomp = memcmp(src, decompBuf, (size_t)srcLen);
    int rc_gzclose_r = gzclose(gzf_r);
    delete [] src;
    delete [] compBuf;
    delete [] readBuf;
    delete [] decompBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate_finish;
    (void)rc_deflate_end;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)bytes_read;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)compressedLen;
    (void)decompProduced;
    (void)flags;
    (void)cmp_read;
    (void)cmp_decomp;
    (void)rc_gzclose_r;
    return 66;
}
// API sequence test completed successfully