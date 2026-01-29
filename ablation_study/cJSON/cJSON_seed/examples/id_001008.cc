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
//<ID> 1008
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
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *str2 = cJSON_CreateString("beta");

    // step 2: Configure
    cJSON_AddItemToArray(items, str1);
    cJSON_AddItemToArray(items, str2);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddNumberToObject(root, "count", 2.0);

    // step 3: Operate & Validate
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(got_items, 0);
    char *first_val = cJSON_GetStringValue(first_item);
    cJSON *first_copy = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON_AddNumberToObject(root, "count_copy", count_val);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}