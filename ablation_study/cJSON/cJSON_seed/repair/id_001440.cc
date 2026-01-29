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
//<ID> 1440
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON_AddTrueToObject(settings, "enabled");
    cJSON *list = cJSON_AddArrayToObject(root, "list");

    // step 2: Configure
    cJSON_AddItemToArray(list, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(list, cJSON_CreateTrue());
    cJSON_AddItemToArray(list, cJSON_CreateNumber(7.0));

    // step 3: Operate and Validate
    cJSON *second_item = cJSON_GetArrayItem(list, 1);
    cJSON_bool second_is_true = cJSON_IsTrue(second_item);
    int count = cJSON_GetArraySize(list);
    char *unformatted = cJSON_PrintUnformatted(root);
    size_t len = unformatted ? (size_t)strlen(unformatted) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(len + 32);
    memset(scratch, 0, len + 32);
    scratch[0] = len > 0 ? unformatted[0] : ' ';
    scratch[1] = (char)('0' + (count % 10));
    scratch[2] = (char)(second_is_true ? '1' : '0');

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(unformatted);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}