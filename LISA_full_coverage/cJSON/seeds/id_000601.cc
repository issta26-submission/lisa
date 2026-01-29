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
//<ID> 601
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
    cJSON *name_item = cJSON_CreateString("original_name");
    cJSON_AddItemToObject(meta, "name", name_item);
    cJSON *age_item = cJSON_CreateNumber(28);
    cJSON_AddItemToObject(root, "age", age_item);

    // step 2: Configure (duplicate and inspect)
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    cJSON *meta_dup = cJSON_GetObjectItem(duplicate, "meta");
    cJSON *name_dup = cJSON_GetObjectItem(meta_dup, "name");
    cJSON_SetValuestring(name_dup, "duplicated_name");

    // step 3: Operate (use duplicated data to add new info to original)
    cJSON *meta_orig = cJSON_GetObjectItem(root, "meta");
    cJSON *name_orig = cJSON_GetObjectItem(meta_orig, "name");
    const char *orig_str = cJSON_GetStringValue(name_orig);
    const char *dup_str = cJSON_GetStringValue(name_dup);
    cJSON *status_item = cJSON_CreateString(dup_str);
    cJSON_AddItemToObject(root, "status", status_item);
    cJSON *extra = cJSON_AddObjectToObject(root, "extra");
    cJSON_AddItemToObject(extra, "note", cJSON_CreateString(orig_str));

    // step 4: Validate & Cleanup
    (void)orig_str;
    (void)dup_str;
    cJSON_Delete(duplicate);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}