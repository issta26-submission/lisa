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
//<ID> 2362
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int values[4] = {10, 20, 30, 40};
    cJSON *root = cJSON_CreateObject();
    cJSON *int_array = cJSON_CreateIntArray(values, 4);
    cJSON *extra_num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToArray(int_array, extra_num);

    // step 2: Configure
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON_AddNumberToObject(root, "threshold", 99.5);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON *second = cJSON_GetArrayItem(int_array, 1);
    double second_val = cJSON_GetNumberValue(second);
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = (char)(int)second_val;

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}