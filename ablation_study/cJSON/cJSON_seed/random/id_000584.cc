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
//<ID> 584
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *values_arr = (cJSON *)0;
    cJSON *num3 = (cJSON *)0;
    cJSON *num4 = (cJSON *)0;
    cJSON *nested = (cJSON *)0;
    cJSON *nested_a = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    const char *endptr = (const char *)0;
    char *printed = (char *)0;
    double val_second = 0.0;
    double val_nested = 0.0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;

    // step 2: Initialize
    const char *json = "{\"values\":[1.0,2.0],\"nested\":{\"a\":42.5}}";
    root = cJSON_ParseWithOpts(json, &endptr, 1);

    // step 3: Configure
    values_arr = cJSON_GetObjectItem(root, "values");
    num3 = cJSON_CreateNumber(3.5);
    add_ok1 = cJSON_AddItemToArray(values_arr, num3);
    num4 = cJSON_CreateNumber(4.5);
    add_ok2 = cJSON_AddItemToArray(values_arr, num4);
    nested = cJSON_GetObjectItemCaseSensitive(root, "nested");
    nested_a = cJSON_GetObjectItemCaseSensitive(nested, "a");

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    val_second = cJSON_GetNumberValue(cJSON_GetArrayItem(retrieved_arr, 1));
    val_nested = cJSON_GetNumberValue(nested_a);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)endptr;
    (void)val_second;
    (void)val_nested;
    (void)printed;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}