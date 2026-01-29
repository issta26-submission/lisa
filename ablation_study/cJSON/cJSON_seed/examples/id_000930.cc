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
//<ID> 930
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
    cJSON *name = cJSON_CreateString("sensor");
    cJSON *num = cJSON_CreateNumber(3.14);
    cJSON *raw_json = cJSON_CreateRaw("{\"raw\":true}");

    // step 2: Configure
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObject(meta, "value", num);
    cJSON_AddItemToObject(meta, "embedded", raw_json);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate & Validate
    cJSON *duplicate_meta = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_copy", duplicate_meta);
    cJSON_bool name_is_string = cJSON_IsString(name);
    cJSON *dup_name = cJSON_GetObjectItem(duplicate_meta, "name");
    cJSON_bool dup_name_is_string = cJSON_IsString(dup_name);
    cJSON_AddBoolToObject(root, "is_name_string", name_is_string);
    cJSON_AddBoolToObject(root, "is_dup_name_string", dup_name_is_string);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}