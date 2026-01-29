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
//<ID> 780
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = cJSON_Parse("{\"user\":\"alice\",\"score\":42}");
    cJSON *user_item = cJSON_GetObjectItem(parsed, "user");
    cJSON *score_item = cJSON_GetObjectItem(parsed, "score");

    // step 2: Configure
    const char *user_name = cJSON_GetStringValue(user_item);
    double score_val = cJSON_GetNumberValue(score_item);
    cJSON_AddStringToObject(root, "source_user", user_name);
    cJSON_AddNumberToObject(root, "source_score", score_val);
    cJSON *detached_user = cJSON_DetachItemFromObject(parsed, "user");
    cJSON_AddItemToObject(root, "detached_user", detached_user);

    // step 3: Operate and Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}