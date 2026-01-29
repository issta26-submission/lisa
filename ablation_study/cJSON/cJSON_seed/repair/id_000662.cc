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
//<ID> 662
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *values = cJSON_AddArrayToObject(info, "values");
    cJSON *manual_array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "manual", manual_array);

    // step 2: Configure
    cJSON_AddStringToObject(info, "name", "api_test");
    cJSON_AddNumberToObject(info, "version", 1.0);
    cJSON_AddItemToArray(values, cJSON_CreateString("one"));
    cJSON_AddItemToArray(values, cJSON_CreateString("two"));
    cJSON_AddItemToArray(manual_array, cJSON_CreateNumber(3.14));
    cJSON_AddItemToArray(manual_array, cJSON_CreateString("three"));

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(values, 0);
    cJSON_bool first_is_str = cJSON_IsString(first);
    cJSON_AddBoolToObject(root, "first_is_string", first_is_str);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, 256, 0);
    (void)printed;
    cJSON_AddStringToObject(root, "snapshot", buffer);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}