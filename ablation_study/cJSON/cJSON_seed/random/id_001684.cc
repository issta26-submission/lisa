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
//<ID> 1684
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *names = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    const char *names_arr[] = { "Alice", "Bob", "Eve" };
    double pi_val = 0.0;
    const char *greet_val = (const char *)0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    names = cJSON_CreateStringArray(names_arr, 3);
    greeting = cJSON_CreateString("hello");
    num_item = cJSON_CreateNumber(3.1415);

    // step 3: Configure
    cJSON_AddItemToObject(root, "names", names);
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddItemToObject(root, "pi", num_item);
    bool_item = cJSON_AddBoolToObject(root, "active", 1);

    // step 4: Operate
    pi_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "pi"));
    greet_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));

    // step 5: Validate
    validation = (int)(pi_val > 3.0) + (int)(greet_val != (char *)0) + (int)(cJSON_HasObjectItem(root, "active"));
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)names;
    (void)greeting;
    (void)num_item;
    (void)bool_item;
    (void)pi_val;
    (void)greet_val;
    // API sequence test completed successfully
    return 66;
}