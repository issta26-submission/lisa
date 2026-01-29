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
//<ID> 82
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream defstrm;
    z_stream infstrm;
    memset(&defstrm, 0, sizeof(z_stream));
    memset(&infstrm, 0, sizeof(z_stream));
    const char src_c[] = "Payload for zlib sequence: init deflate, set header, compress, init inflateBack, checksum";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    unsigned char *windowBuf = (unsigned char *)malloc((size_t)(1 << 15));
    memset(compBuf, 0, (size_t)bound);
    memset(windowBuf, 0, (size_t)(1 << 15));

    // step 2: Setup (initialize deflate and configure gzip header)
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, sizeof(gz_header));
    header.text = 0;
    header.time = 0;
    header.xflags = 0;
    header.os = 255;
    header.hcrc = 0;
    header.done = 0;
    int rc_def_sethdr = deflateSetHeader(&defstrm, &header);

    // step 3: Operate (compress source and initialize inflate back-end with window)
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress((Bytef *)compBuf, &compLen, source, sourceLen);
    uLong checksum = adler32_z(0UL, compBuf, (z_size_t)compLen);
    int rc_inf_back_init = inflateBackInit_(&infstrm, 15, windowBuf, version, (int)sizeof(z_stream));

    // step 4: Validate & Cleanup
    int rc_inf_back_end = inflateBackEnd(&infstrm);
    int rc_def_end = deflateEnd(&defstrm);
    (void)rc_def_init; (void)rc_def_sethdr; (void)rc_comp; (void)checksum; (void)rc_inf_back_init; (void)rc_inf_back_end; (void)rc_def_end;
    free(compBuf);
    free(windowBuf);
    // API sequence test completed successfully
    return 66;
}