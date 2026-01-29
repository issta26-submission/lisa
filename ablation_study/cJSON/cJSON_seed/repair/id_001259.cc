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
//<ID> 1259
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\":[1,2,3],\"name\":\"orig\"}";
    cJSON *root = cJSON_ParseWithLength(json, 29);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *second = cJSON_GetArrayItem(arr, 1);

    // step 2: Configure
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "dup_arr", dup_arr);
    cJSON *s = cJSON_CreateString("created");
    cJSON_SetValuestring(s, "updated");
    cJSON_AddItemToObject(root, "name_updated", s);

    // step 3: Operate and Validate
    double val = cJSON_GetNumberValue(second);
    cJSON *dup_first = cJSON_GetArrayItem(dup_arr, 0);
    double val_dup = cJSON_GetNumberValue(dup_first);
    double sum = val + val_dup;
    cJSON *sumnum = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sumnum);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(16);
    memset(scratch, 0, 16);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}