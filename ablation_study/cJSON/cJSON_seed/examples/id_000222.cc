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
//<ID> 222
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);

    // step 2: Configure
    cJSON *elem1 = cJSON_CreateString("alpha");
    cJSON *elem2 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(array, elem1);
    cJSON_AddItemToArray(array, elem2);

    // step 3: Operate & Validate
    cJSON_bool has_items = cJSON_HasObjectItem(root, "items");
    cJSON *has_flag = cJSON_CreateNumber((double)has_items);
    cJSON_AddItemToObject(root, "has_items", has_flag);
    int count = cJSON_GetArraySize(array);
    char *meta = (char *)cJSON_malloc(3);
    meta[0] = (char)('0' + (count % 10));
    meta[1] = '\0';
    cJSON *meta_str = cJSON_CreateString(meta);
    cJSON_AddItemToObject(root, "count_str", meta_str);
    cJSON_free(meta);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}