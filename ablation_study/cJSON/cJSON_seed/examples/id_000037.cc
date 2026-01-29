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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);

    // step 2: Configure
    cJSON_AddItemToObject(root, "title", cJSON_CreateString("cJSON_demo"));
    cJSON_AddItemToArray(array, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(array, cJSON_CreateString("beta"));
    cJSON_AddItemToArray(array, cJSON_CreateString("gamma"));
    cJSON_AddItemToObject(root, "ok", cJSON_CreateTrue());

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateString("BETA_REPLACED");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(array, 1, replacement);
    cJSON *fetched = cJSON_GetArrayItem(array, 1);
    char *fetched_str = cJSON_GetStringValue(fetched);
    cJSON_AddStringToObject(root, "picked", fetched_str);
    cJSON_bool ok_flag = cJSON_IsTrue(cJSON_GetObjectItem(root, "ok"));
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    (void)replaced;
    (void)ok_flag;

    // API sequence test completed successfully
    return 66;
}