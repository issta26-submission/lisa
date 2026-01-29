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
//<ID> 1217
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib_api_sequence_test_payload";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream defstrm;
    z_stream infstrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    memset(&infstrm, 0, (size_t)sizeof(z_stream));
    Bytef *outbuf = new Bytef[512];
    memset(outbuf, 0, (size_t)512);

    uLong crc_val = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong adler_val = adler32(0UL, (const Bytef *)payload, (uInt)payload_len);

    // step 2: Setup (initialize streams)
    defstrm.next_in = (Bytef *)payload;
    defstrm.avail_in = (uInt)payload_len;
    defstrm.next_out = outbuf;
    defstrm.avail_out = (uInt)512;
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    infstrm.adler = adler_val;

    // step 3: Core operations (compress some data, write bytes to gz, call inflateBackEnd)
    int rc_deflate_step = deflate(&defstrm, 0);
    gzFile gzf = gzopen("zlib_api_seq_output.gz", "wb");
    int rc_put_first = gzputc(gzf, (int)payload[0]);
    int rc_put_last = gzputc(gzf, (int)payload[payload_len - 1]);
    int rc_inflate_backend = inflateBackEnd(&infstrm);

    // step 4: Validate and cleanup
    int rc_deflate_end = deflateEnd(&defstrm);
    int rc_inflate_end = inflateEnd(&infstrm);
    int rc_gzclose = gzclose(gzf);
    delete [] outbuf;
    (void)version;
    (void)crc_val;
    (void)adler_val;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)rc_deflate_step;
    (void)rc_put_first;
    (void)rc_put_last;
    (void)rc_inflate_backend;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}