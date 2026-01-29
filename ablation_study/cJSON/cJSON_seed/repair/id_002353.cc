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
//<ID> 2353
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
    cJSON *num1 = cJSON_CreateNumber(3.5);
    cJSON *num2 = cJSON_CreateNumber(4.25);
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);

    // step 3: Operate
    cJSON *elem0 = cJSON_GetArrayItem(array, 0);
    cJSON *elem1 = cJSON_GetArrayItem(array, 1);
    double val0 = cJSON_GetNumberValue(elem0);
    double val1 = cJSON_GetNumberValue(elem1);
    double sum = val0 + val1;
    cJSON *sumItem = cJSON_CreateNumber(sum);
    cJSON_AddItemToArray(array, sumItem);

    // step 4: Validate and Cleanup
    const int buflen = 32;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (char)(int)sum;
    buffer[1] = (char)(int)val0;
    buffer[2] = (char)(int)val1;
    cJSON_free(buffer);
    cJSON_Delete(array);
    // API sequence test completed successfully
    return 66;
}