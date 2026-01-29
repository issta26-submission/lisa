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
//<ID> 710
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"numbers\":[1,2,3,4,5],\"meta\":{\"name\":\"example\"}}";
    cJSON *root = cJSON_ParseWithLength(json, (size_t)(sizeof(json) - 1));

    // step 2: Configure
    int add_nums[] = {100, 200, 300};
    cJSON *int_arr = cJSON_CreateIntArray(add_nums, 3);
    cJSON_AddItemToObjectCS(root, "extra_numbers", int_arr);

    // step 3: Operate and Validate
    cJSON *numbers = cJSON_GetObjectItemCaseSensitive(root, "numbers");
    cJSON *detached_item = cJSON_DetachItemFromArray(numbers, 2);
    int new_size = cJSON_GetArraySize(numbers);
    cJSON_AddNumberToObject(root, "numbers_length_after_detach", (double)new_size);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(detached_item);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}