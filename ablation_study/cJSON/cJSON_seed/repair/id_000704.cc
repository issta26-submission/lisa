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
//<ID> 704
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "obj", obj);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(obj, "items", arr);

    // step 2: Configure
    cJSON *num_item = cJSON_AddNumberToObject(obj, "value", 123.456);
    cJSON *str_item = cJSON_CreateString("hello_cJSON");
    cJSON_InsertItemInArray(arr, 0, str_item);
    cJSON_AddNumberToObject(root, "root_marker", 7.0);

    // step 3: Operate and Validate
    cJSON *found = cJSON_GetObjectItemCaseSensitive(obj, "value");
    double extracted = cJSON_GetNumberValue(found);
    cJSON_AddNumberToObject(root, "extracted_value", extracted);
    cJSON *arr0 = cJSON_GetArrayItem(arr, 0);
    cJSON_bool is_str = cJSON_IsString(arr0);
    char *sval = cJSON_GetStringValue(arr0);
    cJSON *mirror = cJSON_CreateString(sval);
    cJSON_AddItemToObject(root, "mirror", mirror);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}