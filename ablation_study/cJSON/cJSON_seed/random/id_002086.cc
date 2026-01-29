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
//<ID> 2086
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
    cJSON *str_item = NULL;
    cJSON *true_item = NULL;
    char *unformatted = NULL;
    double set_result = 0.0;
    cJSON_bool is_true = 0;
    cJSON_bool replaced_ok = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(root, "num", num_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Configure
    set_result = cJSON_SetNumberHelper(num_item, 42.0);
    str_item = cJSON_CreateString("now a string");

    // step 4: Operate
    replaced_ok = cJSON_ReplaceItemInObject(root, "num", str_item);
    is_true = cJSON_IsTrue(true_item);
    unformatted = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(unformatted != NULL);
    validation ^= (int)(replaced_ok != 0);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(((int)set_result) == 42);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}