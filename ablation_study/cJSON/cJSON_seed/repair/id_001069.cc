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
//<ID> 1069
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_AddArrayToObject(root, "list");
    cJSON *flag_true = cJSON_AddTrueToObject(root, "enabled");
    cJSON *bool_item = cJSON_CreateBool(0);
    cJSON_AddItemToArray(list, bool_item);

    // step 2: Configure
    cJSON *subobj = cJSON_CreateObject();
    cJSON_AddItemToArray(list, subobj);
    cJSON_AddStringToObject(root, "title", "cJSON Example");

    // step 3: Operate and Validate
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON *got_list = cJSON_GetObjectItem(root, "list");
    int list_size = cJSON_GetArraySize(got_list);
    cJSON *first_item = cJSON_GetArrayItem(got_list, 0);
    cJSON_bool first_is_obj = cJSON_IsObject(first_item);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = (char)('0' + (root_is_obj ? 1 : 0));
    ((char *)scratch)[1] = (char)('0' + (first_is_obj ? 1 : 0));
    ((char *)scratch)[2] = (char)('0' + (list_size & 0xF));
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}