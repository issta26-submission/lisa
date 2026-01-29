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
//<ID> 947
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *num_key = "number";
    const char *cs_key = "labelCS";
    const char *label_val = "example_label";
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *retrieved_num_item = (cJSON *)0;
    cJSON *retrieved_str_item = (cJSON *)0;
    double extracted_number = 0.0;
    const char *extracted_string = (const char *)0;
    int validation_score = 0;

    // step 2: Setup - create root and items
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.456);
    str_item = cJSON_CreateString(label_val);

    // step 3: Configure - add items using both AddItemToObject and AddItemToObjectCS
    cJSON_AddItemToObject(root, num_key, num_item);
    cJSON_AddItemToObjectCS(root, cs_key, str_item);

    // step 4: Operate - retrieve items and extract values
    retrieved_num_item = cJSON_GetObjectItem(root, num_key);
    retrieved_str_item = cJSON_GetObjectItemCaseSensitive(root, cs_key);
    extracted_number = cJSON_GetNumberValue(retrieved_num_item);
    extracted_string = cJSON_GetStringValue(retrieved_str_item);

    // step 5: Validate - compute a simple score using extracted values
    validation_score = (int)(root != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(retrieved_num_item != (cJSON *)0) + (int)(retrieved_str_item != (cJSON *)0) + (int)(extracted_number == 123.456) + (int)(extracted_string != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}