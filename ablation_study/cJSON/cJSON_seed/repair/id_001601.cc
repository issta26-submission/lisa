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
//<ID> 1601
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *item_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag_true", item_true);
    cJSON *item_false = cJSON_CreateBool((cJSON_bool)0);
    cJSON_AddItemToObject(root, "flag_false", item_false);

    // step 2: Configure & Operate
    const char *version = cJSON_Version();
    char *serialized = cJSON_PrintUnformatted(root);
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = serialized[0];
    buffer[1] = version[0];

    // step 3: Validate
    cJSON *parsed = cJSON_Parse(serialized);
    cJSON *got_true = cJSON_GetObjectItem(parsed, "flag_true");
    cJSON *got_false = cJSON_GetObjectItem(parsed, "flag_false");
    cJSON_bool is_true = cJSON_IsTrue(got_true);
    cJSON_bool is_false = cJSON_IsFalse(got_false);
    buffer[2] = (char)('0' + (int)is_true);
    buffer[3] = (char)('0' + (int)is_false);

    // step 4: Cleanup
    cJSON_free(serialized);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}