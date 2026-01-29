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
//<ID> 2087
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
    cJSON *child = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *bool_item = NULL;
    char *unformatted = NULL;
    double set_number = 0.0;
    cJSON_bool is_true_bool = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    num_item = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(child, "n", num_item);
    bool_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", bool_item);

    // step 3: Operate
    set_number = cJSON_SetNumberHelper(num_item, 2.718);
    str_item = cJSON_CreateString("replaced_value");
    cJSON_ReplaceItemInObject(child, "n", str_item);
    unformatted = cJSON_PrintUnformatted(root);
    is_true_bool = cJSON_IsTrue(bool_item);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(child != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(unformatted != NULL);
    validation ^= (int)(is_true_bool != 0);
    validation ^= (int)((int)set_number == 2);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}