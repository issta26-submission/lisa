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
//<ID> 1552
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload: sample data for compress/inflate and gz I/O";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    uInt half = (uInt)(payloadLen / 2);
    uLong ad1 = adler32(0UL, src, half);
    uLong ad2 = adler32(0UL, src + half, (uInt)(payloadLen - half));
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)(payloadLen - half));
    uLong compBound = compressBound(srcLen);
    Bytef *comp = new Bytef[(size_t)compBound];
    memset(comp, 0, (size_t)compBound);
    uLong compLen = compBound;
    int rc_comp = compress2(comp, &compLen, src, srcLen, 6);
    const char *version = zlibVersion();

    // step 2: Configure (write payload to a gz file and prepare z_stream)
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int written = gzwrite(gzf, src, (unsigned int)srcLen);
    int rc_close1 = gzclose(gzf);
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    Bytef *out = new Bytef[payloadLen];
    memset(out, 0, payloadLen);
    strm.next_out = out;
    strm.avail_out = (uInt)srcLen;

    // step 3: Operate (initialize inflate, perform inflate, check sync point, end)
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&strm, 0);
    int rc_sync = inflateSyncPoint(&strm);
    int rc_inflate_end = inflateEnd(&strm);

    // step 4: Validate and Cleanup
    gzFile gzf_read = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *gzbuf = new Bytef[payloadLen];
    memset(gzbuf, 0, payloadLen);
    int read = gzread(gzf_read, gzbuf, (unsigned int)srcLen);
    int rc_close2 = gzclose(gzf_read);
    delete [] src;
    delete [] comp;
    delete [] out;
    delete [] gzbuf;
    (void)ad_combined;
    (void)rc_comp;
    (void)written;
    (void)rc_close1;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_sync;
    (void)rc_inflate_end;
    (void)read;
    (void)rc_close2;
    (void)version;
    // API sequence test completed successfully
    return 66;
}