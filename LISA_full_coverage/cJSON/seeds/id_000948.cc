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
//<ID> 948
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"values\":[1.0,2.0]}";
    cJSON *root = cJSON_Parse(initial_json);

    // step 2: Configure
    cJSON *values_array = cJSON_GetObjectItem(root, "values");
    cJSON *new_number = cJSON_CreateNumber(42.5);
    cJSON_InsertItemInArray(values_array, 1, new_number);

    // step 3: Operate
    cJSON *inserted_item = cJSON_GetArrayItem(values_array, 1);
    double inserted_value = cJSON_GetNumberValue(inserted_item);
    cJSON_AddNumberToObject(root, "inserted", inserted_value);
    cJSON_AddNumberToObject(root, "inserted_plus_half", inserted_value + 0.5);

    // step 4: Validate & Cleanup
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON_PrintPreallocated(root, buffer, 512, 1);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}