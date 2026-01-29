#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 385
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    char *buffer = (char *)0;
    cJSON_bool is_raw = 0;
    int prebuffer = 128;

    // step 2: Initialize
    root = cJSON_CreateArray();
    raw_item = cJSON_CreateRaw("{\"embedded\":true}");

    // step 3: Configure
    cJSON_AddItemToArray(root, raw_item);

    // step 4: Operate
    buffer = cJSON_PrintBuffered(root, prebuffer, 1);
    is_raw = cJSON_IsRaw(raw_item);

    // step 5: Validate
    (void)is_raw;
    (void)buffer;

    // step 6: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}