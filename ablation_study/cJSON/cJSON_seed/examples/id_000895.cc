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
//<ID> 895
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *array = cJSON_CreateArray();
    cJSON *num_a = cJSON_CreateNumber(42.0);
    cJSON *num_b = cJSON_CreateNumber(3.14);
    cJSON *raw = cJSON_CreateRaw("{\"device\":\"sensor\",\"v\":1}");

    // step 2: Configure
    cJSON_AddItemToArray(array, num_a);
    cJSON_AddItemToArray(array, num_b);
    cJSON_AddItemToArray(array, raw);

    // step 3: Operate & Validate
    int size_before = cJSON_GetArraySize(array);
    cJSON *third_item = cJSON_GetArrayItem(array, 2);
    char *raw_text_print = cJSON_Print(third_item);
    double derived_value = (double)size_before + (double)(unsigned char)raw_text_print[0];
    cJSON_free(raw_text_print);
    cJSON *derived_num = cJSON_CreateNumber(derived_value);
    cJSON_AddItemToArray(array, derived_num);
    int size_after = cJSON_GetArraySize(array);
    cJSON *size_marker = cJSON_CreateNumber((double)size_after);
    cJSON_AddItemToArray(array, size_marker);

    // step 4: Cleanup
    cJSON_Delete(array);
    return 66;
}