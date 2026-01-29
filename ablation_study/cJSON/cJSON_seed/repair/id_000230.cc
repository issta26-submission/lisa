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
//<ID> 230
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
    cJSON *items = cJSON_CreateArray();
    cJSON *false_flag = cJSON_CreateFalse();
    cJSON_InsertItemInArray(items, 0, false_flag);

    // step 2: Configure
    cJSON_AddItemToObject(meta, "active", cJSON_CreateTrue());
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "items", items);

    // step 3: Operate / Validate
    cJSON *retrieved_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *retrieved_items = cJSON_GetObjectItem(root, "items");
    cJSON_InsertItemInArray(retrieved_items, 1, cJSON_CreateString("second_entry"));
    cJSON_InsertItemInArray(retrieved_items, 2, cJSON_CreateNumber(42));
    cJSON_bool is_meta_active_false = cJSON_IsFalse(cJSON_GetObjectItem(retrieved_meta, "active"));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}