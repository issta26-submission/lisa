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
//<ID> 983
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *root_key = "root";
    const char *child_key = "child";
    const char *text_key = "text";
    const char *child_num_key = "child_number";
    const char *root_num_key = "value";
    const char *text_val = "hello cJSON";
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *child_num_item = (cJSON *)0;
    cJSON *root_num_item = (cJSON *)0;
    cJSON *fetched_value = (cJSON *)0;
    cJSON *fetched_child = (cJSON *)0;
    cJSON *fetched_text = (cJSON *)0;
    char *serialized = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root and a nested child object with items
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    str_item = cJSON_CreateString(text_val);
    cJSON_AddItemToObject(child, text_key, str_item);
    child_num_item = cJSON_CreateNumber(7.5);
    cJSON_AddItemToObject(child, child_num_key, child_num_item);
    cJSON_AddItemToObject(root, child_key, child);
    root_num_item = cJSON_AddNumberToObject(root, root_num_key, 123.0);

    // step 3: Operate - retrieve items using case-insensitive and case-sensitive getters
    fetched_value = cJSON_GetObjectItem(root, root_num_key);
    fetched_child = cJSON_GetObjectItemCaseSensitive(root, child_key);
    fetched_text = cJSON_GetObjectItemCaseSensitive(fetched_child, text_key);

    // step 4: Serialize and free the serialized buffer with cJSON_free
    serialized = cJSON_Print(root);
    cJSON_free(serialized);

    // step 5: Validate - compute a small validation score using retrieved values
    validation_score = (int)(root != (cJSON *)0) + (int)(root_num_item != (cJSON *)0) + (int)(fetched_value != (cJSON *)0 && cJSON_GetNumberValue(fetched_value) == 123.0) + (int)(fetched_child == child) + (int)(fetched_text != (cJSON *)0);

    // step 6: Cleanup - delete the root (which frees all attached items)
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}