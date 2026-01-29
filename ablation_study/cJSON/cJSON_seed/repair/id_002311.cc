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
//<ID> 2311
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "arr");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddItemToArray(arr, cJSON_CreateString("first"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(3.14));
    cJSON_AddItemToObject(meta, "active", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "count", cJSON_CreateNumber(2.0));

    // step 3: Operate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_arr = cJSON_GetObjectItem(root, "arr");
    cJSON *got_active = cJSON_GetObjectItem(got_meta, "active");
    cJSON *first_elem = cJSON_GetArrayItem(got_arr, 0);
    cJSON_bool active_bool = cJSON_IsTrue(got_active);
    double first_num = cJSON_IsNumber(first_elem) ? cJSON_GetNumberValue(first_elem) : 0.0;
    const char *first_str = cJSON_GetStringValue(first_elem);

    // step 4: Validate and Cleanup
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = active_bool ? '1' : '0';
    buffer[2] = (first_str && first_str[0]) ? first_str[0] : (char)('0' + (int)first_num);
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}