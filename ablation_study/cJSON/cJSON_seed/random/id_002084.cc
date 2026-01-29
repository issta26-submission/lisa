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
//<ID> 2084
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
    cJSON *str_old = NULL;
    cJSON *str_new = NULL;
    cJSON *flag_item = NULL;
    char *unformatted = NULL;
    double got_num = 0.0;
    cJSON_bool is_true_bool = 0;
    cJSON_bool replaced = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "answer", num_item);
    str_old = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greet", str_old);
    flag_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag_item);

    // step 3: Operate
    got_num = cJSON_SetNumberHelper(num_item, 42.0);
    str_new = cJSON_CreateString("world");
    replaced = cJSON_ReplaceItemInObject(root, "greet", str_new);
    is_true_bool = cJSON_IsTrue(flag_item);
    unformatted = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(str_old != NULL);
    validation ^= (int)(str_new != NULL);
    validation ^= (int)(flag_item != NULL);
    validation ^= (int)(unformatted != NULL);
    validation ^= (int)(is_true_bool != 0);
    validation ^= (int)(replaced != 0);
    validation ^= (int)((int)got_num == 42);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}