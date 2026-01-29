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
//<ID> 1278
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"items\":[10,20,30],\"flags\":{\"active\":true}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *second = cJSON_GetArrayItem(items, 1);

    // step 2: Configure
    cJSON *created_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "created_false", created_false);

    // step 3: Operate and Validate
    double val_second = cJSON_GetNumberValue(second);
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "dup_items", dup_items);
    cJSON *dup_first = cJSON_GetArrayItem(dup_items, 0);
    double val_dup_first = cJSON_GetNumberValue(dup_first);
    double total = val_second + val_dup_first;
    cJSON *sum_node = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "sum", sum_node);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(16);
    memset(scratch, 0, 16);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}