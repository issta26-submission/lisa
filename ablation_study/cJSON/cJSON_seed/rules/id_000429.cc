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
//<ID> 429
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
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.5);
    cJSON *name = cJSON_CreateString("demo");
    cJSON *parsed = NULL;
    cJSON *retrieved_array = NULL;
    cJSON *item0 = NULL;
    char *printed = NULL;
    int arr_size = 0;
    double first_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retrieved_array = cJSON_GetObjectItem(parsed, "list");
    arr_size = cJSON_GetArraySize(retrieved_array);
    item0 = cJSON_GetArrayItem(retrieved_array, 0);
    first_val = cJSON_GetNumberValue(item0);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = (char)('A' + ((int)first_val % 26));
    buffer[2] = '_';
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}