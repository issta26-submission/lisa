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
//<ID> 2328
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *str_ref = cJSON_CreateStringReference("example-reference");
    cJSON_AddItemToArray(numbers, str_ref);
    cJSON *num = cJSON_CreateNumber(123.456);
    cJSON_AddItemToArray(numbers, num);
    cJSON_AddStringToObject(meta, "creator", "tester");

    // step 3: Operate
    cJSON *numbers_copy = cJSON_Duplicate(numbers, 1);
    cJSON_AddItemToObject(root, "numbers_copy", numbers_copy);
    char *json = cJSON_PrintUnformatted(root);
    cJSON *first_item = cJSON_GetArrayItem(numbers, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    cJSON *second_item = cJSON_GetArrayItem(numbers, 1);
    double second_val = cJSON_GetNumberValue(second_item);

    // step 4: Validate and Cleanup
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (first_str && first_str[0]) ? first_str[0] : (second_val != 0.0 ? '1' : '0');
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}