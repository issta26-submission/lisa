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
//<ID> 896
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
    cJSON *raw = cJSON_CreateRaw("{\"raw_key\":\"raw_val\"}");

    // step 2: Configure
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToArray(array, raw);

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(array);
    cJSON *retrieved_raw = cJSON_GetArrayItem(array, 2);
    const char *raw_str = cJSON_GetStringValue(retrieved_raw);
    double indicator_times_size = (raw_str != NULL) ? (double)size : 0.0;
    cJSON *computed = cJSON_CreateNumber(indicator_times_size);
    cJSON_AddItemToArray(array, computed);

    // step 4: Cleanup
    cJSON_Delete(array);
    // API sequence test completed successfully
    return 66;
}