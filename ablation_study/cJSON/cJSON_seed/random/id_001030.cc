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
//<ID> 1030
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"name\":\"alice\",\"items\":[\"a\",\"b\",\"c\"]}";
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *items = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *dup_first = (cJSON *)0;
    char *printed = (char *)0;
    char *name_str = (char *)0;
    int array_size = 0;
    cJSON_bool cmp = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - parse JSON into a cJSON structure
    root = cJSON_Parse(json_text);

    // step 3: Configure - obtain object members and duplicate a child item
    name_item = cJSON_GetObjectItem(root, "name");
    items = cJSON_GetObjectItem(root, "items");
    first_item = cJSON_GetArrayItem(items, 0);
    dup_first = cJSON_Duplicate(first_item, (cJSON_bool)1);

    // step 4: Operate - extract string, measure array size, compare duplicate, and produce unformatted print
    name_str = cJSON_GetStringValue(name_item);
    array_size = cJSON_GetArraySize(items);
    cmp = cJSON_Compare(dup_first, first_item, (cJSON_bool)1);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate - derive a small validation score exercising the used APIs
    validation_score = (int)(name_str != (char *)0) + (int)(array_size == 3) + (int)(cmp) + (int)(printed != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - free allocated print buffer and delete cJSON structures
    cJSON_free((void *)printed);
    cJSON_Delete(dup_first);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}