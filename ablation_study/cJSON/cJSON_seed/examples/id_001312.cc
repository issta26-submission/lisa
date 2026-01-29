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
//<ID> 1312
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *obj = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *str = cJSON_CreateString("alpha");
    cJSON *str_ref = cJSON_CreateStringReference("beta");
    cJSON *flag = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemReferenceToArray(arr, str_ref);
    cJSON_AddItemToObject(obj, "items", arr);
    cJSON_AddItemToObject(obj, "flag", flag);
    cJSON_AddItemToObject(root, "node", obj);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *sval = cJSON_GetStringValue(first);
    size_t len = strlen(sval);
    char *copy = (char *)cJSON_malloc(len + 1);
    memset(copy, 0, len + 1);
    memcpy(copy, sval, len + 1);
    cJSON_bool is_bool = cJSON_IsBool(flag);
    cJSON *num = cJSON_CreateNumber((double)is_bool);
    cJSON_AddItemToObject(obj, "flag_num", num);
    cJSON_DeleteItemFromObject(obj, "items");

    // step 4: Cleanup
    cJSON_free(copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}