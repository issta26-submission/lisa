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
//<ID> 251
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
    cJSON *str_ref = cJSON_CreateStringReference("alpha");
    cJSON *num_item = cJSON_CreateNumber(7.0);
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    int arr_size = 0;
    double count_value = 0.0;
    const char *first_str = NULL;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "count", 2.0);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, str_ref);
    cJSON_AddItemToArray(arr, num_item);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    count_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    first_str = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    (void)arr_size;
    (void)count_value;
    (void)first_str;

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "count");
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}