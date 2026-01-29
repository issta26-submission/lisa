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
//<ID> 893
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *array = cJSON_CreateArray();
    cJSON *raw = cJSON_CreateRaw("{\"inner\":123}");
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *n2 = cJSON_CreateNumber(7.5);
    cJSON *str = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_AddItemToArray(array, raw);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON_AddItemToArray(array, str);

    // step 3: Operate & Validate
    int size_before = cJSON_GetArraySize(array);
    const char *sval = cJSON_GetStringValue(str);
    cJSON *str_ref = cJSON_CreateString(sval);
    cJSON_AddItemToArray(array, str_ref);
    double v1 = cJSON_GetNumberValue(n1);
    int size_after = cJSON_GetArraySize(array);
    cJSON *sum = cJSON_CreateNumber(v1 + (double)size_after);
    cJSON_AddItemToArray(array, sum);

    // step 4: Cleanup
    cJSON_Delete(array);
    // API sequence test completed successfully
    return 66;
}