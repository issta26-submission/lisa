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
//<ID> 1437
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"meta\":{\"name\":\"example\",\"count\":2},\"arr\":[10,20]}";
    size_t json_len = strlen(json_text);
    cJSON *root = cJSON_ParseWithLength(json_text, json_len);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *arr = cJSON_GetObjectItem(root, "arr");

    // step 2: Configure & Operate
    cJSON *meta_dup = cJSON_Duplicate(meta, 1);
    cJSON *detached_arr = cJSON_DetachItemViaPointer(root, arr);
    cJSON *raw_added = cJSON_AddRawToObject(root, "raw_added", "{\"new\":true,\"vals\":[1,2,3]}");
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);

    // step 3: Validate
    cJSON *dup_name = cJSON_GetObjectItem(meta_dup, "name");
    const char *name_str = cJSON_GetStringValue(dup_name);
    cJSON *first_elem = cJSON_GetArrayItem(detached_arr, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    (void)printed_ok;
    (void)name_str;
    (void)first_val;

    // step 4: Cleanup
    cJSON_Delete(meta_dup);
    cJSON_Delete(detached_arr);
    cJSON_Delete(root);
    cJSON_free(buffer);

    // API sequence test completed successfully
    return 66;
}