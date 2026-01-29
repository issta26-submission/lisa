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
//<ID> 1250
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"arr\":[1,2,3],\"name\":\"start\"}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first_item);

    // step 2: Configure
    cJSON *arr_dup = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "arr_dup", arr_dup);
    cJSON *s = cJSON_CreateString("temp");
    cJSON_SetValuestring(s, "updated-val");
    cJSON_AddItemToObject(root, "updated", s);

    // step 3: Operate and Validate
    double computed = first_val + 2.0;
    cJSON *computed_num = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_num);
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