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
//<ID> 1562
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib payload for deflateBound, inflateResetKeep, inflateUndermine and gztell usage";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    const char *version = zlibVersion();

    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure (compute bound and compress)
    uLong bound = deflateBound(&dstrm, srcLen);
    Bytef *comp = new Bytef[(size_t)bound];
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_compress = compress2(comp, &compLen, src, srcLen, 6);

    // step 3: Operate (write compressed data to gz file, read position, setup inflate and apply resets/undermine)
    const char *fname = "test_zlib_api_sequence_output.gz";
    gzFile gzf = gzopen(fname, "wb");
    int written = gzwrite(gzf, comp, (unsigned int)compLen);
    int rc_close_write = gzclose(gzf);
    gzFile gzf_read = gzopen(fname, "rb");
    off_t read_pos = gztell(gzf_read);

    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    Bytef *out = new Bytef[payloadLen];
    memset(out, 0, payloadLen);
    istrm.next_out = out;
    istrm.avail_out = (uInt)srcLen;
    int rc_reset_keep = inflateResetKeep(&istrm);
    int rc_undermine = inflateUndermine(&istrm, 1);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_close_read = gzclose(gzf_read);
    delete [] src;
    delete [] comp;
    delete [] out;
    (void)version;
    (void)rc_deflate_init;
    (void)bound;
    (void)rc_compress;
    (void)written;
    (void)rc_close_write;
    (void)read_pos;
    (void)rc_inflate_init;
    (void)rc_reset_keep;
    (void)rc_undermine;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    (void)rc_close_read;
    // API sequence test completed successfully
    return 66;
}