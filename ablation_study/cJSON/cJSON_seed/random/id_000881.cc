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
//<ID> 881
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *key_str = "greeting";
    const char *key_flag = "enabled";
    const char *key_value = "pi";
    const char *hello = "Hello, cJSON!";
    double pi_value = 3.14159;
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *number_item = (cJSON *)0;
    cJSON_bool added_str = (cJSON_bool)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize root object and create items
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString(hello);

    // step 3: Configure object by adding items
    added_str = cJSON_AddItemToObject(root, key_str, str_item);
    true_item = cJSON_AddTrueToObject(root, key_flag);
    number_item = cJSON_AddNumberToObject(root, key_value, pi_value);

    // step 4: Operate / Validate by printing and aggregating simple checks
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)added_str + (int)(true_item != (cJSON *)0) + (int)(number_item != (cJSON *)0) + (int)(printed != (char *)0);
    (void)validation_score;
    cJSON_free(printed);

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalization
    // API sequence test completed successfully
    return 66;
}