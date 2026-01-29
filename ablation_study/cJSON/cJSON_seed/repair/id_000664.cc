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
//<ID> 664
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *arr2 = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr2", arr2);

    // step 2: Configure
    char *tmp = (char *)cJSON_malloc(16);
    memset(tmp, 0, 16);
    tmp[0] = 'h'; tmp[1] = 'e'; tmp[2] = 'l'; tmp[3] = 'l'; tmp[4] = 'o'; tmp[5] = '\0';
    cJSON *s = cJSON_CreateString(tmp);
    cJSON_AddItemToArray(items, s);
    cJSON_AddItemToArray(arr2, cJSON_CreateNumber(3.1415));
    cJSON_free(tmp);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_string = cJSON_IsString(first);
    cJSON_AddBoolToObject(root, "first_is_string", first_is_string);
    cJSON *arr2_copy = cJSON_Duplicate(arr2, 1);
    cJSON_AddItemToObject(root, "arr2_copy", arr2_copy);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}