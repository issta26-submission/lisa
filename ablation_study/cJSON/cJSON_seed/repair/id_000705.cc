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
//<ID> 705
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

    // step 2: Configure
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON_InsertItemInArray(items, 0, s0);
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON_InsertItemInArray(items, 1, s1);
    cJSON *base_num = cJSON_AddNumberToObject(meta, "base", 42.0);

    // step 3: Operate and Validate
    cJSON *meta_ref = cJSON_GetObjectItemCaseSensitive(root, "meta");
    int size = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "item_count", (double)size);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON_bool is_str = cJSON_IsString(first);
    char *val = cJSON_GetStringValue(first);
    cJSON_AddNumberToObject(meta_ref, "first_is_string", (double)is_str);
    cJSON_AddStringToObject(meta_ref, "first_value", val);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}