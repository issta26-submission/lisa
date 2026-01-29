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
//<ID> 1214
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib sequence payload";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream defstrm;
    z_stream infstrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    memset(&infstrm, 0, (size_t)sizeof(z_stream));
    unsigned char *in_window = new unsigned char[256];
    unsigned char *out_buf = new unsigned char[512];
    memset(in_window, 0, (size_t)256);
    memset(out_buf, 0, (size_t)512);

    // step 2: Setup (initialize deflate stream and set I/O pointers)
    uLong crc = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong adl = adler32(0UL, (const Bytef *)payload, (uInt)payload_len);
    defstrm.next_in = (Bytef *)payload;
    defstrm.avail_in = (uInt)payload_len;
    defstrm.next_out = out_buf;
    defstrm.avail_out = (uInt)512;
    defstrm.adler = adl;
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));

    // step 3: Core operations (write compressed-like data to a gz file and clean inflate back resources)
    gzFile gzf = gzopen("zlib_api_seq.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, payload, (unsigned int)payload_len);
    defstrm.total_out = (uLong)crc; // flow computed crc into stream state for realism
    infstrm.next_in = in_window;
    infstrm.avail_in = 0;
    infstrm.adler = defstrm.adler;
    int rc_inflate_back_end = inflateBackEnd(&infstrm);

    // step 4: Validate and cleanup
    int rc_deflate_end = deflateEnd(&defstrm);
    int rc_gzclose = gzclose(gzf);
    delete [] in_window;
    delete [] out_buf;
    (void)version;
    (void)crc;
    (void)adl;
    (void)rc_deflate_init;
    (void)rc_gzwrite;
    (void)rc_inflate_back_end;
    (void)rc_deflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}