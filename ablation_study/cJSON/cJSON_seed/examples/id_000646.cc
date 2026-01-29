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
//<ID> 646
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", array);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToArray(array, maybe_null);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced_null");
    cJSON_bool replace_result = cJSON_ReplaceItemViaPointer(array, maybe_null, replacement);
    cJSON *extra_number = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(array, extra_number);

    // step 3: Operate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(array, buffer, (int)sizeof(buffer), 1);
    (void)printed;
    (void)replace_result;
    (void)version;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}