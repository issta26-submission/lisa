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
//<ID> 846
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *person = (cJSON *)0;
    cJSON *name_str = (cJSON *)0;
    cJSON *values = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved_person = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    cJSON *retrieved_values = (cJSON *)0;
    cJSON *retrieved_num_item = (cJSON *)0;
    char *printed = (char *)0;
    double extracted_number = 0.0;
    cJSON_bool added_num_to_array = 0;
    cJSON_bool added_values_to_root = 0;
    cJSON_bool added_person_to_root = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    person = cJSON_CreateObject();
    name_str = cJSON_CreateString("example");
    values = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(123.456);

    // step 3: Configure
    added_num_to_array = cJSON_AddItemToArray(values, num_item);
    added_values_to_root = cJSON_AddItemToObject(root, "values", values);
    added_person_to_root = cJSON_AddItemToObject(root, "person", person);
    cJSON_AddItemToObject(person, "name", name_str);

    // step 4: Operate
    retrieved_person = cJSON_GetObjectItem(root, "person");
    retrieved_name = cJSON_GetObjectItem(retrieved_person, "name");
    retrieved_values = cJSON_GetObjectItem(root, "values");
    retrieved_num_item = cJSON_GetArrayItem(retrieved_values, 0);
    extracted_number = cJSON_GetNumberValue(retrieved_num_item);
    printed = cJSON_Print(root);

    // step 5: Validate
    validation_score = (retrieved_person != (cJSON *)0) + (retrieved_name != (cJSON *)0) + (retrieved_values != (cJSON *)0) + (retrieved_num_item != (cJSON *)0) + (printed != (char *)0) + (int)added_num_to_array + (int)added_values_to_root + (int)added_person_to_root + (int)(extracted_number > 0.0);
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}