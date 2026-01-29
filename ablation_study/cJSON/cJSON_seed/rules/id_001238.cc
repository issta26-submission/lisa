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
//<ID> 1238
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num = NULL;
    cJSON *bool_false = NULL;
    cJSON *str = NULL;
    cJSON *retrieved = NULL;
    cJSON *bool_item = NULL;
    char *printed = NULL;
    const char *found_str = NULL;
    double set_result = 0.0;
    cJSON_bool is_false = 0;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);
    num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num);
    bool_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, bool_false);
    str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str);

    // step 3: Operate / Validate
    set_result = cJSON_SetNumberHelper(num, 2.71828);
    (void)set_result;
    cJSON_AddItemReferenceToArray(arr, str);
    retrieved = cJSON_GetObjectItem(root, "data");
    bool_item = cJSON_GetArrayItem(retrieved, 1);
    is_false = cJSON_IsFalse(bool_item);
    arr_size = cJSON_GetArraySize(retrieved);
    found_str = cJSON_GetStringValue(cJSON_GetArrayItem(retrieved, 2));
    printed = cJSON_PrintUnformatted(root);
    (void)is_false;
    (void)arr_size;
    (void)found_str;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}