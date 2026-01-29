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
//<ID> 1481
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *num0 = cJSON_CreateNumber(3.14);
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(values, num0);
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);

    // step 3: Operate & Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "values");
    cJSON_bool retrieved_is_array = cJSON_IsArray(retrieved);
    cJSON *first = cJSON_GetArrayItem(retrieved, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *second = cJSON_GetArrayItem(retrieved, 1);
    double second_val = cJSON_GetNumberValue(second);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    (void)retrieved_is_array;
    (void)first_val;
    (void)second_val;
    // API sequence test completed successfully
    return 66;
}