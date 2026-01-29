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
//<ID> 1555
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload for inflateSyncPoint, adler32_combine and gzopen usage";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    uLong compBound = compressBound(srcLen);
    Bytef *comp = new Bytef[(size_t)compBound];
    memset(comp, 0, (size_t)compBound);
    uLong compLen = compBound;
    int rc_compress = compress2(comp, &compLen, src, srcLen, 6);
    const char *version = zlibVersion();

    // step 2: Configure (create gz file, write original payload, and reopen for read)
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int written = gzwrite(gzf, src, (unsigned int)srcLen);
    int rc_close1 = gzclose(gzf);
    gzFile gzf_read = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *readbuf = new Bytef[payloadLen];
    memset(readbuf, 0, payloadLen);
    int read = gzread(gzf_read, readbuf, (unsigned int)srcLen);

    // step 3: Operate (inflate the compressed buffer and query sync point)
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    Bytef *out = new Bytef[payloadLen];
    memset(out, 0, payloadLen);
    strm.next_out = out;
    strm.avail_out = (uInt)srcLen;
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&strm, 0);
    int rc_syncpoint = inflateSyncPoint(&strm);
    int rc_inflate_end = inflateEnd(&strm);

    // step 4: Validate and Cleanup
    uLong ad1 = adler32(0UL, src, (uInt)srcLen);
    uLong ad2 = adler32(0UL, out, (uInt)srcLen);
    off_t len_off = (off_t)srcLen;
    uLong ad_combined = adler32_combine(ad1, ad2, len_off);
    int rc_close2 = gzclose(gzf_read);
    delete [] src;
    delete [] comp;
    delete [] readbuf;
    delete [] out;
    (void)rc_compress;
    (void)version;
    (void)written;
    (void)rc_close1;
    (void)read;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_syncpoint;
    (void)rc_inflate_end;
    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    (void)rc_close2;
    // API sequence test completed successfully
    return 66;
}