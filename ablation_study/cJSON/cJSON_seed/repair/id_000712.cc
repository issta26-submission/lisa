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
//<ID> 712
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers[] = {10, 20, 30, 40, 50};
    int numbers_count = (int)(sizeof(numbers) / sizeof(numbers[0]));
    cJSON *num_array = cJSON_CreateIntArray(numbers, numbers_count);
    cJSON_AddItemToObjectCS(root, "numbers", num_array);

    // step 2: Configure
    const char config_json[] = "{\"moved_index\":2}";
    cJSON *config = cJSON_ParseWithLength(config_json, sizeof(config_json) - 1);
    cJSON *index_item = cJSON_GetObjectItem(config, "moved_index");
    double index_val = cJSON_GetNumberValue(index_item);
    int index = (int)index_val;

    // step 3: Operate and Validate
    cJSON *moved_item = cJSON_DetachItemFromArray(num_array, index);
    cJSON_AddItemToObjectCS(root, "moved", moved_item);
    double moved_number = cJSON_GetNumberValue(moved_item);
    cJSON_AddNumberToObject(root, "moved_value", moved_number);
    int remaining = cJSON_GetArraySize(num_array);
    cJSON_AddNumberToObject(root, "remaining", (double)remaining);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(config);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}