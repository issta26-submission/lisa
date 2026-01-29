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
//<ID> 119
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
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, n1);
    cJSON *n2 = cJSON_CreateNumber(58.5);
    cJSON_AddItemToArray(items, n2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("sample");
    cJSON_AddItemToObject(meta, "name", name);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "scale", 2.0);
    cJSON *dup_meta = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_backup", dup_meta);
    cJSON_AddNumberToObject(dup_meta, "backup_flag", 1.0);

    // step 3: Operate and Validate
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(got_items, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *dup_first = cJSON_Duplicate(first_item, 0);
    cJSON_AddItemToObject(dup_meta, "first_copy", dup_first);
    cJSON *scale_item = cJSON_GetObjectItem(root, "scale");
    double scale_val = cJSON_GetNumberValue(scale_item);
    double scaled = first_val * scale_val;
    cJSON_AddNumberToObject(root, "scaled_first", scaled);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}