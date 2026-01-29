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
//<ID> 1130
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
    cJSON *str_in_array = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str_in_array);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag1", true_item);
    cJSON_AddTrueToObject(root, "flag2");
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *text_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "text", text_item);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, num);
    char *prev_value = cJSON_SetValuestring(text_item, "updated");
    (void)prev_value;

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(arr);
    double computed_val = (double)size * 3.5;
    cJSON *computed = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed);
    char *out = cJSON_PrintUnformatted(root);
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    scratch[0] = out[0];
    scratch[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}