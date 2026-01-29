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
//<ID> 1035
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"name\":\"alice\",\"values\":[10,20,30]}";
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *values_arr = (cJSON *)0;
    cJSON *dup_root = (cJSON *)0;
    cJSON *dup_values = (cJSON *)0;
    char *name_printed = (char *)0;
    char *name_str = (char *)0;
    int arr_size = 0;
    cJSON_bool arrays_equal = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - parse JSON and fetch members
    root = cJSON_Parse(json_text);
    name_item = cJSON_GetObjectItem(root, "name");
    values_arr = cJSON_GetObjectItem(root, "values");

    // step 3: Configure - duplicate the parsed structure and fetch duplicate's array
    dup_root = cJSON_Duplicate(root, (cJSON_bool)1);
    dup_values = cJSON_GetObjectItem(dup_root, "values");

    // step 4: Operate - obtain string value, print it unformatted, measure array size, compare arrays
    name_str = cJSON_GetStringValue(name_item);
    name_printed = cJSON_PrintUnformatted(name_item);
    cJSON_free((void *)name_printed);
    arr_size = cJSON_GetArraySize(values_arr);
    arrays_equal = cJSON_Compare(values_arr, dup_values, (cJSON_bool)1);

    // step 5: Validate - compute a small validation score exercising used APIs
    validation_score = (int)(root != (cJSON *)0) + (int)(name_item != (cJSON *)0) + (int)(values_arr != (cJSON *)0) + (int)(dup_root != (cJSON *)0) + (int)(dup_values != (cJSON *)0) + (int)(arr_size == 3) + (int)(name_str != (char *)0 && name_str[0] == 'a') + (int)arrays_equal;
    (void)validation_score;

    // step 6: Cleanup - delete allocated cJSON structures
    cJSON_Delete(root);
    cJSON_Delete(dup_root);

    // API sequence test completed successfully
    return 66;
}