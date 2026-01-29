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
//<ID> 465
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers and write a gz file
    z_stream infBackStrm;
    memset(&infBackStrm, 0, sizeof(infBackStrm));
    unsigned char input[128];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong orig_adler = adler32(0UL, (const Bytef *)input, (uInt)inputLen);
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gzw, (voidpc)input, (unsigned int)inputLen);
    gzclose(gzw);

    // step 2: Reopen gz file for reading and use gzgetc_
    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    int c = gzgetc_(gzr);
    char one = (char)c;
    uLong single_adler = adler32(0UL, (const Bytef *)&one, (uInt)1);
    uLong combined_adler = adler32_combine(orig_adler, single_adler, (off_t)1);
    (void)combined_adler;
    gzclose(gzr);

    // step 3: Initialize inflateBack, reset keep state, then end inflateBack
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&infBackStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateResetKeep(&infBackStrm);
    inflateBackEnd(&infBackStrm);

    // step 4: Cleanup and finalize
    free(window);
    // API sequence test completed successfully
    return 66;
}