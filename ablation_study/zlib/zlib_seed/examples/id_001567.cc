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
//<ID> 1567
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib sequence payload demonstrating deflateBound, inflateUndermine, inflateResetKeep and gztell";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    z_stream def_strm;
    memset(&def_strm, 0, (size_t)sizeof(z_stream));
    z_stream infl_strm;
    memset(&infl_strm, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();

    // step 2: Configure (initialize deflate stream and compute bound)
    def_strm.zalloc = (alloc_func)0;
    def_strm.zfree = (free_func)0;
    def_strm.opaque = (voidpf)0;
    int rc_deflate_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong compBound = deflateBound(&def_strm, srcLen);
    Bytef *comp = new Bytef[(size_t)compBound];
    memset(comp, 0, (size_t)compBound);
    def_strm.next_in = src;
    def_strm.avail_in = (uInt)srcLen;
    def_strm.next_out = comp;
    def_strm.avail_out = (uInt)compBound;

    // step 3: Operate (perform deflate, write to gz, use gztell, setup inflate and call undermine/reset)
    int rc_deflate = deflate(&def_strm, Z_FINISH);
    uLong compressedLen = def_strm.total_out;
    int rc_deflate_end = deflateEnd(&def_strm);
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int written = gzwrite(gzf, comp, (unsigned int)compressedLen);
    off_t pos = gztell(gzf);
    int rc_gzclose = gzclose(gzf);
    Bytef *outbuf = new Bytef[payloadLen];
    memset(outbuf, 0, payloadLen);
    infl_strm.zalloc = (alloc_func)0;
    infl_strm.zfree = (free_func)0;
    infl_strm.opaque = (voidpf)0;
    int rc_inflate_init = inflateInit_(&infl_strm, version, (int)sizeof(z_stream));
    infl_strm.next_in = comp;
    infl_strm.avail_in = (uInt)compressedLen;
    infl_strm.next_out = outbuf;
    infl_strm.avail_out = (uInt)srcLen;
    int rc_undermine = inflateUndermine(&infl_strm, 1);
    int rc_inflate = inflate(&infl_strm, Z_FINISH);
    int rc_inflate_reset_keep = inflateResetKeep(&infl_strm);
    int rc_inflate_end = inflateEnd(&infl_strm);

    // step 4: Validate and Cleanup
    delete [] src;
    delete [] comp;
    delete [] outbuf;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)written;
    (void)pos;
    (void)rc_gzclose;
    (void)rc_inflate_init;
    (void)rc_undermine;
    (void)rc_inflate;
    (void)rc_inflate_reset_keep;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}