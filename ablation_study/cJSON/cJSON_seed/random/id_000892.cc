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
//<ID> 892
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *name_count = "count";
    const char *name_items = "items";
    double count_value = 3.0;
    cJSON *root = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON *items_array = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *retrieved_items = (cJSON *)0;
    cJSON *retrieved_count = (cJSON *)0;
    cJSON_bool added_count = (cJSON_bool)0;
    cJSON_bool added_items = (cJSON_bool)0;
    cJSON_bool is_array = (cJSON_bool)0;
    double got_count = 0.0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    count_item = cJSON_CreateNumber(count_value);
    items_array = cJSON_CreateArray();
    item0 = cJSON_CreateNumber(1.0);
    item1 = cJSON_CreateNumber(2.0);

    // step 3: Configure
    added_count = cJSON_AddItemToObject(root, name_count, count_item);
    added_items = cJSON_AddItemToObject(root, name_items, items_array);
    cJSON_AddItemToArray(items_array, item0);
    cJSON_AddItemToArray(items_array, item1);

    // step 4: Operate
    retrieved_items = cJSON_GetObjectItem(root, name_items);
    is_array = cJSON_IsArray(retrieved_items);
    retrieved_count = cJSON_GetObjectItem(root, name_count);
    got_count = cJSON_GetNumberValue(retrieved_count);
    printed = cJSON_Print(root);

    // step 5: Validate
    validation_score = (int)added_count + (int)added_items + (int)is_array + (int)(printed != (char *)0) + (int)(got_count == count_value);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}