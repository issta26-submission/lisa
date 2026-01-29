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
//<ID> 1838
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json[] = "{ \"meta\": { \"name\": \"alpha\" }, \"arr\": [1, 2, 3] }";
    cJSON_Minify(json);
    cJSON *root = cJSON_Parse(json);
    cJSON *dup = cJSON_Duplicate(root, 1);

    // step 2: Configure
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON_AddNumberToObject(settings, "threshold", 7.5);
    cJSON *dup_arr = cJSON_GetObjectItem(dup, "arr");
    cJSON_AddItemToArray(dup_arr, cJSON_CreateNumber(4.0));
    cJSON *root_arr = cJSON_GetObjectItem(root, "arr");
    cJSON_DeleteItemFromArray(root_arr, 1);

    // step 3: Operate and Validate
    char *printed_root = cJSON_PrintUnformatted(root);
    char *printed_dup = cJSON_PrintUnformatted(dup);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    const char *name = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "meta"), "name"));
    buffer[0] = printed_root[0];
    buffer[1] = printed_dup[0];
    buffer[2] = name[0];
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(root_arr, 0));
    double dup_last = cJSON_GetNumberValue(cJSON_GetArrayItem(dup_arr, cJSON_GetArraySize(dup_arr) - 1));
    int i0 = (int)first_val;
    int i1 = (int)dup_last;
    buffer[3] = (char)('0' + (i0 % 10));
    buffer[4] = (char)('0' + (i1 % 10));
    (void)buffer; (void)first_val; (void)dup_last; (void)i0; (void)i1;

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_dup);
    cJSON_free(buffer);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}