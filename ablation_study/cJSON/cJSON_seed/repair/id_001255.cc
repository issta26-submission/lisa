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
//<ID> 1255
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"arr\":[10,20,30],\"name\":\"orig\"}";
    size_t len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, len);
    cJSON *s = cJSON_CreateString("created");
    cJSON_SetValuestring(s, "updated");
    cJSON_AddItemToObject(root, "added", s);

    // step 2: Configure
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON_AddItemToObject(root, "dup", dup_root);
    cJSON *dup_name = cJSON_GetObjectItem(dup_root, "name");
    cJSON_SetValuestring(dup_name, "dup-updated");
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *arr_item0 = cJSON_GetArrayItem(arr, 0);

    // step 3: Operate and Validate
    double v0 = cJSON_GetNumberValue(arr_item0);
    cJSON *dup_arr = cJSON_GetObjectItem(dup_root, "arr");
    cJSON *dup_item1 = cJSON_GetArrayItem(dup_arr, 1);
    double v1 = cJSON_GetNumberValue(dup_item1);
    double computed = v0 + v1;
    cJSON *computed_num = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_num);
    cJSON *mirror = cJSON_CreateString(cJSON_GetStringValue(dup_name));
    cJSON_AddItemToObject(root, "mirror", mirror);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}