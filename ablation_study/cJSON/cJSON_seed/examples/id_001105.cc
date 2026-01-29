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
//<ID> 1105
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *stats = cJSON_CreateObject();
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *n2 = cJSON_CreateNumber(3.14);
    cJSON *flag = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddItemToObject(root, "flags", flag);
    cJSON_AddItemToObject(root, "stats", stats);
    int size_before = cJSON_GetArraySize(values);
    cJSON *total_item = cJSON_AddNumberToObject(stats, "total", (double)size_before);

    // step 3: Operate & Validate
    cJSON_bool is_array_before = cJSON_IsArray(values);
    cJSON *newnum = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInArray(values, 0, newnum);
    int size_after = cJSON_GetArraySize(values);
    cJSON_SetNumberHelper(total_item, (double)size_after);
    cJSON_bool is_array_after = cJSON_IsArray(values);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    (void)is_array_before;
    (void)is_array_after;
    (void)size_before;
    (void)size_after;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}