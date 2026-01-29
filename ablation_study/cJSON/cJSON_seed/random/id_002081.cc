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
//<ID> 2081
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
    cJSON *num = NULL;
    cJSON *true_item = NULL;
    cJSON *str_item = NULL;
    char *unformatted = NULL;
    cJSON_bool is_true = 0;
    double setnum = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num = cJSON_AddNumberToObject(root, "val", 1.0);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Configure
    setnum = cJSON_SetNumberHelper(num, 42.0);
    str_item = cJSON_CreateString("forty-two");
    cJSON_ReplaceItemInObject(root, "val", str_item);

    // step 4: Operate
    unformatted = cJSON_PrintUnformatted(root);
    is_true = cJSON_IsTrue(true_item);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(unformatted != NULL);
    validation ^= (int)(is_true != 0);
    validation ^= (int)((int)setnum == 42);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}