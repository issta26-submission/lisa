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
//<ID> 1445
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON *t0 = cJSON_CreateTrue();
    cJSON *f1 = cJSON_CreateFalse();
    cJSON *t2 = cJSON_CreateTrue();
    cJSON_AddItemToArray(flags, t0);
    cJSON_AddItemToArray(flags, f1);
    cJSON_AddItemToArray(flags, t2);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "id", 42.0);
    cJSON_AddStringToObject(root, "name", "cjson-seq-test");

    // step 3: Operate and Validate
    cJSON *first_item = cJSON_GetArrayItem(flags, 0);
    cJSON *third_item = cJSON_GetArrayItem(flags, 2);
    cJSON_bool first_is_true = cJSON_IsTrue(first_item);
    cJSON_bool third_is_true = cJSON_IsTrue(third_item);
    char *json = cJSON_PrintUnformatted(root);
    size_t json_len = json ? (size_t)strlen(json) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(json_len + 16);
    memset(scratch, 0, json_len + 16);
    scratch[0] = json_len > 0 ? json[0] : ' ';
    scratch[1] = (char)(first_is_true ? '1' : '0');
    scratch[2] = (char)(third_is_true ? '1' : '0');

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(json);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}