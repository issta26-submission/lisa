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
//<ID> 1624
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON *s2 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 2: Configure
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON *replacement = cJSON_CreateString("delta");
    cJSON_bool did_replace = cJSON_ReplaceItemInArray(arr, 1, replacement);

    // step 3: Operate and Validate
    cJSON *got = cJSON_GetArrayItem(arr, 1);
    const char *got_str = cJSON_GetStringValue(got);
    cJSON *got_count = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(got_count);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = got_str[0];
    buffer[1] = printed[0];
    buffer[2] = (char)('0' + ((int)count_val % 10));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    (void)did_replace;
    (void)count_item;

    return 66;
    // API sequence test completed successfully
}