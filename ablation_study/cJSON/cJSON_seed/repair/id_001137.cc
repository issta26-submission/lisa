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
//<ID> 1137
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
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "array", arr);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "flag");
    cJSON *message = cJSON_CreateString("placeholder");
    cJSON_AddItemToObject(root, "message", message);
    char *prev_value = cJSON_SetValuestring(message, "updated message");

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *computed = cJSON_CreateNumber((double)arr_size * 2.5);
    cJSON_AddItemToObject(root, "computed", computed);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    (void)prev_value;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}