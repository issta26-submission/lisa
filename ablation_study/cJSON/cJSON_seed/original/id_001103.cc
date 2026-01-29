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
//<ID> 1103
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const double numbers[] = {1.5, 2.5, 3.5};
    cJSON *root = cJSON_CreateObject();
    cJSON *num_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "numbers", num_array);
    cJSON *name_item = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *raw_item = cJSON_CreateRaw("raw_payload");
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "enabled");
    char *set_ret = cJSON_SetValuestring(name_item, "updated_name");

    // step 3: Operate
    cJSON_bool raw_check = cJSON_IsRaw(raw_item);
    cJSON *second_number = cJSON_GetArrayItem(num_array, 1);
    cJSON_bool second_is_number = cJSON_IsNumber(second_number);
    char *unformatted = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 1);
    cJSON_free(unformatted);
    cJSON_free(prealloc);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}