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
//<ID> 2082
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
    cJSON *str_item = NULL;
    cJSON *new_str = NULL;
    cJSON *num_item = NULL;
    cJSON *true_item = NULL;
    cJSON *got_flag = NULL;
    char *unformatted = NULL;
    double set_val = 0.0;
    cJSON_bool is_true = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greet", str_item);
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "count", num_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate
    set_val = cJSON_SetNumberHelper(num_item, 100.5);
    new_str = cJSON_CreateString("world");
    cJSON_ReplaceItemInObject(root, "greet", new_str);
    got_flag = cJSON_GetObjectItem(root, "flag");
    is_true = cJSON_IsTrue(got_flag);
    unformatted = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(new_str != NULL);
    validation ^= (int)(got_flag != NULL);
    validation ^= (int)(is_true != 0);
    validation ^= (int)((int)set_val == 100);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}