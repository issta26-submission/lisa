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
//<ID> 89
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    z_stream infbackstrm;
    memset(&defstrm, 0, sizeof(z_stream));
    memset(&infbackstrm, 0, sizeof(z_stream));
    const char src_c[] = "Sample payload for zlib deflate -> inflateBackInit_ sequence";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    defstrm.next_in = (Bytef *)source;
    defstrm.avail_in = (uInt)sourceLen;
    defstrm.next_out = compBuf;
    defstrm.avail_out = (uInt)bound;
    const char *version = zlibVersion();

    // step 2: Setup (initialize deflate and set gzip header, compute adler checksum)
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, sizeof(gz_header));
    header.text = 1;
    header.time = 123456789UL;
    header.name_max = 16;
    header.name = (Bytef *)malloc(16);
    memset(header.name, 0, 16);
    memcpy(header.name, "zlib_test", 9);
    int rc_set_hdr = deflateSetHeader(&defstrm, &header);
    uLong adler = adler32_z(0UL, source, (z_size_t)sourceLen);

    // step 3: Operate (compress some data and initialize inflateBack with a window buffer)
    int rc_def = deflate(&defstrm, 4);
    uLong compressed_bytes = defstrm.total_out;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_infback_init = inflateBackInit_(&infbackstrm, 15, window, version, (int)sizeof(z_stream));
    int rc_infback_end = inflateBackEnd(&infbackstrm);

    // step 4: Cleanup & finalize
    (void)rc_def_init; (void)rc_set_hdr; (void)adler; (void)rc_def; (void)compressed_bytes; (void)rc_infback_init; (void)rc_infback_end;
    deflateEnd(&defstrm);
    free(compBuf);
    free(header.name);
    free(window);
    // API sequence test completed successfully
    return 66;
}