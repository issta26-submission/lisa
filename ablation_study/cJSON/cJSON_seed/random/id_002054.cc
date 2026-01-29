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
//<ID> 2054
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
    cJSON *explicit_false = NULL;
    cJSON *explicit_true = NULL;
    cJSON *added_flag = NULL;
    cJSON *got_explicit_false = NULL;
    cJSON *got_flag = NULL;
    cJSON_bool is_bool_false = 0;
    cJSON_bool is_bool_flag = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    explicit_false = cJSON_CreateFalse();
    explicit_true = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddItemToObject(root, "explicit_false", explicit_false);
    cJSON_AddItemToObject(root, "explicit_true", explicit_true);
    added_flag = cJSON_AddBoolToObject(root, "flag", 1);

    // step 4: Operate / Inspect
    got_explicit_false = cJSON_GetObjectItemCaseSensitive(root, "explicit_false");
    got_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    is_bool_false = cJSON_IsBool(got_explicit_false);
    is_bool_flag = cJSON_IsBool(got_flag);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(explicit_false != NULL);
    validation ^= (int)(explicit_true != NULL);
    validation ^= (int)(added_flag != NULL);
    validation ^= (int)(got_explicit_false != NULL);
    validation ^= (int)(got_flag != NULL);
    validation ^= (int)(is_bool_false != 0);
    validation ^= (int)(is_bool_flag != 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}