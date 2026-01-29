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
//<ID> 711
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_literal[] = "{\"data\":[1,2,3,4],\"meta\":{\"name\":\"test\"}}";
    size_t json_len = sizeof(json_literal) - 1;
    cJSON *root = cJSON_ParseWithLength(json_literal, json_len);

    // step 2: Configure
    int nums[] = {10, 20, 30};
    cJSON *ints = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObjectCS(root, "ints", ints);
    cJSON *data = cJSON_GetObjectItem(root, "data");
    cJSON *detached = cJSON_DetachItemFromArray(data, 1);
    cJSON_AddItemToArray(ints, detached);

    // step 3: Operate and Validate
    int data_size = cJSON_GetArraySize(data);
    int ints_size = cJSON_GetArraySize(ints);
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON_AddNumberToObject(meta, "data_size", (double)data_size);
    cJSON_AddNumberToObject(meta, "ints_size", (double)ints_size);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}