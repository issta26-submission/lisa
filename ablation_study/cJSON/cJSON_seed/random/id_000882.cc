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
//<ID> 882
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *key_str = "greeting";
    const char *text = "hello world";
    const char *num_key = "value";
    const char *bool_key = "flag";
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON_bool added_str = (cJSON_bool)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    char *printed = (char *)0;
    cJSON *retrieved_str = (cJSON *)0;
    const char *retrieved_text = (const char *)0;
    cJSON *retrieved_num = (cJSON *)0;
    double retrieved_number = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();

    // step 3: Configure (create string and add items using required APIs)
    str_item = cJSON_CreateString(text);
    added_str = cJSON_AddItemToObject(root, key_str, str_item);
    true_item = cJSON_AddTrueToObject(root, bool_key);
    num_item = cJSON_AddNumberToObject(root, num_key, 3.14);

    // step 4: Operate (print and retrieve values)
    printed = cJSON_PrintUnformatted(root);
    retrieved_str = cJSON_GetObjectItem(root, key_str);
    retrieved_text = cJSON_GetStringValue(retrieved_str);
    retrieved_num = cJSON_GetObjectItem(root, num_key);
    retrieved_number = cJSON_GetNumberValue(retrieved_num);

    // step 5: Validate and Cleanup
    validation_score = (int)(root != (cJSON *)0) + (int)added_str + (printed != (char *)0) + (retrieved_text != (const char *)0) + (retrieved_num != (cJSON *)0);
    (void)validation_score;
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}