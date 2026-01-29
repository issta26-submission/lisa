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
//<ID> 2111
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
    cJSON *orig_item = NULL;
    cJSON *replacement = NULL;
    cJSON *got_a = NULL;
    cJSON *flag_item = NULL;
    cJSON *got_flag = NULL;
    cJSON_bool replace_ok = 0;
    cJSON_bool flag_is_true = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "a", 1.0);
    cJSON_AddNumberToObject(root, "b", 2.0);
    flag_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag_item);

    // step 3: Operate
    orig_item = cJSON_GetObjectItemCaseSensitive(root, "a");
    replacement = cJSON_CreateObject();
    cJSON_AddNumberToObject(replacement, "nested", 42.0);
    replace_ok = cJSON_ReplaceItemViaPointer(root, orig_item, replacement);
    got_a = cJSON_GetObjectItemCaseSensitive(root, "a");
    got_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    flag_is_true = cJSON_IsTrue(got_flag);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(orig_item != NULL);
    validation ^= (int)(replacement != NULL);
    validation ^= (int)(replace_ok != 0);
    validation ^= (int)(got_a != NULL);
    validation ^= (int)(got_flag != NULL);
    validation ^= (int)(flag_is_true != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}