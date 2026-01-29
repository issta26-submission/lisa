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
//<ID> 1463
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"arr\":[10,20,30],\"obj\":{\"k\":\"v\"}}";
    cJSON *root = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON *newroot = cJSON_CreateObject();
    cJSON_AddItemToObject(newroot, "dup", dup_arr);

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(dup_arr);
    cJSON *obj = cJSON_GetObjectItem(root, "obj");
    char *kv = cJSON_GetStringValue(cJSON_GetObjectItem(obj, "k"));
    char *out = cJSON_PrintUnformatted(newroot);
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    scratch[0] = (char)('0' + (arr_size % 10));
    scratch[1] = kv[0];
    scratch[2] = out[0];

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(newroot);
    // API sequence test completed successfully
    return 66;
}