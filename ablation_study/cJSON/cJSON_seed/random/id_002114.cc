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
//<ID> 2114
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[8];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *old_item = NULL;
    cJSON *new_item = NULL;
    cJSON *got_item = NULL;
    cJSON *flag_item = NULL;
    double got_value = 0.0;
    cJSON_bool is_true = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    old_item = cJSON_AddNumberToObject(root, "a", 1.0);
    flag_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag_item);

    // step 3: Operate
    new_item = cJSON_CreateNumber(42.0);
    old_item = cJSON_GetObjectItemCaseSensitive(root, "a");
    cJSON_ReplaceItemViaPointer(root, old_item, new_item);
    got_item = cJSON_GetObjectItemCaseSensitive(root, "a");
    got_value = cJSON_GetNumberValue(got_item);
    is_true = cJSON_IsTrue(cJSON_GetObjectItemCaseSensitive(root, "flag"));

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(old_item != NULL);
    validation ^= (int)(new_item != NULL);
    validation ^= (int)(got_item != NULL);
    validation ^= (int)(got_value == 42.0);
    validation ^= (int)(is_true != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}