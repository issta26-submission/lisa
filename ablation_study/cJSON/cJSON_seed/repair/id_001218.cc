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
//<ID> 1218
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(3.14);
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddItemToObject(obj, "inner", cJSON_CreateNumber(7.0));
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, obj);
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(obj, 1);
    cJSON_AddItemToArray(arr, dup);
    cJSON *str = cJSON_CreateString("example");
    cJSON_AddItemToArray(arr, str);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *new_root = cJSON_CreateObject();
    cJSON *moved = cJSON_DetachItemFromObject(root, "items");
    cJSON_AddItemToObject(new_root, "moved", moved);
    cJSON *computed = cJSON_CreateNumber(first_val + 1.0);
    cJSON_AddItemToObject(new_root, "computed", computed);
    char *out = cJSON_PrintUnformatted(new_root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(new_root);
    // API sequence test completed successfully
    return 66;
}