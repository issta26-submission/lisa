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
//<ID> 1100
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
    cJSON *dbl_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "doubles", dbl_array);
    cJSON *raw_item = cJSON_CreateRaw("some_raw_payload");
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_AddNumberToObject(root, "value", 42.0);
    cJSON_AddStringToObject(root, "label", "initial_label");
    cJSON_AddTrueToObject(root, "enabled");

    // step 2: Configure
    cJSON *label_item = cJSON_GetObjectItem(root, "label");
    char *new_label = cJSON_SetValuestring(label_item, "updated_label");

    // step 3: Operate
    cJSON *got_raw = cJSON_GetObjectItem(root, "raw");
    cJSON *got_value = cJSON_GetObjectItem(root, "value");
    cJSON *got_array = cJSON_GetObjectItem(root, "doubles");
    cJSON_bool raw_is_raw = cJSON_IsRaw(got_raw);
    cJSON_bool value_is_number = cJSON_IsNumber(got_value);
    int array_size = cJSON_GetArraySize(got_array);
    cJSON *first_elem = cJSON_GetArrayItem(got_array, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    double derived = (double)array_size + (double)raw_is_raw + (double)value_is_number + first_val + (new_label ? 0.0 : 0.0);

    // step 4: Validate & Cleanup
    char *unformatted = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 1);
    cJSON_free(unformatted);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}