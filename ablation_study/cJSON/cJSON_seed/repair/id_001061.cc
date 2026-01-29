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
//<ID> 1061
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *enabled = cJSON_AddTrueToObject(root, "enabled");
    (void)enabled;

    // step 2: Configure
    cJSON *flag_false = cJSON_CreateBool(0);
    cJSON_AddItemToArray(items, flag_false);
    cJSON *pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToArray(items, pi);
    cJSON *label = cJSON_CreateString("example");
    cJSON_AddItemToArray(items, label);

    // step 3: Operate and Validate
    cJSON_bool root_is_object = cJSON_IsObject(root);
    int items_count = cJSON_GetArraySize(items);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = (char)('0' + (items_count & 0xF));
    ((char *)scratch)[1] = (char)('0' + (root_is_object ? 1 : 0));
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_free(scratch);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}