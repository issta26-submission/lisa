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
//<ID> 1256
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\":[1,2,3],\"name\":\"orig\"}";
    size_t len = sizeof("{\"arr\":[1,2,3],\"name\":\"orig\"}") - 1;
    cJSON *root = cJSON_ParseWithLength(json, len);
    cJSON *s = cJSON_CreateString("placeholder");
    cJSON_SetValuestring(s, "updated");

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double val = cJSON_GetNumberValue(second);
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "arr_dup", dup_arr);
    cJSON_AddItemToObject(root, "new_name", s);

    // step 3: Operate and Validate
    double computed = val + (double)(cJSON_IsString(s));
    cJSON *comp = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", comp);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}