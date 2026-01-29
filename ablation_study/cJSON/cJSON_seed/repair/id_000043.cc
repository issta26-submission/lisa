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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *label = cJSON_CreateString("value");
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "label", label);
    cJSON_AddItemToObject(meta, "flag", flag);

    // step 2: Configure
    cJSON *info = cJSON_AddObjectToObject(root, "info");
    cJSON_AddStringToObject(info, "note", "generated");

    // step 3: Operate and Validate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_label = cJSON_GetObjectItem(got_meta, "label");
    const char *extracted = cJSON_GetStringValue(got_label);
    cJSON_AddStringToObject(root, "extracted", extracted);
    cJSON *got_flag = cJSON_GetObjectItem(got_meta, "flag");
    cJSON_AddNumberToObject(root, "flag_truth", (double)cJSON_IsTrue(got_flag));
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}