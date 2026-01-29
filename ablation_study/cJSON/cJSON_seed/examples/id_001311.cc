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
//<ID> 1311
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

    // step 2: Configure
    cJSON_AddItemToArray(arr, s);
    cJSON_AddItemToArray(arr, cJSON_CreateString("gamma"));
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(root, "label_ref", s_ref);
    cJSON *ok = cJSON_AddTrueToObject(root, "ok");

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *val = cJSON_GetStringValue(first);
    cJSON *copied = cJSON_CreateString(val);
    cJSON_AddItemToObject(root, "first_copy", copied);
    cJSON_bool is_bool = cJSON_IsBool(ok);
    (void)is_bool;
    cJSON_DeleteItemFromObject(root, "label_ref");

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}