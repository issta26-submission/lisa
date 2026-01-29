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
//<ID> 588
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *num3 = (cJSON *)0;
    cJSON *parsed_list = (cJSON *)0;
    cJSON *parsed_list_first = (cJSON *)0;
    cJSON *existing_obj = (cJSON *)0;
    cJSON *nested_item = (cJSON *)0;
    cJSON *appended_arr = (cJSON *)0;
    cJSON *appended_elem1 = (cJSON *)0;
    const char *json = "{\"existing\":{\"nested\":2.5},\"list\":[0.5]}";
    const char *parse_end = (const char *)0;
    double val_parsed_list_first = 0.0;
    double val_nested = 0.0;
    double val_appended_elem1 = 0.0;
    double total = 0.0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool add_obj_ok = 0;

    // step 2: Initialize
    root = cJSON_ParseWithOpts(json, &parse_end, (cJSON_bool)1);
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.75);
    num2 = cJSON_CreateNumber(4.25);
    num3 = cJSON_CreateNumber(5.125);

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, num1);
    add_ok2 = cJSON_AddItemToArray(arr, num2);
    add_ok3 = cJSON_AddItemToArray(arr, num3);
    add_obj_ok = cJSON_AddItemToObject(root, "appended", arr);

    // step 4: Operate
    parsed_list = cJSON_GetObjectItem(root, "list");
    parsed_list_first = cJSON_GetArrayItem(parsed_list, 0);
    val_parsed_list_first = cJSON_GetNumberValue(parsed_list_first);
    existing_obj = cJSON_GetObjectItem(root, "existing");
    nested_item = cJSON_GetObjectItemCaseSensitive(existing_obj, "nested");
    val_nested = cJSON_GetNumberValue(nested_item);
    appended_arr = cJSON_GetObjectItemCaseSensitive(root, "appended");
    appended_elem1 = cJSON_GetArrayItem(appended_arr, 1);
    val_appended_elem1 = cJSON_GetNumberValue(appended_elem1);
    total = val_parsed_list_first + val_nested + val_appended_elem1;

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)add_obj_ok;
    (void)parse_end;
    (void)val_parsed_list_first;
    (void)val_nested;
    (void)val_appended_elem1;
    (void)total;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}