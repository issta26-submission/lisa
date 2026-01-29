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
//<ID> 2324
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
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(7.0));
    cJSON *label = cJSON_CreateStringReference("const-label");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    cJSON *numbers_copy = cJSON_Duplicate(numbers, 1);
    cJSON_AddItemToObject(root, "numbers_copy", numbers_copy);
    cJSON_AddItemToObject(root, "pi", cJSON_CreateNumber(3.14159));

    // step 3: Operate
    cJSON *fetched_copy = cJSON_GetObjectItem(root, "numbers_copy");
    cJSON *first_item = cJSON_GetArrayItem(fetched_copy, 0);
    double first_num = cJSON_GetNumberValue(first_item);
    char *json = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    char *label_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (label_str && label_str[0]) ? label_str[0] : '\0';
    buffer[2] = ((int)first_num) ? (char)('0' + ((int)first_num % 10)) : '0';
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}