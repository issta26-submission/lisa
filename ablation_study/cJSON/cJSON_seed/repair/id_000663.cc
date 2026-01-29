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
//<ID> 663
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *manual_array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "manual_array", manual_array);
    cJSON *auto_array = cJSON_AddArrayToObject(root, "auto_array");
    cJSON_AddStringToObject(root, "title", "cjson_test");

    // step 2: Configure
    cJSON_AddItemToArray(manual_array, cJSON_CreateString("first"));
    cJSON_AddItemToArray(manual_array, cJSON_CreateString("second"));
    cJSON_AddItemToArray(auto_array, cJSON_CreateString("auto_one"));
    void *tmp = cJSON_malloc(16);
    memset(tmp, 0, 16);
    char *p = (char *)tmp;
    p[0] = 'd'; p[1] = 'y'; p[2] = 'n'; p[3] = '1'; p[4] = 0;
    cJSON_AddItemToArray(manual_array, cJSON_CreateString(p));
    cJSON_free(tmp);

    // step 3: Operate and Validate
    cJSON *title_item = cJSON_GetObjectItem(root, "title");
    cJSON_bool title_is_string = cJSON_IsString(title_item);
    cJSON_AddBoolToObject(root, "title_is_string", title_is_string);
    int count = cJSON_GetArraySize(manual_array);
    (void)count;
    cJSON *second_item = cJSON_GetArrayItem(manual_array, 1);
    const char *second_str = cJSON_GetStringValue(second_item);
    cJSON_AddStringToObject(root, "second_readback", second_str);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_AddStringToObject(root, "snapshot", out);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}