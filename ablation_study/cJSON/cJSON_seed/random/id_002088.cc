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
//<ID> 2088
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
    cJSON *num_item = NULL;
    cJSON *true_item = NULL;
    cJSON *new_str = NULL;
    cJSON_bool replace_ok = 0;
    cJSON_bool is_true = 0;
    char *unformatted = NULL;
    double set_ret = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "num", 1.0);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate
    set_ret = cJSON_SetNumberHelper(num_item, 42.42);
    new_str = cJSON_CreateString("forty-two");
    replace_ok = cJSON_ReplaceItemInObject(root, "num", new_str);
    unformatted = cJSON_PrintUnformatted(root);
    is_true = cJSON_IsTrue(true_item);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(new_str != NULL);
    validation ^= (int)(unformatted != NULL);
    validation ^= (int)((int)set_ret == 42);
    validation ^= (int)(replace_ok != 0);
    validation ^= (int)(is_true != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}