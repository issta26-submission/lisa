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
//<ID> 2085
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
    cJSON *orig_str = NULL;
    cJSON *new_str = NULL;
    cJSON *num_item = NULL;
    cJSON *true_item = NULL;
    char *unformatted = NULL;
    cJSON_bool is_true = 0;
    double got_num = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    orig_str = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", orig_str);
    num_item = cJSON_AddNumberToObject(root, "count", 1.0);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Configure
    new_str = cJSON_CreateString("world");
    cJSON_ReplaceItemInObject(root, "greeting", new_str);
    got_num = cJSON_SetNumberHelper(num_item, 42.0);
    unformatted = cJSON_PrintUnformatted(root);

    // step 4: Operate
    is_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "flag"));
    validation ^= (int)(root != NULL);
    validation ^= (int)(orig_str != NULL);
    validation ^= (int)(new_str != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(unformatted != NULL);
    validation ^= (int)(is_true != 0);
    validation ^= (int)((int)got_num == 42);

    // step 5: Validate
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}