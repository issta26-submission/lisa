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
//<ID> 861
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *null_item = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddStringToObject(root, "name", "tester");
    cJSON_AddNumberToObject(root, "version", 1.23);
    cJSON_AddStringToObject(child, "child_note", "note1");
    cJSON_AddNumberToObject(child, "count", 5.0);
    cJSON_bool add_child_ok = cJSON_AddItemToObject(root, "child", child);
    cJSON_bool add_null_ok = cJSON_AddItemToObject(root, "nothing", null_item);

    // step 3: Operate and Validate
    cJSON_bool is_true = cJSON_IsTrue(true_item);
    cJSON_AddNumberToObject(root, "is_true_flag", (double)is_true);
    cJSON_AddNumberToObject(root, "child_added_ok", (double)add_child_ok);
    cJSON_AddNumberToObject(root, "null_added_ok", (double)add_null_ok);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(true_item);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}