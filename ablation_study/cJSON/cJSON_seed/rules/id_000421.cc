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
//<ID> 421
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    char *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *retrieved_array = NULL;
    cJSON *first_item = NULL;
    double first_val = 0.0;
    int arr_size = 0;
    cJSON *newnum = NULL;

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retrieved_array = cJSON_GetObjectItem(parsed, "numbers");
    arr_size = cJSON_GetArraySize(retrieved_array);
    first_item = cJSON_GetArrayItem(retrieved_array, 0);
    first_val = cJSON_GetNumberValue(first_item);
    newnum = cJSON_CreateNumber(first_val + 10.0);
    cJSON_ReplaceItemInArray(retrieved_array, 0, newnum);
    buffer[0] = 'N';
    buffer[1] = (char)('0' + (arr_size % 10));
    buffer[2] = (char)('0' + ((int)first_val % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}