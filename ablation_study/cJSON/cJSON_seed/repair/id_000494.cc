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
//<ID> 494
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"config\":{\"mode\":\"test\"}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *injected = cJSON_AddObjectToObject(root, "injected");

    // step 2: Configure
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(20.5));
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(30.25));

    // step 3: Operate
    cJSON_AddItemToObject(injected, "numbers", numbers);
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(40.75));
    char *serialized = cJSON_PrintUnformatted(root);
    cJSON_free(serialized);

    // step 4: Validate and Cleanup
    int size = cJSON_GetArraySize(numbers);
    cJSON *third = cJSON_GetArrayItem(numbers, 2);
    double third_value = cJSON_GetNumberValue(third);
    (void)size;
    (void)third_value;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}