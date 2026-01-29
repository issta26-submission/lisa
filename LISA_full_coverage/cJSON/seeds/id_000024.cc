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
    const char *json = "{\"num\":1.5,\"text\":\"orig\",\"nested\":{\"v\":10}}";
    size_t json_len = sizeof("{\"num\":1.5,\"text\":\"orig\",\"nested\":{\"v\":10}}") - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);
    cJSON *item_num = cJSON_GetObjectItem(root, "num");
    cJSON *item_text = cJSON_GetObjectItem(root, "text");
    cJSON *item_nested = cJSON_GetObjectItem(root, "nested");

    // step 2: Configure
    double new_num = cJSON_SetNumberHelper(item_num, 3.14159);
    char *set_str_ret = cJSON_SetValuestring(item_text, "modified");
    cJSON *raw_added = cJSON_AddRawToObject(root, "rawField", "{\"inner\":true}");
    cJSON *nested_ref = cJSON_CreateObjectReference(item_nested);
    cJSON_bool add_ref_ok = cJSON_AddItemToObject(root, "nestedRef", nested_ref);

    // step 3: Operate and Validate
    double read_num = cJSON_GetNumberValue(item_num);
    const char *read_text = cJSON_GetStringValue(item_text);
    cJSON *got_raw = cJSON_GetObjectItem(root, "rawField");
    const char *raw_value = cJSON_GetStringValue(got_raw);

    // step 4: Cleanup
    (void)new_num;
    (void)set_str_ret;
    (void)add_ref_ok;
    (void)read_num;
    (void)read_text;
    (void)raw_value;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}