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
//<ID> 1112
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
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *str_item = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "flag", false_item);
    cJSON_AddItemToArray(arr, str_item);

    // step 3: Operate and Validate
    cJSON_bool is_flag_bool = cJSON_IsBool(false_item);
    cJSON_bool is_first_string = cJSON_IsString(str_item);
    cJSON *computed = cJSON_CreateNumber((double)is_flag_bool + (double)is_first_string * 2.0);
    cJSON_AddItemToObject(root, "computed", computed);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(16);
    memset(scratch, 0, 16);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}