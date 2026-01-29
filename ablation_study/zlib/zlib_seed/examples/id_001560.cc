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
//<ID> 1560
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload for deflateBound, compress2 and inflate usage";
    size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, srcLen);
    int rc_def_end = deflateEnd(&dstrm);
    Bytef *comp = new Bytef[(size_t)bound];
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_compress = compress2(comp, &compLen, src, srcLen, 6);

    // step 2: Configure (open gz file and query position)
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    off_t pos_before = gztell(gzf);
    int rc_gzclose_write = gzclose(gzf);

    // step 3: Operate (initialize inflate stream, call inflateUndermine and inflateResetKeep, then inflate)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    Bytef *out = new Bytef[payloadLen];
    memset(out, 0, payloadLen);
    istrm.next_out = out;
    istrm.avail_out = (uInt)srcLen;
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate_undermine = inflateUndermine(&istrm, 1);
    int rc_inflate_resetkeep = inflateResetKeep(&istrm);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] comp;
    delete [] out;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)bound;
    (void)compLen;
    (void)rc_def_init;
    (void)rc_def_end;
    (void)rc_compress;
    (void)pos_before;
    (void)rc_gzclose_write;
    (void)rc_inf_init;
    (void)rc_inflate_undermine;
    (void)rc_inflate_resetkeep;
    (void)rc_inflate;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}