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
//<ID> 319
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *bool_false = cJSON_CreateFalse();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON *arr = NULL;
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON_bool is_false = 0;
    cJSON_bool is_invalid = 0;
    int arr_size = 0;
    cJSON *retrieved_bool = NULL;
    cJSON *first_item = NULL;
    double first_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddBoolToObject(child, "feature_enabled", 1);
    cJSON_AddItemToObject(root, "flag", bool_false);
    arr = cJSON_AddArrayToObject(root, "numbers");
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    arr_size = cJSON_GetArraySize(arr);
    first_item = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(first_item);
    retrieved_bool = cJSON_GetObjectItem(child, "feature_enabled");
    is_false = cJSON_IsFalse(retrieved_bool);
    is_invalid = cJSON_IsInvalid(retrieved_bool);
    buffer[0] = (char)('0' + (int)is_false);
    buffer[1] = (char)('0' + arr_size);
    buffer[2] = (char)('A' + ((int)first_val % 26));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}