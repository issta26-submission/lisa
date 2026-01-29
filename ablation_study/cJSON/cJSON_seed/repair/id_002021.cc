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
//<ID> 2021
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
    cJSON *elem0 = cJSON_CreateString("alpha");
    cJSON *elem1 = cJSON_CreateString("beta");
    cJSON_InsertItemInArray(array, 0, elem0);
    cJSON_InsertItemInArray(array, 1, elem1);

    // step 2: Configure
    cJSON *num = cJSON_AddNumberToObject(root, "version", 3.14159);
    cJSON *flag = cJSON_AddTrueToObject(root, "active");

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(array);
    const char *first_str = cJSON_GetStringValue(cJSON_GetArrayItem(array, 0));
    double version_val = cJSON_GetNumberValue(num);
    char *printed = cJSON_Print(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : 'x';
    buffer[1] = first_str && first_str[0] ? first_str[0] : 'y';
    buffer[2] = (char)('0' + (arr_size % 10));
    buffer[3] = (char)('0' + ((int)version_val % 10));
    buffer[4] = cJSON_IsTrue(flag) ? '1' : '0';
    buffer[5] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}