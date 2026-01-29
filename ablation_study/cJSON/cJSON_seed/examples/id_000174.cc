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
//<ID> 174
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *first = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(meta, "first", first);
    cJSON *second = cJSON_CreateString("beta");
    cJSON_AddItemToObject(meta, "second", second);
    char *first_val = cJSON_GetStringValue(first);
    const char *arr_items[2];
    arr_items[0] = first_val;
    arr_items[1] = "gamma";
    cJSON *arr = cJSON_CreateStringArray(arr_items, 2);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *replace_target = cJSON_CreateString("delta");
    cJSON_AddItemToObject(root, "replace_target", replace_target);

    // step 3: Operate & Validate
    cJSON *new_item = cJSON_CreateString("epsilon");
    cJSON_ReplaceItemInObject(root, "replace_target", new_item);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON *dumped = cJSON_CreateString(buffer);
    cJSON_AddItemToObject(root, "dumped", dumped);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}