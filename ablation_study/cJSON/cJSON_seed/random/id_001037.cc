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
//<ID> 1037
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"name\":\"example\",\"items\":[\"a\",\"b\",\"c\"]}";
    cJSON *root = (cJSON *)0;
    cJSON *items = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *dup_root = (cJSON *)0;
    char *unformatted = (char *)0;
    const char *name_str = (const char *)0;
    int arr_size = 0;
    cJSON_bool equal = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - parse the JSON into a root object
    root = cJSON_Parse(json_text);

    // step 3: Configure - extract object members and measure array size
    name_item = cJSON_GetObjectItem(root, "name");
    items = cJSON_GetObjectItem(root, "items");
    arr_size = cJSON_GetArraySize(items);

    // step 4: Operate - obtain string value, produce unformatted print and duplicate the root
    name_str = cJSON_GetStringValue(name_item);
    unformatted = cJSON_PrintUnformatted(root);
    dup_root = cJSON_Duplicate(root, (cJSON_bool)1);

    // step 5: Validate - compare original and duplicate and derive a small validation score
    equal = cJSON_Compare(root, dup_root, (cJSON_bool)1);
    validation_score = (int)(name_str != (const char *)0) + arr_size + (int)equal;
    (void)validation_score;

    // step 6: Cleanup - free allocated strings and delete cJSON structures
    cJSON_free((void *)unformatted);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    return 66;
}
// API sequence test completed successfully