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
//<ID> 2358
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *array = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(2.718);
    cJSON *num3 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToArray(array, num3);

    // step 2: Configure
    cJSON *subarr = cJSON_CreateArray();
    cJSON *subnum = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(subarr, subnum);
    cJSON_AddItemToArray(array, subarr);

    // step 3: Operate
    cJSON *fetched0 = cJSON_GetArrayItem(array, 0);
    double val0 = cJSON_GetNumberValue(fetched0);
    cJSON *fetched2 = cJSON_GetArrayItem(array, 2);
    double val2 = cJSON_GetNumberValue(fetched2);
    cJSON *fetched_sub0 = cJSON_GetArrayItem(subarr, 0);
    double val_sub0 = cJSON_GetNumberValue(fetched_sub0);

    // step 4: Validate and Cleanup
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (char)(int)val0;
    buffer[1] = (char)(int)val2;
    buffer[2] = (char)(int)val_sub0;
    cJSON_free(buffer);
    cJSON_Delete(array);
    // API sequence test completed successfully
    return 66;
}