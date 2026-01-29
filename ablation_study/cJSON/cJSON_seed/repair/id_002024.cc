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
//<ID> 2024
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", array);
    cJSON_AddNumberToObject(root, "threshold", 42.5);
    cJSON_AddTrueToObject(root, "enabled");

    // step 2: Configure
    cJSON *numA = cJSON_CreateNumber(7.0);
    cJSON *numB = cJSON_CreateNumber(13.0);
    cJSON_InsertItemInArray(array, 0, numB);
    cJSON_AddItemToArray(array, numA);

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(array);
    cJSON *first = cJSON_GetArrayItem(array, 0);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    double v_first = cJSON_GetNumberValue(first);
    double v_second = cJSON_GetNumberValue(second);
    char *printed = cJSON_Print(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (arr_size % 10));
    buffer[2] = (char)('0' + ((int)v_first % 10 + 10) % 10);
    buffer[3] = (char)('0' + ((int)v_second % 10 + 10) % 10);
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}