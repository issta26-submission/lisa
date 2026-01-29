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
//<ID> 2089
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
    cJSON *str_item = NULL;
    cJSON *repl_item = NULL;
    cJSON *num_item = NULL;
    cJSON *true_item = NULL;
    char *unformatted = NULL;
    double setnum_result = 0.0;
    cJSON_bool is_true = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "greeting", str_item);
    num_item = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "answer", num_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate
    repl_item = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObject(root, "greeting", repl_item);
    setnum_result = cJSON_SetNumberHelper(num_item, 42.5);
    is_true = cJSON_IsTrue(true_item);
    unformatted = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(repl_item != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(unformatted != NULL);
    validation ^= (int)(setnum_result == 42.5);
    validation ^= (int)(is_true != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}