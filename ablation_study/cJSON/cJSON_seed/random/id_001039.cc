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
//<ID> 1039
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"name\":\"example\",\"items\":[\"a\",\"bb\",\"ccc\"]}";
    cJSON *root = (cJSON *)0;
    cJSON *items = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *dup = (cJSON *)0;
    char *printed = (char *)0;
    char *name_value = (char *)0;
    int array_size = 0;
    cJSON_bool equal = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - parse JSON into a cJSON structure
    root = cJSON_Parse(json_text);

    // step 3: Configure - obtain child elements and create a duplicate for comparison
    items = cJSON_GetObjectItem(root, "items");
    name_item = cJSON_GetObjectItem(root, "name");
    dup = cJSON_Duplicate(root, (cJSON_bool)1);

    // step 4: Operate - extract string value, measure array size, produce unformatted prints and free them
    name_value = cJSON_GetStringValue(name_item);
    array_size = cJSON_GetArraySize(items);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    printed = cJSON_PrintUnformatted(dup);
    cJSON_free((void *)printed);

    // step 5: Validate - compare original and duplicate and compute a small validation score
    equal = cJSON_Compare(root, dup, (cJSON_bool)1);
    validation_score = (int)(root != (cJSON *)0) + (int)(dup != (cJSON *)0) + (int)(array_size == 3) + (int)(name_value != (char *)0) + (int)(equal);
    (void)validation_score;

    // step 6: Cleanup - free cJSON structures
    cJSON_Delete(root);
    cJSON_Delete(dup);

    // API sequence test completed successfully
    return 66;
}