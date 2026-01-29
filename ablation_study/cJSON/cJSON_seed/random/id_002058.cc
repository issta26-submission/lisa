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
//<ID> 2058
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[16];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    cJSON *auto_item = NULL;
    cJSON *got_true = NULL;
    cJSON *got_false = NULL;
    cJSON *got_auto = NULL;
    cJSON_bool is_bool_true = 0;
    cJSON_bool is_bool_false = 0;
    cJSON_bool is_bool_auto = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToObject(root, "t", true_item);
    cJSON_AddItemToObject(root, "f", false_item);
    auto_item = cJSON_AddBoolToObject(root, "auto", 1);

    // step 4: Operate
    got_true = cJSON_GetObjectItemCaseSensitive(root, "t");
    got_false = cJSON_GetObjectItemCaseSensitive(root, "f");
    got_auto = cJSON_GetObjectItemCaseSensitive(root, "auto");
    is_bool_true = cJSON_IsBool(got_true);
    is_bool_false = cJSON_IsBool(got_false);
    is_bool_auto = cJSON_IsBool(got_auto);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(auto_item != NULL);
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