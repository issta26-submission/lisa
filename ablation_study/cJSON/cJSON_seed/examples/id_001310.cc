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
//<ID> 1310
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
    cJSON *s = cJSON_CreateString("alpha");
    cJSON *s_ref = cJSON_CreateStringReference("beta");
    cJSON *flag = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(arr, s);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddItemToObject(root, "name", s_ref);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *val = cJSON_GetStringValue(first);
    size_t len = strlen(val);
    char *copy = (char *)cJSON_malloc(len + 1);
    memset(copy, 0, len + 1);
    memcpy(copy, val, len + 1);
    cJSON_bool is_bool = cJSON_IsBool(flag);
    cJSON *num = cJSON_CreateNumber((double)is_bool);
    cJSON_AddItemToObject(root, "isbool_num", num);

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(root, "name");
    cJSON_free(copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}