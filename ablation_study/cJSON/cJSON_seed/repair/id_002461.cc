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
//<ID> 2461
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddTrueToObject(meta, "ready");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *payload = cJSON_CreateObject();
    cJSON *f = cJSON_CreateFalse();
    cJSON_AddItemToObject(payload, "enabled", f);
    cJSON_AddItemToObject(root, "payload", payload);

    // step 3: Operate
    char *printed = cJSON_Print(root);
    cJSON_AddItemToObject(root, "dump", cJSON_CreateString(printed));
    cJSON_bool was_false = cJSON_IsFalse(f);
    char flagbuf[2];
    flagbuf[0] = (char)('0' + (int)was_false);
    flagbuf[1] = '\0';
    cJSON_AddItemToObject(root, "flag", cJSON_CreateString(flagbuf));
    char *printed2 = cJSON_Print(root);

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}