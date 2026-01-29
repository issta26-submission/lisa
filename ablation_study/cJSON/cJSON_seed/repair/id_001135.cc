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
//<ID> 1135
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
    cJSON *t1 = cJSON_CreateTrue();
    cJSON *t2 = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, t1);
    cJSON_AddItemToArray(arr, t2);
    cJSON_AddItemToObject(root, "flags", arr);
    cJSON_AddTrueToObject(root, "enabled");

    // step 2: Configure
    cJSON *label = cJSON_CreateString("initial_label");
    cJSON_AddItemToObject(root, "label", label);
    char *prev_label = cJSON_SetValuestring(label, "updated_label");

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *computed = cJSON_CreateNumber((double)arr_size + 1.5);
    cJSON_AddItemToObject(root, "computed", computed);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    ((char *)scratch)[2] = prev_label[0];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}