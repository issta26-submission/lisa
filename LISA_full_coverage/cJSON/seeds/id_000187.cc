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
//<ID> 187
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
    cJSON *num_item = cJSON_CreateNumber(123.5);
    cJSON *str_item = cJSON_CreateString("sample");
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(array, num_item);
    cJSON_AddItemToArray(array, str_item);
    cJSON_AddItemToArray(array, null_item);
    cJSON_AddItemToObject(root, "items", array);
    cJSON_AddStringToObject(root, "label", "test_object");

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    if (printed_len > 0) memcpy(buffer, printed, printed_len);
    cJSON_Minify(buffer);

    // step 3: Operate and Validate
    int array_size = cJSON_GetArraySize(array);
    cJSON *first = cJSON_GetArrayItem(array, 0);
    double first_value = cJSON_GetNumberValue(first);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    const char *second_str = cJSON_GetStringValue(second);
    int summary = array_size + (int)first_value + (int)(second_str ? strlen(second_str) : 0) + (int)buffer[0];
    (void)summary;
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}