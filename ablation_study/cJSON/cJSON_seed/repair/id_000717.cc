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
//<ID> 717
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"numbers\":[10,20,30,40],\"meta\":{\"name\":\"alpha\"}}";
    size_t buffer_length = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, buffer_length);

    // step 2: Configure
    cJSON *numbers = cJSON_GetObjectItemCaseSensitive(root, "numbers");
    int preset[] = {100, 200, 300};
    cJSON *int_arr = cJSON_CreateIntArray(preset, 3);
    cJSON_AddItemToObjectCS(root, "int_array_cs", int_arr);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(numbers, 2);
    cJSON_AddItemToArray(int_arr, detached);
    int left_in_numbers = cJSON_GetArraySize(numbers);
    int size_of_int_arr = cJSON_GetArraySize(int_arr);
    cJSON_AddNumberToObject(root, "numbers_left", (double)left_in_numbers);
    cJSON_AddNumberToObject(root, "int_arr_size", (double)size_of_int_arr);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}