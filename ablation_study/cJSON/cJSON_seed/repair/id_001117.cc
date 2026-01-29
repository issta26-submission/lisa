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
//<ID> 1117
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "note", cJSON_CreateString("metadata"));
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON_bool is_bool = cJSON_IsBool(false_item);
    cJSON_bool is_str = cJSON_IsString(str_item);
    double computed_value = 100.0 + (double)(is_bool) * 10.0 + (double)(is_str) * 1.0;
    cJSON *computed_item = cJSON_CreateNumber(computed_value);
    cJSON_AddItemToObject(root, "computed", computed_item);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : '\0';
    ((char *)scratch)[1] = out ? out[1] : '\0';
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}