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
//<ID> 471
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"meta\":{\"count\":2},\"items\":[10,20]}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *count_item = cJSON_GetObjectItem(meta, "count");
    double orig_count = cJSON_GetNumberValue(count_item);

    // step 2: Configure
    cJSON *moved_array = cJSON_AddArrayToObject(root, "moved_items");
    cJSON *active_flag = cJSON_AddTrueToObject(root, "active");
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON_bool ref_added = cJSON_AddItemReferenceToArray(moved_array, first_item);
    double new_count = cJSON_SetNumberHelper(count_item, orig_count + 1.0);
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON_AddNumberToObject(root, "meta_is_object", (double)meta_is_obj);
    cJSON_AddNumberToObject(root, "ref_added_flag", (double)ref_added);
    cJSON_AddNumberToObject(root, "updated_count_snapshot", new_count);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}