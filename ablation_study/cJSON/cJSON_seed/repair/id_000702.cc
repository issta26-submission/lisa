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
//<ID> 702
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *init_num = cJSON_AddNumberToObject(meta, "initial", 0.0);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_InsertItemInArray(items, 0, s1);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_InsertItemInArray(items, 1, s2);
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 2.0);

    // step 3: Operate and Validate
    cJSON *got_items = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(got_items, 0);
    cJSON_bool first_is_string = cJSON_IsString(first_item);
    char *first_str = cJSON_GetStringValue(first_item);
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(root, "count"));
    cJSON_AddNumberToObject(root, "count_plus_one", count_val + 1.0);
    (void)first_is_string;
    (void)first_str;

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}