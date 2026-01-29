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
//<ID> 2059
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
    cJSON *implicit_bool = NULL;
    cJSON *got_true = NULL;
    cJSON *got_false = NULL;
    cJSON *got_implicit = NULL;
    cJSON_bool is_true_bool = 0;
    cJSON_bool is_false_bool = 0;
    cJSON_bool is_implicit_bool = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToObject(root, "explicit_true", true_item);
    cJSON_AddItemToObject(root, "explicit_false", false_item);
    implicit_bool = cJSON_AddBoolToObject(root, "implicit_flag", 1);

    // step 4: Operate
    got_true = cJSON_GetObjectItemCaseSensitive(root, "explicit_true");
    got_false = cJSON_GetObjectItemCaseSensitive(root, "explicit_false");
    got_implicit = cJSON_GetObjectItemCaseSensitive(root, "implicit_flag");
    is_true_bool = cJSON_IsBool(got_true);
    is_false_bool = cJSON_IsBool(got_false);
    is_implicit_bool = cJSON_IsBool(got_implicit);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(implicit_bool != NULL);
    validation ^= (int)(got_true != NULL);
    validation ^= (int)(got_false != NULL);
    validation ^= (int)(got_implicit != NULL);
    validation ^= (int)(is_true_bool != 0);
    validation ^= (int)(is_false_bool != 0);
    validation ^= (int)(is_implicit_bool != 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}