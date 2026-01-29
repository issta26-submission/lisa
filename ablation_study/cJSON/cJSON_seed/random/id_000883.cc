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
//<ID> 883
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *initial_string = "hello world";
    const char *extracted_string = (const char *)0;
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON_bool added_str = (cJSON_bool)0;
    double extracted_number = 0.0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize root object and create a string item
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString(initial_string);
    added_str = cJSON_AddItemToObject(root, "greeting", str_item);

    // step 3: Add a true value and a number to the same object
    true_item = cJSON_AddTrueToObject(root, "active");
    num_item = cJSON_AddNumberToObject(root, "count", 7.0);

    // step 4: Operate on created items (extract values for validation)
    extracted_string = cJSON_GetStringValue(str_item);
    extracted_number = cJSON_GetNumberValue(num_item);
    validation_score = (int)(added_str) + (int)(extracted_string != (const char *)0) + (int)(extracted_number == 7.0) + (int)(true_item != (cJSON *)0);

    // step 5: Validate by printing and accounting for the printed buffer
    printed = cJSON_PrintUnformatted(root);
    validation_score += (printed != (char *)0);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}