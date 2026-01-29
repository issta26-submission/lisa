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
//<ID> 452
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *parsed = cJSON_Parse("[2.5]");
    cJSON *parsed_item = cJSON_GetArrayItem(parsed, 0);
    double base_value = cJSON_GetNumberValue(parsed_item);
    cJSON *array = cJSON_CreateArray();

    // step 2: Configure
    cJSON *num_base = cJSON_CreateNumber(base_value);
    cJSON_AddItemToArray(array, num_base);
    cJSON *num_scaled = cJSON_CreateNumber(base_value * 10.0);
    cJSON_AddItemToArray(array, num_scaled);

    // step 3: Operate & Validate
    double version_length = (double)strlen(version);
    cJSON *num_verlen = cJSON_CreateNumber(version_length);
    cJSON_AddItemToArray(array, num_verlen);
    cJSON *num_constant = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(array, num_constant);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(array);
    // API sequence test completed successfully
    return 66;
}