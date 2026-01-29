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
//<ID> 1447
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddTrueToObject(root, "flag");
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON_AddItemToArray(items, cJSON_CreateTrue());
    cJSON_AddItemToArray(items, cJSON_CreateFalse());
    cJSON_AddItemToArray(items, cJSON_CreateNumber(42.0));

    // step 2: Configure
    cJSON_AddStringToObject(root, "name", "api_test");
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 3: Operate and Validate
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_bool first_is_true = cJSON_IsTrue(first_item);
    cJSON_bool flag_is_true = cJSON_IsTrue(flag_item);
    char *json_unformatted = cJSON_PrintUnformatted(root);
    size_t len = json_unformatted ? (size_t)strlen(json_unformatted) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(len + 16);
    memset(scratch, 0, len + 16);
    scratch[0] = len > 0 ? json_unformatted[0] : ' ';
    scratch[1] = (char)('0' + (int)flag_is_true);
    scratch[2] = (char)('0' + (int)first_is_true);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}