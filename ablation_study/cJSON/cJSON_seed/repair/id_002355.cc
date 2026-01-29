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
//<ID> 2355
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
    cJSON *num2 = cJSON_CreateNumber(42.0);
    cJSON_bool added1 = cJSON_AddItemToArray(array, num1);
    cJSON_bool added2 = cJSON_AddItemToArray(array, num2);

    // step 2: Configure
    cJSON *num3 = cJSON_CreateNumber(-1.5);
    cJSON_bool added3 = cJSON_AddItemToArray(array, num3);
    int arr_size = cJSON_GetArraySize(array);
    cJSON *item_index_1 = cJSON_GetArrayItem(array, 1);
    double extracted_value = cJSON_GetNumberValue(item_index_1);

    // step 3: Operate
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (char)(int)extracted_value;
    buffer[1] = added1 ? '1' : '0';
    buffer[2] = added2 ? '1' : '0';
    buffer[3] = added3 ? '1' : '0';
    buffer[4] = (char)(arr_size & 0xFF);

    // step 4: Validate and Cleanup
    cJSON_free(buffer);
    cJSON_Delete(array);
    // API sequence test completed successfully
    return 66;
}