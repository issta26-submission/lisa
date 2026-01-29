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
//<ID> 1031
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"name\":\"alpha\",\"dup\":\"alpha\",\"items\":[\"one\",\"two\",\"three\"]}";
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *dup_item = (cJSON *)0;
    cJSON *items = (cJSON *)0;
    char *printed = (char *)0;
    char *name_str = (char *)0;
    char *dup_str = (char *)0;
    int arr_size = 0;
    cJSON_bool cmp_eq = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - parse JSON into root
    root = cJSON_Parse(json_text);

    // step 3: Configure - retrieve object members and the array
    name_item = cJSON_GetObjectItem(root, "name");
    dup_item = cJSON_GetObjectItem(root, "dup");
    items = cJSON_GetObjectItem(root, "items");

    // step 4: Operate - print the array unformatted, extract string values, get array size, compare items
    printed = cJSON_PrintUnformatted(items);
    name_str = cJSON_GetStringValue(name_item);
    dup_str = cJSON_GetStringValue(dup_item);
    arr_size = cJSON_GetArraySize(items);
    cmp_eq = cJSON_Compare(name_item, dup_item, (cJSON_bool)1);

    // step 5: Validate - compute a simple validation score using results from APIs
    validation_score = (int)(root != (cJSON *)0) + (int)(name_item != (cJSON *)0) + (int)(dup_item != (cJSON *)0) + (int)(items != (cJSON *)0) + (int)(name_str != (char *)0) + (int)(dup_str != (char *)0) + (int)(arr_size == 3) + (int)(cmp_eq);
    (void)validation_score;

    // step 6: Cleanup - free printed string and delete parsed structure
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}