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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *first = cJSON_CreateString("first");
    cJSON_AddItemToArray(items, first);
    cJSON *second = cJSON_CreateString("second");
    cJSON_AddItemToArray(items, second);
    char *modified = cJSON_SetValuestring(second, "second_modified");

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "count", (double)count);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    (void)modified;
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}