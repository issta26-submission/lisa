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
//<ID> 283
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
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON *str1 = cJSON_CreateString("hello");
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    int arr_size = 0;
    cJSON *found = NULL;
    double added_num_value = 0.0;
    const char *found_str = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "array", arr);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(child, "str_entry", str1);
    cJSON_AddNumberToObject(child, "added_number", 42.0);
    cJSON_AddStringToObject(root, "greeting", "world");

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    found = cJSON_GetObjectItem(root, "greeting");
    found_str = cJSON_GetStringValue(found);
    added_num_value = cJSON_GetNumberValue(cJSON_GetObjectItem(child, "added_number"));
    buffer[0] = found_str ? found_str[0] : '\0';
    buffer[1] = (char)('0' + (arr_size % 10));
    (void)added_num_value;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}