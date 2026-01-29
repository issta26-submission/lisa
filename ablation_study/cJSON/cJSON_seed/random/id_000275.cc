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
//<ID> 275
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved_str = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    char *printed = (char *)0;
    const char *extracted_str = (const char *)0;
    double set_res = 0.0;
    double num_value = 0.0;
    cJSON_bool added_greeting = (cJSON_bool)0;
    cJSON_bool added_year = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("Hello cJSON");
    num_item = cJSON_CreateNumber(0.0);
    set_res = cJSON_SetNumberHelper(num_item, 2025.5);

    // step 3: Configure
    added_greeting = cJSON_AddItemToObject(root, "greeting", str_item);
    added_year = cJSON_AddItemToObject(root, "year", num_item);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    retrieved_str = cJSON_GetObjectItem(root, "greeting");
    retrieved_num = cJSON_GetObjectItem(root, "year");
    extracted_str = cJSON_GetStringValue(retrieved_str);
    num_value = cJSON_GetNumberValue(retrieved_num);

    // step 5: Validate
    set_res = cJSON_SetNumberHelper(retrieved_num, num_value + 0.5);

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}