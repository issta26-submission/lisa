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
//<ID> 1050
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"arr\":[{\"id\":1,\"name\":\"one\"},{\"id\":2,\"name\":\"two\"}],\"flag\":false}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    cJSON *first_name = cJSON_GetObjectItem(first_item, "name");
    cJSON_bool name_invalid = cJSON_IsInvalid(first_name);
    const char *first_name_str = cJSON_GetStringValue(first_name);
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddStringToObject(summary, "first_name", first_name_str);
    cJSON_AddBoolToObject(summary, "name_invalid", name_invalid);

    // step 3: Operate
    char *pretty_root = cJSON_Print(root);
    char *pretty_summary = cJSON_Print(summary);
    int buf_len = 256;
    char *prealloc_buf = (char *)cJSON_malloc(buf_len);
    memset(prealloc_buf, 0, buf_len);
    cJSON_PrintPreallocated(summary, prealloc_buf, buf_len, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty_root);
    cJSON_free(pretty_summary);
    cJSON_free(prealloc_buf);
    cJSON_Delete(summary);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}