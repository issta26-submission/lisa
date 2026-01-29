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
//<ID> 1118
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
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "arr", arr);

    // step 2: Configure
    cJSON *meta = cJSON_CreateString("meta");
    cJSON_AddItemToObject(root, "note", meta);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool first_is_string = cJSON_IsString(first);
    cJSON_bool second_is_bool = cJSON_IsBool(second);
    double computed = 10.0 + (double)second_is_bool * 2.0 + (double)first_is_string * 3.0;
    cJSON *computed_item = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_item);
    char *out = cJSON_PrintUnformatted(root);
    char *scratch = (char *)cJSON_malloc(32);
    memset(scratch, 0, 32);
    scratch[0] = out[0];
    scratch[1] = out[1];
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}