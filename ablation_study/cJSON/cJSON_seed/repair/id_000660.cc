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
//<ID> 660
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr_added = cJSON_AddArrayToObject(root, "items");
    cJSON *arr_manual = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "manual_items", arr_manual);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr_added, s1);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr_added, s2);
    char *buf = (char *)cJSON_malloc(6);
    memcpy(buf, "gamma", 6);
    cJSON *s3 = cJSON_CreateString(buf);
    cJSON_AddItemToArray(arr_manual, s3);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr_added, 0);
    cJSON_bool first_is_string = cJSON_IsString(first);
    cJSON_AddBoolToObject(root, "first_is_string", first_is_string);
    const char *first_val = cJSON_GetStringValue(first);
    cJSON_AddStringToObject(root, "first_value", first_val);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}