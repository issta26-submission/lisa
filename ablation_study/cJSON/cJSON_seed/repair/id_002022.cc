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
//<ID> 2022
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
    cJSON_AddItemToObject(root, "items", array);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "active");
    cJSON_AddNumberToObject(root, "version", 3.14);
    cJSON *elem_a = cJSON_CreateString("alpha");
    cJSON *elem_b = cJSON_CreateString("beta");
    cJSON_InsertItemInArray(array, 0, elem_a);
    cJSON_InsertItemInArray(array, 1, elem_b);
    cJSON *elem_c = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(array, elem_c);

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(array);
    cJSON *first = cJSON_GetArrayItem(array, 0);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    const char *first_str = cJSON_GetStringValue(first);
    const char *second_str = cJSON_GetStringValue(second);
    cJSON *ver = cJSON_GetObjectItem(root, "version");
    double ver_val = cJSON_GetNumberValue(ver);
    char *printed = cJSON_Print(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = first_str[0];
    buffer[2] = second_str[0];
    buffer[3] = (char)('0' + (arr_size % 10));
    buffer[4] = (char)('0' + ((int)ver_val % 10));
    buffer[5] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}