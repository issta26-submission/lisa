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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *raw_item = cJSON_CreateRaw("embedded raw content");
    cJSON *duplicate = NULL;
    char *json_unformatted = NULL;
    const cJSON *fetched_true = NULL;
    const cJSON *fetched_raw = NULL;
    const cJSON *fetched_null = NULL;
    int validation_sum = 0;

    // step 2: Configure
    cJSON_AddNullToObject(root, "nothing_here");
    cJSON_AddItemToObject(root, "is_true", true_item);
    cJSON_AddItemToObject(root, "raw_blob", raw_item);

    // step 3: Operate / Validate
    json_unformatted = cJSON_PrintUnformatted(root);
    fetched_true = cJSON_GetObjectItem(root, "is_true");
    fetched_raw = cJSON_GetObjectItem(root, "raw_blob");
    fetched_null = cJSON_GetObjectItem(root, "nothing_here");
    validation_sum = 0 + cJSON_IsTrue(fetched_true) + cJSON_IsRaw(fetched_raw) + cJSON_IsNull(fetched_null);

    // step 4: Cleanup
    duplicate = cJSON_Duplicate(root, 1);
    cJSON_free(json_unformatted);
    cJSON_Delete(duplicate);
    cJSON_Delete(root);

    (void)validation_sum;
    // API sequence test completed successfully
    return 66;
}