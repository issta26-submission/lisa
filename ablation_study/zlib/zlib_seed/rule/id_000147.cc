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
//<ID> 147
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    z_stream infstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    unsigned char input[256];
    unsigned char output[512];
    memset(input, 'A', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);

    // step 2: Setup (initialize compressors/inflaters and configure streams)
    deflateInit2_(&defstrm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&infstrm, zlibVersion(), (int)sizeof(z_stream));
    defstrm.zalloc = 0;
    defstrm.zfree = 0;
    defstrm.opaque = 0;
    infstrm.zalloc = 0;
    infstrm.zfree = 0;
    infstrm.opaque = 0;
    defstrm.next_in = (Bytef *)input;
    defstrm.avail_in = (uInt)inputLen;
    defstrm.next_out = comp;
    defstrm.avail_out = (uInt)compBound;

    // step 3: Operate (deflate -> reset-keep -> deflate -> inflate -> use gzopen + gztell64 + gzwrite)
    deflate(&defstrm, 0);
    deflateResetKeep(&defstrm);
    defstrm.next_in = (Bytef *)input;
    defstrm.avail_in = (uInt)inputLen;
    defstrm.next_out = comp + (uInt)defstrm.total_out;
    defstrm.avail_out = (uInt)(compBound - (uLong)defstrm.total_out);
    deflate(&defstrm, 0);
    infstrm.next_in = comp;
    infstrm.avail_in = (uInt)defstrm.total_out;
    infstrm.next_out = (Bytef *)output;
    infstrm.avail_out = (uInt)sizeof(output);
    inflate(&infstrm, 0);
    inflateUndermine(&infstrm, 1);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)output, (unsigned int)infstrm.total_out);
    off64_t pos = gztell64(gz);
    (void)pos;

    // step 4: Validate & Cleanup
    uLong checksum = adler32(0L, output, (uInt)infstrm.total_out);
    deflateEnd(&defstrm);
    inflateEnd(&infstrm);
    gzclose(gz);
    free(comp);
    (void)checksum;

    // API sequence test completed successfully
    return 66;
}