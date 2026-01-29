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
//<ID> 1215
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for crc32/adler32/deflateInit_/inflateBackEnd/gzclose";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream defstrm;
    z_stream infstrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    memset(&infstrm, 0, (size_t)sizeof(z_stream));
    unsigned char *buffer = new unsigned char[512];
    memset(buffer, 0, (size_t)512);

    // step 2: Setup (initialize deflate and inflate streams)
    defstrm.next_in = (Bytef *)payload;
    defstrm.avail_in = (uInt)payload_len;
    defstrm.next_out = buffer;
    defstrm.avail_out = 512;
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));

    // step 3: Core operations (compute checksums, write gz, invoke inflateBackEnd)
    uLong crc = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong adl = adler32(0UL, (const Bytef *)payload, (uInt)payload_len);
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)payload, (unsigned int)(payload_len ? payload_len : 0));
    infstrm.next_in = buffer;
    infstrm.avail_in = 0;
    infstrm.adler = adl;
    int rc_inflate_back_end = inflateBackEnd(&infstrm);

    // step 4: Validate and cleanup
    int rc_deflate_end = deflateEnd(&defstrm);
    int rc_inflate_end = inflateEnd(&infstrm);
    int rc_gzclose = gzclose(gzf);
    delete [] buffer;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)crc;
    (void)adl;
    (void)rc_gzwrite;
    (void)rc_inflate_back_end;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}