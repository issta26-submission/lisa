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
//<ID> 2351
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *array = cJSON_CreateArray();

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);

    // step 3: Operate
    const cJSON *item0 = cJSON_GetArrayItem(array, 0);
    const cJSON *item1 = cJSON_GetArrayItem(array, 1);
    double value0 = cJSON_GetNumberValue(item0);
    double value1 = cJSON_GetNumberValue(item1);
    char *printed = cJSON_PrintUnformatted(array);

    // step 4: Validate and Cleanup
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (printed && printed[0]) ? printed[0] : '\0';
    buffer[1] = (char)(int)value0;
    buffer[2] = (char)(int)value1;
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(array);
    // API sequence test completed successfully
    return 66;
}