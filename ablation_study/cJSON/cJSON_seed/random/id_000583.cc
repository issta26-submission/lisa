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
//<ID> 583
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
    cJSON *num_a = (cJSON *)0;
    cJSON *num_b = (cJSON *)0;
    cJSON *base_item = (cJSON *)0;
    cJSON *arr_first = (cJSON *)0;
    const char *parse_end = (const char *)0;
    double base_value = 0.0;
    double first_value = 0.0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;

    // step 2: Initialize
    const char *json = "{\"arr\": [], \"base\": 5.5}";
    root = cJSON_ParseWithOpts(json, &parse_end, (cJSON_bool)1);

    // step 3: Configure
    arr = cJSON_GetObjectItem(root, "arr");
    num_a = cJSON_CreateNumber(10.25);
    num_b = cJSON_CreateNumber(20.75);
    add_ok1 = cJSON_AddItemToArray(arr, num_a);
    add_ok2 = cJSON_AddItemToArray(arr, num_b);

    // step 4: Operate
    base_item = cJSON_GetObjectItemCaseSensitive(root, "base");
    base_value = cJSON_GetNumberValue(base_item);
    arr_first = cJSON_GetArrayItem(arr, 0);
    first_value = cJSON_GetNumberValue(arr_first);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)parse_end;
    (void)base_value;
    (void)first_value;
    (void)base_item;
    (void)arr_first;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}