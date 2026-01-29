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
//<ID> 1326
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"arr\": [10, 20], \"meta\": \"example\"}";
    cJSON *root = NULL;
    cJSON *arr = NULL;
    int arr_size = 0;
    cJSON *wrapper = NULL;
    cJSON *dup_root = NULL;
    cJSON *cmp_copy = NULL;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    arr = cJSON_GetObjectItem(root, "arr");
    arr_size = cJSON_GetArraySize(arr);
    wrapper = cJSON_CreateObject();
    dup_root = cJSON_Duplicate(root, 1);
    cJSON_AddItemToObject(wrapper, "original", dup_root);
    cJSON_AddNumberToObject(wrapper, "arr_size", (double)arr_size);
    cJSON_AddStringToObject(wrapper, "lib_version", cJSON_Version());

    // step 3: Operate / Validate
    cmp_copy = cJSON_Duplicate(wrapper, 1);
    equal = cJSON_Compare(wrapper, cmp_copy, 1);
    cJSON_AddItemToObject(wrapper, "equal_copy", cJSON_CreateBool(equal));

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(wrapper);
    cJSON_Delete(cmp_copy);
    // API sequence test completed successfully
    return 66;
}