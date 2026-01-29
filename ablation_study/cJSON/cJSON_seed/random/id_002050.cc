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
//<ID> 2050
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    cJSON *auto_bool_item = NULL;
    cJSON *got_true = NULL;
    cJSON *got_false = NULL;
    cJSON *got_auto = NULL;
    cJSON_bool added_true_flag = 0;
    cJSON_bool added_false_flag = 0;
    cJSON_bool is_bool_true = 0;
    cJSON_bool is_bool_false = 0;
    cJSON_bool is_bool_auto = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_true_flag = cJSON_AddItemToObject(root, "createdTrue", true_item);
    added_false_flag = cJSON_AddItemToObject(root, "createdFalse", false_item);
    auto_bool_item = cJSON_AddBoolToObject(root, "addedBool", 1);

    // step 4: Operate / Inspect
    got_true = cJSON_GetObjectItemCaseSensitive(root, "createdTrue");
    got_false = cJSON_GetObjectItemCaseSensitive(root, "createdFalse");
    got_auto = cJSON_GetObjectItemCaseSensitive(root, "addedBool");
    is_bool_true = cJSON_IsBool(got_true);
    is_bool_false = cJSON_IsBool(got_false);
    is_bool_auto = cJSON_IsBool(got_auto);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(added_true_flag != 0);
    validation ^= (int)(added_false_flag != 0);
    validation ^= (int)(auto_bool_item != NULL);
    validation ^= (int)(got_true != NULL);
    validation ^= (int)(got_false != NULL);
    validation ^= (int)(got_auto != NULL);
    validation ^= (int)(is_bool_true != 0);
    validation ^= (int)(is_bool_false != 0);
    validation ^= (int)(is_bool_auto != 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}