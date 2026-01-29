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
//<ID> 954
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
    cJSON *values = cJSON_CreateArray();
    cJSON *num_a = cJSON_CreateNumber(3.14);
    cJSON *num_b = cJSON_CreateNumber(2.718);
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":true}");
    const char *version_cstr = cJSON_Version();
    cJSON *version = cJSON_CreateString(version_cstr);

    // step 2: Configure
    cJSON_AddItemToArray(values, num_a);
    cJSON_AddItemToArray(values, num_b);
    cJSON_AddItemToObject(meta, "raw_payload", raw);
    cJSON_AddItemToObject(meta, "version", version);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "values", values);

    // step 3: Operate & Validate
    cJSON *first_item = cJSON_GetArrayItem(values, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw);
    cJSON *first_val_node = cJSON_CreateNumber(first_val);
    cJSON_AddItemToObject(root, "first_value", first_val_node);
    cJSON_AddItemToObject(root, "raw_is_raw", cJSON_CreateBool(raw_is_raw));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}