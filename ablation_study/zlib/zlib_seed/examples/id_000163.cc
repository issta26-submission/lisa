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
//<ID> 163
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers and source
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(z_stream));
    const char src_c[] = "zlib API sequence payload: demonstrate compress -> uncompress -> crc32_z -> gzputs -> inflateBackInit_";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *uncompBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(uncompBuf, 0, (size_t)sourceLen);

    // step 2: Setup & compress the source into compBuf
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 3: Operate — uncompress, compute CRCs, write a message with gzputs, and initialize inflateBack
    uLongf destLen = (uLongf)sourceLen;
    int rc_uncomp = uncompress(uncompBuf, &destLen, compBuf, (uLong)compLen);
    uLong crc_src = crc32_z(0UL, source, (z_size_t)sourceLen);
    uLong crc_uncomp = crc32_z(0UL, uncompBuf, (z_size_t)destLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzputs = gzputs(gz, "Wrote decompression metadata: CRCs follow\n");
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int rc_infback_init = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));
    int rc_infback_end = inflateBackEnd(&back_strm);

    // step 4: Cleanup & finalize
    int rc_gz_close = gzclose(gz);
    free(compBuf);
    free(uncompBuf);
    (void)rc_comp;
    (void)rc_uncomp;
    (void)crc_src;
    (void)crc_uncomp;
    (void)rc_gzputs;
    (void)rc_infback_init;
    (void)rc_infback_end;
    (void)rc_gz_close;
    // API sequence test completed successfully
    return 66;
}