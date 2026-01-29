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
//<ID> 713
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"numbers\":[1,2,3,4],\"info\":{\"name\":\"samp\"}}";
    cJSON *root = cJSON_ParseWithLength(json, (size_t)(sizeof(json) - 1));

    // step 2: Configure
    int extra_nums[] = {100, 200};
    cJSON *ints = cJSON_CreateIntArray(extra_nums, 2);
    cJSON_AddItemToObjectCS(root, "ints", ints);

    // step 3: Operate and Validate
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *detached = cJSON_DetachItemFromArray(numbers, 1);
    cJSON_AddItemToArray(ints, detached);
    int numbers_size = cJSON_GetArraySize(numbers);
    int ints_size = cJSON_GetArraySize(ints);
    cJSON_AddNumberToObject(root, "numbers_size", (double)numbers_size);
    cJSON_AddNumberToObject(root, "ints_size", (double)ints_size);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}