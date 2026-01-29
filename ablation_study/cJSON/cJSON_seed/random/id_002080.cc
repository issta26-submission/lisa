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
//<ID> 2080
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
    cJSON *repl_str = NULL;
    cJSON *true_item = NULL;
    cJSON *got_true = NULL;
    char *unformatted = NULL;
    double set_res = 0.0;
    cJSON_bool is_true_bool = 0;
    int validation = 0;

    // step 2: Setup (Initialize)
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(1.0);
    str_item = cJSON_CreateString("initial");
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "count", num_item);
    cJSON_AddItemToObject(root, "msg", str_item);
    cJSON_AddItemToObject(root, "ok", true_item);

    // step 3: Configure (modify using helper and replace)
    set_res = cJSON_SetNumberHelper(num_item, 42.0);
    repl_str = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObject(root, "msg", repl_str);

    // step 4: Operate (inspect and print)
    got_true = cJSON_GetObjectItem(root, "ok");
    is_true_bool = cJSON_IsTrue(got_true);
    unformatted = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(repl_str != NULL);
    validation ^= (int)(got_true != NULL);
    validation ^= (int)(unformatted != NULL);
    validation ^= (int)(is_true_bool != 0);
    validation ^= (int)((int)set_res == 42);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}