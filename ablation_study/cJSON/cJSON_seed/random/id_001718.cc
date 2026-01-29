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
//<ID> 1718
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
    cJSON *label_item = (cJSON *)0;
    cJSON *caps_item = (cJSON *)0;
    cJSON *generic_item = (cJSON *)0;
    cJSON *case_item = (cJSON *)0;
    char *json_unformatted = (char *)0;
    const char *extracted_caps = (const char *)0;
    double extracted_number = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(2021.0);
    label_item = cJSON_CreateString("year");
    caps_item = cJSON_CreateString("caps");

    // step 3: Configure
    cJSON_AddItemToObject(root, "number", num_item);
    cJSON_AddItemToObject(root, "label", label_item);
    cJSON_AddItemToObject(root, "Number", caps_item);

    // step 4: Operate
    json_unformatted = cJSON_PrintUnformatted(root);
    generic_item = cJSON_GetObjectItem(root, "number");
    case_item = cJSON_GetObjectItemCaseSensitive(root, "Number");
    extracted_number = cJSON_GetNumberValue(generic_item);
    extracted_caps = cJSON_GetStringValue(case_item);

    // step 5: Validate
    validation = (int)(generic_item != (cJSON *)0) + (int)(case_item != (cJSON *)0) + (int)(json_unformatted != (char *)0) + (int)(extracted_number == 2021.0) + (int)(extracted_caps != (const char *)0 && extracted_caps[0] == 'c');
    (void)validation;

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)root;
    (void)num_item;
    (void)label_item;
    (void)caps_item;
    (void)generic_item;
    (void)case_item;
    (void)extracted_caps;
    (void)extracted_number;
    (void)json_unformatted;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}