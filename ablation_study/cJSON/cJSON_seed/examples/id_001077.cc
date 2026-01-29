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
//<ID> 1077
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *s_alpha = cJSON_CreateString("alpha");
    cJSON *s_beta = cJSON_CreateString("beta");

    // step 2: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, s_alpha);
    cJSON_AddItemToArray(arr, s_beta);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddStringToObject(root, "title", "test_collection");
    cJSON_AddBoolToObject(root, "active", 1);

    // step 3: Operate & Validate
    cJSON *got_arr = cJSON_GetObjectItem(root, "items");
    cJSON *got_title = cJSON_GetObjectItem(root, "title");
    cJSON *got_active = cJSON_GetObjectItem(root, "active");
    cJSON *num2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(got_arr, num2);
    const char *title_str = cJSON_GetStringValue(got_title);
    cJSON_AddStringToObject(root, "copied_title", title_str);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}