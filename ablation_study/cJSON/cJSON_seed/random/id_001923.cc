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
//<ID> 1923
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *added_num = NULL;
    cJSON *got_score = NULL;
    cJSON *bool_item = NULL;
    cJSON *got_enabled = NULL;
    char *out = NULL;
    double got_val = 0.0;
    cJSON_bool has_bonus = 0;
    cJSON_bool enabled_flag = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(99.5);

    // step 3: Core operations
    added_num = cJSON_AddNumberToObject(root, "score", 77.25);
    cJSON_AddItemToObject(root, "bonus", num_item);
    bool_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", bool_item);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    got_score = cJSON_GetObjectItem(root, "score");
    got_enabled = cJSON_GetObjectItem(root, "enabled");
    got_val = cJSON_GetNumberValue(got_score);
    has_bonus = cJSON_HasObjectItem(root, "bonus");
    enabled_flag = cJSON_IsTrue(got_enabled);
    validation ^= (int)(root != NULL);
    validation ^= (int)(added_num != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(got_score != NULL);
    validation ^= (int)(got_enabled != NULL);
    validation ^= (int)(has_bonus != 0);
    validation ^= (int)(enabled_flag != 0);
    validation ^= (int)(got_val == 77.25);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}