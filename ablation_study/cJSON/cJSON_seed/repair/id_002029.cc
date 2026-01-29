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
//<ID> 2029
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
    cJSON_AddNumberToObject(root, "threshold", 42.5);
    cJSON *elem0 = cJSON_CreateString("first");
    cJSON_AddItemToArray(array, elem0);
    cJSON *elem_insert = cJSON_CreateString("second");
    cJSON_InsertItemInArray(array, 1, elem_insert);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    int arr_size = cJSON_GetArraySize(array);
    cJSON *num_item = cJSON_GetObjectItem(root, "threshold");
    double num_val = cJSON_GetNumberValue(num_item);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = (char)('0' + (arr_size % 10));
    buffer[2] = (char)('0' + (((int)num_val) % 10));
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}