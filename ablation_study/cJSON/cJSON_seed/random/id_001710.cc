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
//<ID> 1710
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON *retrieved_str = (cJSON *)0;
    char *json_unformatted = (char *)0;
    double extracted_num = 0.0;
    const char *extracted_str = (const char *)0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.0);
    str_item = cJSON_CreateString("value");

    // step 3: Configure
    cJSON_AddItemToObject(root, "number", num_item);
    cJSON_AddItemToObject(root, "label", str_item);

    // step 4: Operate
    json_unformatted = cJSON_PrintUnformatted(root);
    retrieved_num = cJSON_GetObjectItem(root, "number");
    retrieved_str = cJSON_GetObjectItemCaseSensitive(root, "label");
    extracted_num = cJSON_GetNumberValue(retrieved_num);
    extracted_str = cJSON_GetStringValue(retrieved_str);

    // step 5: Validate
    validation = (int)(extracted_num == 123.0) + (int)(extracted_str != (const char *)0) + (int)(json_unformatted != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)root;
    (void)num_item;
    (void)str_item;
    (void)retrieved_num;
    (void)retrieved_str;
    (void)extracted_num;
    (void)extracted_str;
    (void)json_unformatted;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}