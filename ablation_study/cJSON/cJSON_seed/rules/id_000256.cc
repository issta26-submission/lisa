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
//<ID> 256
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
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.5);
    cJSON *name_ref = cJSON_CreateStringReference("Alice");
    cJSON *name_ref2 = cJSON_CreateStringReference("Bob");
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    int arr_size = 0;
    double first_val = 0.0;
    const char *third_str = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "people", arr);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, name_ref2);
    cJSON_AddItemToObject(root, "name", name_ref);
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    third_str = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 2));
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    (void)arr_size;
    (void)first_val;
    (void)third_str;

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "name");
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}