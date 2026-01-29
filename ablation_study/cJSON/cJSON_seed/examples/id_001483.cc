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
//<ID> 1483
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToObject(root, "values", arr);

    // step 2: Configure
    char *printed1 = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON_bool is_array = cJSON_IsArray(arr);
    double num_val = cJSON_GetNumberValue(num);
    buffer[0] = (char)('0' + (is_array ? 1 : 0));

    // step 3: Operate & Validate
    cJSON *adjusted = cJSON_CreateNumber(num_val + 0.5);
    cJSON_AddItemToObject(root, "adjusted", adjusted);
    char *printed2 = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed1);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}