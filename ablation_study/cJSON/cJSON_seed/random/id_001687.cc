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
//<ID> 1687
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *str_array = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    const char *strings[3];
    int array_size = 0;
    double extracted_number = 0.0;
    int validation = 0;

    // step 2: Initialize
    strings[0] = "first";
    strings[1] = "second";
    strings[2] = "third";
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("example_name");
    str_array = cJSON_CreateStringArray(strings, 3);
    num_item = cJSON_CreateNumber(42.5);

    // step 3: Configure
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "items", str_array);
    cJSON_AddItemToObject(root, "value", num_item);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 4: Operate
    extracted_number = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    array_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "items"));

    // step 5: Validate
    validation = (int)(extracted_number == 42.5) + (int)(array_size == 3) + (int)(cJSON_GetStringValue(cJSON_GetObjectItem(root, "name")) != (char *)0) + (int)cJSON_HasObjectItem(root, "enabled");
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)name_item;
    (void)str_array;
    (void)num_item;
    (void)strings;
    (void)array_size;
    (void)extracted_number;
    (void)validation;

    // API sequence test completed successfully
    return 66;
}