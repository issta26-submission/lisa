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
//<ID> 406
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
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON *n3 = cJSON_CreateNumber(-3.25);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddItemToArray(items, n3);
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *items_dup = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_dup);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "count_original", (double)cJSON_GetArraySize(items));
    cJSON_AddNumberToObject(meta, "count_copy", (double)cJSON_GetArraySize(items_dup));
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    cJSON *found_items = cJSON_GetObjectItem(root, "items");
    int original_size = cJSON_GetArraySize(found_items);
    cJSON *second_item = cJSON_GetArrayItem(found_items, 1);
    cJSON *second_copy = cJSON_Duplicate(second_item, 0);
    cJSON_AddItemToObject(root, "second_copy", second_copy);

    // step 4: Validate and Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}