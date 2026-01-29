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
//<ID> 123
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream src_def;
    z_stream dst_def;
    z_stream inf_strm;
    memset(&src_def, 0, sizeof(src_def));
    memset(&dst_def, 0, sizeof(dst_def));
    memset(&inf_strm, 0, sizeof(inf_strm));
    unsigned char input[256];
    unsigned char outbuf[512];
    memset(input, 'A', sizeof(input));
    memset(outbuf, 0, sizeof(outbuf));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);

    // step 2: Configure (deflate source) and produce initial compressed data
    deflateInit_(&src_def, 6, zlibVersion(), (int)sizeof(z_stream));
    src_def.next_in = (Bytef *)input;
    src_def.avail_in = (uInt)inputLen;
    src_def.next_out = comp;
    src_def.avail_out = (uInt)compBound;
    deflate(&src_def, 0);
    unsigned int comp_len_initial = (unsigned int)src_def.total_out;

    // step 3: Copy deflate state and continue compression on the copy
    deflateCopy(&dst_def, &src_def);
    dst_def.next_in = (Bytef *)input;
    dst_def.avail_in = (uInt)inputLen;
    dst_def.next_out = comp + comp_len_initial;
    dst_def.avail_out = (uInt)(compBound - comp_len_initial);
    deflate(&dst_def, 0);
    unsigned int comp_len_total = (unsigned int)dst_def.total_out;

    // step 4: Inflate to validate and write out via gzdopen/gzclose, then cleanup
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    inf_strm.next_in = comp;
    inf_strm.avail_in = (uInt)comp_len_total;
    inf_strm.next_out = (Bytef *)outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    inflate(&inf_strm, 0);
    unsigned int decomp_len = (unsigned int)inf_strm.total_out;
    gzFile gz = gzdopen(1, "wb");
    gzwrite(gz, (voidpc)outbuf, decomp_len);
    gzclose(gz);
    deflateEnd(&src_def);
    deflateEnd(&dst_def);
    inflateEnd(&inf_strm);
    free(comp);
    // API sequence test completed successfully
    return 66;
}