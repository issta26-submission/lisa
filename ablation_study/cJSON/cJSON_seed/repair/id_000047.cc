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
//<ID> 47
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_AddObjectToObject(root, "child");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *greet_str = cJSON_CreateString("hello");

    // step 2: Configure
    cJSON_AddItemToObject(child, "flag", true_item);
    cJSON_AddItemToObject(root, "greeting", greet_str);

    // step 3: Operate and Validate
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON *got_flag = cJSON_GetObjectItem(got_child, "flag");
    cJSON_bool is_true = cJSON_IsTrue(got_flag);
    cJSON_AddNumberToObject(root, "flag_valid", (double)(int)is_true);
    cJSON *got_greet = cJSON_GetObjectItem(root, "greeting");
    const char *gval = cJSON_GetStringValue(got_greet);
    cJSON_AddStringToObject(child, "copy", gval);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}