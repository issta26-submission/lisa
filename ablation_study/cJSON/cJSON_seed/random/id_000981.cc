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
//<ID> 981
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *str_key = "title";
    const char *year_key = "year";
    const char *num_key = "value";
    const char *str_val = "test-string";
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *year_item = (cJSON *)0;
    cJSON *value_item = (cJSON *)0;
    cJSON *fetched_title = (cJSON *)0;
    cJSON *fetched_year_cs = (cJSON *)0;
    char *json_str = (char *)0;
    const char *retrieved_str = (const char *)0;
    double retrieved_year = 0.0;
    double retrieved_value = 0.0;
    cJSON_bool add_ok_title = (cJSON_bool)0;
    cJSON_bool add_ok_year = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root and items
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString(str_val);
    year_item = cJSON_CreateNumber(2021.0);

    // step 3: Configure - attach the string via AddItemToObject and add a number via AddNumberToObject
    add_ok_title = cJSON_AddItemToObject(root, str_key, str_item);
    value_item = cJSON_AddNumberToObject(root, num_key, 2022.0);
    add_ok_year = cJSON_AddItemToObject(root, year_key, year_item);

    // step 4: Operate - retrieve items using both case-insensitive and case-sensitive helpers and serialize
    fetched_title = cJSON_GetObjectItem(root, str_key);
    fetched_year_cs = cJSON_GetObjectItemCaseSensitive(root, year_key);
    retrieved_str = cJSON_GetStringValue(fetched_title);
    retrieved_year = cJSON_GetNumberValue(fetched_year_cs);
    retrieved_value = cJSON_GetNumberValue(value_item);
    json_str = cJSON_Print(root);

    // step 5: Validate - compute a simple score exercising the retrieved values and flags
    validation_score = (int)(root != (cJSON *)0) + (int)(add_ok_title) + (int)(add_ok_year) + (int)(value_item != (cJSON *)0) + (int)(json_str != (char *)0) + (int)(retrieved_str != (const char *)0 && retrieved_str[0] == 't') + (int)(retrieved_year == 2021.0) + (int)(retrieved_value == 2022.0);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete root (which frees attached items)
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}