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
//<ID> 2350
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
    cJSON *n1 = cJSON_CreateNumber(3.1415);
    cJSON *n2 = cJSON_CreateNumber(2.7180);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);

    // step 3: Operate
    cJSON *item0 = cJSON_GetArrayItem(array, 0);
    cJSON *item1 = cJSON_GetArrayItem(array, 1);
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(item1);
    double sum = v0 + v1;
    char *printed = cJSON_PrintUnformatted(array);

    // step 4: Validate and Cleanup
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (printed && printed[0]) ? printed[0] : '\0';
    buffer[1] = (char)(int)sum;
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(array);
    // API sequence test completed successfully
    return 66;
}