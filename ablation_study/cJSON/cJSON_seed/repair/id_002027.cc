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
//<ID> 2027
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
    cJSON_AddItemToObject(root, "list", array);
    cJSON *num_a = cJSON_CreateNumber(10.5);
    cJSON *num_c = cJSON_CreateNumber(42.0);
    cJSON *str_b = cJSON_CreateString("elemB");

    // step 2: Configure
    cJSON_InsertItemInArray(array, 0, num_a);
    cJSON_InsertItemInArray(array, 1, num_c);
    cJSON_AddItemToArray(array, str_b);
    cJSON_AddNumberToObject(root, "version", 1.23);
    cJSON_AddTrueToObject(root, "enabled");

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(array);
    cJSON *first = cJSON_GetArrayItem(array, 0);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    cJSON *third = cJSON_GetArrayItem(array, 2);
    double first_val = cJSON_GetNumberValue(first);
    double second_val = cJSON_GetNumberValue(second);
    const char *third_str = cJSON_GetStringValue(third);
    char *printed = cJSON_Print(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : 'X';
    buffer[1] = (char)('0' + (size % 10));
    buffer[2] = (char)('0' + ((int)first_val % 10));
    buffer[3] = (char)('0' + ((int)second_val % 10));
    buffer[4] = third_str && third_str[0] ? third_str[0] : 'Y';
    buffer[5] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}