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
//<ID> 1038
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"name\":\"alpha\",\"items\":[\"alpha\",\"beta\",\"gamma\"]}";
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *items_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *created_name = (cJSON *)0;
    char *printed_root = (char *)0;
    char *printed_name = (char *)0;
    char *first_str = (char *)0;
    int array_size = 0;
    cJSON_bool equal = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - parse JSON into root
    root = cJSON_Parse(json_text);

    // step 3: Configure - obtain items and create/add a matching string node
    name_item = cJSON_GetObjectItem(root, "name");
    items_arr = cJSON_GetObjectItem(root, "items");
    created_name = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(root, "created_name", created_name);

    // step 4: Operate - inspect array, get string value, compare nodes, and produce unformatted prints
    array_size = cJSON_GetArraySize(items_arr);
    first_elem = cJSON_GetArrayItem(items_arr, 0);
    first_str = cJSON_GetStringValue(first_elem);
    equal = cJSON_Compare(name_item, created_name, (cJSON_bool)1);
    printed_root = cJSON_PrintUnformatted(root);
    printed_name = cJSON_PrintUnformatted(name_item);

    // step 5: Validate - derive a small validation score exercising the used APIs
    validation_score = (int)(root != (cJSON *)0) + array_size + (int)(first_str != (char *)0) + (int)equal + (int)(printed_root != (char *)0) + (int)(printed_name != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - free printed buffers and delete root which frees attached items
    cJSON_free((void *)printed_root);
    cJSON_free((void *)printed_name);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}