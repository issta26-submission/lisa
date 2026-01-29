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
//<ID> 24
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON *str_item = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, false_item);
    cJSON_AddItemToArray(items, str_item);
    cJSON_AddNumberToObject(meta, "init_size", (double)cJSON_GetArraySize(items));

    // step 3: Operate and Validate
    cJSON_bool was_bool = cJSON_IsBool(false_item);
    cJSON *replacement = cJSON_CreateNumber(7.0);
    cJSON_ReplaceItemViaPointer(items, false_item, replacement);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(meta, "first_value", first_val);
    cJSON_AddNumberToObject(meta, "was_bool_flag", (double)was_bool);
    cJSON_AddNumberToObject(meta, "final_size", (double)cJSON_GetArraySize(items));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}