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
//<ID> 1062
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "published");
    cJSON *false_node = cJSON_CreateBool(0);
    cJSON_AddItemToArray(items, false_node);
    cJSON *num_node = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, num_node);
    cJSON *name_node = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "name", name_node);

    // step 3: Operate and Validate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON_bool meta_is_obj = cJSON_IsObject(got_meta);
    int items_count = cJSON_GetArraySize(items);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    char *flat = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = (char)('0' + (meta_is_obj & 1));
    ((char *)scratch)[1] = (char)('0' + (items_count & 0xF));
    ((char *)scratch)[2] = (char)('0' + (((int)first_value) & 0xF));
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}