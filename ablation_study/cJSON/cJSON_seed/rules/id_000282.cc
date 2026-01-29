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
//<ID> 282
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    int arr_size = 0;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    double num_value = 0.0;
    const char *str_value = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON_AddStringToObject(root, "greet", "hello");
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("three"));

    // step 3: Operate / Validate
    num_item = cJSON_GetObjectItem(root, "pi");
    str_item = cJSON_GetObjectItem(root, "greet");
    num_value = cJSON_GetNumberValue(num_item);
    str_value = cJSON_GetStringValue(str_item);
    arr_size = cJSON_GetArraySize(arr);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = (str_value != NULL) ? str_value[0] : '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}