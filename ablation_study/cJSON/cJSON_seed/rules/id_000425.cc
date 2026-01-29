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
//<ID> 425
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(10.5);
    cJSON *num2 = cJSON_CreateNumber(20.0);
    cJSON *strItem = cJSON_CreateString("item");
    char *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *retrieved_array = NULL;
    int array_size = 0;
    cJSON *first_elem = NULL;
    double first_val = 0.0;
    const char *third_str = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, strItem);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddStringToObject(root, "name", "example");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retrieved_array = cJSON_GetObjectItem(parsed, "array");
    array_size = cJSON_GetArraySize(retrieved_array);
    first_elem = cJSON_GetArrayItem(retrieved_array, 0);
    first_val = cJSON_GetNumberValue(first_elem);
    third_str = cJSON_GetStringValue(cJSON_GetArrayItem(retrieved_array, 2));
    buffer[0] = (char)('0' + (array_size % 10));
    buffer[1] = (char)('0' + (((int)first_val) % 10));
    buffer[2] = third_str[0];
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}