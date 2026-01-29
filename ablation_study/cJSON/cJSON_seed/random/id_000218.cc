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
//<ID> 218
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *objChild = (cJSON *)0;
    cJSON *trueItem = (cJSON *)0;
    cJSON *size_item = (cJSON *)0;
    char *json_text = (char *)0;
    const char *version = (const char *)0;
    cJSON_bool is_arr = (cJSON_bool)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    int size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    objChild = cJSON_CreateObject();
    trueItem = cJSON_AddTrueToObject(objChild, "active");
    cJSON_AddStringToObject(objChild, "note", "child_object");
    version = cJSON_Version();
    cJSON_AddStringToObject(root, "cjson_version", version);

    // step 4: Operate
    add_ok = cJSON_AddItemToArray(arr, objChild);
    is_arr = cJSON_IsArray(arr);
    cJSON_AddNumberToObject(root, "is_array_flag", (double)is_arr);

    // step 5: Validate
    size = cJSON_GetArraySize(arr);
    size_item = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObject(root, "array_size", size_item);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}