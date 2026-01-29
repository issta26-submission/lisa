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
//<ID> 1034
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"name\":\"alpha\",\"mirror\":\"alpha\",\"items\":[\"one\",\"two\",\"three\"]}";
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *mirror_item = (cJSON *)0;
    cJSON *items_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    char *printed_unformatted = (char *)0;
    char *name_str = (char *)0;
    char *first_elem_str = (char *)0;
    int items_count = 0;
    cJSON_bool name_equals_mirror = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - parse JSON into a root object
    root = cJSON_Parse(json_text);

    // step 3: Configure - fetch object members and produce an unformatted print
    name_item = cJSON_GetObjectItem(root, "name");
    mirror_item = cJSON_GetObjectItem(root, "mirror");
    items_arr = cJSON_GetObjectItem(root, "items");
    printed_unformatted = cJSON_PrintUnformatted(root);

    // step 4: Operate - extract string values, array size, and compare two items
    name_str = cJSON_GetStringValue(name_item);
    first_elem = cJSON_GetArrayItem(items_arr, 0);
    first_elem_str = cJSON_GetStringValue(first_elem);
    items_count = cJSON_GetArraySize(items_arr);
    name_equals_mirror = cJSON_Compare(name_item, mirror_item, (cJSON_bool)1);

    // step 5: Validate - derive a small validation score using obtained values
    validation_score = (int)(root != (cJSON *)0) + (int)(name_item != (cJSON *)0) + (int)(mirror_item != (cJSON *)0) + (int)(items_arr != (cJSON *)0) + (int)(printed_unformatted != (char *)0) + (int)(name_str != (char *)0) + (int)(first_elem_str != (char *)0) + (int)(items_count == 3) + (int)(name_equals_mirror);
    (void)validation_score;

    // step 6: Cleanup - free allocated print buffer and delete the parsed structure
    cJSON_free(printed_unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}