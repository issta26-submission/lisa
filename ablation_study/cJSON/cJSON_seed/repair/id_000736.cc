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
//<ID> 736
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON *score = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(metrics, "score", score);

    // step 2: Configure
    cJSON *score_backup = cJSON_Duplicate(score, 0);
    cJSON *new_score = cJSON_CreateNumber(99.9);
    cJSON *score_ptr = cJSON_GetObjectItem(metrics, "score");
    cJSON_ReplaceItemViaPointer(metrics, score_ptr, new_score);

    // step 3: Operate and Validate
    cJSON *snapshot = cJSON_Duplicate(root, 1);
    cJSON *snap_metrics = cJSON_GetObjectItem(snapshot, "metrics");
    cJSON *snap_score = cJSON_GetObjectItem(snap_metrics, "score");
    char *printed = cJSON_PrintUnformatted(snapshot);
    cJSON_free(printed);
    cJSON_Delete(snapshot);

    // step 4: Cleanup
    cJSON_Delete(score_backup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}