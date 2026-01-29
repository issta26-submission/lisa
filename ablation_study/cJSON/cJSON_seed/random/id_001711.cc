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
//<ID> 1711
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *nested = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *got_case_insensitive = (cJSON *)0;
    cJSON *got_case_sensitive = (cJSON *)0;
    char *json_unformatted = (char *)0;
    const char *extracted_str = (const char *)0;
    double extracted_num = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    nested = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(7.5);
    str_item = cJSON_CreateString("value");

    // step 3: Configure
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON_AddItemToObject(nested, "num", num_item);
    cJSON_AddItemToObject(nested, "Str", str_item);

    // step 4: Operate
    json_unformatted = cJSON_PrintUnformatted(root);
    got_case_insensitive = cJSON_GetObjectItem(nested, "str");
    got_case_sensitive = cJSON_GetObjectItemCaseSensitive(nested, "Str");
    extracted_num = cJSON_GetNumberValue(cJSON_GetObjectItem(nested, "num"));
    extracted_str = cJSON_GetStringValue(got_case_insensitive);

    // step 5: Validate
    validation = (int)(extracted_num == 7.5) + (int)(extracted_str != (const char *)0) + (int)(json_unformatted != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)root;
    (void)nested;
    (void)num_item;
    (void)str_item;
    (void)got_case_insensitive;
    (void)got_case_sensitive;
    (void)extracted_str;
    (void)extracted_num;
    (void)json_unformatted;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}