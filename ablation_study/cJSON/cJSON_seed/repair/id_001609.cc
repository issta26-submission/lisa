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
//<ID> 1609
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version_str = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *false_item = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "trueVal", true_item);
    cJSON_AddItemToObject(root, "falseVal", false_item);

    // step 2: Configure
    cJSON_AddStringToObject(root, "version", version_str);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "id", 7.0);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON *got_true = cJSON_GetObjectItemCaseSensitive(root, "trueVal");
    cJSON_bool is_true = cJSON_IsTrue(got_true);
    cJSON *got_false = cJSON_GetObjectItem(root, "falseVal");
    cJSON_bool is_false = cJSON_IsFalse(got_false);
    cJSON *ver_item = cJSON_GetObjectItem(root, "version");
    char *ver_val = cJSON_GetStringValue(ver_item);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = ver_val[0];
    buffer[1] = (char)('0' + (int)is_true);
    buffer[2] = (char)('0' + (int)is_false);
    buffer[3] = unformatted[0];

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}