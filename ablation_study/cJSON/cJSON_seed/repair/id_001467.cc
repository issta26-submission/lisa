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
//<ID> 1467
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"root\":{\"arr\":[1,2,3],\"name\":\"example-name\"}}";
    cJSON *parsed = cJSON_Parse(json);

    // step 2: Configure
    cJSON *root = cJSON_GetObjectItem(parsed, "root");
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON *new_obj = cJSON_CreateObject();
    cJSON_AddItemToObject(new_obj, "dup_root", dup_root);
    cJSON *arr_dup = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(new_obj, "arr", arr_dup);
    cJSON_AddItemToObject(new_obj, "name", cJSON_CreateString(name_str));

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(arr);
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    double n0 = cJSON_GetNumberValue(item0);
    double n1 = cJSON_GetNumberValue(item1);
    cJSON_AddNumberToObject(new_obj, "count", (double)count);
    cJSON_AddNumberToObject(new_obj, "sum01", n0 + n1);
    char *out = cJSON_PrintUnformatted(new_obj);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    scratch[0] = out_len ? out[0] : ' ';
    scratch[1] = (char)('0' + (count % 10));
    scratch[2] = (char)('0' + (((int)(n0 + n1)) % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(parsed);
    cJSON_Delete(new_obj);
    // API sequence test completed successfully
    return 66;
}