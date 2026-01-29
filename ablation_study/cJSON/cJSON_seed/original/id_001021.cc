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
//<ID> 1021
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"example\",\"values\":[1,2,3],\"meta\":{\"label\":\"orig\",\"count\":3}}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *label_item = cJSON_GetObjectItem(meta, "label");
    const char *label_str = cJSON_GetStringValue(label_item);
    cJSON *copied_label = cJSON_CreateString(label_str);
    cJSON_bool add_label_res = cJSON_AddItemToObject(root, "copied_label", copied_label);
    int nums[] = {10, 20, 30};
    cJSON *int_array = cJSON_CreateIntArray(nums, 3);
    cJSON_bool add_array_res = cJSON_AddItemToObject(meta, "new_numbers", int_array);
    cJSON *meta_copy = cJSON_Duplicate(meta, 1);
    cJSON_bool add_meta_copy_res = cJSON_AddItemToObject(root, "meta_copy", meta_copy);
    cJSON_AddBoolToObject(root, "label_added", add_label_res);
    cJSON_AddBoolToObject(root, "array_added", add_array_res);
    cJSON_AddBoolToObject(root, "meta_copy_added", add_meta_copy_res);

    // step 3: Operate
    char *out_unformatted = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_bool pre_ret = cJSON_PrintPreallocated(root, print_buf, 512, 0);
    cJSON_AddBoolToObject(root, "preprinted", pre_ret);
    cJSON_DeleteItemFromObject(root, "values");

    // step 4: Validate & Cleanup
    cJSON_free(out_unformatted);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}