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
//<ID> 1098
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for deflateBound, gz write and inflateBackInit_";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize deflate stream and inflateBack stream)
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    unsigned char * window = new unsigned char[32768];
    int rc_inflate_back_init = inflateBackInit_(&istrm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Core operations (compute bound, allocate buffer, write via gz layer, query position)
    uLong bound = deflateBound(&dstrm, payload_len);
    Bytef * bound_buf = new Bytef[(bound ? (size_t)bound : 1)];
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (const void *)payload, (unsigned int)payload_len);
    off_t pos = gztell(gzf);
    int rc_gzclose = gzclose(gzf);

    // step 4: Validate and cleanup (checksums, end streams, free buffers)
    uLong adler = adler32(0UL, (const Bytef *)payload, (uInt)payload_len);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_back_end = inflateBackEnd(&istrm);
    delete [] window;
    delete [] bound_buf;
    (void)rc_deflate_init;
    (void)rc_inflate_back_init;
    (void)bound;
    (void)rc_gzwrite;
    (void)pos;
    (void)rc_gzclose;
    (void)adler;
    (void)rc_deflate_end;
    (void)rc_inflate_back_end;
    // API sequence test completed successfully
    return 66;
}