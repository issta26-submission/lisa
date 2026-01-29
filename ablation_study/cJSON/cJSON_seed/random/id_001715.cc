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
//<ID> 1715
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
    cJSON *retrieved = (cJSON *)0;
    cJSON *retrieved_cs = (cJSON *)0;
    char *json_unformatted = (char *)0;
    double extracted_value1 = 0.0;
    double extracted_value2 = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);
    str_item = cJSON_CreateString("example");

    // step 3: Configure
    cJSON_AddItemToObject(root, "number", num_item);
    cJSON_AddItemToObject(root, "label", str_item);

    // step 4: Operate
    retrieved = cJSON_GetObjectItem(root, "number");
    retrieved_cs = cJSON_GetObjectItemCaseSensitive(root, "number");
    json_unformatted = cJSON_PrintUnformatted(root);
    extracted_value1 = cJSON_GetNumberValue(retrieved);
    extracted_value2 = cJSON_GetNumberValue(retrieved_cs);

    // step 5: Validate
    validation = (int)(extracted_value1 == 42.0) + (int)(extracted_value2 == 42.0) + (int)(json_unformatted != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)root;
    (void)num_item;
    (void)str_item;
    (void)retrieved;
    (void)retrieved_cs;
    (void)extracted_value1;
    (void)extracted_value2;
    (void)json_unformatted;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}