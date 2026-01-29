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
//<ID> 1063
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *entries = cJSON_AddArrayToObject(root, "entries");
    cJSON *enabled = cJSON_AddTrueToObject(root, "enabled");

    // step 2: Configure
    cJSON *flag_item = cJSON_CreateBool(0);
    cJSON_AddItemToArray(entries, flag_item);
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(entries, num_item);

    // step 3: Operate and Validate
    cJSON_bool root_is_object = cJSON_IsObject(root);
    int entries_size = cJSON_GetArraySize(entries);
    cJSON *first_item = cJSON_GetArrayItem(entries, 0);
    (void)first_item;
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = (char)('0' + (root_is_object & 1));
    ((char *)scratch)[1] = (char)('0' + (entries_size & 0xF));
    ((char *)scratch)[2] = (char)('0' + (cJSON_IsTrue(enabled) & 1));
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}