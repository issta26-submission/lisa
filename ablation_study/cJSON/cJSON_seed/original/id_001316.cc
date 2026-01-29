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
//<ID> 1316
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *feature_false = cJSON_AddFalseToObject(settings, "feature_x");
    cJSON *status = cJSON_AddObjectToObject(root, "status");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(status, "ok", true_item);
    cJSON_bool is_false = cJSON_IsFalse(feature_false);
    double flag_val = 1.0 - (double)is_false;
    cJSON_AddNumberToObject(status, "flag_value", flag_val);

    // step 3: Operate
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}