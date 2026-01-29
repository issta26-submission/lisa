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
//<ID> 1446
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
    cJSON_AddStringToObject(meta, "id", "test-flag-array");
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON_AddItemToArray(flags, cJSON_CreateTrue());
    cJSON_AddItemToArray(flags, cJSON_CreateFalse());
    cJSON_AddItemToArray(flags, cJSON_CreateTrue());

    // step 2: Configure
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON_AddStringToObject(root, "status", "ok");

    // step 3: Operate and Validate
    cJSON *third_flag = cJSON_GetArrayItem(flags, 2);
    cJSON_bool third_is_true = cJSON_IsTrue(third_flag);
    char *unfmt = cJSON_PrintUnformatted(root);
    size_t len = unfmt ? (size_t)strlen(unfmt) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(len + 16);
    memset(scratch, 0, len + 16);
    scratch[0] = len > 0 ? unfmt[0] : ' ';
    scratch[1] = (char)('0' + (third_is_true != 0));
    scratch[2] = (char)('0' + (cJSON_GetArraySize(flags) % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(unfmt);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}