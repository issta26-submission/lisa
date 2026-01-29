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
//<ID> 1033
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"original\",\"data\":{\"raw\":\"<raw>\"},\"arr\":[1,2,3]}";
    size_t json_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *data_item = cJSON_GetObjectItem(root, "data");
    cJSON *raw_item = cJSON_GetObjectItem(data_item, "raw");
    cJSON *arr_item = cJSON_GetObjectItem(root, "arr");
    cJSON_bool raw_flag = cJSON_IsRaw(raw_item);
    char *updated_name = cJSON_SetValuestring(name_item, "updated_name");
    cJSON *out = cJSON_CreateObject();
    cJSON *dup_arr = cJSON_Duplicate(arr_item, 1);
    cJSON_AddItemToObject(out, "copied_arr", dup_arr);
    cJSON_AddItemToObject(out, "source_name", cJSON_CreateString(updated_name));
    cJSON_AddItemToObject(out, "raw_was_raw", cJSON_CreateBool(raw_flag));

    // step 3: Operate
    char *pretty = cJSON_Print(out);
    char *compact = cJSON_PrintUnformatted(out);
    char *prebuf = (char *)cJSON_malloc(256);
    memset(prebuf, 0, 256);
    cJSON_PrintPreallocated(out, prebuf, 256, 0);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_free(prebuf);
    cJSON_Delete(root);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}