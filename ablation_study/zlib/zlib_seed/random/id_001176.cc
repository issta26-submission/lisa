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
//<ID> 1176
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for compress/gz/inflateBack usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(payload_len);
    uLongf destLen = (uLongf)(bound ? bound : 1);
    Bytef *comp_buf = new Bytef[(size_t)destLen];
    memset(comp_buf, 0, (size_t)destLen);
    z_stream infl_stream;
    memset(&infl_stream, 0, (size_t)sizeof(z_stream));
    unsigned char *window = new unsigned char[(1 << 15)];
    memset(window, 0, (size_t)(1 << 15));
    gzFile gzf = gzopen64("zlib_seq_out.gz", "w+b");

    // step 2: Setup (compress payload and write to gz)
    int rc_compress = compress2(comp_buf, &destLen, (const Bytef *)payload, payload_len, 6);
    int rc_gzwrite = gzwrite(gzf, comp_buf, (unsigned int)(destLen ? destLen : 0));
    int rc_gzungetc = gzungetc((int)comp_buf[0], gzf);

    // step 3: Core operations (initialize inflateBack and tear down)
    int rc_inflate_back_init = inflateBackInit_(&infl_stream, 15, window, version, (int)sizeof(z_stream));
    int rc_inflate_back_end = inflateBackEnd(&infl_stream);

    // step 4: Validate and cleanup
    uLong crc_orig = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong crc_comp = crc32(0UL, comp_buf, (uInt)(destLen ? destLen : 0));
    int rc_gzclose = gzclose(gzf);
    delete [] comp_buf;
    delete [] window;
    (void)version;
    (void)rc_compress;
    (void)rc_gzwrite;
    (void)rc_gzungetc;
    (void)rc_inflate_back_init;
    (void)rc_inflate_back_end;
    (void)crc_orig;
    (void)crc_comp;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}