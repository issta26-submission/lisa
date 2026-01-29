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
//<ID> 2056
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
    cJSON *added_bool_item = NULL;
    cJSON *got_item = NULL;
    cJSON_bool is_bool = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToObject(root, "explicit_true", true_item);
    cJSON_AddItemToObject(root, "explicit_false", false_item);
    added_bool_item = cJSON_AddBoolToObject(root, "auto_bool", 1);

    // step 4: Operate
    got_item = cJSON_GetObjectItemCaseSensitive(root, "auto_bool");
    is_bool = cJSON_IsBool(got_item);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(added_bool_item != NULL);
    validation ^= (int)(got_item != NULL);
    validation ^= (int)(is_bool != 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}