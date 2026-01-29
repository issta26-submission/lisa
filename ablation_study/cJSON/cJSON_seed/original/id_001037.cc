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
//<ID> 1037
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"initial\",\"nums\":[1,2,3],\"meta\":{\"id\":9},\"s\":\"old\"}";
    size_t json_len = sizeof(initial_json) - 1;
    const char *parse_end = nullptr;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, json_len, &parse_end, 0);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *nums_item = cJSON_GetObjectItem(root, "nums");
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON *s_item = cJSON_GetObjectItem(root, "s");
    char *set_res = cJSON_SetValuestring(s_item, "new_value");
    cJSON *out = cJSON_CreateObject();
    cJSON *dup_nums = cJSON_Duplicate(nums_item, 1);
    cJSON_AddItemToObject(out, "owner", cJSON_CreateString(cJSON_GetStringValue(name_item)));
    cJSON_AddItemToObject(out, "copied_nums", dup_nums);
    cJSON_AddItemToObject(out, "meta_copy", cJSON_Duplicate(meta_item, 1));
    cJSON_AddStringToObject(out, "s_value", set_res);

    // step 3: Operate
    cJSON_bool raw_flag = cJSON_IsRaw(name_item);
    cJSON_AddBoolToObject(out, "name_is_raw", raw_flag);
    char *out_print = cJSON_Print(out);
    char *out_unformatted = cJSON_PrintUnformatted(out);
    char *print_buf = (char *)cJSON_malloc(256);
    memset(print_buf, 0, 256);

    // step 4: Validate & Cleanup
    cJSON_free(out_print);
    cJSON_free(out_unformatted);
    cJSON_free(print_buf);
    cJSON_Delete(root);
    cJSON_Delete(out);
    // API sequence test completed successfully
    return 66;
}