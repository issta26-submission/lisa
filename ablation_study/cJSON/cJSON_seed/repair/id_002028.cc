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
//<ID> 2028
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "version", 2.0);
    cJSON_AddTrueToObject(meta, "enabled");

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);
    cJSON *item_a = cJSON_CreateNumber(10.0);
    cJSON *item_b = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, item_b);
    cJSON_InsertItemInArray(arr, 0, item_a);

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *ver_item = cJSON_GetObjectItem(meta, "version");
    double ver_val = cJSON_GetNumberValue(ver_item);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = printed ? printed[0] : 'x';
    buffer[2] = (char)('0' + ((int)first_val % 10));
    buffer[3] = (char)('0' + ((int)ver_val % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}