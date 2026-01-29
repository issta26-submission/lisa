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
//<ID> 1600
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "example payload for zlib usage: deflateBound, compress2, inflate, gzwrite/gzopen/gzgetc_";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    const char *version = zlibVersion();
    z_stream defstrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&defstrm, srcLen);

    // step 2: Configure (compress to buffer and write to gzip file)
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = bound;
    int rc_compress = compress2(compBuf, &compLen, src, srcLen, 6);
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf_write = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf_write, compBuf, (unsigned int)compLen);
    int rc_gzclose_write = gzclose(gzf_write);

    // step 3: Operate (read one byte with gzgetc_, inspect error string, inflate compressed data)
    gzFile gzf_read = gzopen(fname, "rb");
    int read_char = gzgetc_(gzf_read);
    int gz_errnum = 0;
    const char *errstr = gzerror(gzf_read, &gz_errnum);
    z_stream infstrm;
    memset(&infstrm, 0, (size_t)sizeof(z_stream));
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compLen;
    Bytef *outBuf = new Bytef[payloadLen + 16];
    memset(outBuf, 0, payloadLen + 16);
    infstrm.next_out = outBuf;
    infstrm.avail_out = (uInt)(payloadLen + 16);
    int rc_inflate_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&infstrm, 0);
    int rc_inflate_end = inflateEnd(&infstrm);

    // step 4: Validate & Cleanup
    int rc_deflate_end = deflateEnd(&defstrm);
    int rc_gzclose_read = gzclose(gzf_read);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)rc_deflate_init;
    (void)bound;
    (void)compLen;
    (void)rc_compress;
    (void)rc_gzwrite;
    (void)rc_gzclose_write;
    (void)read_char;
    (void)gz_errnum;
    (void)errstr;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    (void)rc_gzclose_read;
    // API sequence test completed successfully
    return 66;
}