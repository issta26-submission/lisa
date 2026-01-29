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
//<ID> 1131
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
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *enabled = cJSON_AddTrueToObject(root, "enabled");

    // step 2: Configure
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "name", name);
    char *replaced = cJSON_SetValuestring(name, "updated");

    // step 3: Operate and Validate
    cJSON *name_ref = cJSON_GetObjectItem(root, "name");
    const char *name_val = cJSON_GetStringValue(name_ref);
    int array_size = cJSON_GetArraySize(arr);
    cJSON *enabled_ref = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool enabled_true = cJSON_IsTrue(enabled_ref);
    double computed = (double)array_size + (double)enabled_true;
    cJSON *computed_num = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_num);
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    char *out = cJSON_PrintUnformatted(root);
    char *scratch = (char *)cJSON_malloc(32);
    memset(scratch, 0, 32);
    scratch[0] = out[0];
    scratch[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}