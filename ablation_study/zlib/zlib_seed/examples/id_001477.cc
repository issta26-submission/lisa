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
//<ID> 1477
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for gzread/deflateTune/crc32/adler32_combine/inflateBackEnd";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream defstrm;
    z_stream infstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;

    // step 2: Setup
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gzf_w, src, (unsigned int)srcLen);
    int rc_gzflush = gzflush(gzf_w, 0);
    int rc_gzclose_w = gzclose(gzf_w);
    int rc_definit = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_tune = deflateTune(&defstrm, 20, 10, 15, 4);
    int rc_infinit = inflateInit_(&infstrm, version, (int)sizeof(z_stream));

    // step 3: Core operations
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    uInt readBufSize = (uInt)(srcLen + 8u);
    Bytef *readBuf = new Bytef[(size_t)readBufSize];
    memset(readBuf, 0, (size_t)readBufSize);
    int rc_gzread = gzread(gzf_r, readBuf, (unsigned int)readBufSize);
    uLong crc = crc32(0L, readBuf, (uInt)rc_gzread);
    uLong ad1 = adler32(0L, src, (uInt)srcLen);
    uLong ad2 = adler32(0L, readBuf, (uInt)rc_gzread);
    uLong combined_adler = adler32_combine(ad1, ad2, (off_t)rc_gzread);
    int rc_inflate_back_end = inflateBackEnd(&infstrm);

    // step 4: Cleanup
    int rc_gzclose_r = gzclose(gzf_r);
    int rc_defend = deflateEnd(&defstrm);
    int rc_inflateend = inflateEnd(&infstrm);
    delete [] src;
    delete [] readBuf;
    (void)version;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose_w;
    (void)rc_definit;
    (void)rc_def_tune;
    (void)rc_infinit;
    (void)rc_gzread;
    (void)crc;
    (void)ad1;
    (void)ad2;
    (void)combined_adler;
    (void)rc_inflate_back_end;
    (void)rc_gzclose_r;
    (void)rc_defend;
    (void)rc_inflateend;
    // API sequence test completed successfully
    return 66;
}