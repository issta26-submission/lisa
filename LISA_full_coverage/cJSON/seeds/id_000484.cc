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
//<ID> 484
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"original\",\"flag\":false}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    cJSON *orig_name = cJSON_GetObjectItem(root, "name");
    cJSON_bool is_name_string = cJSON_IsString(orig_name);
    cJSON *orig_flag = cJSON_GetObjectItem(root, "flag");

    // step 2: Configure
    char *new_name = (char *)cJSON_malloc(8);
    memset(new_name, 0, 8);
    new_name[0] = 'u'; new_name[1] = 'p'; new_name[2] = 'd'; new_name[3] = 'a';
    new_name[4] = 't'; new_name[5] = 'e'; new_name[6] = 'd';
    cJSON *new_name_ref = cJSON_CreateStringReference(new_name);
    cJSON_bool replaced_name = cJSON_ReplaceItemInObject(root, "name", new_name_ref);
    cJSON *new_bool = cJSON_CreateBool(1);
    cJSON_bool replaced_flag = cJSON_ReplaceItemInObject(root, "flag", new_bool);

    // step 3: Operate
    cJSON *curr_name = cJSON_GetObjectItem(root, "name");
    const char *curr_name_str = cJSON_GetStringValue(curr_name);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)is_name_string;
    (void)replaced_name;
    (void)replaced_flag;
    (void)curr_name_str;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    cJSON_free(new_name);
    // API sequence test completed successfully
    return 66;
}