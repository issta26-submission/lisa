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
//<ID> 1218
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Sample zlib payload for checksums and streams";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream bstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&bstrm, 0, (size_t)sizeof(z_stream));
    uLong crc = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong adl = adler32(0UL, (const Bytef *)payload, (uInt)payload_len);

    // step 2: Setup (initialize deflate stream and inflate-back stream)
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    unsigned long out_capacity = (unsigned long)(payload_len + 64);
    unsigned char *out_buf = new unsigned char[out_capacity];
    memset(out_buf, 0, (size_t)out_capacity);
    dstrm.next_out = out_buf;
    dstrm.avail_out = (uInt)out_capacity;
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    unsigned char *window_buf = new unsigned char[32768];
    memset(window_buf, 0, (size_t)32768);
    int rc_inflate_back_init = inflateBackInit_(&bstrm, 15, window_buf, version, (int)sizeof(z_stream));

    // step 3: Core operations (use checksums, operate on streams, write gz, tear down back-inflate)
    bstrm.adler = adl;
    (void)bstrm.adler; // ensure usage flows are visible to optimizer
    gzFile gzf = gzopen("zlib_seq_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)payload, (unsigned int)(payload_len ? payload_len : 0));
    int rc_gzclose = gzclose(gzf);
    int rc_inflate_back_end = inflateBackEnd(&bstrm);

    // step 4: Cleanup and finalize
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] out_buf;
    delete [] window_buf;
    (void)version;
    (void)crc;
    (void)adl;
    (void)rc_deflate_init;
    (void)rc_inflate_back_init;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_inflate_back_end;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}