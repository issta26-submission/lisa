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
//<ID> 401
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"root\":{\"items\":[10,20,30],\"meta\":{\"name\":\"example\"}}}";
    const char *parse_end = NULL;
    cJSON *doc = cJSON_ParseWithOpts(json, &parse_end, 1);
    cJSON *root_obj = cJSON_GetObjectItem(doc, "root");
    cJSON *items = cJSON_GetObjectItem(root_obj, "items");
    cJSON *meta = cJSON_GetObjectItem(root_obj, "meta");

    // step 2: Configure
    cJSON *new_arr = cJSON_CreateArray();
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    cJSON *dup_second = cJSON_Duplicate(second_item, 1);
    cJSON_AddItemToArray(new_arr, dup_second);
    cJSON_AddItemToObject(doc, "copied_items", new_arr);

    // step 3: Operate
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 0));
    cJSON *dup_in_newarr = cJSON_GetArrayItem(new_arr, 0);
    double combined = first_val + cJSON_GetNumberValue(dup_in_newarr);
    cJSON_SetNumberHelper(dup_in_newarr, combined);

    // step 4: Validate and Cleanup
    int orig_size = cJSON_GetArraySize(items);
    int copied_size = cJSON_GetArraySize(new_arr);
    cJSON *name_item = cJSON_GetObjectItem(meta, "name");
    char *out = cJSON_PrintUnformatted(doc);
    (void)orig_size;
    (void)copied_size;
    (void)name_item;
    cJSON_free(out);
    cJSON_Delete(doc);
    // API sequence test completed successfully
    return 66;
}