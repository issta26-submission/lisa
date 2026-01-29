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
//<ID> 886
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *name_text = "example";
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    cJSON *number_item = (cJSON *)0;
    cJSON_bool added_name = (cJSON_bool)0;
    char *printed = (char *)0;
    cJSON *retrieved_name = (cJSON *)0;
    const char *retrieved_name_str = (const char *)0;
    cJSON *retrieved_number = (cJSON *)0;
    double number_val = 0.0;
    cJSON_bool is_active = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize root object
    root = cJSON_CreateObject();

    // step 3: Create string and add to object
    name_item = cJSON_CreateString(name_text);
    added_name = cJSON_AddItemToObject(root, "name", name_item);

    // step 4: Add boolean and number to object (using required APIs)
    flag_item = cJSON_AddTrueToObject(root, "active");
    number_item = cJSON_AddNumberToObject(root, "count", 42.0);

    // step 5: Operate and validate by retrieving values and printing
    printed = cJSON_PrintUnformatted(root);
    retrieved_name = cJSON_GetObjectItem(root, "name");
    retrieved_name_str = cJSON_GetStringValue(retrieved_name);
    retrieved_number = cJSON_GetObjectItem(root, "count");
    number_val = cJSON_GetNumberValue(retrieved_number);
    is_active = cJSON_IsTrue(cJSON_GetObjectItem(root, "active"));
    validation_score = (int)(root != (cJSON *)0) + (int)added_name + (printed != (char *)0) + (int)(retrieved_name_str != (const char *)0) + (int)(number_val == 42.0) + (int)is_active;
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}