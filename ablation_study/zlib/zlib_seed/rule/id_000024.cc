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
//<ID> 24
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream inf;
    z_stream backstrm;
    unsigned char input[128];
    unsigned char outbuf[256];
    unsigned char window[32768];
    gzFile gz = (gzFile)0;
    void * comp = (void *)0;
    uLong sourceLen = (uLong)sizeof(input);
    uLongf compLen = 0;
    memset(&inf, 0, sizeof(inf));
    memset(&backstrm, 0, sizeof(backstrm));
    memset(input, 'A', sizeof(input));
    memset(outbuf, 0, sizeof(outbuf));
    memset(window, 0, sizeof(window));

    // step 2: Setup & Configure
    uLong bound = compressBound(sourceLen);
    comp = malloc((size_t)bound);
    compLen = (uLongf)bound;
    compress((Bytef *)comp, &compLen, (const Bytef *)input, sourceLen);
    inf.next_in = (Bytef *)comp;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outbuf;
    inf.avail_out = (uInt)sizeof(outbuf);
    inflateInit_(&inf, zlibVersion(), (int)sizeof(z_stream));
    inflateBackInit_(&backstrm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate & Validate
    inflate(&inf, 0);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, outbuf, (unsigned int)inf.total_out);
    gzclose(gz);
    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    int first_byte = gzgetc_(gz);
    (void)first_byte;

    // step 4: Cleanup
    inflateBackEnd(&backstrm);
    inflateEnd(&inf);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}