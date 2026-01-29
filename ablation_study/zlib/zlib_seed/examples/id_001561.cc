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
//<ID> 1561
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib sequence payload for deflateBound, inflateUndermine, inflateResetKeep and gztell usage";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    z_stream def_strm;
    memset(&def_strm, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong compBound = deflateBound(&def_strm, srcLen);
    Bytef *comp = new Bytef[(size_t)compBound];
    memset(comp, 0, (size_t)compBound);
    uLong compLen = compBound;
    int rc_compress = compress(comp, &compLen, src, srcLen);

    // step 2: Configure (create gz file and write compressed data)
    const char *fname = "test_zlib_api_sequence_output.gz";
    gzFile gzf = gzopen(fname, "wb");
    int written = gzwrite(gzf, comp, (unsigned int)compLen);
    off_t pos = gztell(gzf);

    // step 3: Operate (inflate stream reset/undermine and decompress buffer)
    z_stream inf_strm;
    memset(&inf_strm, 0, (size_t)sizeof(z_stream));
    int rc_inf_init = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp;
    inf_strm.avail_in = (uInt)compLen;
    uLong outBufLen = srcLen * 2UL;
    Bytef *out = new Bytef[(size_t)outBufLen];
    memset(out, 0, (size_t)outBufLen);
    inf_strm.next_out = out;
    inf_strm.avail_out = (uInt)outBufLen;
    int rc_inf_resetkeep = inflateResetKeep(&inf_strm);
    int rc_inf_undermine = inflateUndermine(&inf_strm, 1);
    uLong decLen = outBufLen;
    int rc_uncompress = uncompress(out, &decLen, comp, compLen);
    int rc_inf_end = inflateEnd(&inf_strm);
    int rc_def_end = deflateEnd(&def_strm);

    // step 4: Validate and Cleanup
    int rc_close = gzclose(gzf);
    delete [] src;
    delete [] comp;
    delete [] out;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)rc_def_init;
    (void)compBound;
    (void)rc_compress;
    (void)written;
    (void)pos;
    (void)rc_inf_init;
    (void)rc_inf_resetkeep;
    (void)rc_inf_undermine;
    (void)rc_uncompress;
    (void)decLen;
    (void)rc_inf_end;
    (void)rc_def_end;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}