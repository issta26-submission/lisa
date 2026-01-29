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
//<ID> 468
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare data and gz file for write
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    const char input[] = "ZLIB_TEST_SEQUENCE";
    unsigned int inputLen = (unsigned int)(sizeof(input) - 1);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, (voidpc)input, inputLen);
    gzclose(gw);

    // step 2: Reopen gz file for reading and consume a byte
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    int first_char = gzgetc_(gr);
    (void)first_char;

    // step 3: Initialize inflate stream and apply reset-keep
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    infStrm.next_in = NULL;
    infStrm.avail_in = 0;
    infStrm.next_out = NULL;
    infStrm.avail_out = 0;
    inflateResetKeep(&infStrm);

    // step 4: Teardown inflate back-end and finalize resources
    inflateBackEnd(&infStrm);
    inflateEnd(&infStrm);
    gzclose(gr);
    return 66;
    // API sequence test completed successfully
}